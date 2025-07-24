/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Global header for the embroidermodder GUI.
 *
 * New developers should try reading the comments in and altering `script.cpp`
 * or creating/altering files in `scripts/`. If you are unsure what part of the
 * source code to add your new feature in: put it in `script.cpp`.
 */

#ifndef EMBROIDERMODDER_H
#define EMBROIDERMODDER_H

#include <QAbstractItemModel>
#include <QAction>
#include <QApplication>
#include <QBrush>
#include <QBasicTimer>
#include <QCheckBox>
#include <QClipboard>
#include <QCloseEvent>
#include <QColor>
#include <QComboBox>
#include <QContextMenuEvent>
#include <QDate>
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
#include <QFrame>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QHash>
#include <QImage>
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
#include <QRgb>
#include <QRubberBand>
#include <QScrollArea>
#include <QScrollBar>
#include <QSettings>
#include <QSignalMapper>
#include <QSortFilterProxyModel>
#include <QSplitter>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QString>
#include <QStringList>
#include <QTabWidget>
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
#include <QTreeView>
#include <QUndoCommand>
#include <QUndoGroup>
#include <QUndoStack>
#include <QUndoView>
#include <QUrl>
#include <QVBoxLayout>
#include <QWhatsThis>
#include <QWidget>

#include <QtCore/qmath.h>
//#include <QOpenGLWidget>

#include <cinttypes>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_map>

#include <chrono>
#include <thread>

#include "embroidery.h"
#include "constants.h"

/* NOTE: this is Qt dependant so this means that needs to be here not in "constants.h"
 * after the necessary Qt header.
 */
#if defined(Q_OS_AIX)
#define OS_STR                    "AIX"
#elif defined(Q_OS_BSD4)
#define OS_STR                    "BSD 4.4"
#elif defined(Q_OS_BSDI)
#define OS_STR                    "BSD/OS"
#elif defined(Q_OS_CYGWIN)
#define OS_STR                    "Cygwin"
#elif defined(Q_OS_DARWIN)
#define OS_STR                    "Mac OS"
#elif defined(Q_OS_DGUX)
#define OS_STR                    "DG/UX"
#elif defined(Q_OS_DYNIX)
#define OS_STR                    "DYNIX/ptx"
#elif defined(Q_OS_FREEBSD)
#define OS_STR                    "FreeBSD"
#elif defined(Q_OS_HPUX)
#define OS_STR                    "HP-UX"
#elif defined(Q_OS_HURD)
#define OS_STR                    "GNU Hurd"
#elif defined(Q_OS_IRIX)
#define OS_STR                    "SGI Irix"
#elif defined(Q_OS_LINUX)
#define OS_STR                    "Linux"
#elif defined(Q_OS_LYNX)
#define OS_STR                    "LynxOS"
#elif defined(Q_OS_MAC)
#define OS_STR                    "Mac OS"
#elif defined(Q_OS_MSDOS)
#define OS_STR                    "MS-DOS"
#elif defined(Q_OS_NETBSD)
#define OS_STR                    "NetBSD"
#elif defined(Q_OS_OS2)
#define OS_STR                    "OS/2"
#elif defined(Q_OS_OPENBSD)
#define OS_STR                    "OpenBSD"
#elif defined(Q_OS_OS2EMX)
#define OS_STR                    "XFree86 on OS/2"
#elif defined(Q_OS_OSF)
#define OS_STR                    "HP Tru64 UNIX"
#elif defined(Q_OS_QNX)
#define OS_STR                    "QNX Neutrino"
#elif defined(Q_OS_RELIANT)
#define OS_STR                    "Reliant UNIX"
#elif defined(Q_OS_SCO)
#define OS_STR                    "SCO OpenServer 5"
#elif defined(Q_OS_SOLARIS)
#define OS_STR                    "Sun Solaris"
#elif defined(Q_OS_SYMBIAN)
#define OS_STR                    "Symbian"
#elif defined(Q_OS_ULTRIX)
#define OS_STR                    "DEC Ultrix"
#elif defined(Q_OS_UNIX)
#define OS_STR                    "UNIX BSD/SYSV"
#elif defined(Q_OS_UNIXWARE)
#define OS_STR                    "UnixWare"
#elif defined(Q_OS_WIN32)
#define OS_STR                    "Windows"
#elif defined(Q_OS_WINCE)
#define OS_STR                    "Windows CE"
#else
#define OS_STR                    "Unknown System"
#endif

/* ---- Forward class declarations ------------------------------------------ */
class MainWindow;
class View;
class ArcObject;
class BaseObject;
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
class StatusBarButton;
class StatusBar;
class ImageWidget;
class SelectBox;
class CmdPrompt;
class MdiArea;
class MdiWindow;
class PropertyEditor;
class UndoEditor;

/* ---- C++ Typedefs -------------------------------------------------------- */
typedef std::string String;
typedef std::vector<std::string> StringList;

typedef struct ToolbarData_ {
    QString label;
    StringList entries;
    bool mdi_only;
} ToolbarData;

typedef struct MenuData_ {
    QString label;
    StringList entries;
    bool mdi_only;
    bool top_level;
} MenuData;

typedef struct Command_ {
    QString icon;
    QString command;
    QString tooltip;
    QString statustip;
    QString shortcut;
    QString macos;
    unsigned char checkable;
} Command;

typedef struct ScriptValue_ {
    String s;
    QStringList l;
    int32_t i;
    uint32_t u;
    double r;
    bool b;
} ScriptValue;

/** @brief Data for a given interface setting. */
typedef struct SettingsData_ {
    String section;
    String key;
    String default_value;
    char type;
    bool enabled;
    String description;
    String icon;
    double lower;
    double upper;
    double single_step;
} SettingsData;

/** @brief Property data for a given geometric object. */
typedef struct PropertiesData_ {
    String key;
    String label;
    String icon;
    char type;
    bool editable;
} PropertiesData;

/* ---- Function declarations (that aren't in a class) ---------------------- */
bool script_env_boot(void);
void script_env_free(void);
void load_data();

QRgb getCurrentColor();

MdiWindow* activeMdiWindow(void);
View* activeView(void);
QGraphicsScene* activeScene(void);
QUndoStack* activeUndoStack(void);

int get_id(const char **table, const char *cmd);

/* ---- Global data --------------------------------------------------------- */
extern MainWindow* _mainWin;
extern MdiArea* mdiArea;
extern CmdPrompt* prompt;
extern PropertyEditor* dockPropEdit;
extern UndoEditor* dockUndoEdit;
extern StatusBar* statusbar;

extern QHash<QString, QAction*> actionHash;
extern std::vector<Command> command_map;
extern SettingsData settings_table[N_SETTINGS];
extern ScriptValue st[N_SETTINGS];
extern std::unordered_map<std::string, ToolbarData> toolbar_table;
extern std::unordered_map<std::string, MenuData> menu_table;
extern std::unordered_map<std::string, StringList> string_tables;
extern std::unordered_map<std::string, std::vector<PropertiesData>> properties_table;
extern std::unordered_map<QString, QString> aliases;
extern const char *command_names[MAX_COMMANDS];

extern int context_flag;
extern bool testing;
extern int test_script_pos;

/* ---- Class declarations -------------------------------------------------- */

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

    CmdPromptInput* promptInput;
    CmdPromptHistory* promptHistory;
    QVBoxLayout* promptVBoxLayout;
    QFrame* promptDivider;

    CmdPromptSplitter* promptSplitter;

    QHash<QString, QString>* styleHash;
    void updateStyle();
    QTimer* blinkTimer;
    bool blinkState;

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
};

class EmbDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    EmbDetailsDialog(QGraphicsScene* theScene, QWidget *parent = 0);
    ~EmbDetailsDialog();

private:
    QWidget* mainWidget;

    void getInfo();
    QWidget* createMainWidget();
    QWidget* createHistogram();

    QDialogButtonBox* buttonBox;

    quint32 stitchesTotal;
    quint32 stitchesReal;
    quint32 stitchesJump;
    quint32 stitchesTrim;
    quint32 colorTotal;
    quint32 colorChanges;

    QRectF boundingRect;
};

/* ---- Class declarations -------------------------------------------------- */
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

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void cmd(const char *line);
    void run(const char *filename);
    ScriptValue get(const char *key);
    void set(const char *key, ScriptValue value);

    QPrinter printer;
    QBasicTimer timer;

    void timerEvent(QTimerEvent *);

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    QIcon createIcon(QString label);
    void addToToolbar(const char *toolbar, const char *name, const char *list);
    void addToMenu(QMenu *menu, StringList data);
    void addToComboBox(QComboBox *box, StringList data);
    void place_toolbars(Qt::ToolBarArea toolbar_area, const char *toolbar_list);

    bool isCommandActive() { return prompt->isCommandActive(); }
    QString activeCommand() { return prompt->activeCommand(); }

    QString platformString();

    void generate(const char *type);
    void fill(const char *type);

public slots:
    void onCloseWindow();
    virtual void onCloseMdiWin(MdiWindow*);

    void recentMenuAboutToShow();

    void onWindowActivated (QMdiSubWindow* w);
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

    QByteArray layoutState;

    int numOfDocs;
    int docIndex;

    QList<MdiWindow*> listMdiWin;
    QMdiSubWindow* findMdiWindow(const QString &fileName);
    QString openFilesPath;

    QAction* myFileSeparator;

    QWizard* wizardTipOfTheDay;
    QLabel* labelTipOfTheDay;
    QCheckBox* checkBoxTipOfTheDay;
    QStringList listTipOfTheDay;

    void createAllActions(void);
    void createAllToolbars();

    // Selectors
    QComboBox* layerSelector;
    QComboBox* colorSelector;
    QComboBox* linetypeSelector;
    QComboBox* lineweightSelector;
    QFontComboBox* textFontSelector;
    QComboBox* textSizeSelector;

    // Menus
    void createAllMenus();


private slots:
    void hideUnimplemented();

public slots:

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
    void runCommandPrompt(const QString& cmd, const QString& str);

    void openFile(bool recent = false, const QString& recentFile = "");
    void openFilesSelected(const QStringList&);
    void openrecentfile();
    // Help Menu
    void tipOfTheDay();
    void buttonTipOfTheDayClicked(int);
    void checkBoxTipOfTheDayStateChanged(int);
    void about();

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void toggleGrid();
    void toggleRuler();
    void toggleLwt();

    void iconResize(int iconSize);

    // Selectors
    void layerSelectorIndexChanged(int index);
    void colorSelectorIndexChanged(int index);
    void linetypeSelectorIndexChanged(int index);
    void lineweightSelectorIndexChanged(int index);
    void textFontSelectorCurrentFontChanged(const QFont& font);
    void textSizeSelectorIndexChanged(int index);

    QString getCurrentLayer();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    // Standard Slots
    bool isShiftPressed();

    void deletePressed();
    void escapePressed();

    // Layer Toolbar
    void makeLayerActive();
    void layerManager();
    void layerPrevious();

public:
    //Natives
    void nativeMessageBox(const QString& type, const QString& title, const QString& text);

    void nativePrintArea(qreal x, qreal y, qreal w, qreal h);

    bool nativeAllowRubber();
    void nativeSpareRubber(qint64 id);
    // TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void nativeSetRubberMode(int mode);
    void nativeSetRubberPoint(const QString& key, qreal x, qreal y);
    void nativeSetRubberText(const QString& key, const QString& txt);

    /* Geometry */
    void add_text_multi(const QString& str, EmbVector position, qreal rot, bool fill, int rubberMode);
    void add_text_single(const QString& str, EmbVector position, qreal rot, bool fill, int rubberMode);
    void add_infinite_line(EmbVector point1, EmbVector point2, qreal rot);
    void add_ray(EmbVector start, EmbVector point, qreal rot);
    void add_line(EmbVector start, EmbVector end, qreal rot, int rubberMode);
    void add_triangle(EmbVector point1, EmbVector point2, EmbVector point3, qreal rot, bool fill);
    void add_rectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode);
    void add_rounded_rectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill);
    void add_arc(EmbVector start, EmbVector mid, EmbVector end, int rubberMode);
    void add_circle(EmbVector center, qreal radius, bool fill, int rubberMode);
    void add_slot(EmbVector center, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode);
    void add_ellipse(EmbVector center, qreal width, qreal height, qreal rot, bool fill, int rubberMode);
    void add_point(EmbVector position);
    void add_regular_polygon(EmbVector center, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill);
    void add_polygon(EmbVector start, const QPainterPath& p, int rubberMode);
    void add_polyline(EmbVector start, const QPainterPath& p, int rubberMode);
    void add_path(EmbVector start, const QPainterPath& p, int rubberMode);
    void add_image(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot);

    /* Annotations */
    void add_dim_leader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode);
    void add_horizontal_dimension(EmbVector start, EmbVector end, qreal legHeight);
    void add_vertical_dimension(EmbVector start, EmbVector end, qreal legHeight);

    void  nativeSetCursorShape(const QString& str);
    qreal nativeCalculateAngle(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal nativeCalculateDistance(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal nativePerpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2);

    void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativePasteSelected(qreal x, qreal y);
    void nativeMoveSelected(qreal dx, qreal dy);
    void nativeScaleSelected(qreal x, qreal y, qreal factor);
    void nativeRotateSelected(qreal x, qreal y, qreal rot);
    void nativeMirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2);
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

    virtual QSize sizeHint() const;
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

    void saveBMC();

    void promptHistoryAppended(const QString& txt);
    void logPromptInput(const QString& txt);
    void promptInputPrevious();
    void promptInputNext();

public:
    MainWindow* mainWin;
    QMdiArea* mdiArea;
    QGraphicsScene* gscene;
    View* gview;

    bool fileWasLoaded;

    QString promptHistory;
    QList<QString> promptInputList;
    int promptInputNum;

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
    qreal   objTextSize;
    bool    objTextBold;
    bool    objTextItalic;
    bool    objTextUnderline;
    bool    objTextStrikeOut;
    bool    objTextOverline;
    bool    objTextBackward;
    bool    objTextUpsideDown;
    QPainterPath objTextPath;
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

    QComboBox*   createComboBoxSelected();
    QToolButton* createToolButtonQSelect();
    QToolButton* createToolButtonPickAdd();

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
    Settings_Dialog(const QString& showTab = QString(), QWidget *parent = 0);
    ~Settings_Dialog();

    QCheckBox *basic_checkbox(QGroupBox *gb, int id);
    QDoubleSpinBox *basic_double_spinbox(QGroupBox *gb, int id);
    QSpinBox *basic_spinbox(QGroupBox *gb, int id);

private:
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

#endif
