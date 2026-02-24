#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QHash>
#include <QDir>
#include <QJSEngine>

#include "mdiarea.h"
#include "mdiwindow.h"
#include "cmdprompt.h"
#include "core.h"

class MdiArea;
class MdiWindow;
class View;
class StatusBar;
class StatusBarButton;
class CmdPrompt;
class PropertyEditor;
class UndoEditor;

QT_BEGIN_NAMESPACE
class QComboBox;
class QAction;
class QToolBar;
class QCloseEvent;
class QMenu;
class QScriptProgram;
class QUndoStack;
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    int loadData(void);

    QJSEngine engine;
    void javaInitNatives(void);
    void javaLoadCommand(const QString& cmdName);

    MdiArea* getMdiArea();
    MainWindow* getApplication();
    MdiWindow* activeMdiWindow();
    View* activeView();
    QGraphicsScene* activeScene();
    QUndoStack* activeUndoStack();

    void setUndoCleanIcon(bool opened);

    virtual void updateMenuToolbarStatusbar();

    MainWindow*     mainWin;
    MdiArea*        mdiArea;
    CmdPrompt*      prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor*     dockUndoEdit;
    StatusBar*      statusbar;

    QList<QGraphicsItem*> cutCopyObjectList;

    QHash<int, QAction*> actionHash;
    QHash<QString, QToolBar*> toolbarHash;
    QHash<QString, QMenu*> menuHash;

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

    static bool                     validFileFormat(const QString &fileName);

protected:
    virtual void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *event);
    QAction*                        getFileSeparator();
    void loadFormats();

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

    void createAllActions();
    QAction*                        createAction(CommandData command, bool scripted = false);
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
    void createWindowMenu();
    void createHelpMenu();

    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* viewMenu;
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

    int call(QString name);
    void debug(QString txt);

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
    void runCommandPrompt(const QString& cmd, const QString& str);

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

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void toggleGrid();
    void toggleRuler();
    void toggleLwt();

    // Icons
    void iconResize(int iconSize);
    void icon16();
    void icon24();
    void icon32();
    void icon48();
    void icon64();
    void icon128();

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
    void panLeft();
    void panRight();
    void panUp();
    void panDown();

    void dayVision();
    void nightVision();

    /* Prompt */
    void alert(const QString& txt);
    void blinkPrompt();
    void setPromptPrefix(const QString& txt);
    void appendPromptHistory(const QString& txt);

    void initCommand();
    void endCommand();
    void messageBox(const QString& type, const QString& title, const QString& text);

    void printArea(qreal x, qreal y, qreal w, qreal h);

    void setBackgroundColor(quint8 r, quint8 g, quint8 b);
    void setCrossHairColor(quint8 r, quint8 g, quint8 b);
    void setGridColor(quint8 r, quint8 g, quint8 b);

    void previewOn(int clone, int mode, qreal x, qreal y, qreal data);
    void previewOff();

    /* Rubber */
    void vulcanize();
    void clearRubber();
    bool allowRubber();
    void spareRubber(qint64 id);
    //TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void setRubberMode(int mode);
    void setRubberPoint(const QString& key, qreal x, qreal y);
    void setRubberText(const QString& key, const QString& txt);

    /* Geometry */
    void addTextMulti(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode);
    void addTextSingle(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode);

    void addInfiniteLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    void addRay(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    void addLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode);
    void addTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill);
    void addRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode);
    void addRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill);
    void addArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, int rubberMode);
    void addCircle(qreal centerX, qreal centerY, qreal radius, bool fill, int rubberMode);
    void addSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode);
    void addEllipse(qreal centerX, qreal centerY, qreal width, qreal height, qreal rot, bool fill, int rubberMode);
    void addPoint(qreal x, qreal y);
    void addRegularPolygon(qreal centerX, qreal centerY, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill);
    void addPolygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
    void addPolyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
    void addPath(qreal startX, qreal startY, const QPainterPath& p, int rubberMode);
    void addHorizontalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight);
    void addVerticalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight);
    void addImage(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot);

    void addDimLeader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode);

    void setCursorShape(const QString& str);
    qreal calculateAngle(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal calculateDistance(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal perpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2);

    /* Selection Management */
    void cut();
    void copy();
    void paste();
    int numSelected();
    void selectAll();
    void addToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);
    void clearSelection();
    void deleteSelected();
    void cutSelected(qreal x, qreal y);
    void copySelected(qreal x, qreal y);
    void pasteSelected(qreal x, qreal y);
    void moveSelected(qreal dx, qreal dy);
    void scaleSelected(qreal x, qreal y, qreal factor);
    void rotateSelected(qreal x, qreal y, qreal rot);
    void mirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2);

    qreal qSnapX();
    qreal qSnapY();
    qreal mouseX();
    qreal mouseY();
};

/* Pointer access for Qt based types */
typedef struct QtScriptEnv_ {
    MainWindow *mainWin;
} QtScriptEnv;

extern QtScriptEnv script_env;

#endif

