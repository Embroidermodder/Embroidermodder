#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

#include "../extern/libembroidery/src/embroidery.h"
#undef bufferSize

#include "scheme.h"
#include "scheme-private.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <string>

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#include <QHeaderView>
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
//#include <QOpenGLWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QPixmap>
#include <QPointF>
#include <QProcess>
#include <QPushButton>
#include <QRubberBand>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QSettings>
#include <QSignalMapper>
#include <QSplitter>
#include <QStandardPaths>
#include <QStatusBar>
#include <QString>
#include <QStyle>
#include <QStyleOption>
#include <QtCore/qmath.h>
#include <QTextBrowser>
#include <QTextLayout>
#include <QTextStream>
#include <QtGlobal>
#include <QtGui>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QtPrintSupport>
#include <QTransform>
#include <QUndoCommand>
#include <QUndoGroup>
#include <QUndoStack>
#include <QUndoView>
#include <QUrl>
#include <QVBoxLayout>
#include <QWhatsThis>
#include <QWidget>
#include <QXmlStreamReader>

class MainWindow;
class MdiArea;
class MdiWindow;
class View;
class StatusBar;
class StatusBarButton;
class CmdPrompt;
class PropertyEditor;
class UndoEditor;

class ArcObject;
class BlockObject;
class CircleObject;
class DimAlignedObject;
class DimAngularObject;
class DimArcLengthObject;
class DimDiameterObject;
class DimLeaderObject;
class DimLinearObject;
class DimOrdinateObject;
class DimRadiusObject;
class EllipseObject;
class EllipseArcObject;
class HatchObject;
class ImageObject;
class InfiniteLineObject;
class LineObject;
class PathObject;
class PointObject;
class PolygonObject;
class PolylineObject;
class RayObject;
class RectObject;
class SplineObject;
class TextMultiObject;
class TextSingleObject;

typedef pointer SchemeFunction(scheme *, pointer);

/*!
 * If the function is an in-built then the flag is set to 1 and main, click,
 * move and context arrays don't contain the function definition: instead
 * the function pointer to command contains the definition.
 *
 * TODO: replace the alias system with simple wrappers:
 * (define (alias) (function))
 */
typedef struct SchemeData_ {
    const char name[100];
    const char tooltip[1000];
    const char statustip[1000];
    SchemeFunction *function;
} SchemeData;

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
    QHash<QString, QString>*  aliasHash;

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
    CmdPromptInput*    promptInput;
    CmdPromptHistory*  promptHistory;
    QVBoxLayout*       promptVBoxLayout;
    QFrame*            promptDivider;

    CmdPromptSplitter* promptSplitter;

    QHash<QString, QString>*  styleHash;
    void updateStyle();
    QTimer* blinkTimer;
    bool blinkState;
};

class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();

private:
    QWidget*          mainWidget;

    void              getInfo();
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
    void setMainWin(MainWindow* mainWin) { _mainWin = mainWin; }
protected:
    virtual bool event(QEvent *e);
private:
    MainWindow* _mainWin;
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

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    std::unordered_map<std::string, QAction*> actionHash;

    void loadToolbars(void);
    void loadMenus(void);

    MainWindow();
    ~MainWindow();

    MdiArea* getMdiArea();
    MainWindow* getApplication();
    MdiWindow* activeMdiWindow();
    View* activeView();
    QGraphicsScene* activeScene();
    QUndoStack* activeUndoStack();

    void setUndoCleanIcon(bool opened);

    virtual void                    updateMenuToolbarStatusbar();

    MainWindow*     mainWin;
    MdiArea*        mdiArea;
    CmdPrompt*      prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor*     dockUndoEdit;
    StatusBar*      statusbar;

    QList<QGraphicsItem*> cutCopyObjectList;

    QHash<QString, QToolBar*>       toolbarHash;
    QHash<QString, QMenu*>          menuHash;

    QString                         formatFilterOpen;
    QString                         formatFilterSave;

    bool                            isCommandActive() { return prompt->isCommandActive(); }
    QString                         activeCommand() { return prompt->activeCommand(); }

    QString platformString();

public slots:

    void                            onCloseWindow();
    virtual void                    onCloseMdiWin(MdiWindow*);

    void                            recentMenuAboutToShow();

    void                            onWindowActivated (QMdiSubWindow* w);
    void                            windowMenuAboutToShow();
    void                            windowMenuActivated( bool checked/*int id*/ );

    void                            updateAllViewScrollBars(bool val);
    void                            updateAllViewCrossHairColors(QRgb color);
    void                            updateAllViewBackgroundColors(QRgb color);
    void                            updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void                            updateAllViewGridColors(QRgb color);
    void                            updateAllViewRulerColors(QRgb color);

    void                            updatePickAddMode(bool val);
    void                            pickAddModeToggled();

    void                            settingsPrompt();

    void                            settingsDialog(const QString& showTab = QString());
    void                            readSettings();
    void                            writeSettings();

    static bool                     validFileFormat(const QString &fileName);

protected:
    virtual void                    resizeEvent(QResizeEvent*);
    void                            closeEvent(QCloseEvent *event);
    QAction*                        getFileSeparator();
    void                            loadFormats();

private:


    bool                            shiftKeyPressedState;

    QByteArray                      layoutState;

    int                             numOfDocs;
    int                             docIndex;

    QList<MdiWindow*>               listMdiWin;
    QMdiSubWindow*                  findMdiWindow(const QString &fileName);
    QString                         openFilesPath;

    QAction*                        myFileSeparator;

    QWizard*    wizardTipOfTheDay;
    QLabel*     labelTipOfTheDay;
    QCheckBox*  checkBoxTipOfTheDay;
    QStringList listTipOfTheDay;

    void                            createAllActions();
    QAction*                        createAction(const QString icon, const QString toolTip, const QString statusTip, bool scripted = false);
    //====================================================
    //Toolbars
    //====================================================
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
    //====================================================
    //Selectors
    //====================================================
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
    void runCommandMain(const QString& cmd);
    void runCommandClick(const QString& cmd,  qreal x, qreal y);
    void runCommandMove(const QString& cmd,  qreal x, qreal y);
    void runCommandContext(const QString& cmd, const QString& str);
    void runCommandPrompt(const QString& line);

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
    void about();
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

    void iconResize(int iconSize);

    //Selectors
    void layerSelectorIndexChanged(int index);
    void colorSelectorIndexChanged(int index);
    void linetypeSelectorIndexChanged(int index);
    void lineweightSelectorIndexChanged(int index);
    void textFontSelectorCurrentFontChanged(const QFont& font);
    void textSizeSelectorIndexChanged(int index);

    QString textFont();
    qreal   textSize();
    qreal   textAngle();
    bool    textBold();
    bool    textItalic();
    bool    textUnderline();
    bool    textStrikeOut();
    bool    textOverline();

    void setTextFont(const QString& str);
    void setTextSize(qreal num);
    void setTextAngle(qreal num);
    void setTextBold(bool val);
    void setTextItalic(bool val);
    void setTextUnderline(bool val);
    void setTextStrikeOut(bool val);
    void setTextOverline(bool val);

    QString getCurrentLayer();
    QRgb    getCurrentColor();
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
    void zoomRealtime();
    void zoomPrevious();
    void zoomWindow();
    void zoomDynamic();
    void zoomScale();
    void zoomCenter();
    void zoomIn();
    void zoomOut();
    void zoomSelected();
    void zoomAll();
    void zoomExtents();
    // Pan SubMenu
    void panrealtime();
    void panpoint();
};

class MainWindow;

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
    MainWindow* mainWin;

    bool useLogo;
    bool useTexture;
    bool useColor;

    QPixmap bgLogo;
    QPixmap bgTexture;
    QColor  bgColor;

    void zoomExtentsAllSubWindows();
    void forceRepaint();
};

class MdiWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiWindow(const int theIndex, MainWindow* mw, QMdiArea* parent, Qt::WindowFlags wflags);
    ~MdiWindow();

    virtual QSize              sizeHint() const;
    QString                    getCurrentFile()   { return curFile; }
    QString                    getShortCurrentFile();
    View*                      getView() { return gview; }
    QGraphicsScene*            getScene() { return gscene; }
    QString                    getCurrentLayer() { return curLayer; }
    QRgb                       getCurrentColor() { return curColor; }
    QString                    getCurrentLineType() { return curLineType; }
    QString                    getCurrentLineWeight() { return curLineWeight; }
    void                       setCurrentLayer(const QString& layer) { curLayer = layer; }
    void                       setCurrentColor(const QRgb& color) { curColor = color; }
    void                       setCurrentLineType(const QString& lineType) { curLineType = lineType; }
    void                       setCurrentLineWeight(const QString& lineWeight) { curLineWeight = lineWeight; }
    void                       designDetails();
    bool                       loadFile(const QString &fileName);
    bool                       saveFile(const QString &fileName);
signals:
    void                       sendCloseMdiWin(MdiWindow*);

public slots:
    void                       closeEvent(QCloseEvent* e);
    void                       onWindowActivated();

    void                       currentLayerChanged(const QString& layer);
    void                       currentColorChanged(const QRgb& color);
    void                       currentLinetypeChanged(const QString& type);
    void                       currentLineweightChanged(const QString& weight);

    void                       updateColorLinetypeLineweight();
    void                       deletePressed();
    void                       escapePressed();

    void                       showViewScrollBars(bool val);
    void                       setViewCrossHairColor(QRgb color);
    void                       setViewBackgroundColor(QRgb color);
    void                       setViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha);
    void                       setViewGridColor(QRgb color);
    void                       setViewRulerColor(QRgb color);

    void                       print();
    void                       saveBMC();

    void promptHistoryAppended(const QString& txt);
    void logPromptInput(const QString& txt);
    void promptInputPrevious();
    void promptInputNext();

protected:

private:
    MainWindow*                mainWin;
    QMdiArea*                  mdiArea;
    QGraphicsScene*            gscene;
    View*                      gview;

    bool                       fileWasLoaded;

    QString promptHistory;
    QList<QString> promptInputList;
    int promptInputNum;

    QPrinter                   printer;

    QString                    curFile;
    void                       setCurrentFile(const QString& fileName);
    QString                    fileExtension(const QString& fileName);

    int                        myIndex;

    QString                    curLayer;
    QRgb                       curColor;
    QString                    curLineType;
    QString                    curLineWeight;

    void promptInputPrevNext(bool prev);
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

const char* const ENABLE_SNAP   = "ENABLE_SNAP";
const char* const ENABLE_GRID   = "ENABLE_GRID";
const char* const ENABLE_RULER  = "ENABLE_RULER";
const char* const ENABLE_ORTHO  = "ENABLE_ORTHO";
const char* const ENABLE_POLAR  = "ENABLE_POLAR";
const char* const ENABLE_QSNAP  = "ENABLE_QSNAP";
const char* const ENABLE_QTRACK = "ENABLE_QTRACK";
const char* const ENABLE_LWT    = "ENABLE_LWT";
const char* const ENABLE_REAL   = "ENABLE_REAL";

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
    QRgb         objectColorRGB()                      const { return objPen.color().rgb(); }
    Qt::PenStyle objectLineType()                      const { return objPen.style(); }
    qreal        objectLineWeight()                    const { return lwtPen.widthF(); }
    QPainterPath objectPath()                          const { return path(); }
    int          objectRubberMode()                    const { return objRubberMode; }
    QPointF      objectRubberPoint(const QString& key) const;
    QString      objectRubberText(const QString& key) const;

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
    inline qreal pi() const { return (qAtan(1.0)*4.0); }
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
    qreal   objectCenterX()       const { return scenePos().x(); }
    qreal   objectCenterY()       const { return scenePos().y(); }
    qreal   objectRadius()        const { return rect().width()/2.0*scale(); }
    qreal   objectStartAngle()    const;
    qreal   objectEndAngle()      const;
    QPointF objectStartPoint()    const;
    qreal   objectStartX()        const;
    qreal   objectStartY()        const;
    QPointF objectMidPoint()      const;
    qreal   objectMidX()          const;
    qreal   objectMidY()          const;
    QPointF objectEndPoint()      const;
    qreal   objectEndX()          const;
    qreal   objectEndY()          const;
    qreal   objectArea()          const;
    qreal   objectArcLength()     const;
    qreal   objectChord()         const;
    qreal   objectIncludedAngle() const;
    bool    objectClockwise()     const;

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
    qreal   objectCenterX()       const { return scenePos().x(); }
    qreal   objectCenterY()       const { return scenePos().y(); }
    qreal   objectRadius()        const { return rect().width()/2.0*scale(); }
    qreal   objectDiameter()      const { return rect().width()*scale(); }
    qreal   objectArea()          const { return pi()*objectRadius()*objectRadius(); }
    qreal   objectCircumference() const { return pi()*objectDiameter(); }
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
    qreal   objectX1()        const { return objectEndPoint1().x(); }
    qreal   objectY1()        const { return objectEndPoint1().y(); }
    qreal   objectX2()        const { return objectEndPoint2().x(); }
    qreal   objectY2()        const { return objectEndPoint2().y(); }
    qreal   objectDeltaX()    const { return (objectX2() - objectX1()); }
    qreal   objectDeltaY()    const { return (objectY2() - objectY1()); }
    qreal   objectAngle()     const;
    qreal   objectLength()    const { return line().length(); }

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
    qreal   objectCenterX()       const { return scenePos().x(); }
    qreal   objectCenterY()       const { return scenePos().y(); }
    qreal   objectRadiusMajor()   const { return qMax(rect().width(), rect().height())/2.0*scale(); }
    qreal   objectRadiusMinor()   const { return qMin(rect().width(), rect().height())/2.0*scale(); }
    qreal   objectDiameterMajor() const { return qMax(rect().width(), rect().height())*scale(); }
    qreal   objectDiameterMinor() const { return qMin(rect().width(), rect().height())*scale(); }
    qreal   objectWidth()         const { return rect().width()*scale(); }
    qreal   objectHeight()        const { return rect().height()*scale(); }
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
    qreal   objectWidth()       const { return rect().width()*scale(); }
    qreal   objectHeight()      const { return rect().height()*scale(); }
    qreal   objectArea()        const { return qAbs(objectWidth()*objectHeight()); }

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
    qreal   objectX1()        const { return objectEndPoint1().x(); }
    qreal   objectY1()        const { return objectEndPoint1().y(); }
    qreal   objectX2()        const { return objectEndPoint2().x(); }
    qreal   objectY2()        const { return objectEndPoint2().y(); }
    qreal   objectDeltaX()    const { return (objectX2() - objectX1()); }
    qreal   objectDeltaY()    const { return (objectY2() - objectY1()); }
    qreal   objectAngle()     const;
    qreal   objectLength()    const { return line().length()*scale(); }

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
    qreal   objectX()   const { return scenePos().x(); }
    qreal   objectY()   const { return scenePos().y(); }

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
    qreal   objectX()   const { return scenePos().x(); }
    qreal   objectY()   const { return scenePos().y(); }

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
    qreal   objectX()   const { return scenePos().x(); }
    qreal   objectY()   const { return scenePos().y(); }

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
    qreal   objectX()   const { return scenePos().x(); }
    qreal   objectY()   const { return scenePos().y(); }

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
    qreal   objectWidth()       const { return rect().width()*scale(); }
    qreal   objectHeight()      const { return rect().height()*scale(); }
    qreal   objectArea()        const { return qAbs(objectWidth()*objectHeight()); }

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
    void addPoint        (EmbPattern* pattern, QGraphicsItem* item);
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
    qreal   objectTextSize()       const { return objTextSize; }
    bool    objectTextBold()       const { return objTextBold; }
    bool    objectTextItalic()     const { return objTextItalic; }
    bool    objectTextUnderline()  const { return objTextUnderline; }
    bool    objectTextStrikeOut()  const { return objTextStrikeOut; }
    bool    objectTextOverline()   const { return objTextOverline; }
    bool    objectTextBackward()   const { return objTextBackward; }
    bool    objectTextUpsideDown() const { return objTextUpsideDown; }
    QPointF objectPos()            const { return scenePos(); }
    qreal   objectX()              const { return scenePos().x(); }
    qreal   objectY()              const { return scenePos().y(); }

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

class ImageWidget;

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
    QWidget* focusWidget;

    //Helper functions
    QToolButton*   createToolButton(const QString& iconName, const QString& txt);
    QLineEdit*     createLineEdit(const QString& validatorType = QString(), bool readOnly = false);
    QComboBox*     createComboBox(bool disable = false);
    QFontComboBox* createFontComboBox(bool disable = false);

    void updateLineEditStrIfVaries(QLineEdit* lineEdit, const QString& str);
    void updateLineEditNumIfVaries(QLineEdit* lineEdit, qreal num, bool useAnglePrecision);
    void updateFontComboBoxStrIfVaries(QFontComboBox* fontComboBox, const QString& str);
    void updateComboBoxStrIfVaries(QComboBox* comboBox, const QString& str, const QStringList& strList);
    void updateComboBoxBoolIfVaries(QComboBox* comboBox, bool val, bool yesOrNoText);

    QSignalMapper* signalMapper;
    void mapSignal(QObject* fieldObj, const QString& name, QVariant value);

    //====================
    //Selection
    //====================
    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

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
    QGroupBox*   createGroupBoxGeometryDimOrdinate();
    QGroupBox*   createGroupBoxGeometryDimLeader();
    QGroupBox*   createGroupBoxGeometryDimLinear();
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

#include <stdbool.h>
#include <stdint.h>

#include "scheme.h"
#include "scheme-private.h"
#include "dynload.h"

#define TypeError                 0
#define NotEnoughArgumentsError   1
#define GeneralError              2

/*!
 * NOTE: main() is run every time the command is started.
 *       Use it to reset variables so they are ready to go.
 *
 * NOTE: click() is run only for left clicks.
 *       Middle clicks are used for panning.
 *       Right clicks bring up the context menu.
 *
 * NOTE: context() is run when a context menu entry is chosen.
 *
 * NOTE: prompt() is run when Enter is pressed.
 *       appendPromptHistory is automatically called before prompt()
 *       is called so calling it is only needed for erroneous input.
 *       Any text is in the command prompt is sent as an uppercase string.
 */
#define CONTEXT_MAIN              0
#define CONTEXT_CLICK             1
#define CONTEXT_CONTEXT           2
#define CONTEXT_PROMPT            3

int load_scheme(void);
int run_command(const char *line);

int streq(char *, const char *);

/* Main Window functions */
void debug(const char *s);

void alert(const char *s);
void blink_prompt(void);
void set_prompt_prefix(const char *s);
void append_prompt_history(const char *s);

void init_command(void);
void clear_selection(void);
void end_command(void);

const char *platform_string(void);

void message_box(const char *type, const char *title, const char *text);

void print_area(double x, double y, double w, double h);

void set_background_color(uint8_t r, uint8_t g, uint8_t b);
void set_crosshair_color(uint8_t r, uint8_t g, uint8_t b);
void set_grid_color(uint8_t r, uint8_t g, uint8_t b);

const char *text_font(void);
double text_size(void);
double text_angle(void);
bool text_bold(void);
bool text_italic(void);
bool text_underline(void);
bool text_strikeout(void);
bool text_overline(void);

void set_text_font(const char *s);
void set_text_size(double size);
void set_text_angle(double angle);
void set_text_bold(bool bold);
void set_text_italic(bool italic);
void set_text_underline(bool underline);
void set_text_strikeout(bool strikeout);
void set_text_overline(bool overline);

void preview_on(int clone, int mode, double x, double y, double data);
void preview_off(void);

void vulcanize(void);
void clear_rubber(void);
bool allow_rubber(void);
void spare_rubber(int64_t id);
void set_rubber_filter(int64_t id);
void set_rubber_mode(int mode);
void set_rubber_point(const char *key, double x, double y);
void set_rubber_text(const char *key, const char *txt);

void add_text_multi(const char *s, double x, double y, double rot, bool fill, int rubberMode);
void add_text_single(const char *s, double x, double y, double roy, bool fill, int rubberMode);
void add_infinite_line(double x1, double y1, double x2, double y2, double rot);
void add_ray(double x1, double y1, double x2, double y2, double rot);
void add_line(double x1, double y1, double x2, double y2, double rot, int rubberMode);
void add_triangle(double x1, double y1, double x2, double y2, double x3, double y3, double rot, bool fill);
void add_rectangle(double x, double y, double w, double h, double rot, bool fill, int rubberMode);
void add_rounded_rectangle(double x, double y, double w, double h, double rad, double rot, bool fill);
void add_arc(double startX, double startY, double midX, double midY, double endX, double endY, int rubberMode);
void add_circle(double centerX, double centerY, double radius, bool fill, int rubberMode);
void add_slot(double centerX, double centerY, double diameter, double length, double rot, bool fill, int rubberMode);
void add_ellipse(double centerX, double centerY, double width, double height, double rot, bool fill, int rubberMode);
void add_point(double x, double y);
void add_regular_polygon(double centerX, double centerY, int sides, int mode, double rad, double rot, bool fill);
void add_polygon(double startX, double startY, int rubberMode);
void add_polyline(double startX, double startY, int rubberMode);
void add_path(double startX, double startY, int rubberMode);
void add_horizontal_dimension(double x1, double y1, double x2, double y2, double legHeight);
void add_veritical_dimension(double x1, double y1, double x2, double y2, double legHeight);
void add_image(const char *img, double x, double y, double w, double h, double rot);
void add_dimleader(double x1, double y1, double x2, double y2, double rot, int rubberMode);

void set_cursor_shape(const char *shape);

double calculate_angle(double x1, double y1, double x2, double y2);
double calculate_distance(double x1, double y1, double x2, double y2);
double perpendicular_distance(double px, double py, double x1, double y1, double x2, double y2);

int num_selected(void);
void select_all(void);
void add_to_selection(void);
void delete_selected(void);
void cut_selected(double x, double y);
void copy_selected(double x, double y);
void paste_selected(double x, double y);
void move_selected(double dx, double dy);
void scale_selected(double x, double y, double factor);
void rotate_selected(double x, double y, double rot);
void mirror_selected(double x1, double y1, double x2, double y2);

double qsnapx(void);
double qsnapy(void);
double mousex(void);
double mousey(void);

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

class Settings_Dialog : public QDialog
{
    Q_OBJECT

public:
    Settings_Dialog(MainWindow* mw, const QString& showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

private:
    MainWindow*       mainWin;

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
    MainWindow* mainWin;
    StatusBar*  statusbar;
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

protected:

private slots:

private:

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
    qreal       dx;
    qreal       dy;
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
    qreal       pivotX;
    qreal       pivotY;
    qreal       angle;
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
    qreal       dx;
    qreal       dy;
    qreal       factor;
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
    bool    done;
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

    QString     iconDir;
    int         iconSize;

    QUndoGroup* undoGroup;
    QUndoView*  undoView;
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
    int  numSelected();

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
    qreal   previewData;
    int     previewMode;

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

    MainWindow* mainWin;
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
    QPoint  viewMousePoint;
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

    inline qreal pi() { return (qAtan(1.0)*4.0); };
    inline qreal radians(qreal degrees) { return (degrees*pi()/180.0); };
};

extern QString settings_general_language;
extern QString settings_general_icon_theme;
extern int settings_general_icon_size;
extern bool settings_general_mdi_bg_use_logo;
extern bool settings_general_mdi_bg_use_texture;
extern bool settings_general_mdi_bg_use_color;
extern QString settings_general_mdi_bg_logo;
extern QString settings_general_mdi_bg_texture;
extern QRgb settings_general_mdi_bg_color;
extern bool settings_general_tip_of_the_day;
extern quint16 settings_general_current_tip;
extern bool settings_general_system_help_browser;
extern bool settings_general_check_for_updates;
extern bool settings_display_use_opengl;
extern bool settings_display_renderhint_aa;
extern bool settings_display_renderhint_text_aa;
extern bool settings_display_renderhint_smooth_pix;
extern bool settings_display_renderhint_high_aa;
extern bool settings_display_renderhint_noncosmetic;
extern bool settings_display_show_scrollbars;
extern int settings_display_scrollbar_widget_num;
extern QRgb settings_display_crosshair_color;
extern QRgb settings_display_bg_color;
extern QRgb settings_display_selectbox_left_color;
extern QRgb settings_display_selectbox_left_fill;
extern QRgb settings_display_selectbox_right_color;
extern QRgb settings_display_selectbox_right_fill;
extern quint8 settings_display_selectbox_alpha;
extern qreal settings_display_zoomscale_in;
extern qreal settings_display_zoomscale_out;
extern quint8 settings_display_crosshair_percent;
extern QString settings_display_units;
extern QRgb settings_prompt_text_color;
extern QRgb settings_prompt_bg_color;
extern QString settings_prompt_font_family;
extern QString settings_prompt_font_style;
extern quint8 settings_prompt_font_size;
extern bool settings_prompt_save_history;
extern bool settings_prompt_save_history_as_html;
extern QString settings_prompt_save_history_filename;
extern QStringList settings_opensave_recent_list_of_files;
extern QString settings_opensave_custom_filter;
extern QString settings_opensave_open_format;
extern bool settings_opensave_open_thumbnail;
extern QString settings_opensave_save_format;
extern bool settings_opensave_save_thumbnail;
extern quint8 settings_opensave_recent_max_files;
extern QStringList settings_opensave_recent_list_of_files;
extern QString settings_opensave_recent_directory;
extern quint8 settings_opensave_trim_dst_num_jumps;
extern QString settings_printing_default_device;
extern bool settings_printing_use_last_device;
extern bool settings_printing_disable_bg;
extern bool settings_grid_show_on_load;
extern bool settings_grid_show_origin;
extern bool settings_grid_color_match_crosshair;
extern QRgb settings_grid_color;
extern bool settings_grid_load_from_file;
extern QString settings_grid_type;
extern bool settings_grid_center_on_origin;
extern qreal settings_grid_center_x;
extern qreal settings_grid_center_y;
extern qreal settings_grid_size_x;
extern qreal settings_grid_size_y;
extern qreal settings_grid_spacing_x;
extern qreal settings_grid_spacing_y;
extern qreal settings_grid_size_radius;
extern qreal settings_grid_spacing_radius;
extern qreal settings_grid_spacing_angle;
extern bool settings_ruler_show_on_load;
extern bool settings_ruler_metric;
extern QRgb settings_ruler_color;
extern quint8 settings_ruler_pixel_size;
extern bool settings_qsnap_enabled;
extern QRgb settings_qsnap_locator_color;
extern quint8 settings_qsnap_locator_size;
extern quint8 settings_qsnap_aperture_size;
extern bool settings_qsnap_endpoint;
extern bool settings_qsnap_midpoint;
extern bool settings_qsnap_center;
extern bool settings_qsnap_node;
extern bool settings_qsnap_quadrant;
extern bool settings_qsnap_intersection;
extern bool settings_qsnap_extension;
extern bool settings_qsnap_insertion;
extern bool settings_qsnap_perpendicular;
extern bool settings_qsnap_tangent;
extern bool settings_qsnap_nearest;
extern bool settings_qsnap_apparent;
extern bool settings_qsnap_parallel;
extern bool settings_lwt_show_lwt;
extern bool settings_lwt_real_render;
extern qreal settings_lwt_default_lwt;
extern bool settings_selection_mode_pickfirst;
extern bool settings_selection_mode_pickadd;
extern bool settings_selection_mode_pickdrag;
extern QRgb settings_selection_coolgrip_color;
extern QRgb settings_selection_hotgrip_color;
extern quint8 settings_selection_grip_size;
extern quint8 settings_selection_pickbox_size;
extern QString settings_text_font;
extern qreal settings_text_size;
extern qreal settings_text_angle;
extern bool settings_text_style_bold;
extern bool settings_text_style_italic;
extern bool settings_text_style_underline;
extern bool settings_text_style_overline;
extern bool settings_text_style_strikeout;

#endif

