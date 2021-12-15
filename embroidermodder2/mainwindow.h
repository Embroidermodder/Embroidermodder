#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QHash>
#include <QDir>
#include <QtScript>
#include <QtScriptTools>

#include "mdiarea.h"
#include "mdiwindow.h"
#include "mainwindow-actions.h"
#include "cmdprompt.h"

class MdiArea;
class MDIWindow;
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

    MdiArea*                        getMdiArea();
    MainWindow*                     getApplication();
    View*                           activeView();
    QGraphicsScene*                 activeScene();
    QUndoStack*                     activeUndoStack();

    virtual void                    updateMenuToolbarStatusbar();

    MainWindow*     mainWin;
    MdiArea*        mdiArea;
    CmdPrompt*      prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor*     dockUndoEdit;
    StatusBar*      statusbar;

    QList<QGraphicsItem*> cutCopyObjectList;

    QString getSettingsGeneralIconTheme()             { return settings_general_icon_theme;             }
    int     getSettingsGeneralIconSize()              { return settings_general_icon_size;              }
    bool    getSettingsGeneralMdiBGUseLogo()          { return settings_general_mdi_bg_use_logo;        }
    bool    getSettingsGeneralMdiBGUseTexture()       { return settings_general_mdi_bg_use_texture;     }
    bool    getSettingsGeneralMdiBGUseColor()         { return settings_general_mdi_bg_use_color;       }
    QString getSettingsGeneralMdiBGLogo()             { return settings_general_mdi_bg_logo;            }
    QString getSettingsGeneralMdiBGTexture()          { return settings_general_mdi_bg_texture;         }
    QRgb    getSettingsGeneralMdiBGColor()            { return settings_general_mdi_bg_color;           }
    bool    getSettingsGeneralTipOfTheDay()           { return settings_general_tip_of_the_day;         }
    int     getSettingsGeneralCurrentTip()            { return settings_general_current_tip;            }
    bool    getSettingsGeneralCheckForUpdates()       { return settings_general_check_for_updates;      }
    bool    getSettingsDisplayUseOpenGL()             { return settings_display_use_opengl;             }
    bool    getSettingsDisplayRenderHintAA()          { return settings_display_renderhint_aa;          }
    bool    getSettingsDisplayRenderHintTextAA()      { return settings_display_renderhint_text_aa;     }
    bool    getSettingsDisplayRenderHintSmoothPix()   { return settings_display_renderhint_smooth_pix;  }
    bool    getSettingsDisplayRenderHintHighAA()      { return settings_display_renderhint_high_aa;     }
    bool    getSettingsDisplayRenderHintNonCosmetic() { return settings_display_renderhint_noncosmetic; }
    bool    getSettingsDisplayShowScrollBars()        { return settings_display_show_scrollbars;        }
    int     getSettingsDisplayScrollBarWidgetNum()    { return settings_display_scrollbar_widget_num;   }
    QRgb    getSettingsDisplayCrossHairColor()        { return settings_display_crosshair_color;        }
    QRgb    getSettingsDisplayBGColor()               { return settings_display_bg_color;               }
    qreal   getSettingsDisplayZoomScaleIn()           { return settings_display_zoomscale_in;           }
    qreal   getSettingsDisplayZoomScaleOut()          { return settings_display_zoomscale_out;          }
    quint8  getSettingsDisplayCrossHairPercent()      { return settings_display_crosshair_percent;      }
    QString getSettingsDisplayUnits()                 { return settings_display_units;                  }
    QRgb    getSettingsPromptTextColor()              { return settings_prompt_text_color;              }
    QRgb    getSettingsPromptBGColor()                { return settings_prompt_bg_color;                }
    QString getSettingsPromptFontFamily()             { return settings_prompt_font_family;             }
    QString getSettingsPromptFontStyle()              { return settings_prompt_font_style;              }
    quint8  getSettingsPromptFontSize()               { return settings_prompt_font_size;               }
    bool    getSettingsPromptSaveHistory()            { return settings_prompt_save_history;            }
    bool    getSettingsPromptSaveHistoryAsHtml()      { return settings_prompt_save_history_as_html;    }
    QString getSettingsPromptSaveHistoryFilename()    { return settings_prompt_save_history_filename;   }
    QString getSettingsCustomFilter()                 { return settings_opensave_custom_filter;         }
    QString getSettingsOpenFormat()                   { return settings_opensave_open_format;           }
    bool    getSettingsOpenThumbnail()                { return settings_opensave_open_thumbnail;        }
    QString getSettingsSaveFormat()                   { return settings_opensave_save_format;           }
    bool    getSettingsSaveThumbnail()                { return settings_opensave_save_thumbnail;        }
    quint8  getSettingsRecentMaxFiles()               { return settings_opensave_recent_max_files;      }
    quint8  getSettingsOpenSaveTrimDstNumJumps()      { return settings_opensave_trim_dst_num_jumps;    }
    QString getSettingsPrintingDefaultDevice()        { return settings_printing_default_device;        }
    bool    getSettingsPrintingUseLastDevice()        { return settings_printing_use_last_device;       }
    bool    getSettingsPrintingDisableBG()            { return settings_printing_disable_bg;            }
    bool    getSettingsGridShowOnLoad()               { return settings_grid_show_on_load;              }
    bool    getSettingsGridShowOrigin()               { return settings_grid_show_origin;               }
    bool    getSettingsGridColorMatchCrossHair()      { return settings_grid_color_match_crosshair;     }
    QRgb    getSettingsGridColor()                    { return settings_grid_color;                     }
    bool    getSettingsGridLoadFromFile()             { return settings_grid_load_from_file;            }
    QString getSettingsGridType()                     { return settings_grid_type;                      }
    bool    getSettingsGridCenterOnOrigin()           { return settings_grid_center_on_origin;          }
    qreal   getSettingsGridCenterX()                  { return settings_grid_center_x;                  }
    qreal   getSettingsGridCenterY()                  { return settings_grid_center_y;                  }
    qreal   getSettingsGridSizeX()                    { return settings_grid_size_x;                    }
    qreal   getSettingsGridSizeY()                    { return settings_grid_size_y;                    }
    qreal   getSettingsGridSpacingX()                 { return settings_grid_spacing_x;                 }
    qreal   getSettingsGridSpacingY()                 { return settings_grid_spacing_y;                 }
    qreal   getSettingsGridSizeRadius()               { return settings_grid_size_radius;               }
    qreal   getSettingsGridSpacingRadius()            { return settings_grid_spacing_radius;            }
    qreal   getSettingsGridSpacingAngle()             { return settings_grid_spacing_angle;             }
    bool    getSettingsRulerShowOnLoad()              { return settings_ruler_show_on_load;             }
    bool    getSettingsRulerMetric()                  { return settings_ruler_metric;                   }
    QRgb    getSettingsRulerColor()                   { return settings_ruler_color;                    }
    quint8  getSettingsRulerPixelSize()               { return settings_ruler_pixel_size;               }
    bool    getSettingsQSnapEnabled()                 { return settings_qsnap_enabled;                  }
    QRgb    getSettingsQSnapLocatorColor()            { return settings_qsnap_locator_color;            }
    quint8  getSettingsQSnapLocatorSize()             { return settings_qsnap_locator_size;             }
    quint8  getSettingsQSnapApertureSize()            { return settings_qsnap_aperture_size;            }
    bool    getSettingsQSnapEndPoint()                { return settings_qsnap_endpoint;                 }
    bool    getSettingsQSnapMidPoint()                { return settings_qsnap_midpoint;                 }
    bool    getSettingsQSnapCenter()                  { return settings_qsnap_center;                   }
    bool    getSettingsQSnapNode()                    { return settings_qsnap_node;                     }
    bool    getSettingsQSnapQuadrant()                { return settings_qsnap_quadrant;                 }
    bool    getSettingsQSnapIntersection()            { return settings_qsnap_intersection;             }
    bool    getSettingsQSnapExtension()               { return settings_qsnap_extension;                }
    bool    getSettingsQSnapInsertion()               { return settings_qsnap_insertion;                }
    bool    getSettingsQSnapPerpendicular()           { return settings_qsnap_perpendicular;            }
    bool    getSettingsQSnapTangent()                 { return settings_qsnap_tangent;                  }
    bool    getSettingsQSnapNearest()                 { return settings_qsnap_nearest;                  }
    bool    getSettingsQSnapApparent()                { return settings_qsnap_apparent;                 }
    bool    getSettingsQSnapParallel()                { return settings_qsnap_parallel;                 }
    QRgb    getSettingsSelectionCoolGripColor()       { return settings_selection_coolgrip_color;       }
    QRgb    getSettingsSelectionHotGripColor()        { return settings_selection_hotgrip_color;        }
    quint8  getSettingsSelectionGripSize()            { return settings_selection_grip_size;            }
    quint8  getSettingsSelectionPickBoxSize()         { return settings_selection_pickbox_size;         }

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
    void setSettingsQSnapEndPoint(bool newValue)                       { settings_qsnap_endpoint                 = newValue; }
    void setSettingsQSnapMidPoint(bool newValue)                       { settings_qsnap_midpoint                 = newValue; }
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
    void setSettingsSelectionCoolGripColor(QRgb newValue)              { settings_selection_coolgrip_color       = newValue; }
    void setSettingsSelectionHotGripColor(QRgb newValue)               { settings_selection_hotgrip_color        = newValue; }
    void setSettingsSelectionGripSize(quint8 newValue)                 { settings_selection_grip_size            = newValue; }
    void setSettingsSelectionPickBoxSize(quint8 newValue)              { settings_selection_pickbox_size         = newValue; }

    QHash<int, QAction*>            actionDict;
    QHash<QString, QToolBar*>       toolbarHash;
    QHash<QString, QMenu*>          menuHash;

    bool                            isCommandActive() { return prompt->isCommandActive(); }
    QString                         activeCommand() { return prompt->activeCommand(); }

public slots:

    void                            onCloseWindow();
    virtual void                    onCloseMdiWin(MDIWindow*);

    void                            recentMenuAboutToShow();

    void                            onWindowActivated (QMdiSubWindow* w);
    void                            windowMenuAboutToShow();
    void                            windowMenuActivated( bool checked/*int id*/ );
    QAction*                        getAction(int actionEnum);

    void                            updateAllViewScrollBars(bool val);
    void                            updateAllViewCrossHairColors(QRgb color);
    void                            updateAllViewBackgroundColors(QRgb color);
    void                            updateAllViewGridColors(QRgb color);
    void                            updateAllViewRulerColors(QRgb color);

    void                            settingsDialog(const QString& showTab = QString());
    void                            readSettings();
    void                            writeSettings();

    static int                      validFileFormat(const QString &fileName);

protected:
    virtual void                    resizeEvent(QResizeEvent*);
    void                            closeEvent(QCloseEvent *event);
    QAction*                        getFileSeparator();
    QString                         fileFormatFilterString();

private:

    QString                         settings_general_icon_theme;
    int                             settings_general_icon_size;
    bool                            settings_general_mdi_bg_use_logo;
    bool                            settings_general_mdi_bg_use_texture;
    bool                            settings_general_mdi_bg_use_color;
    QString                         settings_general_mdi_bg_logo;
    QString                         settings_general_mdi_bg_texture;
    QRgb                            settings_general_mdi_bg_color;
    bool                            settings_general_tip_of_the_day;
    quint16                         settings_general_current_tip;
    bool                            settings_general_check_for_updates;
    bool                            settings_display_use_opengl;
    bool                            settings_display_renderhint_aa;
    bool                            settings_display_renderhint_text_aa;
    bool                            settings_display_renderhint_smooth_pix;
    bool                            settings_display_renderhint_high_aa;
    bool                            settings_display_renderhint_noncosmetic;
    bool                            settings_display_show_scrollbars;
    int                             settings_display_scrollbar_widget_num;
    QRgb                            settings_display_crosshair_color;
    QRgb                            settings_display_bg_color;
    qreal                           settings_display_zoomscale_in;
    qreal                           settings_display_zoomscale_out;
    quint8                          settings_display_crosshair_percent;
    QString                         settings_display_units;
    QRgb                            settings_prompt_text_color;
    QRgb                            settings_prompt_bg_color;
    QString                         settings_prompt_font_family;
    QString                         settings_prompt_font_style;
    quint8                          settings_prompt_font_size;
    bool                            settings_prompt_save_history;
    bool                            settings_prompt_save_history_as_html;
    QString                         settings_prompt_save_history_filename;
    QString                         settings_opensave_custom_filter;
    QString                         settings_opensave_open_format;
    bool                            settings_opensave_open_thumbnail;
    QString                         settings_opensave_save_format;
    bool                            settings_opensave_save_thumbnail;
    quint8                          settings_opensave_recent_max_files;
    QStringList                     settings_opensave_recent_list_of_files;
    QString                         settings_opensave_recent_directory;
    quint8                          settings_opensave_trim_dst_num_jumps;
    QString                         settings_printing_default_device;
    bool                            settings_printing_use_last_device;
    bool                            settings_printing_disable_bg;
    bool                            settings_grid_show_on_load;
    bool                            settings_grid_show_origin;
    bool                            settings_grid_color_match_crosshair;
    QRgb                            settings_grid_color;
    bool                            settings_grid_load_from_file;
    QString                         settings_grid_type;
    bool                            settings_grid_center_on_origin;
    qreal                           settings_grid_center_x;
    qreal                           settings_grid_center_y;
    qreal                           settings_grid_size_x;
    qreal                           settings_grid_size_y;
    qreal                           settings_grid_spacing_x;
    qreal                           settings_grid_spacing_y;
    qreal                           settings_grid_size_radius;
    qreal                           settings_grid_spacing_radius;
    qreal                           settings_grid_spacing_angle;
    bool                            settings_ruler_show_on_load;
    bool                            settings_ruler_metric;
    QRgb                            settings_ruler_color;
    quint8                          settings_ruler_pixel_size;
    bool                            settings_qsnap_enabled;
    QRgb                            settings_qsnap_locator_color;
    quint8                          settings_qsnap_locator_size;
    quint8                          settings_qsnap_aperture_size;
    bool                            settings_qsnap_endpoint;
    bool                            settings_qsnap_midpoint;
    bool                            settings_qsnap_center;
    bool                            settings_qsnap_node;
    bool                            settings_qsnap_quadrant;
    bool                            settings_qsnap_intersection;
    bool                            settings_qsnap_extension;
    bool                            settings_qsnap_insertion;
    bool                            settings_qsnap_perpendicular;
    bool                            settings_qsnap_tangent;
    bool                            settings_qsnap_nearest;
    bool                            settings_qsnap_apparent;
    bool                            settings_qsnap_parallel;
    QRgb                            settings_selection_coolgrip_color;
    QRgb                            settings_selection_hotgrip_color;
    quint8                          settings_selection_grip_size;
    quint8                          settings_selection_pickbox_size;

    QByteArray                      layoutState;

    int                             numOfDocs;
    int                             docIndex;

    QList<MDIWindow*>               listMdiWin;
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
    void createZoomToolbar();
    void createPanToolbar();
    void createIconToolbar();
    void createHelpToolbar();
    void createLayerToolbar();
    void createPropertiesToolbar();
    void createPromptToolbar();

    QToolBar* toolbarFile;
    QToolBar* toolbarEdit;
    QToolBar* toolbarZoom;
    QToolBar* toolbarPan;
    QToolBar* toolbarIcon;
    QToolBar* toolbarHelp;
    QToolBar* toolbarLayer;
    QToolBar* toolbarProperties;
    QToolBar* toolbarPrompt;
    //====================================================
    //Selectors
    //====================================================
    QComboBox* layerSelector;
    QComboBox* colorSelector;
    QComboBox* linetypeSelector;
    QComboBox* lineweightSelector;
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

    void runCommand();
    void runCommandMain(const QString& cmd);
    void runCommandClick(const QString& cmd,  qreal x, qreal y);
    void runCommandContext(const QString& cmd, const QString& str);
    void runCommandPrompt(const QString& cmd, const QString& str);

    void newfile();
    void openfile(bool recent = false, const QString& recentFile = "");
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

    void cut();
    void copy();
    void paste();
    void selectAll();

    void closeToolBar(QAction*);
    void floatingChangedToolBar(bool);

    void toggleGrid();
    void toggleRuler();

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

    QString getCurrentLayer();
    QRgb    getCurrentColor();
    QString getCurrentLineType();
    QString getCurrentLineWeight();

    // Standard Slots
    void undo();
    void redo();

    void deletePressed();
    void escapePressed();

    // Layer Toolbar
    void makeLayerActive();
    void layerManager();
    void layerPrevious();
    // Zoom Toolbar
    void zoomrealtime();
    void zoomprevious();
    void zoomwindow();
    void zoomdynamic();
    void zoomscale();
    void zoomcenter();
    void zoomin();
    void zoomout();
    void zoomall();
    void zoomextents();
    // Pan SubMenu
    void panrealtime();
    void panpoint();
    void panLeft();
    void panRight();
    void panUp();
    void panDown();

    void doNothing();

private:
    QScriptEngine*         engine;
    QScriptEngineDebugger* debugger;
    void                   javaInitNatives(QScriptEngine* engine);
    void                   javaLoadCommand(const QString& cmdName);

public:
    //Natives
    void nativeSetPromptPrefix        (const QString& txt);
    void nativeAppendPromptHistory    (const QString& txt);
    void nativeEndCommand             ();

    void nativeHelp                   ();

    void nativeUndo                   ();
    void nativeRedo                   ();

    void nativePanLeft                ();
    void nativePanRight               ();
    void nativePanUp                  ();
    void nativePanDown                ();

    void nativePrintArea              (qreal x, qreal y, qreal w, qreal h);

    void nativeSetTextFont            (const QString& str);
    void nativeSetTextSize            (quint32 num);
    void nativeSetTextBold            (bool val);
    void nativeSetTextItalic          (bool val);
    void nativeSetTextUnderline       (bool val);
    void nativeSetTextStrikeOut       (bool val);
    void nativeSetTextOverline        (bool val);

    void nativeVulcanize              ();
    bool nativeAllowRubber            ();
    void nativeSetRubberMode          (int mode);
    void nativeSetRubberPoint         (const QString& key, qreal x, qreal y);

    void nativeAddTextMulti           (const QString& str, qreal x, qreal y, bool fill);
    void nativeAddTextSingle          (const QString& str, qreal x, qreal y, bool fill);

    void nativeAddInfiniteLine        (qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    void nativeAddRay                 (qreal x1, qreal y1, qreal x2, qreal y2, qreal rot);
    void nativeAddLine                (qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode);
    void nativeAddTriangle            (qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill);
    void nativeAddRectangle           (qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode);
    void nativeAddRoundedRectangle    (qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill);
    void nativeAddArc                 (qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, int rubberMode);
    void nativeAddCircle              (qreal centerX, qreal centerY, qreal radius, bool fill, int rubberMode);
    void nativeAddSlot                (qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode);
    void nativeAddEllipse             (qreal centerX, qreal centerY, qreal width, qreal height, qreal rot, bool fill, int rubberMode);
    void nativeAddPoint               (qreal x, qreal y);
    void nativeAddRegularPolygon      (qreal centerX, qreal centerY, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill);
    void nativeAddPolygon             (qreal startX, qreal startY);
    void nativeAddPolyline            (qreal startX, qreal startY);
    void nativeAddPath                (qreal startX, qreal startY);
    void nativeAddHorizontalDimension (qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight);
    void nativeAddVerticalDimension   (qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight);
    void nativeAddImage               (const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot);

    void nativeAddDimLeader           (qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode);

    void  nativeSetCursorShape        (const QString& str);
    qreal nativeCalculateAngle        (qreal x1, qreal y1, qreal x2, qreal y2);
    qreal nativeCalculateDistance     (qreal x1, qreal y1, qreal x2, qreal y2);
    qreal nativePerpendicularDistance (qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2);

    void nativeSelectAll              ();
    void nativeAddToSelection         (const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativeClearSelection         ();
    void nativeDeleteSelected         ();
    void nativeCutSelected            (qreal x, qreal y);
    void nativeCopySelected           (qreal x, qreal y);
    void nativePasteSelected          (qreal x, qreal y);
    void nativeMoveSelected           (qreal dx, qreal dy);
    void nativeScaleSelected          (qreal x, qreal y, qreal factor);
    void nativeRotateSelected         (qreal x, qreal y, qreal rot);

    qreal nativeQSnapX                ();
    qreal nativeQSnapY                ();
    qreal nativeMouseX                ();
    qreal nativeMouseY                ();
};

#endif

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
