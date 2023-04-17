/**
 * \file mainwindow.h
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cinttypes>
#include <string>

#include "embroidermodder.h"

#include <QMainWindow>
#include <QList>
#include <QHash>
#include <QDir>
//#include <QtScript>
//#include <QtScriptTools>

#include "mdiarea.h"
#include "mdiwindow.h"
#include "cmdprompt.h"

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

/**
 * @brief The MainWindow class
 */
class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    MdiArea*                        getMdiArea();
    MainWindow*                     getApplication();
    MdiWindow*                      activeMdiWindow();
    View*                           activeView();
    QGraphicsScene*                 activeScene();
    QUndoStack*                     activeUndoStack();

    void setUndoCleanIcon(bool opened);

    virtual void                   updateMenuToolbarStatusbar();

    MainWindow*     mainWin;
    MdiArea*        mdiArea;
    CmdPrompt*      prompt;
    PropertyEditor* dockPropEdit;
    UndoEditor*     dockUndoEdit;
    StatusBar*      statusbar;

    QList<QGraphicsItem*> cutCopyObjectList;

    std::string actuator(std::string command);
    std::string run_script_file(std::string fname);
    std::string run_script(std::vector<std::string> script);

    QString getSettingsGeneralLanguage()              { return settings_general_language;               }
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
    bool    getSettingsGeneralSystemHelpBrowser()     { return settings_general_system_help_browser;    }
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
    QRgb    getSettingsDisplaySelectBoxLeftColor()    { return settings_display_selectbox_left_color;   }
    QRgb    getSettingsDisplaySelectBoxLeftFill()     { return settings_display_selectbox_left_fill;    }
    QRgb    getSettingsDisplaySelectBoxRightColor()   { return settings_display_selectbox_right_color;  }
    QRgb    getSettingsDisplaySelectBoxRightFill()    { return settings_display_selectbox_right_fill;   }
    uint8_t  getSettingsDisplaySelectBoxAlpha()        { return settings_display_selectbox_alpha;        }
    EmbReal   getSettingsDisplayZoomScaleIn()           { return settings_display_zoomscale_in;           }
    EmbReal   getSettingsDisplayZoomScaleOut()          { return settings_display_zoomscale_out;          }
    uint8_t  getSettingsDisplayCrossHairPercent()      { return settings_display_crosshair_percent;      }
    QString getSettingsDisplayUnits()                 { return settings_display_units;                  }
    QRgb    getSettingsPromptTextColor()              { return settings_prompt_text_color;              }
    QRgb    getSettingsPromptBGColor()                { return settings_prompt_bg_color;                }
    QString getSettingsPromptFontFamily()             { return settings_prompt_font_family;             }
    QString getSettingsPromptFontStyle()              { return settings_prompt_font_style;              }
    uint8_t  getSettingsPromptFontSize()               { return settings_prompt_font_size;               }
    bool    getSettingsPromptSaveHistory()            { return settings_prompt_save_history;            }
    bool    getSettingsPromptSaveHistoryAsHtml()      { return settings_prompt_save_history_as_html;    }
    QString getSettingsPromptSaveHistoryFilename()    { return settings_prompt_save_history_filename;   }
    QString getSettingsCustomFilter()                 { return settings_opensave_custom_filter;         }
    QString getSettingsOpenFormat()                   { return settings_opensave_open_format;           }
    bool    getSettingsOpenThumbnail()                { return settings_opensave_open_thumbnail;        }
    QString getSettingsSaveFormat()                   { return settings_opensave_save_format;           }
    bool    getSettingsSaveThumbnail()                { return settings_opensave_save_thumbnail;        }
    uint8_t  getSettingsRecentMaxFiles()               { return settings_opensave_recent_max_files;      }
    uint8_t  getSettingsOpenSaveTrimDstNumJumps()      { return settings_opensave_trim_dst_num_jumps;    }
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
    EmbReal   getSettingsGridCenterX()                  { return settings_grid_center_x;                  }
    EmbReal   getSettingsGridCenterY()                  { return settings_grid_center_y;                  }
    EmbReal   getSettingsGridSizeX()                    { return settings_grid_size_x;                    }
    EmbReal   getSettingsGridSizeY()                    { return settings_grid_size_y;                    }
    EmbReal   getSettingsGridSpacingX()                 { return settings_grid_spacing_x;                 }
    EmbReal   getSettingsGridSpacingY()                 { return settings_grid_spacing_y;                 }
    EmbReal   getSettingsGridSizeRadius()               { return settings_grid_size_radius;               }
    EmbReal   getSettingsGridSpacingRadius()            { return settings_grid_spacing_radius;            }
    EmbReal   getSettingsGridSpacingAngle()             { return settings_grid_spacing_angle;             }
    bool    getSettingsRulerShowOnLoad()              { return settings_ruler_show_on_load;             }
    bool    getSettingsRulerMetric()                  { return settings_ruler_metric;                   }
    QRgb    getSettingsRulerColor()                   { return settings_ruler_color;                    }
    uint8_t  getSettingsRulerPixelSize()               { return settings_ruler_pixel_size;               }
    bool    getSettingsQSnapEnabled()                 { return settings_qsnap_enabled;                  }
    QRgb    getSettingsQSnapLocatorColor()            { return settings_qsnap_locator_color;            }
    uint8_t  getSettingsQSnapLocatorSize()             { return settings_qsnap_locator_size;             }
    uint8_t  getSettingsQSnapApertureSize()            { return settings_qsnap_aperture_size;            }
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
    bool    getSettingsLwtShowLwt()                   { return settings_lwt_show_lwt;                   }
    bool    getSettingsLwtRealRender()                { return settings_lwt_real_render;                }
    EmbReal   getSettingsLwtDefaultLwt()                { return settings_lwt_default_lwt;                }
    bool    getSettingsSelectionModePickFirst()       { return settings_selection_mode_pickfirst;       }
    bool    getSettingsSelectionModePickAdd()         { return settings_selection_mode_pickadd;         }
    bool    getSettingsSelectionModePickDrag()        { return settings_selection_mode_pickdrag;        }
    QRgb    getSettingsSelectionCoolGripColor()       { return settings_selection_coolgrip_color;       }
    QRgb    getSettingsSelectionHotGripColor()        { return settings_selection_hotgrip_color;        }
    uint8_t  getSettingsSelectionGripSize()            { return settings_selection_grip_size;            }
    uint8_t  getSettingsSelectionPickBoxSize()         { return settings_selection_pickbox_size;         }
    QString getSettingsTextFont()                     { return settings_text_font;                      }
    EmbReal   getSettingsTextSize()                     { return settings_text_size;                      }
    EmbReal   getSettingsTextAngle()                    { return settings_text_angle;                     }
    bool    getSettingsTextStyleBold()                { return settings_text_style_bold;                }
    bool    getSettingsTextStyleItalic()              { return settings_text_style_italic;              }
    bool    getSettingsTextStyleUnderline()           { return settings_text_style_underline;           }
    bool    getSettingsTextStyleStrikeOut()           { return settings_text_style_strikeout;           }
    bool    getSettingsTextStyleOverline()            { return settings_text_style_overline;            }

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
    void setSettingsDisplaySelectBoxAlpha(uint8_t newValue)             { settings_display_selectbox_alpha        = newValue; }
    void setSettingsDisplayZoomScaleIn(EmbReal newValue)                 { settings_display_zoomscale_in           = newValue; }
    void setSettingsDisplayZoomScaleOut(EmbReal newValue)                { settings_display_zoomscale_out          = newValue; }
    void setSettingsDisplayCrossHairPercent(uint8_t newValue)           { settings_display_crosshair_percent      = newValue; }
    void setSettingsDisplayUnits(const QString& newValue)              { settings_display_units                  = newValue; }
    void setSettingsPromptTextColor(QRgb newValue)                     { settings_prompt_text_color              = newValue; }
    void setSettingsPromptBGColor(QRgb newValue)                       { settings_prompt_bg_color                = newValue; }
    void setSettingsPromptFontFamily(const QString& newValue)          { settings_prompt_font_family             = newValue; }
    void setSettingsPromptFontStyle(const QString& newValue)           { settings_prompt_font_style              = newValue; }
    void setSettingsPromptFontSize(uint8_t newValue)                    { settings_prompt_font_size               = newValue; }
    void setSettingsPromptSaveHistory(bool newValue)                   { settings_prompt_save_history            = newValue; }
    void setSettingsPromptSaveHistoryAsHtml(bool newValue)             { settings_prompt_save_history_as_html    = newValue; }
    void setSettingsPromptSaveHistoryFilename(const QString& newValue) { settings_prompt_save_history_filename   = newValue; }
    void setSettingsCustomFilter(const QString& newValue)              { settings_opensave_custom_filter         = newValue; }
    void setSettingsOpenFormat(const QString& newValue)                { settings_opensave_open_format           = newValue; }
    void setSettingsOpenThumbnail(bool newValue)                       { settings_opensave_open_thumbnail        = newValue; }
    void setSettingsSaveFormat(const QString& newValue)                { settings_opensave_save_format           = newValue; }
    void setSettingsSaveThumbnail(bool newValue)                       { settings_opensave_save_thumbnail        = newValue; }
    void setSettingsRecentMaxFiles(uint8_t newValue)                    { settings_opensave_recent_max_files      = newValue; }
    void setSettingsOpenSaveTrimDstNumJumps(uint8_t newValue)           { settings_opensave_trim_dst_num_jumps    = newValue; }
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
    void setSettingsGridCenterX(EmbReal newValue)                        { settings_grid_center_x                  = newValue; }
    void setSettingsGridCenterY(EmbReal newValue)                        { settings_grid_center_y                  = newValue; }
    void setSettingsGridSizeX(EmbReal newValue)                          { settings_grid_size_x                    = newValue; }
    void setSettingsGridSizeY(EmbReal newValue)                          { settings_grid_size_y                    = newValue; }
    void setSettingsGridSpacingX(EmbReal newValue)                       { settings_grid_spacing_x                 = newValue; }
    void setSettingsGridSpacingY(EmbReal newValue)                       { settings_grid_spacing_y                 = newValue; }
    void setSettingsGridSizeRadius(EmbReal newValue)                     { settings_grid_size_radius               = newValue; }
    void setSettingsGridSpacingRadius(EmbReal newValue)                  { settings_grid_spacing_radius            = newValue; }
    void setSettingsGridSpacingAngle(EmbReal newValue)                   { settings_grid_spacing_angle             = newValue; }
    void setSettingsRulerShowOnLoad(bool newValue)                     { settings_ruler_show_on_load             = newValue; }
    void setSettingsRulerMetric(bool newValue)                         { settings_ruler_metric                   = newValue; }
    void setSettingsRulerColor(QRgb newValue)                          { settings_ruler_color                    = newValue; }
    void setSettingsRulerPixelSize(uint8_t newValue)                    { settings_ruler_pixel_size               = newValue; }
    void setSettingsQSnapEnabled(bool newValue)                        { settings_qsnap_enabled                  = newValue; }
    void setSettingsQSnapLocatorColor(QRgb newValue)                   { settings_qsnap_locator_color            = newValue; }
    void setSettingsQSnapLocatorSize(uint8_t newValue)                  { settings_qsnap_locator_size             = newValue; }
    void setSettingsQSnapApertureSize(uint8_t newValue)                 { settings_qsnap_aperture_size            = newValue; }
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
    void setSettingsLwtShowLwt(bool newValue)                          { settings_lwt_show_lwt                   = newValue; }
    void setSettingsLwtRealRender(bool newValue)                       { settings_lwt_real_render                = newValue; }
    void setSettingsLwtDefaultLwt(EmbReal newValue)                      { settings_lwt_default_lwt                = newValue; }
    void setSettingsSelectionModePickFirst(bool newValue)              { settings_selection_mode_pickfirst       = newValue; }
    void setSettingsSelectionModePickAdd(bool newValue)                { settings_selection_mode_pickadd         = newValue; }
    void setSettingsSelectionModePickDrag(bool newValue)               { settings_selection_mode_pickdrag        = newValue; }
    void setSettingsSelectionCoolGripColor(QRgb newValue)              { settings_selection_coolgrip_color       = newValue; }
    void setSettingsSelectionHotGripColor(QRgb newValue)               { settings_selection_hotgrip_color        = newValue; }
    void setSettingsSelectionGripSize(uint8_t newValue)                 { settings_selection_grip_size            = newValue; }
    void setSettingsSelectionPickBoxSize(uint8_t newValue)              { settings_selection_pickbox_size         = newValue; }
    void setSettingsTextFont(const QString& newValue)                  { settings_text_font                      = newValue; }
    void setSettingsTextSize(EmbReal newValue)                           { settings_text_size                      = newValue; }
    void setSettingsTextAngle(EmbReal newValue)                          { settings_text_angle                     = newValue; }
    void setSettingsTextStyleBold(bool newValue)                       { settings_text_style_bold                = newValue; }
    void setSettingsTextStyleItalic(bool newValue)                     { settings_text_style_italic              = newValue; }
    void setSettingsTextStyleUnderline(bool newValue)                  { settings_text_style_underline           = newValue; }
    void setSettingsTextStyleStrikeOut(bool newValue)                  { settings_text_style_strikeout           = newValue; }
    void setSettingsTextStyleOverline(bool newValue)                   { settings_text_style_overline            = newValue; }

    QHash<int, QAction*>            actionHash;
    QHash<QString, QToolBar*>       toolbarHash;
    QHash<QString, QMenu*>          menuHash;

    QString                         formatFilterOpen;
    QString                         formatFilterSave;

    bool                            isCommandActive() { return prompt->isCommandActive(); }
    QString                         activeCommand() { return prompt->activeCommand(); }

    QString platformString();

public slots:

    void enablePromptRapidFire();
    void disablePromptRapidFire();

    void enableMoveRapidFire();
    void disableMoveRapidFire();

    void onCloseWindow();
    virtual void                   onCloseMdiWin(MdiWindow*);

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
    virtual void                   resizeEvent(QResizeEvent*);
  void closeEvent(QCloseEvent *event);
    QAction*                        getFileSeparator();
  void loadFormats();

private:

    QString settings_general_language;
    QString settings_general_icon_theme;
    int settings_general_icon_size;
    bool settings_general_mdi_bg_use_logo;
    bool settings_general_mdi_bg_use_texture;
    bool                            settings_general_mdi_bg_use_color;
    QString                         settings_general_mdi_bg_logo;
    QString                         settings_general_mdi_bg_texture;
    QRgb                            settings_general_mdi_bg_color;
    bool                            settings_general_tip_of_the_day;
    quint16                         settings_general_current_tip;
    bool                            settings_general_system_help_browser;
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
    QRgb                            settings_display_selectbox_left_color;
    QRgb                            settings_display_selectbox_left_fill;
    QRgb                            settings_display_selectbox_right_color;
    QRgb                            settings_display_selectbox_right_fill;
    uint8_t                          settings_display_selectbox_alpha;
    EmbReal                           settings_display_zoomscale_in;
    EmbReal                           settings_display_zoomscale_out;
    uint8_t                          settings_display_crosshair_percent;
    QString                         settings_display_units;
    QRgb                            settings_prompt_text_color;
    QRgb                            settings_prompt_bg_color;
    QString                         settings_prompt_font_family;
    QString                         settings_prompt_font_style;
    uint8_t                          settings_prompt_font_size;
    bool                            settings_prompt_save_history;
    bool                            settings_prompt_save_history_as_html;
    QString                         settings_prompt_save_history_filename;
    QString                         settings_opensave_custom_filter;
    QString                         settings_opensave_open_format;
    bool                            settings_opensave_open_thumbnail;
    QString                         settings_opensave_save_format;
    bool                            settings_opensave_save_thumbnail;
    uint8_t                          settings_opensave_recent_max_files;
    QStringList                     settings_opensave_recent_list_of_files;
    QString                         settings_opensave_recent_directory;
    uint8_t                          settings_opensave_trim_dst_num_jumps;
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
    EmbReal                           settings_grid_center_x;
    EmbReal                           settings_grid_center_y;
    EmbReal                           settings_grid_size_x;
    EmbReal                           settings_grid_size_y;
    EmbReal                           settings_grid_spacing_x;
    EmbReal                           settings_grid_spacing_y;
    EmbReal                           settings_grid_size_radius;
    EmbReal                           settings_grid_spacing_radius;
    EmbReal                           settings_grid_spacing_angle;
    bool                            settings_ruler_show_on_load;
    bool                            settings_ruler_metric;
    QRgb                            settings_ruler_color;
    uint8_t                          settings_ruler_pixel_size;
    bool                            settings_qsnap_enabled;
    QRgb                            settings_qsnap_locator_color;
    uint8_t                          settings_qsnap_locator_size;
    uint8_t                          settings_qsnap_aperture_size;
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
    bool                            settings_lwt_show_lwt;
    bool                            settings_lwt_real_render;
    EmbReal                           settings_lwt_default_lwt;
    bool                            settings_selection_mode_pickfirst;
    bool                            settings_selection_mode_pickadd;
    bool                            settings_selection_mode_pickdrag;
    QRgb                            settings_selection_coolgrip_color;
    QRgb                            settings_selection_hotgrip_color;
    uint8_t                          settings_selection_grip_size;
    uint8_t                          settings_selection_pickbox_size;
    QString                         settings_text_font;
    EmbReal settings_text_size;
    EmbReal settings_text_angle;
    bool                            settings_text_style_bold;
    bool                            settings_text_style_italic;
    bool                            settings_text_style_underline;
    bool                            settings_text_style_overline;
    bool                            settings_text_style_strikeout;

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

  void createAllActions();
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
    void runCommandClick(const QString& cmd,  EmbReal x, EmbReal y);
    void runCommandMove(const QString& cmd,  EmbReal x, EmbReal y);
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
    EmbReal   textSize();
    EmbReal   textAngle();
    bool    textBold();
    bool    textItalic();
    bool    textUnderline();
    bool    textStrikeOut();
    bool    textOverline();

    void setTextFont(const QString& str);
    void setTextSize(EmbReal num);
    void setTextAngle(EmbReal num);
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

    void doNothing();

private:
    QScriptEngine*         engine;
    QScriptEngineDebugger* debugger;
   void                  javaInitNatives(QScriptEngine* engine);
   void                  javaLoadCommand(const QString& cmdName);

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

    void nativeNewFile();
    void nativeOpenFile();

    void nativeExit();
    void nativeTipOfTheDay();
    void nativeWindowCascade();
    void nativeWindowTile();
    void nativeWindowClose();
    void nativeWindowCloseAll();
    void nativeWindowNext();
    void nativeWindowPrevious();

    void nativeMessageBox(const QString& type, const QString& title, const QString& text);

    void nativeUndo();
    void nativeRedo();

    void nativeIcon16();
    void nativeIcon24();
    void nativeIcon32();
    void nativeIcon48();
    void nativeIcon64();
    void nativeIcon128();

    void nativePanLeft();
    void nativePanRight();
    void nativePanUp();
    void nativePanDown();

    void nativeZoomIn();
    void nativeZoomOut();
    void nativeZoomExtents();

    void nativePrintArea(EmbReal x, EmbReal y, EmbReal w, EmbReal h);

    void nativeDayVision();
    void nativeNightVision();

    void nativeSetBackgroundColor(uint8_t r, uint8_t g, uint8_t b);
    void nativeSetCrossHairColor(uint8_t r, uint8_t g, uint8_t b);
    void nativeSetGridColor(uint8_t r, uint8_t g, uint8_t b);

    QString nativeTextFont();
    EmbReal   nativeTextSize();
    EmbReal   nativeTextAngle();
    bool    nativeTextBold();
    bool    nativeTextItalic();
    bool    nativeTextUnderline();
    bool    nativeTextStrikeOut();
    bool    nativeTextOverline();

    void nativeSetTextFont(const QString& str);
    void nativeSetTextSize(EmbReal num);
    void nativeSetTextAngle(EmbReal num);
    void nativeSetTextBold(bool val);
    void nativeSetTextItalic(bool val);
    void nativeSetTextUnderline(bool val);
    void nativeSetTextStrikeOut(bool val);
    void nativeSetTextOverline(bool val);

    void nativePreviewOn(int clone, int mode, EmbReal x, EmbReal y, EmbReal data);
    void nativePreviewOff();

    void nativeVulcanize();
    void nativeClearRubber();
    bool nativeAllowRubber();
    void nativeSpareRubber(qint64 id);
    //TODO: void nativeSetRubberFilter(qint64 id); //TODO: This is so more than 1 rubber object can exist at one time without updating all rubber objects at once
    void nativeSetRubberMode(int mode);
    void nativeSetRubberPoint(const QString& key, EmbReal x, EmbReal y);
    void nativeSetRubberText(const QString& key, const QString& txt);

    void nativeAddTextMulti(const QString& str, EmbReal x, EmbReal y, EmbReal rot, bool fill, int rubberMode);
    void nativeAddTextSingle(const QString& str, EmbReal x, EmbReal y, EmbReal rot, bool fill, int rubberMode);

    void nativeAddInfiniteLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot);
    void nativeAddRay(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot);
    void nativeAddLine(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, int rubberMode);
    void nativeAddTriangle(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal x3, EmbReal y3, EmbReal rot, bool fill);
    void nativeAddRectangle(EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot, bool fill, int rubberMode);
    void nativeAddRoundedRectangle(EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rad, EmbReal rot, bool fill);
    void nativeAddArc(EmbReal startX, EmbReal startY, EmbReal midX, EmbReal midY, EmbReal endX, EmbReal endY, int rubberMode);
    void nativeAddCircle(EmbReal centerX, EmbReal centerY, EmbReal radius, bool fill, int rubberMode);
    void nativeAddSlot(EmbReal centerX, EmbReal centerY, EmbReal diameter, EmbReal length, EmbReal rot, bool fill, int rubberMode);
    void nativeAddEllipse(EmbReal centerX, EmbReal centerY, EmbReal width, EmbReal height, EmbReal rot, bool fill, int rubberMode);
    void nativeAddPoint(EmbReal x, EmbReal y);
    void nativeAddRegularPolygon(EmbReal centerX, EmbReal centerY, quint16 sides, uint8_t mode, EmbReal rad, EmbReal rot, bool fill);
    void nativeAddPolygon(EmbReal startX, EmbReal startY, const QPainterPath& p, int rubberMode);
    void nativeAddPolyline(EmbReal startX, EmbReal startY, const QPainterPath& p, int rubberMode);
    void nativeAddPath(EmbReal startX, EmbReal startY, const QPainterPath& p, int rubberMode);
    void nativeAddHorizontalDimension(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight);
    void nativeAddVerticalDimension(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal legHeight);
    void nativeAddImage(const QString& img, EmbReal x, EmbReal y, EmbReal w, EmbReal h, EmbReal rot);

    void nativeAddDimLeader(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2, EmbReal rot, int rubberMode);

    void nativeSetCursorShape(const QString& str);
    EmbReal nativeCalculateAngle(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
    EmbReal nativeCalculateDistance(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);
    EmbReal nativePerpendicularDistance(EmbReal px, EmbReal py, EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

    int  nativeNumSelected();
    void nativeSelectAll();
    void nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode);
    void nativeClearSelection();
    void nativeDeleteSelected();
    void nativeCutSelected(EmbReal x, EmbReal y);
    void nativeCopySelected(EmbReal x, EmbReal y);
    void nativePasteSelected(EmbReal x, EmbReal y);
    void nativeMoveSelected(EmbReal dx, EmbReal dy);
    void nativeScaleSelected(EmbReal x, EmbReal y, EmbReal factor);
    void nativeRotateSelected(EmbReal x, EmbReal y, EmbReal rot);
    void nativeMirrorSelected(EmbReal x1, EmbReal y1, EmbReal x2, EmbReal y2);

    EmbReal nativeQSnapX();
    EmbReal nativeQSnapY();
    EmbReal nativeMouseX();
    EmbReal nativeMouseY();
};

MainWindow* mainWin();

#endif
