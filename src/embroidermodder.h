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
 */

#ifndef __EMBROIDERMODDER_H__
#define __EMBROIDERMODDER_H__

#include <vector>
#include <string>

/* From this source code directory. */
#include "core.h"

/* Qt 6.0+ libraries. */
#include <QAction>
#include <QApplication>
#include <QOpenGLWidget>
#include <QtPrintSupport>

#define CSTR(str) ( (char*)((str).toStdString().c_str()) )

class ImageWidget;
class MdiArea;
class MdiWindow;
class View;
class StatusBar;
class CmdPrompt;
class PropertyEditor;
class UndoEditor;
class MainWindow;
class Geometry;

/* Global variables. */
extern MdiArea* mdiArea;
extern MainWindow* _mainWin;
extern CmdPrompt* prompt;
extern PropertyEditor* dockPropEdit;
extern UndoEditor* dockUndoEdit;
extern StatusBar* statusbar;
extern QAction* actionHash[MAX_ACTIONS];

/* Functions in the global namespace */
QString translate_str(const char *str);
bool contains(std::vector<std::string>, std::string);
bool validFileFormat(std::string fileName);
QString fileExtension(std::string fileName);

std::string convert_args_to_type(
    std::string label,
    std::vector<std::string> args,
    const char *args_template,
    std::vector<Node> a);

void add_polyline(QPainterPath p, std::string rubberMode);

View *activeView(void);
QGraphicsScene* activeScene();

void set_enabled(QObject *parent, const char *key, bool enabled);
void set_visibility(QObject *parent, const char *name, bool visibility);
QPainterPath add_to_path(QPainterPath path, EmbVector scale, std::string s);

QPointF to_QPointF(EmbVector a);
EmbVector to_EmbVector(QPointF a);

QIcon swatch(int32_t c);
QWidget *make_widget(QWidget *parent, Node *d, WidgetData data);

/* The Geometry class
 *
 * Combine all geometry objects into one class that uses the Type
 * flag to determine the behaviour of overlapping functions and
 * bar the use of nonsensical function calls.
 */
class Geometry : public QGraphicsPathItem
{
public:
    GeometryData gdata;

    QPen objPen;
    QPen lwtPen;
    QLineF objLine;
    std::string objRubberMode = "OBJ_RUBBER_OFF";
    std::vector<RubberPoint> rubber_points;
    QHash<QString, QPointF> objRubberPoints;
    QHash<QString, QString> objRubberTexts;
    int64_t objID;
    int64_t mode;

    QPointF arcStartPoint;
    QPointF arcMidPoint;
    QPointF arcEndPoint;

    uint64_t flags = 0;

    EmbReal text_size = 20.0;

    QPainterPath lineStylePath;
    QPainterPath arrowStylePath;
    EmbReal arrowStyleAngle;
    EmbReal arrowStyleLength;
    EmbReal lineStyleAngle;
    EmbReal lineStyleLength;

    QPainterPath normalPath;

    QString objText;
    QString objTextFont;
    QString objTextJustify;
    QPainterPath objTextPath;

    EmbVector positions[MAX_POSITIONS];

    int gripIndex;

    int Type = OBJ_TYPE_BASE;
    virtual int type(){ return Type; }

    /* Constructors. */
    Geometry(int object_type, QGraphicsItem* parent = 0);
    Geometry(Geometry *obj, QGraphicsItem* parent = 0);
    void init(int object_type, QRgb rgb, Qt::PenStyle lineType,
        QGraphicsItem* parent = 0);

    /* Destructor. */
    ~Geometry();

    /* Alter state */
    void setFlag_(uint64_t new_flag) { flags |= new_flag; }
    void unsetFlag_(uint64_t new_flag) { flags ^= new_flag; }

    /* Getters */
    Qt::PenStyle objectLineType() { return objPen.style(); }
    EmbReal objectLineWeight() { return lwtPen.widthF(); }
    QPointF objectRubberPoint(QString key);
    QString objectRubberText(QString key);

    QPointF objectTopLeft();
    QPointF objectTopRight();
    QPointF objectBottomLeft();
    QPointF objectBottomRight();
    EmbReal objectArea();
    QPointF objectStartPoint();
    QPointF objectMidPoint();
    QPointF objectEndPoint();

    QRectF rect();

    EmbReal objectWidth();
    EmbReal objectHeight();
    EmbReal objectRadiusMajor();
    EmbReal objectRadiusMinor();
    EmbReal objectDiameterMajor();
    EmbReal objectDiameterMinor();
    QPointF objectEndPoint1();
    QPointF objectEndPoint2();
    EmbReal objectStartAngle();
    EmbReal objectEndAngle();
    EmbReal objectArcLength();
    EmbReal objectChord();
    EmbReal objectIncludedAngle();
    bool objectClockwise();
    EmbReal objectAngle();
    QPointF objectDelta() { return objectEndPoint2() - objectEndPoint1(); }
    EmbReal objectLength() { return objLine.length()*scale(); }
    EmbReal objectRadius();
    EmbReal objectDiameter();
    EmbReal objectCircumference();
    QPointF objectQuadrant0();
    QPointF objectQuadrant90();
    QPointF objectQuadrant180();
    QPointF objectQuadrant270();
    QPainterPath objectCopyPath();
    QPainterPath objectSavePath();

    std::vector<QPainterPath> objectSavePathList() { return subPathList(); }
    std::vector<QPainterPath> subPathList();

    int findIndex(const QPointF& point);

    EmbReal objectReal(int64_t real_type);
    void setObjectPoint(EmbVector pt, int64_t point_type);

    void setObjectEndPoint1(EmbVector endPt1);
    void setObjectEndPoint2(EmbVector endPt2);

    void updatePath();
    void updatePath(const QPainterPath& p);
    void updateLeader(void);

    virtual QRectF boundingRect();

    void drawRubberLine(const QLineF& rubLine, QPainter* painter = 0, const char* colorFromScene = 0);

    void updateRubber(QPainter* painter = 0);
    void vulcanize(void);
    QPointF mouseSnapPoint(const QPointF& mousePoint);
    std::vector<QPointF> allGripPoints();
    void gripEdit(const QPointF& before, const QPointF& after);

    void realRender(QPainter* painter, const QPainterPath& renderPath);
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

    /* Updaters, todo: combine */
    void calculateArcData(EmbArc arc);
    void updateArcRect(EmbReal radius);
    void update(void);

    /* Setters */
    void init_line(EmbLine line);
    void init_rect(EmbRect rect);
    void init_text_single(QString str, EmbVector position);
    void init_path(QPainterPath p);
    void init_point(EmbVector pos);

    void setObjectPos(const QPointF& point) { setPos(point.x(), point.y()); }
    void setObjectCenter(EmbVector center);
    void setObjectSize(EmbReal width, EmbReal height);
    void setObjectRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h);
    void setRect(EmbReal x, EmbReal y, EmbReal w, EmbReal h);
    void setLine(const QLineF& li);
    void setLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
    void setObjectLineWeight(std::string lineWeight);
    void setObjectRadius(EmbReal radius);
    void setObjectStartAngle(EmbReal angle);
    void setObjectEndAngle(EmbReal angle);
    void setObjectStartPoint(EmbVector point);
    void setObjectMidPoint(EmbVector point);
    void setObjectEndPoint(EmbVector point);
    void setObjectDiameter(EmbReal diameter);
    void setObjectArea(EmbReal area);
    void setObjectCircumference(EmbReal circumference);
    void setObjectText(QString str);
    void setObjectTextFont(QString font);
    void setObjectTextJustify(QString justify);
    void setObjectTextSize(EmbReal size);
    void setObjectTextStyle(bool bold, bool italic, bool under, bool strike, bool over);
    void setObjectTextBold(bool val);
    void setObjectTextItalic(bool val);
    void setObjectTextUnderline(bool val);
    void setObjectTextStrikeOut(bool val);
    void setObjectTextOverline(bool val);
    void setObjectTextBackward(bool val);
    void setObjectTextUpsideDown(bool val);
    void setObjectRadiusMajor(EmbReal radius);
    void setObjectRadiusMinor(EmbReal radius);
    void setObjectDiameterMajor(EmbReal diameter);
    void setObjectDiameterMinor(EmbReal diameter);

    /* Scripted commands, uses the script string in */
    void script_main(void);
    void script_click(EmbVector v);
    void script_context(std::string str);
    void script_prompt(std::string str);
};

/* The Command Prompt object. */
class CmdPromptInput : public QLineEdit
{
    Q_OBJECT

public:
    CmdPromptInput(QWidget* parent = 0);
    ~CmdPromptInput() {}

    QString curText;
    QString defaultPrefix;
    QString prefix;

    QString lastCmd;
    QString curCmd;
    bool cmdActive;

    bool rapidFireEnabled;
    bool isBlinking;

    void changeFormatting(std::vector<QTextLayout::FormatRange> formats);
    void clearFormatting();
    void applyFormatting();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void appendHistory(QString txt, int prefixLength);

    //These connect to the CmdPrompt signals
    void startCommand(QString cmd);
    void runCommand(QString cmd, QString cmdtxt);
    void showSettings();
    void stopBlinking();

public slots:
    void endCommand();
    void processInput(void);
    void checkSelection();
    void updateCurrentText(QString txt);
    void checkEditedText(QString txt);
    void checkChangedText(QString txt);
    void checkCursorPosition(int oldpos, int newpos);
};

/* The Command Prompt History class. */
class CmdPromptHistory : public QTextBrowser
{
    Q_OBJECT

public:
    CmdPromptHistory(QWidget* parent = 0);
    ~CmdPromptHistory() {}

    int tmpHeight;
    QString applyFormatting(QString txt, int prefixLength);

protected:
    void contextMenuEvent(QContextMenuEvent* event);

public slots:
    void appendHistory(QString txt, int prefixLength);
    void resizeHistory(int y);

signals:
    void historyAppended(QString txt);
};

/* . */
class CmdPrompt : public QWidget
{
    Q_OBJECT

public:
    CmdPrompt(QWidget* parent = 0);
    ~CmdPrompt() {}

    CmdPromptInput* promptInput;
    CmdPromptHistory* promptHistory;
    QVBoxLayout* promptVBoxLayout;
    QFrame* promptDivider;

    QString styleHash[10];
    void updateStyle();
    QTimer* blinkTimer;
    bool blinkState;

public slots:
    void setCurrentText(QString txt) {
        promptInput->curText = promptInput->prefix + txt;
        promptInput->setText(promptInput->curText);
    }
    void setHistory(QString txt) {
        promptHistory->setHtml(txt);
        promptHistory->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    }
    void setPrefix(QString txt);
    void appendHistory(QString txt);

    void alert(QString txt);

    void startBlinking();
    void stopBlinking();

    void setPromptTextColor(const QColor&);
    void setPromptBackgroundColor(const QColor&);
    void setPromptFontFamily(QString );
    void setPromptFontStyle(QString );
    void setPromptFontSize(int);

    void saveHistory(QString fileName, bool html);

signals:
    void appendTheHistory(QString txt, int prefixLength);

    //For connecting outside of command prompt
    void startCommand(QString cmd);
    void runCommand(QString cmd, QString cmdtxt);

    void shiftPressed();
    void shiftReleased();

    void showSettings();

    void historyAppended(QString txt);
};

/* . */
class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();

    QWidget* mainWidget;

    void getInfo();
    QWidget* createMainWidget();
    QWidget* createHistogram();

    QDialogButtonBox* buttonBox;

    uint32_t stitchesTotal;
    uint32_t stitchesReal;
    uint32_t stitchesJump;
    uint32_t stitchesTrim;
    uint32_t colorTotal;
    uint32_t colorChanges;

    QRectF boundingRect;
};

/* The Image widget object. */
class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    QImage img;
    ImageWidget(QString filename, QWidget* parent = 0);
    ~ImageWidget();

    bool load(QString fileName);
    bool save(QString fileName);

protected:
    void paintEvent(QPaintEvent* event);
};

/*  . */
class LayerManager : public QDialog
{
    Q_OBJECT

public:
    QStandardItemModel* layerModel;
    QSortFilterProxyModel* layerModelSorted;
    QTreeView* treeView;

    LayerManager(QWidget *parent = 0);
    ~LayerManager();

    void addLayer(QString name, const bool visible, const bool frozen,
        const EmbReal zValue, const QRgb color, QString lineType,
        QString lineWeight, const bool print);
};

/* The MainWindow class. */
class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    MdiWindow* activeMdiWindow();
    QUndoStack* activeUndoStack();

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    std::vector<QGraphicsItem*> cutCopyObjectList;

    QString formatFilterOpen;
    QString formatFilterSave;

    bool isCommandActive() { return prompt->promptInput->cmdActive; }
    QString activeCommand() { return prompt->promptInput->curCmd; }

    QString platformString();

    void create_toolbar(ToolbarData t);
    void populate_toolbars(Qt::ToolBarArea area, int32_t list[]);
    QIcon create_icon(QString stub);
    QIcon create_icon(int32_t stub);

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

protected:
    virtual void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
    void loadFormats();

    bool shiftKeyPressedState;

    QByteArray layoutState;

    int numOfDocs;
    int docIndex;

    std::vector<MdiWindow*> listMdiWin;
    QMdiSubWindow* findMdiWindow(std::string fileName);

    QAction* myFileSeparator;

    void createAllActions();
    void createAllMenus();
    void createAllToolbars();

    // Selectors
    QComboBox* layerSelector;
    QComboBox* colorSelector;
    QComboBox* linetypeSelector;
    QComboBox* lineweightSelector;
    QFontComboBox* textFontSelector;
    QComboBox* textSizeSelector;

public slots:
    void stub_testing();

    void promptHistoryAppended(QString txt);
    void logPromptInput(QString txt);
    void promptInputPrevious();
    void promptInputNext();

    void about(void);
    void tipOfTheDay(void);

    void newFile();
    void openFile(bool recent = false, std::string recentFile = "");
    void openFilesSelected(QStringList files);
    void openrecentfile();
    void savefile();
    void saveasfile();
    void quit();
    void checkForUpdates();
    // Help Menu
    void buttonTipOfTheDayClicked(int);

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

    void setTextFont(QString str);
    void setTextSize(EmbReal num);

    QString getCurrentLayer();
    QRgb getCurrentColor();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    bool isShiftPressed();
    void setShiftPressed();
    void setShiftReleased();

    void deletePressed();
    void escapePressed();
};

class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow();

    QMdiArea* mdiArea;
    QGraphicsScene* gscene;
    View* gview;

    bool fileWasLoaded;

    QString promptHistory;
    std::vector<QString> promptInputList;
    int promptInputNum;

    QPrinter printer;

    QString curFile;
    void setCurrentFile(QString fileName);

    int myIndex;

    QString curLayer;
    QRgb curColor;
    QString curLineType;
    QString curLineWeight;

    void promptInputPrevNext(bool prev);

    virtual QSize sizeHint();
    QString getShortCurrentFile();
    void designDetails();
    bool loadFile(std::string fileName);
    bool saveFile(std::string fileName);
signals:
    void sendCloseMdiWin(MdiWindow*);

public slots:
    void closeEvent(QCloseEvent* e);
    void onWindowActivated();

    void print();
    void saveBMC();
};

/* . */
class MdiArea : public QMdiArea
{
    Q_OBJECT

public:
    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();

    MdiArea(QWidget* parent = 0)
    {
        setTabsClosable(true);

        useLogo = false;
        useTexture = false;
        useColor = false;
    }

    ~MdiArea() {}
public slots:
    /* Cascade the MDI windows. */
    void cascade()
    {
        cascadeSubWindows();
        zoomExtentsAllSubWindows();
    }

    /* Tile the MDI windows. */
    void tile()
    {
        tileSubWindows();
        zoomExtentsAllSubWindows();
    }
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
};

/* . */
class PreviewDialog : public QFileDialog
{
    Q_OBJECT

public:
    PreviewDialog(QWidget* parent = 0,
       QString caption = QString(),
       QString directory = QString(),
       QString filter = QString());
    ~PreviewDialog();

    ImageWidget* imgWidget;
};


class PropertyEditor : public QDockWidget
{
    Q_OBJECT

public:
    PropertyEditor(QString iconDirectory = QString(), bool pickAddMode = true, QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
    ~PropertyEditor();

    QWidget* focusWidget;

    QString iconDir;
    int iconSize;
    Qt::ToolButtonStyle propertyEditorButtonStyle;

    bool pickAdd;

    std::vector<QGraphicsItem*> selectedItemList;

    QToolButton* createToolButton(QString iconName, QString txt);

    int precisionAngle;
    int precisionLength;

    void updateLineEditStrIfVaries(QLineEdit* lineEdit, QString str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, EmbReal num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, QString str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, QString str, std::vector<std::string> strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, QString name, QVariant value);

    // Selection
    // ====================
    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

    QComboBox*   comboBoxSelected;
    QToolButton* toolButtonQSelect;
    QToolButton* toolButtonPickAdd;

    //TODO: Alphabetic/Categorized TabWidget

    QFontComboBox* comboBoxTextSingleFont;

    QGroupBox *createGroupBox(int group_box_key, const char *title);

    QGroupBox *groupBoxes[GB_TOTAL];

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void pickAddModeToggled();

public slots:
    void setSelectedItems(std::vector<QGraphicsItem*> itemList);
    void updatePickAddModeButton(bool pickAddMode);

private slots:
    void fieldEdited(QObject* fieldObj);
    void showGroups(int objType);
    void showOneType(int index);
    void hideAllGroups();
    void clearAllFields();
    void togglePickAddMode();
};


class SelectBox : public QRubberBand
{
    Q_OBJECT

public:
    SelectBox(Shape s, QWidget* parent = 0);

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

public slots:
    void setDirection(int dir);
    void setColors(const QColor& colorL, const QColor& fillL, const QColor& colorR, const QColor& fillR, int newAlpha);

protected:
    void paintEvent(QPaintEvent*);
};

/* . */
class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(QString showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

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

    void create_float_spinbox(
        QGroupBox *gb,
        QGridLayout* gridLayout,
        const char *label_in,
        EmbReal single_step,
        EmbReal lower,
        EmbReal upper,
        int key,
        int row);
    QCheckBox* create_checkbox(QGroupBox *groupbox, std::string label);

    void chooseColor(int color_id);
    void chooseIndex(int setting_id);
    void colorChanged(const QColor& color, Node *d, int color_id);
    void indexChanged(int value, Node *d, int setting_id);

    void acceptChanges();
    void rejectChanges();
};

/* . */
class StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    StatusBar(QWidget* parent = 0);
    QToolButton *buttons[20];
    QLabel* statusBarMouseCoord;
    void setMouseCoord(EmbReal x, EmbReal y);
    void context_menu_action(QToolButton *button, const char *icon, const char *label, QMenu *menu, std::string setting_page);
    void toggle(std::string key, bool on);
    void context_menu_event(QContextMenuEvent *event, QToolButton *button);
};

/* . */
class UndoEditor : public QDockWidget
{
    Q_OBJECT

public:
    UndoEditor(QString iconDirectory = QString(), QWidget* widgetToFocus = 0, QWidget* parent = 0); //, Qt::WindowFlags flags = 0);
    ~UndoEditor();

    void addStack(QUndoStack* stack);

    bool canUndo();
    bool canRedo();

    QWidget* focusWidget;

    QString iconDir;
    int iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;

    QString undoText();
    QString redoText();
protected:

public slots:
    void undo();
    void redo();

    void updateCleanIcon(bool opened);
};

/* . */
class UndoableCommand : public QUndoCommand
{
public:
    UndoableCommand(std::string command, QString text, Geometry* obj, View* v, QUndoCommand* parent = 0);
    UndoableCommand(EmbVector d, QString text, Geometry* obj, View* v, QUndoCommand* parent = 0);
    UndoableCommand(std::string command, EmbVector pivot, EmbReal angle, QString text, Geometry* obj, View* v, QUndoCommand* parent = 0);
    UndoableCommand(QString type, View* v, QUndoCommand* parent = 0);
    UndoableCommand(const QPointF beforePoint, const QPointF afterPoint, QString text, Geometry* obj, View* v, QUndoCommand* parent = 0);
    UndoableCommand(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, QString text, Geometry* obj, View* v, QUndoCommand* parent = 0);

    int id(){ return 1234; }
    bool mergeWith(const QUndoCommand* command);
    void undo();
    void redo();
    void mirror();
    void rotate(EmbVector pivot, EmbReal rot);

    Geometry* object;
    View* gview;
    std::string command;
    EmbVector delta;
    EmbVector pivot;
    QPointF before;
    QPointF after;
    EmbReal angle;
    EmbReal factor;
    QString navType;
    QTransform fromTransform;
    QTransform toTransform;
    QPointF fromCenter;
    QPointF toCenter;
    QLineF mirrorLine;
    bool done;
};

/* . */
class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(QGraphicsScene* theScene, QWidget* parent);
    ~View();

    uint64_t state;

    int formatType;
    EmbPattern *pattern;

    QColor gridColor;
    QPainterPath gridPath;
    QPainterPath originPath;
    bool rulerMetric;
    QColor rulerColor;
    uint8_t rulerPixelSize;

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

    Geometry* gripBaseObj;
    Geometry* tempBaseObj;

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

    QHash<int64_t, QGraphicsItem*> hashDeletedObjects;
    std::vector<std::string> spareRubberList;
    std::vector<QGraphicsItem*> rubberRoomList;
    std::vector<QGraphicsItem*> previewObjectList;
    QPointF previewPoint;
    EmbReal previewData;
    uint32_t previewMode;
    QPointF cutCopyMousePoint;
    QPointF pasteDelta;
    uint32_t panDistance;
    EmbVector panStartPos;

    /* Graphics functions */
    void draw_line(QPainter *painter, QPointF start, QPointF end);
    void draw_rect(QPainter *painter, QPointF start, QPointF end);
    void draw_rulers(QPainter* painter, const QRectF& rect);
    void draw_crosshair(QPainter* painter, const QRectF& rect);
    QRectF rect_from_center(QPointF center, float radius);

    void recalculateLimits();
    void zoomToPoint(const QPoint& mousePoint, int zoomDir);
    void centerAt(const QPointF& centerPoint);
    QPointF center() { return mapToScene(rect().center()); }
    void updateMouseCoords(int x, int y);

    void addObject(Geometry* obj);
    void deleteObject(Geometry* obj);
    void vulcanizeObject(Geometry* obj);

    std::vector<QGraphicsItem*> selected_items();

    bool allowZoomIn();
    bool allowZoomOut();

    void createGridRect();
    void createGridPolar();
    void createGridIso();
    void createOrigin();

    void loadRulerSettings();

    QPainterPath createRulerTextPath(EmbVector position, QString str, EmbReal height);

    QGraphicsItemGroup* previewObjectItemGroup;

    std::vector<QGraphicsItem*> createObjectList(std::vector<QGraphicsItem*> list);
    QGraphicsItemGroup* pasteObjectItemGroup;

    void copySelected();

    void startGripping(Geometry* obj);
    void stopGripping(bool accept = false);

    void panStart(const QPoint& point);

    void alignScenePointWithViewPoint(const QPointF& scenePoint, const QPoint& viewPoint);

    void action() {}

    void zoomIn();
    void zoomOut();
    void zoomWindow();
    void zoomSelected();
    void zoomExtents();
    void panRealTime();
    void panPoint();
    void panLeft();
public slots:
    void panRight();
    void panUp();
    void panDown();
    void selectAll();
    void selectionChanged();
    void clearSelection();
    void deleteSelected();
    void moveSelected(EmbVector delta);
    void cut();
    void copy();
    void paste();
    void repeatAction();
    void moveAction();
    void scaleAction();
    void scaleSelected(EmbVector point, EmbReal factor);
    void rotateAction();
    void rotateSelected(EmbVector pivot, EmbReal rot);
    void mirrorSelected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
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
    void setFlag(uint64_t flag);
    void unsetFlag(uint64_t flag);

    void setGridColor(QRgb color);
    void createGrid(void);
    void setRulerColor(QRgb color);

    void previewOn(uint32_t clone, uint32_t mode, EmbVector v, EmbReal data);
    void previewOff();

    bool allowRubber();
    void addToRubberRoom(QGraphicsItem* item);
    void vulcanizeRubberRoom();
    void clearRubberRoom();
    void spareRubber(int64_t id);
    void setRubberMode(std::string mode);
    void setRubberPoint(QString key, const QPointF& point);
    void setRubberText(QString key, QString txt);

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

#endif
