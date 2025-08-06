#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QHash>
#include <QDir>

#include "mdiarea.h"
#include "mdiwindow.h"
#include "cmdprompt.h"

#include "script.h"

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
class QScriptEngine;
class QScriptEngineDebugger;
class QScriptProgram;
class QUndoStack;
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    /* Pointer access */
    MainWindow* mainWin;
    MdiArea* mdiArea;
    CmdPrompt* prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor* dockUndoEdit;
    StatusBar* statusbar;

    MdiArea* getMdiArea();
    MainWindow* getApplication();
    MdiWindow* activeMdiWindow();
    View* activeView();
    QGraphicsScene* activeScene();
    QUndoStack* activeUndoStack();

    /* Scipting environment */
    bool script_env_boot(void);
    void script_env_free(void);
    void load_command(const QString& cmdName);

    void setUndoCleanIcon(bool opened);

    virtual void                    updateMenuToolbarStatusbar();

    QList<QGraphicsItem*> cutCopyObjectList;

    QHash<QString, QAction*> actionHash;
    QHash<QString, QToolBar*> toolbarHash;
    QHash<QString, QMenu*> menuHash;

    QString formatFilterOpen;
    QString formatFilterSave;

    bool                            isCommandActive() { return prompt->isCommandActive(); }
    QString                         activeCommand() { return prompt->activeCommand(); }

    QString platformString();

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

    void createAllActions(void);
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

    void setTextFont(const QString& str);
    void setTextSize(qreal num);
    void setTextAngle(qreal num);

    QString getCurrentLayer();
    QRgb    getCurrentColor();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    // Standard Slots
    bool isShiftPressed();
    void setShiftPressed();
    void setShiftReleased();

    void deletePressed();
    void escapePressed();

    // Layer Toolbar
    void makeLayerActive();
    void layerManager();
    void layerPrevious();

    void dayVision();
    void nightVision();

    void doNothing();

public:
    //Natives
    void nativeEndCommand             ();

    void nativeNewFile                ();
    void nativeOpenFile               ();

    void nativeExit                   ();
    void nativeHelp                   ();
    void nativeAbout                  ();
    void nativeTipOfTheDay            ();

    QString nativePlatformString      ();

    void nativeMessageBox             (const QString& type, const QString& title, const QString& text);

    void nativePrintArea              (qreal x, qreal y, qreal w, qreal h);

    void nativeDayVision              ();
    void nativeNightVision            ();

    void nativeSetBackgroundColor     (quint8 r, quint8 g, quint8 b);
    void nativeSetCrossHairColor      (quint8 r, quint8 g, quint8 b);
    void nativeSetGridColor           (quint8 r, quint8 g, quint8 b);

    QString nativeTextFont            ();
    qreal   nativeTextSize            ();
    qreal   nativeTextAngle           ();

    void nativeSetTextFont            (const QString& str);
    void nativeSetTextSize            (qreal num);
    void nativeSetTextAngle           (qreal num);

    void nativePreviewOn              (int clone, int mode, qreal x, qreal y, qreal data);
    void nativePreviewOff             ();

    void nativeVulcanize              ();
    void nativeClearRubber            ();
    bool nativeAllowRubber            ();
    void nativeSpareRubber            (qint64 id);
    //TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void nativeSetRubberMode          (int mode);
    void nativeSetRubberPoint         (const QString& key, qreal x, qreal y);
    void nativeSetRubberText          (const QString& key, const QString& txt);

    void nativeAddTextMulti           (const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode);
    void nativeAddTextSingle          (const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode);

    void nativeAddInfiniteLine        (qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    void nativeAddRay                 (qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    void nativeAddLine                (qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode);
    void nativeAddTriangle            (qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill);
    void nativeAddRectangle           (qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode);
    void nativeAddRoundedRectangle    (qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill);
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

    void  nativeSetCursorShape(const QString& str);
    qreal nativeCalculateAngle(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal nativeCalculateDistance(qreal x1, qreal y1, qreal x2, qreal y2);
    qreal nativePerpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2);

    int  nativeNumSelected();
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
};

#endif

