/* Embroidermodder 2: SCRIPTING
 *
 * Copyright (C) 2011-2025 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * -----------------------------------------------------------------------------
 *
 * This is the most frequently updated part of the source and imports all
 * of the other headers. An alteration here has a faster turn around than
 * anywhere else in the compiled part of the source. New core developers would
 * benefit most from understanding the rough approach of this file and making
 * small changes here.
 *
 * Having scripting available for users to alter their software reflects a
 * core principle of open source software. The full build environment for
 * Embroidermodder is too complex for most users to set up, especially on
 * Windows. So the "freedom" to alter the program would be seldom used.
 * Being able to write a script in any text editor and add it to
 * the build by adding a load call to "commands.scm" is therefore a core
 * feature. We as core developers won't be using this flexibility much.
 *
 * For core developers, scheme is providing a means of logging all user
 * interaction through a unified, text-based command system and a way of
 * allowing user made designs to be described parametrically along with their
 * custom UIs. While relying more on scheme would make some interactions easier to
 * write like loading settings or writing out the state for debugging (see
 * `report_state_f`), going back and forth will be slower,
 * harder to debug and potentially will lead to invalid memory more often.
 * Having every `settings_` variable part of the global C/C++ state of the
 * program rather than as scheme symbols has faster load times and easier to
 * access for over 99% of the code and run-time.
 *
 * TODO: inline all MainWindow native functions, replace MainWindow calls that
 * can be scheme registerable functions.
 */

#include "embroidery.h"

#include "mainwindow.h"
#include "view.h"
#include "statusbar.h"
#include "statusbar-button.h"
#include "imagewidget.h"
#include "layer-manager.h"
#include "object-data.h"
#include "object-arc.h"
#include "object-circle.h"
#include "object-dimleader.h"
#include "object-ellipse.h"
#include "object-image.h"
#include "object-line.h"
#include "object-path.h"
#include "object-point.h"
#include "object-polygon.h"
#include "object-polyline.h"
#include "object-rect.h"
#include "object-textsingle.h"
#include "property-editor.h"
#include "undo-editor.h"
#include "undo-commands.h"
#include "embdetails-dialog.h"

#include <stdlib.h>

#include <QLabel>
#include <QDesktopServices>
#include <QApplication>
#include <QUrl>
#include <QProcess>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMdiArea>
#include <QGraphicsScene>
#include <QComboBox>
#include <QWhatsThis>
#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMdiArea>
#include <QWidget>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QToolBar>
#include <QFileDialog>
#include <QApplication>
#include <QDate>
#include <QFileInfo>
#include <QLabel>
#include <QComboBox>
#include <QCloseEvent>
#include <QMetaObject>
#include <QLocale>

/* ---- State ------------------------------------------------------------------
 *
 * TODO: Set defaults for all state variables.
 */
scheme *root;
unsigned char context_flag = CONTEXT_MAIN;

QString settings_general_language;
QString settings_general_icon_theme;
int settings_general_icon_size;
bool settings_general_mdi_bg_use_logo;
bool settings_general_mdi_bg_use_texture;
bool settings_general_mdi_bg_use_color;
QString settings_general_mdi_bg_logo;
QString settings_general_mdi_bg_texture;
QRgb settings_general_mdi_bg_color;
bool settings_general_tip_of_the_day;
quint16 settings_general_current_tip;
bool settings_general_system_help_browser;
bool settings_general_check_for_updates;
bool settings_display_use_opengl;
bool settings_display_renderhint_aa;
bool settings_display_renderhint_text_aa;
bool settings_display_renderhint_smooth_pix;
bool settings_display_renderhint_high_aa;
bool settings_display_renderhint_noncosmetic;
bool settings_display_show_scrollbars;
int settings_display_scrollbar_widget_num;
QRgb settings_display_crosshair_color;
QRgb settings_display_bg_color;
QRgb settings_display_selectbox_left_color;
QRgb settings_display_selectbox_left_fill;
QRgb settings_display_selectbox_right_color;
QRgb settings_display_selectbox_right_fill;
quint8 settings_display_selectbox_alpha;
qreal settings_display_zoomscale_in;
qreal settings_display_zoomscale_out;
quint8 settings_display_crosshair_percent;
QString settings_display_units;
QRgb settings_prompt_text_color;
QRgb settings_prompt_bg_color;
QString settings_prompt_font_family;
QString settings_prompt_font_style;
quint8 settings_prompt_font_size;
bool settings_prompt_save_history;
bool settings_prompt_save_history_as_html;
QString settings_prompt_save_history_filename;
QString settings_opensave_custom_filter;
QString settings_opensave_open_format;
bool settings_opensave_open_thumbnail;
QString settings_opensave_save_format;
bool settings_opensave_save_thumbnail;
quint8 settings_opensave_recent_max_files;
QStringList settings_opensave_recent_list_of_files;
QString settings_opensave_recent_directory;
quint8 settings_opensave_trim_dst_num_jumps;
QString settings_printing_default_device;
bool settings_printing_use_last_device;
bool settings_printing_disable_bg;
bool settings_grid_show_on_load;
bool settings_grid_show_origin;
bool settings_grid_color_match_crosshair;
QRgb settings_grid_color;
bool settings_grid_load_from_file;
QString settings_grid_type;
bool settings_grid_center_on_origin;
qreal settings_grid_center_x;
qreal settings_grid_center_y;
qreal settings_grid_size_x;
qreal settings_grid_size_y;
qreal settings_grid_spacing_x;
qreal settings_grid_spacing_y;
qreal settings_grid_size_radius;
qreal settings_grid_spacing_radius;
qreal settings_grid_spacing_angle;
bool settings_ruler_show_on_load;
bool settings_ruler_metric;
QRgb settings_ruler_color;
quint8 settings_ruler_pixel_size;
bool settings_qsnap_enabled;
QRgb settings_qsnap_locator_color;
quint8 settings_qsnap_locator_size;
quint8 settings_qsnap_aperture_size;
bool settings_qsnap_endpoint;
bool settings_qsnap_midpoint;
bool settings_qsnap_center;
bool settings_qsnap_node;
bool settings_qsnap_quadrant;
bool settings_qsnap_intersection;
bool settings_qsnap_extension;
bool settings_qsnap_insertion;
bool settings_qsnap_perpendicular;
bool settings_qsnap_tangent;
bool settings_qsnap_nearest;
bool settings_qsnap_apparent;
bool settings_qsnap_parallel;
bool settings_lwt_show_lwt;
bool settings_lwt_real_render;
qreal settings_lwt_default_lwt;
bool settings_selection_mode_pickfirst;
bool settings_selection_mode_pickadd;
bool settings_selection_mode_pickdrag;
QRgb settings_selection_coolgrip_color;
QRgb settings_selection_hotgrip_color;
quint8 settings_selection_grip_size;
quint8 settings_selection_pickbox_size;
QString settings_text_font;
qreal settings_text_size;
qreal settings_text_angle;
bool settings_text_style_bold;
bool settings_text_style_italic;
bool settings_text_style_underline;
bool settings_text_style_overline;
bool settings_text_style_strikeout;

MainWindow* _mainWin = 0;

/* TODO: inline all get/set functions for `settings_` variables. */
QString getSettingsGeneralLanguage() { return settings_general_language; }
QString getSettingsGeneralIconTheme() { return settings_general_icon_theme; }
int     getSettingsGeneralIconSize() { return settings_general_icon_size; }
bool    getSettingsGeneralMdiBGUseLogo() { return settings_general_mdi_bg_use_logo;        }
bool    getSettingsGeneralMdiBGUseTexture() { return settings_general_mdi_bg_use_texture;     }
bool    getSettingsGeneralMdiBGUseColor() { return settings_general_mdi_bg_use_color;       }
QString getSettingsGeneralMdiBGLogo() { return settings_general_mdi_bg_logo;            }
QString getSettingsGeneralMdiBGTexture() { return settings_general_mdi_bg_texture;         }
QRgb    getSettingsGeneralMdiBGColor() { return settings_general_mdi_bg_color;           }
bool    getSettingsGeneralTipOfTheDay() { return settings_general_tip_of_the_day;         }
int     getSettingsGeneralCurrentTip() { return settings_general_current_tip;            }
bool    getSettingsGeneralSystemHelpBrowser() { return settings_general_system_help_browser;    }
bool    getSettingsGeneralCheckForUpdates() { return settings_general_check_for_updates;      }
bool    getSettingsDisplayUseOpenGL() { return settings_display_use_opengl;             }
bool    getSettingsDisplayRenderHintAA() { return settings_display_renderhint_aa;          }
bool    getSettingsDisplayRenderHintTextAA() { return settings_display_renderhint_text_aa;     }
bool    getSettingsDisplayRenderHintSmoothPix() { return settings_display_renderhint_smooth_pix;  }
bool    getSettingsDisplayRenderHintHighAA() { return settings_display_renderhint_high_aa;     }
bool    getSettingsDisplayRenderHintNonCosmetic() { return settings_display_renderhint_noncosmetic; }
bool    getSettingsDisplayShowScrollBars() { return settings_display_show_scrollbars;        }
int     getSettingsDisplayScrollBarWidgetNum() { return settings_display_scrollbar_widget_num;   }
QRgb    getSettingsDisplayCrossHairColor() { return settings_display_crosshair_color;        }
QRgb    getSettingsDisplayBGColor() { return settings_display_bg_color;               }
QRgb    getSettingsDisplaySelectBoxLeftColor() { return settings_display_selectbox_left_color;   }
QRgb    getSettingsDisplaySelectBoxLeftFill() { return settings_display_selectbox_left_fill;    }
QRgb    getSettingsDisplaySelectBoxRightColor() { return settings_display_selectbox_right_color;  }
QRgb    getSettingsDisplaySelectBoxRightFill() { return settings_display_selectbox_right_fill;   }
quint8  getSettingsDisplaySelectBoxAlpha() { return settings_display_selectbox_alpha;        }
qreal   getSettingsDisplayZoomScaleIn() { return settings_display_zoomscale_in;           }
qreal   getSettingsDisplayZoomScaleOut() { return settings_display_zoomscale_out;          }
quint8  getSettingsDisplayCrossHairPercent() { return settings_display_crosshair_percent;      }
QString getSettingsDisplayUnits() { return settings_display_units;                  }
QRgb    getSettingsPromptTextColor() { return settings_prompt_text_color;              }
QRgb    getSettingsPromptBGColor() { return settings_prompt_bg_color;                }
QString getSettingsPromptFontFamily() { return settings_prompt_font_family;             }
QString getSettingsPromptFontStyle() { return settings_prompt_font_style;              }
quint8  getSettingsPromptFontSize() { return settings_prompt_font_size;               }
bool    getSettingsPromptSaveHistory() { return settings_prompt_save_history;            }
bool    getSettingsPromptSaveHistoryAsHtml() { return settings_prompt_save_history_as_html;    }
QString getSettingsPromptSaveHistoryFilename() { return settings_prompt_save_history_filename;   }
QString getSettingsCustomFilter() { return settings_opensave_custom_filter;         }
QString getSettingsOpenFormat()   { return settings_opensave_open_format;           }
bool    getSettingsOpenThumbnail() { return settings_opensave_open_thumbnail;        }
QString getSettingsSaveFormat()   { return settings_opensave_save_format;           }
bool    getSettingsSaveThumbnail() { return settings_opensave_save_thumbnail;        }
quint8  getSettingsRecentMaxFiles() { return settings_opensave_recent_max_files;      }
quint8  getSettingsOpenSaveTrimDstNumJumps() { return settings_opensave_trim_dst_num_jumps;    }
QString getSettingsPrintingDefaultDevice() { return settings_printing_default_device;        }
bool    getSettingsPrintingUseLastDevice() { return settings_printing_use_last_device;       }
bool    getSettingsPrintingDisableBG() { return settings_printing_disable_bg;            }
bool    getSettingsGridShowOnLoad() { return settings_grid_show_on_load;              }
bool    getSettingsGridShowOrigin() { return settings_grid_show_origin;               }
bool    getSettingsGridColorMatchCrossHair() { return settings_grid_color_match_crosshair;     }
QRgb    getSettingsGridColor()    { return settings_grid_color;                     }
bool    getSettingsGridLoadFromFile() { return settings_grid_load_from_file;            }
QString getSettingsGridType()     { return settings_grid_type;                      }
bool    getSettingsGridCenterOnOrigin() { return settings_grid_center_on_origin;          }
qreal   getSettingsGridCenterX()  { return settings_grid_center_x;                  }
qreal   getSettingsGridCenterY()  { return settings_grid_center_y;                  }
qreal   getSettingsGridSizeX()    { return settings_grid_size_x;                    }
qreal   getSettingsGridSizeY()    { return settings_grid_size_y;                    }
qreal   getSettingsGridSpacingX() { return settings_grid_spacing_x;                 }
qreal   getSettingsGridSpacingY() { return settings_grid_spacing_y;                 }
qreal   getSettingsGridSizeRadius() { return settings_grid_size_radius;               }
qreal   getSettingsGridSpacingRadius() { return settings_grid_spacing_radius;            }
qreal   getSettingsGridSpacingAngle() { return settings_grid_spacing_angle;             }
bool    getSettingsRulerShowOnLoad() { return settings_ruler_show_on_load;             }
bool    getSettingsRulerMetric()  { return settings_ruler_metric;                   }
QRgb    getSettingsRulerColor()   { return settings_ruler_color;                    }
quint8  getSettingsRulerPixelSize() { return settings_ruler_pixel_size;               }
bool    getSettingsQSnapEnabled() { return settings_qsnap_enabled;                  }
QRgb    getSettingsQSnapLocatorColor() { return settings_qsnap_locator_color;            }
quint8  getSettingsQSnapLocatorSize() { return settings_qsnap_locator_size;             }
quint8  getSettingsQSnapApertureSize() { return settings_qsnap_aperture_size;            }
bool    getSettingsQSnapEndPoint() { return settings_qsnap_endpoint;                 }
bool    getSettingsQSnapMidPoint() { return settings_qsnap_midpoint;                 }
bool    getSettingsQSnapCenter()  { return settings_qsnap_center;                   }
bool    getSettingsQSnapNode()    { return settings_qsnap_node;                     }
bool    getSettingsQSnapQuadrant() { return settings_qsnap_quadrant;                 }
bool    getSettingsQSnapIntersection() { return settings_qsnap_intersection;             }
bool    getSettingsQSnapExtension() { return settings_qsnap_extension;                }
bool    getSettingsQSnapInsertion() { return settings_qsnap_insertion;                }
bool    getSettingsQSnapPerpendicular() { return settings_qsnap_perpendicular;            }
bool    getSettingsQSnapTangent() { return settings_qsnap_tangent;                  }
bool    getSettingsQSnapNearest() { return settings_qsnap_nearest;                  }
bool    getSettingsQSnapApparent() { return settings_qsnap_apparent;                 }
bool    getSettingsQSnapParallel() { return settings_qsnap_parallel;                 }
bool    getSettingsLwtShowLwt()   { return settings_lwt_show_lwt;                   }
bool    getSettingsLwtRealRender() { return settings_lwt_real_render;                }
qreal   getSettingsLwtDefaultLwt() { return settings_lwt_default_lwt;                }
bool    getSettingsSelectionModePickFirst() { return settings_selection_mode_pickfirst;       }
bool    getSettingsSelectionModePickAdd() { return settings_selection_mode_pickadd;         }
bool    getSettingsSelectionModePickDrag() { return settings_selection_mode_pickdrag;        }
QRgb    getSettingsSelectionCoolGripColor() { return settings_selection_coolgrip_color;       }
QRgb    getSettingsSelectionHotGripColor() { return settings_selection_hotgrip_color;        }
quint8  getSettingsSelectionGripSize() { return settings_selection_grip_size;            }
quint8  getSettingsSelectionPickBoxSize() { return settings_selection_pickbox_size;         }
QString getSettingsTextFont()     { return settings_text_font;                      }
qreal   getSettingsTextSize()     { return settings_text_size;                      }
qreal   getSettingsTextAngle()    { return settings_text_angle;                     }
bool    getSettingsTextStyleBold() { return settings_text_style_bold;                }
bool    getSettingsTextStyleItalic() { return settings_text_style_italic;              }
bool    getSettingsTextStyleUnderline() { return settings_text_style_underline;           }
bool    getSettingsTextStyleStrikeOut() { return settings_text_style_strikeout;           }
bool    getSettingsTextStyleOverline() { return settings_text_style_overline;            }

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
void setSettingsDisplaySelectBoxAlpha(quint8 newValue)             { settings_display_selectbox_alpha        = newValue; }
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
void setSettingsLwtShowLwt(bool newValue)                          { settings_lwt_show_lwt                   = newValue; }
void setSettingsLwtRealRender(bool newValue)                       { settings_lwt_real_render                = newValue; }
void setSettingsLwtDefaultLwt(qreal newValue)                      { settings_lwt_default_lwt                = newValue; }
void setSettingsSelectionModePickFirst(bool newValue)              { settings_selection_mode_pickfirst       = newValue; }
void setSettingsSelectionModePickAdd(bool newValue)                { settings_selection_mode_pickadd         = newValue; }
void setSettingsSelectionModePickDrag(bool newValue)               { settings_selection_mode_pickdrag        = newValue; }
void setSettingsSelectionCoolGripColor(QRgb newValue)              { settings_selection_coolgrip_color       = newValue; }
void setSettingsSelectionHotGripColor(QRgb newValue)               { settings_selection_hotgrip_color        = newValue; }
void setSettingsSelectionGripSize(quint8 newValue)                 { settings_selection_grip_size            = newValue; }
void setSettingsSelectionPickBoxSize(quint8 newValue)              { settings_selection_pickbox_size         = newValue; }
void setSettingsTextFont(const QString& newValue)                  { settings_text_font                      = newValue; }
void setSettingsTextSize(qreal newValue)                           { settings_text_size                      = newValue; }
void setSettingsTextAngle(qreal newValue)                          { settings_text_angle                     = newValue; }
void setSettingsTextStyleBold(bool newValue)                       { settings_text_style_bold                = newValue; }
void setSettingsTextStyleItalic(bool newValue)                     { settings_text_style_italic              = newValue; }
void setSettingsTextStyleUnderline(bool newValue)                  { settings_text_style_underline           = newValue; }
void setSettingsTextStyleStrikeOut(bool newValue)                  { settings_text_style_strikeout           = newValue; }
void setSettingsTextStyleOverline(bool newValue)                   { settings_text_style_overline            = newValue; }

void
stub_implement(QString txt)
{
    qDebug("TODO: %s", qPrintable(txt));
}

void MainWindow::stub_testing()
{
    QMessageBox::warning(this, tr("Testing Feature"), tr("<b>This feature is in testing.</b>"));
}

void MainWindow::exit()
{
    qDebug("exit()");
    if(getSettingsPromptSaveHistory())
    {
        prompt->saveHistory("prompt.log", getSettingsPromptSaveHistoryAsHtml()); //TODO: get filename from settings
    }
    qApp->closeAllWindows();
    this->deleteLater(); //Force the MainWindow destructor to run before exiting. Makes Valgrind "still reachable" happy :)
}

void MainWindow::quit()
{
    qDebug("quit()");
    exit();
}

void MainWindow::checkForUpdates()
{
    qDebug("checkForUpdates()");
    //TODO: Check website for new versions, commands, etc...
}

void MainWindow::cut()
{
    qDebug("cut()");
    View* gview = activeView();
    if(gview) { gview->cut(); }
}

void MainWindow::copy()
{
    qDebug("copy()");
    View* gview = activeView();
    if(gview) { gview->copy(); }
}

void MainWindow::paste()
{
    qDebug("paste()");
    View* gview = activeView();
    if(gview) { gview->paste(); }
}

void MainWindow::selectAll()
{
    qDebug("selectAll()");
    View* gview = activeView();
    if(gview) { gview->selectAll(); }
}

QString MainWindow::platformString()
{
    //TODO: Append QSysInfo to string where applicable.
    QString os;
    #if   defined(Q_OS_AIX)
    os = "AIX";
    #elif defined(Q_OS_BSD4)
    os = "BSD 4.4";
    #elif defined(Q_OS_BSDI)
    os = "BSD/OS";
    #elif defined(Q_OS_CYGWIN)
    os = "Cygwin";
    #elif defined(Q_OS_DARWIN)
    os = "Mac OS";
    #elif defined(Q_OS_DGUX)
    os = "DG/UX";
    #elif defined(Q_OS_DYNIX)
    os = "DYNIX/ptx";
    #elif defined(Q_OS_FREEBSD)
    os = "FreeBSD";
    #elif defined(Q_OS_HPUX)
    os = "HP-UX";
    #elif defined(Q_OS_HURD)
    os = "GNU Hurd";
    #elif defined(Q_OS_IRIX)
    os = "SGI Irix";
    #elif defined(Q_OS_LINUX)
    os = "Linux";
    #elif defined(Q_OS_LYNX)
    os = "LynxOS";
    #elif defined(Q_OS_MAC)
    os = "Mac OS";
    #elif defined(Q_OS_MSDOS)
    os = "MS-DOS";
    #elif defined(Q_OS_NETBSD)
    os = "NetBSD";
    #elif defined(Q_OS_OS2)
    os = "OS/2";
    #elif defined(Q_OS_OPENBSD)
    os = "OpenBSD";
    #elif defined(Q_OS_OS2EMX)
    os = "XFree86 on OS/2";
    #elif defined(Q_OS_OSF)
    os = "HP Tru64 UNIX";
    #elif defined(Q_OS_QNX)
    os = "QNX Neutrino";
    #elif defined(Q_OS_RELIANT)
    os = "Reliant UNIX";
    #elif defined(Q_OS_SCO)
    os = "SCO OpenServer 5";
    #elif defined(Q_OS_SOLARIS)
    os = "Sun Solaris";
    #elif defined(Q_OS_SYMBIAN)
    os = "Symbian";
    #elif defined(Q_OS_ULTRIX)
    os = "DEC Ultrix";
    #elif defined(Q_OS_UNIX)
    os = "UNIX BSD/SYSV";
    #elif defined(Q_OS_UNIXWARE)
    os = "UnixWare";
    #elif defined(Q_OS_WIN32)
    os = "Windows";
    #elif defined(Q_OS_WINCE)
    os = "Windows CE";
    #endif
    qDebug("Platform: %s", qPrintable(os));
    return os;
}

void MainWindow::designDetails()
{
    QGraphicsScene* scene = activeScene();
    if(scene)
    {
        EmbDetailsDialog dialog(scene, this);
        dialog.exec();
    }
}

void MainWindow::about()
{
    //TODO: QTabWidget for about dialog
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    qDebug("about()");
    QString appDir = qApp->applicationDirPath();
    QString appName = QApplication::applicationName();
    QString title = "About " + appName;

    QDialog dialog(this);
    ImageWidget img(appDir + "/images/logo-small");
    QLabel text(appName + tr("\n\n") +
                          tr("http://embroidermodder.github.io") +
                          tr("\n\n") +
                          tr("Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi") +
                          tr("\n\n") +
                          tr("Embroidery formats by Josh Varga.") +
                          tr("\n") +
                          tr("User Interface by Jonathan Greig.") +
                          tr("\n\n") +
                          tr("Free under the zlib/libpng license.")
                          #if defined(BUILD_GIT_HASH)
                          + tr("\n\n") +
                          tr("Build Hash: ") + qPrintable(BUILD_GIT_HASH)
                          #endif
                          );
    text.setWordWrap(true);

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Oh, Yeah!");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    QVBoxLayout layout;
    layout.setAlignment(Qt::AlignCenter);
    layout.addWidget(&img);
    layout.addWidget(&text);
    layout.addWidget(&buttonbox);

    dialog.setWindowTitle(title);
    dialog.setMinimumWidth(img.minimumWidth()+30);
    dialog.setMinimumHeight(img.minimumHeight()+50);
    dialog.setLayout(&layout);
    dialog.exec();
    QApplication::restoreOverrideCursor();
}

void MainWindow::whatsThisContextHelp()
{
    qDebug("whatsThisContextHelp()");
    QWhatsThis::enterWhatsThisMode();
}

void MainWindow::print()
{
    qDebug("print()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->print(); }
}

void MainWindow::tipOfTheDay()
{
    qDebug("tipOfTheDay()");

    QString appDir = qApp->applicationDirPath();

    wizardTipOfTheDay = new QWizard(this);
    wizardTipOfTheDay->setAttribute(Qt::WA_DeleteOnClose);
    wizardTipOfTheDay->setWizardStyle(QWizard::ModernStyle);
    wizardTipOfTheDay->setMinimumSize(550, 400);

    QWizardPage* page = new QWizardPage(wizardTipOfTheDay);

    ImageWidget* imgBanner = new ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay);

    if(settings_general_current_tip >= listTipOfTheDay.size())
        settings_general_current_tip = 0;
    labelTipOfTheDay = new QLabel(listTipOfTheDay.value(settings_general_current_tip), wizardTipOfTheDay);
    labelTipOfTheDay->setWordWrap(true);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("&Show tips on startup"), wizardTipOfTheDay);
    settings_general_tip_of_the_day = getSettingsGeneralTipOfTheDay();
    checkBoxTipOfTheDay->setChecked(settings_general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout(wizardTipOfTheDay);
    layout->addWidget(imgBanner);
    layout->addStrut(1);
    layout->addWidget(labelTipOfTheDay);
    layout->addStretch(1);
    layout->addWidget(checkBoxTipOfTheDay);
    page->setLayout(layout);
    wizardTipOfTheDay->addPage(page);

    wizardTipOfTheDay->setWindowTitle("Tip of the Day");

    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonPrevious)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonNext)
    //TODO: Add icons to buttons by using wizardTipOfTheDay->setButton(QWizard::CustomButton1, buttonClose)
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton1, tr("&Previous"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton2, tr("&Next"));
    wizardTipOfTheDay->setButtonText(QWizard::CustomButton3, tr("&Close"));
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton1, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton2, true);
    wizardTipOfTheDay->setOption(QWizard::HaveCustomButton3, true);
    connect(wizardTipOfTheDay, SIGNAL(customButtonClicked(int)), this, SLOT(buttonTipOfTheDayClicked(int)));

    QList<QWizard::WizardButton> listTipOfTheDayButtons;
    listTipOfTheDayButtons << QWizard::Stretch << QWizard::CustomButton1 << QWizard::CustomButton2 << QWizard::CustomButton3;
    wizardTipOfTheDay->setButtonLayout(listTipOfTheDayButtons);

    wizardTipOfTheDay->exec();
}

void MainWindow::checkBoxTipOfTheDayStateChanged(int checked)
{
    settings_general_tip_of_the_day = checked;
}

void MainWindow::buttonTipOfTheDayClicked(int button)
{
    qDebug("buttonTipOfTheDayClicked(%d)", button);
    if(button == QWizard::CustomButton1)
    {
        if(settings_general_current_tip > 0)
            settings_general_current_tip--;
        else
            settings_general_current_tip = listTipOfTheDay.size()-1;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if(button == QWizard::CustomButton2)
    {
        settings_general_current_tip++;
        if(settings_general_current_tip >= listTipOfTheDay.size())
            settings_general_current_tip = 0;
        labelTipOfTheDay->setText(listTipOfTheDay.value(settings_general_current_tip));
    }
    else if(button == QWizard::CustomButton3)
    {
        wizardTipOfTheDay->close();
    }
}

void MainWindow::help()
{
    qDebug("help()");

    // Open the HTML Help in the default browser
    QUrl helpURL("file:///" + qApp->applicationDirPath() + "/help/doc-index.html");
    QDesktopServices::openUrl(helpURL);

    //TODO: This is how to start an external program. Use this elsewhere...
    //QString program = "firefox";
    //QStringList arguments;
    //arguments << "help/commands.html";
    //QProcess *myProcess = new QProcess(this);
    //myProcess->start(program, arguments);
}

void MainWindow::changelog()
{
    qDebug("changelog()");

    QUrl changelogURL("help/changelog.html");
    QDesktopServices::openUrl(changelogURL);
}

// Standard Slots
bool MainWindow::isShiftPressed()
{
    return shiftKeyPressedState;
}

void MainWindow::setShiftPressed()
{
    shiftKeyPressedState = true;
}

void MainWindow::setShiftReleased()
{
    shiftKeyPressedState = false;
}

// Icons
void MainWindow::iconResize(int iconSize)
{
    this->setIconSize(QSize(iconSize, iconSize));
    layerSelector->     setIconSize(QSize(iconSize*4, iconSize));
    colorSelector->     setIconSize(QSize(iconSize,   iconSize));
    linetypeSelector->  setIconSize(QSize(iconSize*4, iconSize));
    lineweightSelector->setIconSize(QSize(iconSize*4, iconSize));
    //set the minimum combobox width so the text is always readable
    layerSelector->     setMinimumWidth(iconSize*4);
    colorSelector->     setMinimumWidth(iconSize*2);
    linetypeSelector->  setMinimumWidth(iconSize*4);
    lineweightSelector->setMinimumWidth(iconSize*4);

    //TODO: low-priority: open app with iconSize set to 128. resize the icons to a smaller size.

    setSettingsGeneralIconSize(iconSize);
}

MdiWindow* MainWindow::activeMdiWindow()
{
    qDebug("activeMdiWindow()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    return mdiWin;
}

View* MainWindow::activeView()
{
    qDebug("activeView()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin)
    {
        View* v = mdiWin->getView();
        return v;
    }
    return 0;
}

QGraphicsScene* MainWindow::activeScene()
{
    qDebug("activeScene()");
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin)
    {
        QGraphicsScene* s = mdiWin->getScene();
        return s;
    }
    return 0;
}

QUndoStack* MainWindow::activeUndoStack()
{
    qDebug("activeUndoStack()");
    View* v = activeView();
    if(v)
    {
        QUndoStack* u = v->getUndoStack();
        return u;
    }
    return 0;
}

void MainWindow::setUndoCleanIcon(bool opened)
{
    dockUndoEdit->updateCleanIcon(opened);
}

void MainWindow::updateAllViewScrollBars(bool val)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->showViewScrollBars(val); }
    }
}

void MainWindow::updateAllViewCrossHairColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewCrossHairColor(color); }
    }
}

void MainWindow::updateAllViewBackgroundColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewBackgroundColor(color); }
    }
}

void MainWindow::updateAllViewSelectBoxColors(QRgb colorL, QRgb fillL, QRgb colorR, QRgb fillR, int alpha)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha); }
    }
}

void MainWindow::updateAllViewGridColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewGridColor(color); }
    }
}

void MainWindow::updateAllViewRulerColors(QRgb color)
{
    QList<QMdiSubWindow*> windowList = mdiArea->subWindowList();
    for(int i = 0; i < windowList.count(); ++i)
    {
        MdiWindow* mdiWin = qobject_cast<MdiWindow*>(windowList.at(i));
        if(mdiWin) { mdiWin->setViewRulerColor(color); }
    }
}

void MainWindow::updatePickAddMode(bool val)
{
    setSettingsSelectionModePickAdd(val);
    dockPropEdit->updatePickAddModeButton(val);
}

void MainWindow::pickAddModeToggled()
{
    bool val = !getSettingsSelectionModePickAdd();
    updatePickAddMode(val);
}

// Layer ToolBar
void MainWindow::makeLayerActive()
{
    qDebug("makeLayerActive()");
    stub_implement("Implement makeLayerActive.");
}

void MainWindow::layerManager()
{
    qDebug("layerManager()");
    stub_implement("Implement layerManager.");
    LayerManager layman(this, this);
    layman.exec();
}

void MainWindow::layerPrevious()
{
    qDebug("layerPrevious()");
    stub_implement("Implement layerPrevious.");
}

void MainWindow::dayVision()
{
    View* gview = activeView();
    if(gview)
    {
        gview->setBackgroundColor(qRgb(255,255,255)); //TODO: Make day vision color settings.
        gview->setCrossHairColor(qRgb(0,0,0));        //TODO: Make day vision color settings.
        gview->setGridColor(qRgb(0,0,0));             //TODO: Make day vision color settings.
    }
}

void MainWindow::nightVision()
{
    View* gview = activeView();
    if(gview)
    {
        gview->setBackgroundColor(qRgb(0,0,0));      //TODO: Make night vision color settings.
        gview->setCrossHairColor(qRgb(255,255,255)); //TODO: Make night vision color settings.
        gview->setGridColor(qRgb(255,255,255));      //TODO: Make night vision color settings.
    }
}

void MainWindow::doNothing()
{
    //This function intentionally does nothing.
    qDebug("doNothing()");
}

void MainWindow::layerSelectorIndexChanged(int index)
{
    qDebug("layerSelectorIndexChanged(%d)", index);
}

void MainWindow::colorSelectorIndexChanged(int index)
{
    qDebug("colorSelectorIndexChanged(%d)", index);

    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb newColor;
    if(comboBox)
    {
        bool ok = 0;
        //TODO: Handle ByLayer and ByBlock and Other...
        newColor = comboBox->itemData(index).toUInt(&ok);
        if(!ok)
            QMessageBox::warning(this, tr("Color Selector Conversion Error"), tr("<b>An error has occured while changing colors.</b>"));
    }
    else
        QMessageBox::warning(this, tr("Color Selector Pointer Error"), tr("<b>An error has occured while changing colors.</b>"));

    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->currentColorChanged(newColor); }
}

void MainWindow::linetypeSelectorIndexChanged(int index)
{
    qDebug("linetypeSelectorIndexChanged(%d)", index);
}

void MainWindow::lineweightSelectorIndexChanged(int index)
{
    qDebug("lineweightSelectorIndexChanged(%d)", index);
}

void MainWindow::textFontSelectorCurrentFontChanged(const QFont& font)
{
    qDebug("textFontSelectorCurrentFontChanged()");
    setTextFont(font.family());
}

void MainWindow::textSizeSelectorIndexChanged(int index)
{
    qDebug("textSizeSelectorIndexChanged(%d)", index);
    setSettingsTextSize(qFabs(textSizeSelector->itemData(index).toReal())); //TODO: check that the toReal() conversion is ok
}

QString MainWindow::textFont()
{
    return getSettingsTextFont();
}

qreal MainWindow::textSize()
{
    return getSettingsTextSize();
}

qreal MainWindow::textAngle()
{
    return getSettingsTextAngle();
}

bool MainWindow::textBold()
{
    return getSettingsTextStyleBold();
}

bool MainWindow::textItalic()
{
    return getSettingsTextStyleItalic();
}

bool MainWindow::textUnderline()
{
    return getSettingsTextStyleUnderline();
}

bool MainWindow::textStrikeOut()
{
    return getSettingsTextStyleStrikeOut();
}

bool MainWindow::textOverline()
{
    return getSettingsTextStyleOverline();
}

void MainWindow::setTextFont(const QString& str)
{
    textFontSelector->setCurrentFont(QFont(str));
    setSettingsTextFont(str);
}

void MainWindow::setTextSize(qreal num)
{
    setSettingsTextSize(qFabs(num));
    int index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if(index != -1)
        textSizeSelector->removeItem(index);
    textSizeSelector->addItem("Custom " + QString().setNum(num, 'f', 2) + " pt", num);
    index = textSizeSelector->findText("Custom", Qt::MatchContains);
    if(index != -1)
        textSizeSelector->setCurrentIndex(index);
}

void MainWindow::setTextAngle(qreal num)
{
    setSettingsTextAngle(num);
}

void MainWindow::setTextBold(bool val)
{
    setSettingsTextStyleBold(val);
}

void MainWindow::setTextItalic(bool val)
{
    setSettingsTextStyleItalic(val);
}

void MainWindow::setTextUnderline(bool val)
{
    setSettingsTextStyleUnderline(val);
}

void MainWindow::setTextStrikeOut(bool val)
{
    setSettingsTextStyleStrikeOut(val);
}

void MainWindow::setTextOverline(bool val)
{
    setSettingsTextStyleOverline(val);
}

QString MainWindow::getCurrentLayer()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLayer(); }
    return "0";
}

QRgb MainWindow::getCurrentColor()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentColor(); }
    return 0; //TODO: return color ByLayer
}

QString MainWindow::getCurrentLineType()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLineType(); }
    return "ByLayer";
}

QString MainWindow::getCurrentLineWeight()
{
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { return mdiWin->getCurrentLineWeight(); }
    return "ByLayer";
}

void MainWindow::deletePressed()
{
    qDebug("deletePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->deletePressed(); }
    QApplication::restoreOverrideCursor();
}

void MainWindow::escapePressed()
{
    qDebug("escapePressed()");
    QApplication::setOverrideCursor(Qt::WaitCursor);
    MdiWindow* mdiWin = qobject_cast<MdiWindow*>(mdiArea->activeSubWindow());
    if(mdiWin) { mdiWin->escapePressed(); }
    QApplication::restoreOverrideCursor();

    nativeEndCommand();
}

void MainWindow::toggleGrid()
{
    qDebug("toggleGrid()");
    statusbar->statusBarGridButton->toggle();
}

void MainWindow::toggleRuler()
{
    qDebug("toggleRuler()");
    statusbar->statusBarRulerButton->toggle();
}

void MainWindow::toggleLwt()
{
    qDebug("toggleLwt()");
    statusbar->statusBarLwtButton->toggle();
}

void MainWindow::enablePromptRapidFire()
{
    prompt->enableRapidFire();
}

void MainWindow::disablePromptRapidFire()
{
    prompt->disableRapidFire();
}

void MainWindow::enableMoveRapidFire()
{
    View* gview = activeView();
    if(gview) gview->enableMoveRapidFire();
}

void MainWindow::disableMoveRapidFire()
{
    View* gview = activeView();
    if(gview) gview->disableMoveRapidFire();
}

void MainWindow::promptHistoryAppended(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptHistoryAppended(txt);
}

void MainWindow::logPromptInput(const QString& txt)
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->logPromptInput(txt);
}

void MainWindow::promptInputPrevious()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptInputPrevious();
}

void MainWindow::promptInputNext()
{
    MdiWindow* mdiWin = activeMdiWindow();
    if(mdiWin) mdiWin->promptInputNext();
}

void
run(const char *command)
{
    _mainWin->prompt->appendHistory(command);
    scheme_load_string(root, command);
}

void MainWindow::runCommand()
{
    QAction* act = qobject_cast<QAction*>(sender());
    if (act) {
        qDebug("runCommand(%s)", qPrintable(act->objectName()));
        prompt->endCommand();
        prompt->setCurrentText(act->objectName());
        prompt->processInput();
    }
}

void
MainWindow::runCommandMain(const QString& cmd)
{
    qDebug("runCommandMain(%s)", qPrintable(cmd));
    // TODO: Uncomment this line when post-selection is available
    // if (!getSettingsSelectionModePickFirst()) {
    //     clear_selection();
    // }
    context_flag = CONTEXT_MAIN;
    run(qPrintable("("+cmd+")"));
}

void
MainWindow::runCommandClick(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandClick(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    char mouse_pos[100];
    sprintf(mouse_pos, "(define mouse '(%f %f))", x, y);
    context_flag = CONTEXT_CLICK;
    scheme_load_string(root, mouse_pos);
    run(qPrintable("("+cmd+")"));
}

void
MainWindow::runCommandMove(const QString& cmd, qreal x, qreal y)
{
    qDebug("runCommandMove(%s, %.2f, %.2f)", qPrintable(cmd), x, y);
    char mouse_pos[100];
    sprintf(mouse_pos, "(define mouse '(%f %f))", x, y);
    context_flag = CONTEXT_MOVE;
    scheme_load_string(root, mouse_pos);
    run(qPrintable("("+cmd+")"));
}

void
MainWindow::runCommandContext(const QString& cmd, const QString& str)
{
    qDebug("runCommandContext(%s, %s)", qPrintable(cmd), qPrintable(str));
    char context_str[100];
    sprintf(context_str, "(define context-str \"%s\")", qPrintable(str));
    context_flag = CONTEXT_CONTEXT;
    scheme_load_string(root, context_str);
    run(qPrintable("("+cmd+")"));
}

void MainWindow::runCommandPrompt(const QString& cmd, const QString& str)
{
    qDebug("runCommandPrompt(%s, %s)", qPrintable(cmd), qPrintable(str));
    char prompt[100];
    sprintf(prompt, "(define prompt \"%s\")", qPrintable(str));
    QString safeStr = str;
    safeStr.replace("\\", "\\\\");
    safeStr.replace("\'", "\\\'");
    /*
    if (prompt->isRapidFireEnabled()) {
        engine->evaluate(cmd + "_prompt('" + safeStr + "')", fileName);
    }
    else {
        engine->evaluate(cmd + "_prompt('" + safeStr.toUpper() + "')", fileName);
    }
    */
    context_flag = CONTEXT_PROMPT;
    scheme_load_string(root, prompt);
    run(qPrintable("("+cmd+")"));
}

void MainWindow::nativeEnablePromptRapidFire()
{
    enablePromptRapidFire();
}

void MainWindow::nativeDisablePromptRapidFire()
{
    disablePromptRapidFire();
}

void MainWindow::nativeEnableMoveRapidFire()
{
    enableMoveRapidFire();
}

void MainWindow::nativeDisableMoveRapidFire()
{
    disableMoveRapidFire();
}

void MainWindow::nativeInitCommand()
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
}

void MainWindow::nativeEndCommand()
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    prompt->endCommand();
}

void MainWindow::nativeNewFile()
{
    newFile();
}

void MainWindow::nativeOpenFile()
{
    openFile();
}

void MainWindow::nativeExit()
{
    exit();
}

void MainWindow::nativeHelp()
{
    help();
}

void MainWindow::nativeAbout()
{
    about();
}

void MainWindow::nativeTipOfTheDay()
{
    tipOfTheDay();
}

QString MainWindow::nativePlatformString()
{
    return platformString();
}

void MainWindow::nativeMessageBox(const QString& type, const QString& title, const QString& text)
{
    QString msgType = type.toLower();
    if     (msgType == "critical")    { QMessageBox::critical   (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "information") { QMessageBox::information(this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "question")    { QMessageBox::question   (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else if(msgType == "warning")     { QMessageBox::warning    (this, tr(qPrintable(title)), tr(qPrintable(text))); }
    else                              { QMessageBox::critical   (this, tr("Native MessageBox Error"), tr("Incorrect use of the native messageBox function.")); }
}

void MainWindow::nativePrintArea(qreal x, qreal y, qreal w, qreal h)
{
    qDebug("nativePrintArea(%.2f, %.2f, %.2f, %.2f)", x, y, w, h);
    //TODO: Print Setup Stuff
    print();
}

void MainWindow::nativeDayVision()
{
    dayVision();
}

void MainWindow::nativeNightVision()
{
    nightVision();
}

void MainWindow::nativeSetBackgroundColor(quint8 r, quint8 g, quint8 b)
{
    setSettingsDisplayBGColor(qRgb(r,g,b));
    updateAllViewBackgroundColors(qRgb(r,g,b));
}

void MainWindow::nativeSetCrossHairColor(quint8 r, quint8 g, quint8 b)
{
    setSettingsDisplayCrossHairColor(qRgb(r,g,b));
    updateAllViewCrossHairColors(qRgb(r,g,b));
}

void MainWindow::nativeSetGridColor(quint8 r, quint8 g, quint8 b)
{
    setSettingsGridColor(qRgb(r,g,b));
    updateAllViewGridColors(qRgb(r,g,b));
}

QString MainWindow::nativeTextFont()
{
    return textFont();
}

qreal MainWindow::nativeTextSize()
{
    return textSize();
}

qreal MainWindow::nativeTextAngle()
{
    return textAngle();
}

bool MainWindow::nativeTextBold()
{
    return textBold();
}

bool MainWindow::nativeTextItalic()
{
    return textItalic();
}

bool MainWindow::nativeTextUnderline()
{
    return textUnderline();
}

bool MainWindow::nativeTextStrikeOut()
{
    return textStrikeOut();
}

bool MainWindow::nativeTextOverline()
{
    return textOverline();
}

void MainWindow::nativeSetTextFont(const QString& str)
{
    setTextFont(str);
}

void MainWindow::nativeSetTextSize(qreal num)
{
    setTextSize(num);
}

void MainWindow::nativeSetTextAngle(qreal num)
{
    setTextAngle(num);
}

void MainWindow::nativeSetTextBold(bool val)
{
    setTextBold(val);
}

void MainWindow::nativeSetTextItalic(bool val)
{
    setTextItalic(val);
}

void MainWindow::nativeSetTextUnderline(bool val)
{
    setTextUnderline(val);
}

void MainWindow::nativeSetTextStrikeOut(bool val)
{
    setTextStrikeOut(val);
}

void MainWindow::nativeSetTextOverline(bool val)
{
    setTextOverline(val);
}

void MainWindow::nativePreviewOn(int clone, int mode, qreal x, qreal y, qreal data)
{
    View* gview = activeView();
    if(gview) gview->previewOn(clone, mode, x, -y, data);
}

void MainWindow::nativePreviewOff()
{
    View* gview = activeView();
    if(gview) gview->previewOff();
}

void MainWindow::nativeVulcanize()
{
    View* gview = activeView();
    if(gview) gview->vulcanizeRubberRoom();
}

void MainWindow::nativeClearRubber()
{
    View* gview = activeView();
    if(gview) gview->clearRubberRoom();
}

bool MainWindow::nativeAllowRubber()
{
    View* gview = activeView();
    if(gview) return gview->allowRubber();
    return false;
}

void MainWindow::nativeSpareRubber(qint64 id)
{
    View* gview = activeView();
    if(gview) gview->spareRubber(id);
}

void MainWindow::nativeSetRubberMode(int mode)
{
    View* gview = activeView();
    if(gview) gview->setRubberMode(mode);
}

void MainWindow::nativeSetRubberPoint(const QString& key, qreal x, qreal y)
{
    View* gview = activeView();
    if(gview) gview->setRubberPoint(key, QPointF(x, -y));
}

void MainWindow::nativeSetRubberText(const QString& key, const QString& txt)
{
    View* gview = activeView();
    if(gview) gview->setRubberText(key, txt);
}

void MainWindow::nativeAddTextMulti(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
}

void MainWindow::nativeAddTextSingle(const QString& str, qreal x, qreal y, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        TextSingleObject* obj = new TextSingleObject(str, x, -y, getCurrentColor());
        obj->setObjectTextFont(getSettingsTextFont());
        obj->setObjectTextSize(getSettingsTextSize());
        obj->setObjectTextStyle(getSettingsTextStyleBold(),
                                getSettingsTextStyleItalic(),
                                getSettingsTextStyleUnderline(),
                                getSettingsTextStyleStrikeOut(),
                                getSettingsTextStyleOverline());
        obj->setObjectTextBackward(false);
        obj->setObjectTextUpsideDown(false);
        obj->setRotation(-rot);
        //TODO: single line text fill
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddInfiniteLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::nativeAddRay(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot)
{
}

void MainWindow::nativeAddLine(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        LineObject* obj = new LineObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddTriangle(qreal x1, qreal y1, qreal x2, qreal y2, qreal x3, qreal y3, qreal rot, bool fill)
{
}

void MainWindow::nativeAddRectangle(qreal x, qreal y, qreal w, qreal h, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        RectObject* obj = new RectObject(x, -y, w, -h, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: rect fill
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddRoundedRectangle(qreal x, qreal y, qreal w, qreal h, qreal rad, qreal rot, bool fill)
{
}

void MainWindow::nativeAddArc(qreal startX, qreal startY, qreal midX, qreal midY, qreal endX, qreal endY, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* scene = activeScene();
    if(gview && scene)
    {
        ArcObject* arcObj = new ArcObject(startX, -startY, midX, -midY, endX, -endY, getCurrentColor());
        arcObj->setObjectRubberMode(rubberMode);
        if(rubberMode) gview->addToRubberRoom(arcObj);
        scene->addItem(arcObj);
        scene->update();
    }
}

void MainWindow::nativeAddCircle(qreal centerX, qreal centerY, qreal radius, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        CircleObject* obj = new CircleObject(centerX, -centerY, radius, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        //TODO: circle fill
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddSlot(qreal centerX, qreal centerY, qreal diameter, qreal length, qreal rot, bool fill, int rubberMode)
{
    //TODO: Use UndoableAddCommand for slots
    /*
    SlotObject* slotObj = new SlotObject(centerX, -centerY, diameter, length, getCurrentColor());
    slotObj->setRotation(-rot);
    slotObj->setObjectRubberMode(rubberMode);
    if(rubberMode) gview->addToRubberRoom(slotObj);
    scene->addItem(slotObj);
    //TODO: slot fill
    scene->update();
    */
}

void MainWindow::nativeAddEllipse(qreal centerX, qreal centerY, qreal width, qreal height, qreal rot, bool fill, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        EllipseObject* obj = new EllipseObject(centerX, -centerY, width, height, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        //TODO: ellipse fill
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeAddPoint(qreal x, qreal y)
{
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && stack)
    {
        PointObject* obj = new PointObject(x, -y, getCurrentColor());
        UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
        stack->push(cmd);
    }
}

void MainWindow::nativeAddRegularPolygon(qreal centerX, qreal centerY, quint16 sides, quint8 mode, qreal rad, qreal rot, bool fill)
{
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPolygon(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolygonObject* obj = new PolygonObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPolyline(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        PolylineObject* obj = new PolylineObject(startX, startY, p, getCurrentColor());
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

//NOTE: This native is different than the rest in that the Y+ is down (scripters need not worry about this)
void MainWindow::nativeAddPath(qreal startX, qreal startY, const QPainterPath& p, int rubberMode)
{
}

void MainWindow::nativeAddHorizontalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::nativeAddVerticalDimension(qreal x1, qreal y1, qreal x2, qreal y2, qreal legHeight)
{
}

void MainWindow::nativeAddImage(const QString& img, qreal x, qreal y, qreal w, qreal h, qreal rot)
{
}

void MainWindow::nativeAddDimLeader(qreal x1, qreal y1, qreal x2, qreal y2, qreal rot, int rubberMode)
{
    View* gview = activeView();
    QGraphicsScene* gscene = gview->scene();
    QUndoStack* stack = gview->getUndoStack();
    if(gview && gscene && stack)
    {
        DimLeaderObject* obj = new DimLeaderObject(x1, -y1, x2, -y2, getCurrentColor());
        obj->setRotation(-rot);
        obj->setObjectRubberMode(rubberMode);
        if(rubberMode)
        {
            gview->addToRubberRoom(obj);
            gscene->addItem(obj);
            gscene->update();
        }
        else
        {
            UndoableAddCommand* cmd = new UndoableAddCommand(obj->data(OBJ_NAME).toString(), obj, gview, 0);
            stack->push(cmd);
        }
    }
}

void MainWindow::nativeSetCursorShape(const QString& str)
{
    View* gview = activeView();
    if(gview)
    {
        QString shape = str.toLower();
        if     (shape == "arrow")           gview->setCursor(QCursor(Qt::ArrowCursor));
        else if(shape == "uparrow")         gview->setCursor(QCursor(Qt::UpArrowCursor));
        else if(shape == "cross")           gview->setCursor(QCursor(Qt::CrossCursor));
        else if(shape == "wait")            gview->setCursor(QCursor(Qt::WaitCursor));
        else if(shape == "ibeam")           gview->setCursor(QCursor(Qt::IBeamCursor));
        else if(shape == "resizevert")      gview->setCursor(QCursor(Qt::SizeVerCursor));
        else if(shape == "resizehoriz")     gview->setCursor(QCursor(Qt::SizeHorCursor));
        else if(shape == "resizediagleft")  gview->setCursor(QCursor(Qt::SizeBDiagCursor));
        else if(shape == "resizediagright") gview->setCursor(QCursor(Qt::SizeFDiagCursor));
        else if(shape == "move")            gview->setCursor(QCursor(Qt::SizeAllCursor));
        else if(shape == "blank")           gview->setCursor(QCursor(Qt::BlankCursor));
        else if(shape == "splitvert")       gview->setCursor(QCursor(Qt::SplitVCursor));
        else if(shape == "splithoriz")      gview->setCursor(QCursor(Qt::SplitHCursor));
        else if(shape == "handpointing")    gview->setCursor(QCursor(Qt::PointingHandCursor));
        else if(shape == "forbidden")       gview->setCursor(QCursor(Qt::ForbiddenCursor));
        else if(shape == "handopen")        gview->setCursor(QCursor(Qt::OpenHandCursor));
        else if(shape == "handclosed")      gview->setCursor(QCursor(Qt::ClosedHandCursor));
        else if(shape == "whatsthis")       gview->setCursor(QCursor(Qt::WhatsThisCursor));
        else if(shape == "busy")            gview->setCursor(QCursor(Qt::BusyCursor));
        #if QT_VERSION >= 0x040700
        else if(shape == "dragmove")        gview->setCursor(QCursor(Qt::DragMoveCursor));
        else if(shape == "dragcopy")        gview->setCursor(QCursor(Qt::DragCopyCursor));
        else if(shape == "draglink")        gview->setCursor(QCursor(Qt::DragLinkCursor));
        #endif

    }
}

qreal MainWindow::nativeCalculateAngle(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return QLineF(x1, -y1, x2, -y2).angle();
}

qreal MainWindow::nativeCalculateDistance(qreal x1, qreal y1, qreal x2, qreal y2)
{
    return QLineF(x1, y1, x2, y2).length();
}

qreal MainWindow::nativePerpendicularDistance(qreal px, qreal py, qreal x1, qreal y1, qreal x2, qreal y2)
{
    QLineF line(x1, y1, x2, y2);
    QLineF norm = line.normalVector();
    qreal dx = px-x1;
    qreal dy = py-y1;
    norm.translate(dx, dy);
    QPointF iPoint;
    norm.intersects(line, &iPoint);
    return QLineF(px, py, iPoint.x(), iPoint.y()).length();
}

int MainWindow::nativeNumSelected()
{
    View* gview = activeView();
    if(gview) { return gview->numSelected(); }
    return 0;
}

void MainWindow::nativeSelectAll()
{
    View* gview = activeView();
    if(gview) { gview->selectAll(); }
}

void MainWindow::nativeAddToSelection(const QPainterPath path, Qt::ItemSelectionMode mode)
{
}

void MainWindow::nativeClearSelection()
{
    View* gview = activeView();
    if(gview) { gview->clearSelection(); }
}

void MainWindow::nativeDeleteSelected()
{
    View* gview = activeView();
    if(gview) { gview->deleteSelected(); }
}

void MainWindow::nativeCutSelected(qreal x, qreal y)
{
}

void MainWindow::nativeCopySelected(qreal x, qreal y)
{
}

void MainWindow::nativePasteSelected(qreal x, qreal y)
{
}

void MainWindow::nativeMoveSelected(qreal dx, qreal dy)
{
    View* gview = activeView();
    if (gview) {
        gview->moveSelected(dx, -dy);
    }
}

void MainWindow::nativeScaleSelected(qreal x, qreal y, qreal factor)
{
    if (factor <= 0.0) {
        QMessageBox::critical(this, tr("ScaleFactor Error"),
                                tr("Hi there. If you are not a developer, report this as a bug. "
                                "If you are a developer, your code needs examined, and possibly your head too."));
    }

    View* gview = activeView();
    if(gview) { gview->scaleSelected(x, -y, factor); }
}

void MainWindow::nativeRotateSelected(qreal x, qreal y, qreal rot)
{
    View* gview = activeView();
    if(gview) { gview->rotateSelected(x, -y, -rot); }
}

void MainWindow::nativeMirrorSelected(qreal x1, qreal y1, qreal x2, qreal y2)
{
    View* gview = activeView();
    if(gview) { gview->mirrorSelected(x1, -y1, x2, -y2); }
}

/* -- Basic wrappers of MainWindow functions -------------------------------- */
View*
activeView(void)
{
    return _mainWin->activeView();
}

QGraphicsScene*
activeScene(void)
{
    return activeView()->scene();
}

QRgb
getCurrentColor(void)
{
    return _mainWin->getCurrentColor();
}

void
debug(const char *msg)
{
    qDebug(msg);
}

void
init_command(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
    }
}

void
clear_selection(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearSelection();
    }
}

void
end_command(void)
{
    View* gview = activeView();
    if (gview) {
        gview->clearRubberRoom();
        gview->previewOff();
        gview->disableMoveRapidFire();
    }
    _mainWin->prompt->endCommand();
}

/* Warns user if they passed arguments to function that takes none. This is
 * different from missing_args because it does not block the function and
 * therefore doesn't return an error code.
 */
void
no_arg(pointer args, const char *name)
{
    if (args != root->NIL) {
        char warning[100];
        sprintf(warning,
            "WARNING: no arguments required by %s but one or more were passed.",
            name);
        debug(warning);
    }
}

/* Warns user if they didn't pass enough arguments to function that takes more.
 * Also returns an error code to allow for the function to be blocked.
 */
int
missing_args(pointer args, const char *name, int expected)
{
    pointer current = args;
    for (int i=0; i<expected; i++) {
        if (current != root->NIL) {
            char warning[100];
            sprintf(warning,
                "MISSING ARGUMENT(S) %s: expected %d arguments but only %d were supplied.",
                name, expected, i);
            debug(warning);
            return 1;
        }
        current = pair_cdr(current);
    }
    return 0;
}

/* Don't inline missing_args because some functions can have different modes of
 * operation based on the number of arguments.
 */
int
argtype_check(pointer args, const char *name, char *type_str)
{
    if (missing_args(args, name, strlen(type_str))) {
        return 0;
    }

    pointer current = args;
    for (int i=0; type_str[i]; i++) {
        switch (type_str[i]) {
        case 's': {
            if (!is_string(current)) {
                char warning[100];
                sprintf(warning,
                    "TYPE ERROR %s: argument %d is not a string.",
                    name, i);
                debug(warning);
                return 0;
            }
            break;
        }
        default:
            break;
        }
        current = pair_cdr(current);
    }
    return 1;
}

/* -- Scheme registerables --------------------------------------------------
 *
 * Try to keep this section of the file alphabetized in the function name to
 * match the declarations above.
 *
 * All function names should end in `_f`.
 */

/* Show the about dialog. */
pointer
about_f(scheme *sc, pointer args)
{
    no_arg(args, "about-f");
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    _mainWin->about();
    end_command();
    return sc->NIL;
}

/* Adds the scheme function (alert "EXAMPLE ALERT").
 */
pointer
alert_f(scheme *sc, pointer args)
{
    if (!argtype_check(args, "debug-f", "s")) {
        return sc->NIL;
    }

    pointer arg = pair_car(args);
    _mainWin->prompt->alert(string_value(arg));
    return sc->NIL;
}

/* Adds the scheme function (blink-prompt).
 */
pointer
blink_f(scheme *sc, pointer args)
{
    no_arg(args, "blink-f");
    _mainWin->prompt->startBlinking();
    return sc->NIL;
}

/* Adds the scheme function:
 *     (debug "message")
 */
pointer
debug_f(scheme* sc, pointer args)
{
    if (!argtype_check(args, "debug-f", "s")) {
        return sc->NIL;
    }

    pointer arg1 = pair_car(args);
    qDebug("%s", string_value(arg1));
    return sc->NIL;
}

/* Adds the scheme function (error "EXAMPLE ERROR").
 */
pointer
error_f(scheme *sc, pointer args)
{
    if (!argtype_check(args, "error-f", "ss")) {
        return sc->NIL;
    }

    QString strCmd = string_value(pair_car(args));
    args = pair_cdr(args);
    QString strErr = string_value(pair_car(args));

    _mainWin->prompt->setPrefix("ERROR: (" + strCmd + ") " + strErr);
    _mainWin->prompt->appendHistory(QString());
    _mainWin->nativeEndCommand();
    return sc->NIL;
}

/* Reports the entire state for debugging purposes: this means we don't have to
 * be paranoid about losing aspects of the state in scheme variables because
 * we can check the contents of state before and after a given user action
 * during testing.
 *
 * TODO: finish reporting here.
 */
pointer
report_state_f(scheme *sc, pointer args)
{
    FILE *f = fopen("state.txt", "w");
    if (!f) {
        return sc->NIL;
    }
    fprintf(f, "context_flag = %d\n", context_flag);
    fclose(f);
    return sc->NIL;
}

/* Adds the scheme function:
 *     (todo "Missing feature description")
 */
pointer
todo_f(scheme *sc, pointer args)
{
    if (!argtype_check(args, "todo-f", "ss")) {
        return sc->NIL;
    }

    QString strCmd = string_value(pair_car(args));
    args = pair_cdr(args);
    QString strTodo = string_value(pair_car(args));

    _mainWin->prompt->alert("TODO: (" + strCmd + ") " + strTodo);
    end_command();
    return sc->NIL;
}

/* . */
pointer
set_prompt_prefix_f(scheme* sc, pointer args)
{
    if (!argtype_check(args, "set-prompt-prefix-f", "s")) {
        return sc->NIL;
    }

    QString s = string_value(pair_car(args));
    _mainWin->prompt->setPrefix(s);
    return sc->NIL;
}

/* . */
pointer
changelog_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
circle_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
color_selector_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
copy_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
cut_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
day_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
design_details_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
distance_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
dolphin_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
do_nothing_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
ellipse_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
erase_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
exit_program_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
freeze_all_layers_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
heart_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
help_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
hide_all_layers_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
icon128_f(scheme *sc, pointer args)
{
    no_arg(args, "icon128-f");
    _mainWin->iconResize(128);
    return sc->NIL;
}

/* . */
pointer
icon16_f(scheme *sc, pointer args)
{
    no_arg(args, "icon16-f");
    _mainWin->iconResize(16);
    return sc->NIL;
}

/* . */
pointer
icon24_f(scheme *sc, pointer args)
{
    no_arg(args, "icon24-f");
    _mainWin->iconResize(24);
    return sc->NIL;
}

/* . */
pointer
icon32_f(scheme *sc, pointer args)
{
    no_arg(args, "icon32-f");
    _mainWin->iconResize(32);
    return sc->NIL;
}

/* . */
pointer
icon48_f(scheme *sc, pointer args)
{
    no_arg(args, "icon48-f");
    _mainWin->iconResize(48);
    return sc->NIL;
}

/* . */
pointer
icon64_f(scheme *sc, pointer args)
{
    no_arg(args, "icon64-f");
    _mainWin->iconResize(64);;
    return sc->NIL;
}

/* . */
pointer
layers_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
layer_previous_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
layer_selector_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
line_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
line_type_selector_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
line_weight_selector_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
locate_point_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
lock_all_layers_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
make_layer_current_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
move_f(scheme *sc, pointer args)
{
    return sc->NIL;
}


/* . */
pointer
new_file_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("newFile() requires zero arguments");
    }

    _mainWin->nativeNewFile();
    return sc->NIL;
}

pointer
night_vision_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
open_file_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
path_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
platform_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
point_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
polygon_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
polyline_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
quickleader_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
rectangle_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
redo_f(scheme *sc, pointer args)
{
    no_arg(args, "redo-f");
    QString prefix = _mainWin->prompt->getPrefix();
    if (_mainWin->dockUndoEdit->canRedo()) {
        _mainWin->prompt->setPrefix("Redo " + _mainWin->dockUndoEdit->redoText());
        _mainWin->prompt->appendHistory(QString());
        _mainWin->dockUndoEdit->redo();
        _mainWin->prompt->setPrefix(prefix);
    }
    else {
        _mainWin->prompt->alert("Nothing to redo");
        _mainWin->prompt->setPrefix(prefix);
    }
    return sc->NIL;
}

pointer
rgb_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
rotate_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
selectall_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
singlelinetext_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* Moves the view down. */
pointer
pan_down_f(scheme *sc, pointer args)
{
    if (args != sc->NIL) {
        debug("panDown() requires zero arguments");
    }
    if (context_flag == CONTEXT_MAIN) {
        init_command();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanDown", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return sc->NIL;
}

/* Moves the view to the left. */
pointer
pan_left_f(scheme *sc, pointer args)
{
    if (args != sc->NIL) {
        debug("pan-left-f requires zero arguments");
    }
    if (context_flag == CONTEXT_MAIN) {
        init_command();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanLeft", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return sc->NIL;
}

/* . */
pointer
pan_point_f(scheme *sc, pointer args)
{
    if (args != sc->NIL) {
        debug("pan-point-f requires zero arguments");
    }
    View* gview = activeView();
    if (gview) {
        gview->panPoint();
    }
    return sc->NIL;
}

/* . */
pointer
pan_real_time_f(scheme *sc, pointer args)
{
    if (args != sc->NIL) {
        debug("pan-point-f requires zero arguments");
    }
    View* gview = activeView();
    if (gview) {
        gview->panRealTime();
    }
    return sc->NIL;
}

/* Moves the view to the right. */
pointer
pan_right_f(scheme *sc, pointer args)
{
    if (args != sc->NIL) {
        debug("pan-right-f requires zero arguments");
    }
    if (context_flag == CONTEXT_MAIN) {
        init_command();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanRight", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return sc->NIL;
}

/* Moves the view up. */
pointer
pan_up_f(scheme *sc, pointer args)
{
    if (args != sc->NIL) {
        debug("pan-up-f requires zero arguments");
    }
    if (context_flag == CONTEXT_MAIN) {
        init_command();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("PanUp", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return sc->NIL;
}

/* . */
pointer
paste_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
print_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
save_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
saveas_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
scale_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
settings_dialog_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
show_all_layers_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
snowflake_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
star_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
syswindows_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
text_bold_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
text_italic_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
text_overline_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
text_strikeout_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
text_underline_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
thaw_all_layers_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
tip_of_the_day_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
undo_f(scheme *sc, pointer args)
{
    no_arg(args, "undo-f");
    QString prefix = _mainWin->prompt->getPrefix();
    if (_mainWin->dockUndoEdit->canUndo()) {
        _mainWin->prompt->setPrefix("Undo " + _mainWin->dockUndoEdit->undoText());
        _mainWin->prompt->appendHistory(QString());
        _mainWin->dockUndoEdit->undo();
        _mainWin->prompt->setPrefix(prefix);
    }
    else {
        _mainWin->prompt->alert("Nothing to undo");
        _mainWin->prompt->setPrefix(prefix);
    }
    return sc->NIL;
}

pointer
unlock_all_layers_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

pointer
whats_this_f(scheme *sc, pointer args)
{
    return sc->NIL;
}

/* . */
pointer
window_cascade_f(scheme *sc, pointer args)
{
    no_arg(args, "window-cascade-f");
    _mainWin->mdiArea->cascade();
    return sc->NIL;
}

/* . */
pointer
window_close_f(scheme *sc, pointer args)
{
    no_arg(args, "window-close-f");
    _mainWin->onCloseWindow();
    return sc->NIL;
}

/* . */
pointer
window_close_all_f(scheme *sc, pointer args)
{
    no_arg(args, "window-close-all-f");
    _mainWin->mdiArea->closeAllSubWindows();
    return sc->NIL;
}

/* . */
pointer
window_next_f(scheme *sc, pointer args)
{
    no_arg(args, "window-next-f");
    _mainWin->mdiArea->activateNextSubWindow();
    return sc->NIL;
}

pointer
window_previous_f(scheme *sc, pointer args)
{
    no_arg(args, "window-previous-f");
    _mainWin->mdiArea->activatePreviousSubWindow();
    return sc->NIL;
}

pointer
window_tile_f(scheme *sc, pointer args)
{
    no_arg(args, "window-tile-f");
    _mainWin->mdiArea->tile();
    return sc->NIL;
}

pointer
zoom_all_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-all-f");
    stub_implement("Implement zoomAll.");
    return sc->NIL;
}

pointer
zoom_center_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-center-f");
    stub_implement("Implement zoomCenter.");
    return sc->NIL;
}

pointer
zoom_dynamic_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-dynamic-f");
    stub_implement("Implement zoomDynamic.");
    return sc->NIL;
}

/* Zooms to display the drawing extents. */
pointer
zoom_extents_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-extents-f");
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomExtents", gview, 0);
        stack->push(cmd);
    }
    end_command();
    return sc->NIL;
}

/* Zooms to increase the apparent size of objects. */
pointer
zoom_in_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-in-f");
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    View* gview = activeView();
    if (gview) {
        gview->zoomIn();
    }
    end_command();
    return sc->NIL;
}

/* Zooms to decrease the apparent size of objects. */
pointer
zoom_out_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-out-f");
    if (context_flag == CONTEXT_MAIN) {
        init_command();
        clear_selection();
    }
    View* gview = activeView();
    if (gview) {
        gview->zoomOut();
    }
    end_command();
    return sc->NIL;
}

/* . */
pointer
zoom_scale_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-scale-f");
    stub_implement("Implement zoomScale.");
    return sc->NIL;
}

/* . */
pointer
zoom_previous_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-previous-f");
    stub_implement("Implement zoomPrevious.");
    return sc->NIL;
}

/* . */
pointer
zoom_real_time_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-real-time-f");
    stub_implement("Implement zoomRealtime.");
    return sc->NIL;
}

/* . */
pointer
zoom_selected_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-selected-f");
    View* gview = activeView();
    QUndoStack* stack = gview->getUndoStack();
    if (gview && stack) {
        UndoableNavCommand* cmd = new UndoableNavCommand("ZoomSelected", gview, 0);
        stack->push(cmd);
    }
    return sc->NIL;
}

/* . */
pointer
zoom_window_f(scheme *sc, pointer args)
{
    no_arg(args, "zoom-window-f");
    View* gview = activeView();
    if (gview) {
        gview->zoomWindow();
    }
    return sc->NIL;
}

#if 0
/* . */
pointer
append_prompt_history_f(scheme* sc, pointer args)
{
    if (args == sc->NIL) {
        _mainWin->prompt->appendHistory(QString());
        return sc->NIL;
    }
    
    _mainWin->prompt->appendHistory(string_value(arg1));
    
    return sc->NIL;
}

/* . */
pointer
EnablePromptRapidFire_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("enablePromptRapidFire() requires zero arguments");
    }

    _mainWin->nativeEnablePromptRapidFire();
    return sc->NIL;
}

/* . */
pointer
DisablePromptRapidFire_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("disablePromptRapidFire() requires zero arguments");
    }

    _mainWin->nativeDisablePromptRapidFire();
    return sc->NIL;
}

/* . */
pointer
EnableMoveRapidFire_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("enableMoveRapidFire() requires zero arguments");

    _mainWin->nativeEnableMoveRapidFire();
    return sc->NIL;
}

/* . */
pointer
DisableMoveRapidFire_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("disableMoveRapidFire() requires zero arguments");

    _mainWin->nativeDisableMoveRapidFire();
    return sc->NIL;
}

/* . */
pointer
InitCommand_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("initCommand() requires zero arguments");

    _mainWin->nativeInitCommand();
    return sc->NIL;
}

/* . */
pointer
EndCommand_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("endCommand() requires zero arguments");

    _mainWin->nativeEndCommand();
    return sc->NIL;
}

/* . */
pointer
OpenFile_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("openFile() requires zero arguments");

    _mainWin->nativeOpenFile();
    return sc->NIL;
}

/* . */
pointer
Exit_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("exit() requires zero arguments");

    _mainWin->nativeExit();
    return sc->NIL;
}

/* . */
pointer
Help_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("help() requires zero arguments");

    _mainWin->nativeHelp();
    return sc->NIL;
}

/* . */
pointer
TipOfTheDay_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("tipOfTheDay() requires zero arguments");

    _mainWin->nativeTipOfTheDay();
    return sc->NIL;
}

/* . */
pointer
PlatformString_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("platformString() requires zero arguments");
    return QScriptValue(_mainWin->nativePlatformString());
}

/* . */
pointer
MessageBox_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return debug("messageBox() requires three arguments");
    if (!context->argument(0).isString()) return debug(TypeError, "messageBox(): first argument is not a string");
    if (!context->argument(1).isString()) return debug(TypeError, "messageBox(): second argument is not a string");
    if (!context->argument(2).isString()) return debug(TypeError, "messageBox(): third argument is not a string");

    QString type  = context->argument(0).toString().toLower();
    QString title = context->argument(1).toString();
    QString text  = context->argument(2).toString();

    if (type != "critical" && type != "information" && type != "question" && type != "warning")
        return debug(UnknownError, "messageBox(): first argument must be \"critical\", \"information\", \"question\" or \"warning\".");

    _mainWin->nativeMessageBox(type, title, text);
    return sc->NIL;
}

pointer
print_area_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4) {
        return debug("printArea() requires four arguments");
    }
    if (!context->argument(0).isNumber()) return debug(TypeError, "printArea(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "printArea(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "printArea(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "printArea(): fourth argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();
    qreal w = context->argument(2).toNumber();
    qreal h = context->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x)) {
        return debug(TypeError, "printArea(): first argument failed isNaN check. There is an error in your code.");
    }
    if (qIsNaN(y)) {
        return debug(TypeError, "printArea(): second argument failed isNaN check. There is an error in your code.");
    }
    if (qIsNaN(w)) {
        return debug(TypeError, "printArea(): third argument failed isNaN check. There is an error in your code.");
    }
    if (qIsNaN(h)) {
        return debug(TypeError, "printArea(): fourth argument failed isNaN check. There is an error in your code.");
    }

    _mainWin->nativePrintArea(x, y, w, h);
    return sc->NIL;
}

pointer
day_vision_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("dayVision() requires zero arguments");

    _mainWin->nativeDayVision();
    return sc->NIL;
}

pointer
NightVision_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("nightVision() requires zero arguments");

    _mainWin->nativeNightVision();
    return sc->NIL;
}

pointer
SetBackgroundColor_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return debug("setBackgroundColor() requires three arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "setBackgroundColor(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "setBackgroundColor(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "setBackgroundColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return debug(TypeError, "setBackgroundColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return debug(TypeError, "setBackgroundColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return debug(TypeError, "setBackgroundColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return debug(UnknownError, "setBackgroundColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return debug(UnknownError, "setBackgroundColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return debug(UnknownError, "setBackgroundColor(): b value must be in range 0-255"); }

    _mainWin->nativeSetBackgroundColor(r, g, b);
    return sc->NIL;
}

pointer
SetCrossHairColor_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return debug("setCrossHairColor() requires three arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "setCrossHairColor(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "setCrossHairColor(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "setCrossHairColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return debug(TypeError, "setCrossHairColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return debug(TypeError, "setCrossHairColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return debug(TypeError, "setCrossHairColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return debug(UnknownError, "setCrossHairColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return debug(UnknownError, "setCrossHairColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return debug(UnknownError, "setCrossHairColor(): b value must be in range 0-255"); }

    _mainWin->nativeSetCrossHairColor(r, g, b);
    return sc->NIL;
}

pointer
SetGridColor_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return debug("setGridColor() requires three arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "setGridColor(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "setGridColor(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "setGridColor(): third argument is not a number");

    qreal r = context->argument(0).toNumber();
    qreal g = context->argument(1).toNumber();
    qreal b = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(r)) return debug(TypeError, "setGridColor(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(g)) return debug(TypeError, "setGridColor(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(b)) return debug(TypeError, "setGridColor(): third argument failed isNaN check. There is an error in your code.");

    if (r < 0 || r > 255) { return debug(UnknownError, "setGridColor(): r value must be in range 0-255"); }
    if (g < 0 || g > 255) { return debug(UnknownError, "setGridColor(): g value must be in range 0-255"); }
    if (b < 0 || b > 255) { return debug(UnknownError, "setGridColor(): b value must be in range 0-255"); }

    _mainWin->nativeSetGridColor(r, g, b);
    return sc->NIL;
}

pointer
TextFont_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("textFont() requires zero arguments");
    return QScriptValue(_mainWin->nativeTextFont());
}

pointer
TextSize_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("textSize() requires zero arguments");
    return QScriptValue(_mainWin->nativeTextSize());
}

pointer
TextAngle_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("textAngle() requires zero arguments");
    return QScriptValue(_mainWin->nativeTextAngle());
}

pointer
TextBold_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("textBold() requires zero arguments");
    return QScriptValue(_mainWin->nativeTextBold());
}

pointer
TextItalic_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("textItalic() requires zero arguments");
    return QScriptValue(_mainWin->nativeTextItalic());
}

pointer
TextUnderline_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("textUnderline() requires zero arguments");
    return QScriptValue(_mainWin->nativeTextUnderline());
}

pointer
TextStrikeOut_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("textStrikeOut() requires zero arguments");
    return QScriptValue(_mainWin->nativeTextStrikeOut());
}

pointer
TextOverline_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("textOverline() requires zero arguments");
    return QScriptValue(_mainWin->nativeTextOverline());
}

pointer
SetTextFont_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setTextFont() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "setTextFont(): first argument is not a string");

    _mainWin->nativeSetTextFont(context->argument(0).toString());
    return sc->NIL;
}

pointer
SetTextSize_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setTextSize() requires one argument");
    if (!context->argument(0).isNumber()) return debug(TypeError, "setTextSize(): first argument is not a number");

    qreal num = context->argument(0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return debug(TypeError, "setTextSize(): first argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeSetTextSize(num);
    return sc->NIL;
}

pointer
SetTextAngle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setTextAngle() requires one argument");
    if (!context->argument(0).isNumber()) return debug(TypeError, "setTextAngle(): first argument is not a number");

    qreal num = context->argument(0).toNumber();

    //isNaN check
    if (qIsNaN(num)) return debug(TypeError, "setTextAngle(): first argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeSetTextAngle(num);
    return sc->NIL;
}

pointer
SetTextBold_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setTextBold() requires one argument");
    if (!context->argument(0).isBool()) return debug(TypeError, "setTextBold(): first argument is not a bool");

    _mainWin->nativeSetTextBold(context->argument(0).toBool());
    return sc->NIL;
}

pointer
SetTextItalic_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setTextItalic() requires one argument");
    if (!context->argument(0).isBool()) return debug(TypeError, "setTextItalic(): first argument is not a bool");

    _mainWin->nativeSetTextItalic(context->argument(0).toBool());
    return sc->NIL;
}

pointer
SetTextUnderline_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setTextUnderline() requires one argument");
    if (!context->argument(0).isBool()) return debug(TypeError, "setTextUnderline(): first argument is not a bool");

    _mainWin->nativeSetTextUnderline(context->argument(0).toBool());
    return sc->NIL;
}

pointer
SetTextStrikeOut_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setTextStrikeOut() requires one argument");
    if (!context->argument(0).isBool()) return debug(TypeError, "setTextStrikeOut(): first argument is not a bool");

    _mainWin->nativeSetTextStrikeOut(context->argument(0).toBool());
    return sc->NIL;
}

pointer
SetTextOverline_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setTextOverline() requires one argument");
    if (!context->argument(0).isBool()) return debug(TypeError, "setTextOverline(): first argument is not a bool");

    _mainWin->nativeSetTextOverline(context->argument(0).toBool());
    return sc->NIL;
}

pointer
PreviewOn_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return debug("previewOn() requires five arguments");
    if (!context->argument(0).isString()) return debug(TypeError, "previewOn(): first argument is not a string");
    if (!context->argument(1).isString()) return debug(TypeError, "previewOn(): second argument is not a string");
    if (!context->argument(2).isNumber()) return debug(TypeError, "previewOn(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "previewOn(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "previewOn(): fifth argument is not a number");

    QString cloneStr = context->argument(0).toString().toUpper();
    QString modeStr  = context->argument(1).toString().toUpper();
    qreal x          = context->argument(2).toNumber();
    qreal y          = context->argument(3).toNumber();
    qreal data       = context->argument(4).toNumber();

    int clone = PREVIEW_CLONE_NULL;
    int mode = PREVIEW_MODE_NULL;
    if     (cloneStr == "SELECTED") { clone = PREVIEW_CLONE_SELECTED; }
    else if (cloneStr == "RUBBER")   { clone = PREVIEW_CLONE_RUBBER;   }
    else            { return debug(UnknownError, "previewOn(): first argument must be \"SELECTED\" or \"RUBBER\"."); }

    if     (modeStr == "MOVE")   { mode = PREVIEW_MODE_MOVE;   }
    else if (modeStr == "ROTATE") { mode = PREVIEW_MODE_ROTATE; }
    else if (modeStr == "SCALE")  { mode = PREVIEW_MODE_SCALE;  }
    else         { return debug(UnknownError, "previewOn(): second argument must be \"MOVE\", \"ROTATE\" or \"SCALE\"."); }

    //isNaN check
    if (qIsNaN(x))    return debug(TypeError, "previewOn(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))    return debug(TypeError, "previewOn(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(data)) return debug(TypeError, "previewOn(): fifth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativePreviewOn(clone, mode, x, y, data);
    return sc->NIL;
}

pointer
preview_off_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("previewOff() requires zero arguments");
    }

    _mainWin->nativePreviewOff();
    return sc->NIL;
}

pointer
vulcanize_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("vulcanize() requires zero arguments");
    }

    _mainWin->nativeVulcanize();
    return sc->NIL;
}

pointer
AllowRubber_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("allowRubber() requires zero arguments");

    return QScriptValue(_mainWin->nativeAllowRubber());
}

pointer
SetRubberMode_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setRubberMode() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "setRubberMode(): first argument is not a string");

    QString mode = context->argument(0).toString().toUpper();

    if     (mode == "CIRCLE_1P_RAD")                     { _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_RAD); }
    else if (mode == "CIRCLE_1P_DIA")                     { _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_1P_DIA); }
    else if (mode == "CIRCLE_2P")                         { _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_2P); }
    else if (mode == "CIRCLE_3P")                         { _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_3P); }
    else if (mode == "CIRCLE_TTR")                        { _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTR); }
    else if (mode == "CIRCLE_TTR")                        { _mainWin->nativeSetRubberMode(OBJ_RUBBER_CIRCLE_TTT); }

    else if (mode == "DIMLEADER_LINE")                    { _mainWin->nativeSetRubberMode(OBJ_RUBBER_DIMLEADER_LINE); }

    else if (mode == "ELLIPSE_LINE")                      { _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_LINE); }
    else if (mode == "ELLIPSE_MAJORDIAMETER_MINORRADIUS") { _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORDIAMETER_MINORRADIUS); }
    else if (mode == "ELLIPSE_MAJORRADIUS_MINORRADIUS")   { _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_MAJORRADIUS_MINORRADIUS); }
    else if (mode == "ELLIPSE_ROTATION")                  { _mainWin->nativeSetRubberMode(OBJ_RUBBER_ELLIPSE_ROTATION); }

    else if (mode == "LINE")                              { _mainWin->nativeSetRubberMode(OBJ_RUBBER_LINE); }

    else if (mode == "POLYGON")                           { _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON); }
    else if (mode == "POLYGON_INSCRIBE")                  { _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON_INSCRIBE); }
    else if (mode == "POLYGON_CIRCUMSCRIBE")              { _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYGON_CIRCUMSCRIBE); }

    else if (mode == "POLYLINE")                          { _mainWin->nativeSetRubberMode(OBJ_RUBBER_POLYLINE); }

    else if (mode == "RECTANGLE")                         { _mainWin->nativeSetRubberMode(OBJ_RUBBER_RECTANGLE); }

    else if (mode == "TEXTSINGLE")                        { _mainWin->nativeSetRubberMode(OBJ_RUBBER_TEXTSINGLE); }

    else                                 { return debug(UnknownError, "setRubberMode(): unknown rubberMode value"); }

    return sc->NIL;
}

pointer
SetRubberPoint_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return debug("setRubberPoint() requires three arguments");
    if (!context->argument(0).isString()) return debug(TypeError, "setRubberPoint(): first argument is not a string");
    if (!context->argument(1).isNumber()) return debug(TypeError, "setRubberPoint(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "setRubberPoint(): third argument is not a number");

    QString key = context->argument(0).toString().toUpper();
    qreal x     = context->argument(1).toNumber();
    qreal y     = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x)) return debug(TypeError, "setRubberPoint(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug(TypeError, "setRubberPoint(): third argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeSetRubberPoint(key, x, y);
    return sc->NIL;
}

pointer
SetRubberText_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return debug("setRubberText() requires two arguments");
    if (!context->argument(0).isString()) return debug(TypeError, "setRubberText(): first argument is not a string");
    if (!context->argument(1).isString()) return debug(TypeError, "setRubberText(): second argument is not a string");

    QString key = context->argument(0).toString().toUpper();
    QString txt = context->argument(1).toString();

    _mainWin->nativeSetRubberText(key, txt);
    return sc->NIL;
}

pointer
AddRubber_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("addRubber() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "addRubber(): first argument is not a string");

    QString objType = context->argument(0).toString().toUpper();

    if (!_mainWin->nativeAllowRubber())
        return debug(UnknownError, "addRubber(): You must use vulcanize() before you can add another rubber object.");

    qreal mx = _mainWin->nativeMouseX();
    qreal my = _mainWin->nativeMouseY();

    if     (objType == "ARC")          {} //TODO: handle this type
    else if (objType == "BLOCK")        {} //TODO: handle this type
    else if (objType == "CIRCLE")       { _mainWin->nativeAddCircle(mx, my, 0, false, OBJ_RUBBER_ON); }
    else if (objType == "DIMALIGNED")   {} //TODO: handle this type
    else if (objType == "DIMANGULAR")   {} //TODO: handle this type
    else if (objType == "DIMARCLENGTH") {} //TODO: handle this type
    else if (objType == "DIMDIAMETER")  {} //TODO: handle this type
    else if (objType == "DIMLEADER")    { _mainWin->nativeAddDimLeader(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "DIMLINEAR")    {} //TODO: handle this type
    else if (objType == "DIMORDINATE")  {} //TODO: handle this type
    else if (objType == "DIMRADIUS")    {} //TODO: handle this type
    else if (objType == "ELLIPSE")      { _mainWin->nativeAddEllipse(mx, my, 0, 0, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "ELLIPSEARC")   {} //TODO: handle this type
    else if (objType == "HATCH")        {} //TODO: handle this type
    else if (objType == "IMAGE")        {} //TODO: handle this type
    else if (objType == "INFINITELINE") {} //TODO: handle this type
    else if (objType == "LINE")         { _mainWin->nativeAddLine(mx, my, mx, my, 0, OBJ_RUBBER_ON); }
    else if (objType == "PATH")         {} //TODO: handle this type
    else if (objType == "POINT")        {} //TODO: handle this type
    else if (objType == "POLYGON")      { _mainWin->nativeAddPolygon(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "POLYLINE")     { _mainWin->nativeAddPolyline(mx, my, QPainterPath(), OBJ_RUBBER_ON); }
    else if (objType == "RAY")          {} //TODO: handle this type
    else if (objType == "RECTANGLE")    { _mainWin->nativeAddRectangle(mx, my, mx, my, 0, 0, OBJ_RUBBER_ON); }
    else if (objType == "SPLINE")       {} //TODO: handle this type
    else if (objType == "TEXTMULTI")    {} //TODO: handle this type
    else if (objType == "TEXTSINGLE")   { _mainWin->nativeAddTextSingle("", mx, my, 0, false, OBJ_RUBBER_ON); }

    return sc->NIL;
}

pointer
ClearRubber_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("clearRubber() requires zero arguments");

    _mainWin->nativeClearRubber();
    return sc->NIL;
}

pointer
SpareRubber_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("spareRubber() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "spareRubber(): first argument is not a string");

    QString objID = context->argument(0).toString().toUpper();

    if     (objID == "PATH")     { _mainWin->nativeSpareRubber(SPARE_RUBBER_PATH);     }
    else if (objID == "POLYGON")  { _mainWin->nativeSpareRubber(SPARE_RUBBER_POLYGON);  }
    else if (objID == "POLYLINE") { _mainWin->nativeSpareRubber(SPARE_RUBBER_POLYLINE); }
    else
    {
        bool ok = false;
        qint64 id = objID.toLongLong(&ok);
        if (!ok) return debug(TypeError, "spareRubber(): error converting object ID into an int64");
        _mainWin->nativeSpareRubber(id);
    }

    return sc->NIL;
}

pointer
AddTextMulti_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return debug("addTextMulti() requires five arguments");
    if (!context->argument(0).isString()) return debug(TypeError, "addTextMulti(): first argument is not a string");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addTextMulti(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addTextMulti(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addTextMulti(): fourth argument is not a number");
    if (!context->argument(4).isBool())   return debug(TypeError, "addTextMulti(): fifth argument is not a bool");

    QString str   = context->argument(0).toString();
    qreal   x     = context->argument(1).toNumber();
    qreal   y     = context->argument(2).toNumber();
    qreal   rot   = context->argument(3).toNumber();
    bool    fill  = context->argument(4).toBool();

    //isNaN check
    if (qIsNaN(x))   return debug(TypeError, "addTextMulti(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug(TypeError, "addTextMulti(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug(TypeError, "addTextMulti(): fourth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddTextMulti(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddTextSingle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return debug("addTextSingle() requires five arguments");
    if (!context->argument(0).isString()) return debug(TypeError, "addTextSingle(): first argument is not a string");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addTextSingle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addTextSingle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addTextSingle(): fourth argument is not a number");
    if (!context->argument(4).isBool())   return debug(TypeError, "addTextSingle(): fifth argument is not a bool");

    QString str   = context->argument(0).toString();
    qreal   x     = context->argument(1).toNumber();
    qreal   y     = context->argument(2).toNumber();
    qreal   rot   = context->argument(3).toNumber();
    bool    fill  = context->argument(4).toBool();

    //isNaN check
    if (qIsNaN(x))   return debug(TypeError, "addTextSingle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug(TypeError, "addTextSingle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug(TypeError, "addTextSingle(): fourth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddTextSingle(str, x, y, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddInfiniteLine_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addInfiniteLine command");
    return sc->NIL;
}

pointer
AddRay_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRay command");
    return sc->NIL;
}

pointer
AddLine_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return debug("addLine() requires five arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addLine(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addLine(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addLine(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addLine(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "addLine(): fifth argument is not a number");

    qreal x1  = context->argument(0).toNumber();
    qreal y1  = context->argument(1).toNumber();
    qreal x2  = context->argument(2).toNumber();
    qreal y2  = context->argument(3).toNumber();
    qreal rot = context->argument(4).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug(TypeError, "addLine(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug(TypeError, "addLine(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug(TypeError, "addLine(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug(TypeError, "addLine(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug(TypeError, "addLine(): fifth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddLine(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddTriangle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 8)    return debug("addTriangle() requires eight arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addTriangle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addTriangle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addTriangle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addTriangle(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "addTriangle(): fifth argument is not a number");
    if (!context->argument(5).isNumber()) return debug(TypeError, "addTriangle(): sixth argument is not a number");
    if (!context->argument(6).isNumber()) return debug(TypeError, "addTriangle(): seventh argument is not a number");
    if (!context->argument(7).isBool())   return debug(TypeError, "addTriangle(): eighth argument is not a bool");

    qreal x1     = context->argument(0).toNumber();
    qreal y1     = context->argument(1).toNumber();
    qreal x2     = context->argument(2).toNumber();
    qreal y2     = context->argument(3).toNumber();
    qreal x3     = context->argument(4).toNumber();
    qreal y3     = context->argument(5).toNumber();
    qreal rot    = context->argument(6).toNumber();
    bool  fill   = context->argument(7).toBool();

    //isNaN check
    if (qIsNaN(x1))  return debug(TypeError, "addTriangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug(TypeError, "addTriangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug(TypeError, "addTriangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug(TypeError, "addTriangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x3))  return debug(TypeError, "addTriangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y3))  return debug(TypeError, "addTriangle(): sixth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug(TypeError, "addTriangle(): seventh argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddTriangle(x1, y1, x2, y2, x3, y3, rot, fill);
    return sc->NIL;
}

pointer
AddRectangle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return debug("addRectangle() requires six arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addRectangle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addRectangle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addRectangle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addRectangle(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "addRectangle(): fifth argument is not a number");
    if (!context->argument(5).isBool())   return debug(TypeError, "addRectangle(): sixth argument is not a bool");

    qreal x    = context->argument(0).toNumber();
    qreal y    = context->argument(1).toNumber();
    qreal w    = context->argument(2).toNumber();
    qreal h    = context->argument(3).toNumber();
    qreal rot  = context->argument(4).toNumber();
    bool  fill = context->argument(5).toBool();

    //isNaN check
    if (qIsNaN(x))   return debug(TypeError, "addRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug(TypeError, "addRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w))   return debug(TypeError, "addRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h))   return debug(TypeError, "addRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug(TypeError, "addRectangle(): fifth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddRectangle(x, y, w, h, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddRoundedRectangle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 7)    return debug("addRoundedRectangle() requires seven arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addRoundedRectangle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addRoundedRectangle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addRoundedRectangle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addRoundedRectangle(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "addRoundedRectangle(): fifth argument is not a number");
    if (!context->argument(5).isNumber()) return debug(TypeError, "addRoundedRectangle(): sixth argument is not a number");
    if (!context->argument(6).isBool())   return debug(TypeError, "addRoundedRectangle(): seventh argument is not a bool");

    qreal x    = context->argument(0).toNumber();
    qreal y    = context->argument(1).toNumber();
    qreal w    = context->argument(2).toNumber();
    qreal h    = context->argument(3).toNumber();
    qreal rad  = context->argument(4).toNumber();
    qreal rot  = context->argument(5).toNumber();
    bool  fill = context->argument(6).toBool();

    //isNaN check
    if (qIsNaN(x))   return debug(TypeError, "addRoundedRectangle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug(TypeError, "addRoundedRectangle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(w))   return debug(TypeError, "addRoundedRectangle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(h))   return debug(TypeError, "addRoundedRectangle(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rad)) return debug(TypeError, "addRoundedRectangle(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug(TypeError, "addRoundedRectangle(): sixth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddRoundedRectangle(x, y, w, h, rad, rot, fill);
    return sc->NIL;
}

pointer
AddArc_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return debug("addArc() requires six arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addArc(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addArc(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addArc(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addArc(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "addArc(): fifth argument is not a number");
    if (!context->argument(5).isNumber()) return debug(TypeError, "addArc(): sixth argument is not a number");

    qreal startX = context->argument(0).toNumber();
    qreal startY = context->argument(1).toNumber();
    qreal midX   = context->argument(2).toNumber();
    qreal midY   = context->argument(3).toNumber();
    qreal endX   = context->argument(4).toNumber();
    qreal endY   = context->argument(5).toNumber();

    //isNaN check
    if (qIsNaN(startX)) return debug(TypeError, "addArc(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(startY)) return debug(TypeError, "addArc(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(midX))   return debug(TypeError, "addArc(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(midY))   return debug(TypeError, "addArc(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(endX))   return debug(TypeError, "addArc(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(endY))   return debug(TypeError, "addArc(): sixth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddArc(startX, startY, midX, midY, endX, endY, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddCircle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return debug("addCircle() requires four arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addCircle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addCircle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addCircle(): third argument is not a number");
    if (!context->argument(3).isBool())   return debug(TypeError, "addCircle(): fourth argument is not a bool");

    qreal centerX = context->argument(0).toNumber();
    qreal centerY = context->argument(1).toNumber();
    qreal radius  = context->argument(2).toNumber();
    bool  fill    = context->argument(3).toBool();

    //isNaN check
    if (qIsNaN(centerX)) return debug(TypeError, "addCircle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY)) return debug(TypeError, "addCircle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radius))  return debug(TypeError, "addCircle(): third argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddCircle(centerX, centerY, radius, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddSlot_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return debug("addSlot() requires six arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addSlot(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addSlot(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addSlot(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addSlot(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "addSlot(): fifth argument is not a number");
    if (!context->argument(5).isBool())   return debug(TypeError, "addSlot(): sixth argument is not a bool");

    qreal centerX  = context->argument(0).toNumber();
    qreal centerY  = context->argument(1).toNumber();
    qreal diameter = context->argument(2).toNumber();
    qreal length   = context->argument(3).toNumber();
    qreal rot      = context->argument(4).toNumber();
    bool  fill     = context->argument(5).toBool();

    //isNaN check
    if (qIsNaN(centerX))  return debug(TypeError, "addSlot(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY))  return debug(TypeError, "addSlot(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(diameter)) return debug(TypeError, "addSlot(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(length))   return debug(TypeError, "addSlot(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot))      return debug(TypeError, "addSlot(): fifth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddSlot(centerX, centerY, diameter, length, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddEllipse_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return debug("addEllipse() requires six arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addEllipse(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addEllipse(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addEllipse(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addEllipse(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "addEllipse(): fifth argument is not a number");
    if (!context->argument(5).isBool())   return debug(TypeError, "addEllipse(): sixth argument is not a bool");

    qreal centerX = context->argument(0).toNumber();
    qreal centerY = context->argument(1).toNumber();
    qreal radX    = context->argument(2).toNumber();
    qreal radY    = context->argument(3).toNumber();
    qreal rot     = context->argument(4).toNumber();
    bool  fill    = context->argument(5).toBool();

    //isNaN check
    if (qIsNaN(centerX)) return debug(TypeError, "addEllipse(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(centerY)) return debug(TypeError, "addEllipse(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radX))    return debug(TypeError, "addEllipse(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(radY))    return debug(TypeError, "addEllipse(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot))     return debug(TypeError, "addEllipse(): fifth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddEllipse(centerX, centerY, radX, radY, rot, fill, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddPoint_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return debug("addPoint() requires two arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addPoint(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addPoint(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return debug(TypeError, "addPoint(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug(TypeError, "addPoint(): second argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddPoint(x,y);
    return sc->NIL;
}

pointer
AddRegularPolygon_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addRegularPolygon command");
    return sc->NIL;
}

pointer
AddPolygon_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)   return debug("addPolygon() requires one argument");
    if (!context->argument(0).isArray()) return debug(TypeError, "addPolygon(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug(TypeError, "addPolygon(): array must contain at least two elements");
    if (varSize % 2) return debug(TypeError, "addPolygon(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if (var.canConvert(QVariant::Double))
        {
            if (xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return debug(TypeError, "addPolygon(): array contains one or more invalid elements");
    }

    //Close the polygon
    path.closeSubpath();

    path.translate(-startX, -startY);

    _mainWin->nativeAddPolygon(startX, startY, path, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddPolyline_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)   return debug("addPolyline() requires one argument");
    if (!context->argument(0).isArray()) return debug(TypeError, "addPolyline(): first argument is not an array");

    QVariantList varList = context->argument(0).toVariant().toList();
    int varSize = varList.size();
    if (varSize < 2) return debug(TypeError, "addPolyline(): array must contain at least two elements");
    if (varSize % 2) return debug(TypeError, "addPolyline(): array cannot contain an odd number of elements");

    bool lineTo = false;
    bool xCoord = true;
    qreal x = 0;
    qreal y = 0;
    qreal startX = 0;
    qreal startY = 0;
    QPainterPath path;
    foreach(QVariant var, varList)
    {
        if (var.canConvert(QVariant::Double))
        {
            if (xCoord)
            {
                xCoord = false;
                x = var.toReal();
            }
            else
            {
                xCoord = true;
                y = -var.toReal();

                if (lineTo) { path.lineTo(x,y); }
                else       { path.moveTo(x,y); lineTo = true; startX = x; startY = y; }
            }
        }
        else
            return debug(TypeError, "addPolyline(): array contains one or more invalid elements");
    }

    path.translate(-startX, -startY);

    _mainWin->nativeAddPolyline(startX, startY, path, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
AddPath_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addPath command");
    return sc->NIL;
}

pointer
AddHorizontalDimension_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addHorizontalDimension command");
    return sc->NIL;
}

pointer
AddVerticalDimension_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addVerticalDimension command");
    return sc->NIL;
}

pointer
AddImage_f(scheme* sc, pointer args)
{
    //TODO: parameter error checking
    qDebug("TODO: finish addImage command");
    return sc->NIL;
}

pointer
AddDimLeader_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 5)    return debug("addDimLeader() requires five arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "addDimLeader(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "addDimLeader(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "addDimLeader(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "addDimLeader(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "addDimLeader(): fifth argument is not a number");

    qreal x1  = context->argument(0).toNumber();
    qreal y1  = context->argument(1).toNumber();
    qreal x2  = context->argument(2).toNumber();
    qreal y2  = context->argument(3).toNumber();
    qreal rot = context->argument(4).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug(TypeError, "addDimLeader(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug(TypeError, "addDimLeader(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug(TypeError, "addDimLeader(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug(TypeError, "addDimLeader(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug(TypeError, "addDimLeader(): fifth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeAddDimLeader(x1, y1, x2, y2, rot, OBJ_RUBBER_OFF);
    return sc->NIL;
}

pointer
SetCursorShape_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 1)    return debug("setCursorShape() requires one argument");
    if (!context->argument(0).isString()) return debug(TypeError, "setCursorShape(): first argument is not a string");

    QString shape = context->argument(0).toString();
    _mainWin->nativeSetCursorShape(shape);
    return sc->NIL;
}

pointer
CalculateAngle_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return debug("calculateAngle() requires four arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "calculateAngle(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "calculateAngle(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "calculateAngle(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "calculateAngle(): fourth argument is not a number");

    qreal x1 = context->argument(0).toNumber();
    qreal y1 = context->argument(1).toNumber();
    qreal x2 = context->argument(2).toNumber();
    qreal y2 = context->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug(TypeError, "calculateAngle(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug(TypeError, "calculateAngle(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug(TypeError, "calculateAngle(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug(TypeError, "calculateAngle(): fourth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(_mainWin->nativeCalculateAngle(x1, y1, x2, y2));
}

pointer
CalculateDistance_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return debug("calculateDistance() requires four arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "calculateDistance(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "calculateDistance(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "calculateDistance(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "calculateDistance(): fourth argument is not a number");

    qreal x1 = context->argument(0).toNumber();
    qreal y1 = context->argument(1).toNumber();
    qreal x2 = context->argument(2).toNumber();
    qreal y2 = context->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug(TypeError, "calculateDistance(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug(TypeError, "calculateDistance(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug(TypeError, "calculateDistance(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug(TypeError, "calculateDistance(): fourth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(_mainWin->nativeCalculateDistance(x1, y1, x2, y2));
}

pointer
PerpendicularDistance_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 6)    return debug("perpendicularDistance() requires six arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "perpendicularDistance(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "perpendicularDistance(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "perpendicularDistance(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "perpendicularDistance(): fourth argument is not a number");
    if (!context->argument(4).isNumber()) return debug(TypeError, "perpendicularDistance(): fifth argument is not a number");
    if (!context->argument(5).isNumber()) return debug(TypeError, "perpendicularDistance(): sixth argument is not a number");

    qreal px = context->argument(0).toNumber();
    qreal py = context->argument(1).toNumber();
    qreal x1 = context->argument(2).toNumber();
    qreal y1 = context->argument(3).toNumber();
    qreal x2 = context->argument(4).toNumber();
    qreal y2 = context->argument(5).toNumber();

    //isNaN check
    if (qIsNaN(px))  return debug(TypeError, "perpendicularDistance(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(py))  return debug(TypeError, "perpendicularDistance(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x1))  return debug(TypeError, "perpendicularDistance(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug(TypeError, "perpendicularDistance(): fourth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug(TypeError, "perpendicularDistance(): fifth argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug(TypeError, "perpendicularDistance(): sixth argument failed isNaN check. There is an error in your code.");

    return QScriptValue(_mainWin->nativePerpendicularDistance(px, py, x1, y1, x2, y2));
}

pointer
NumSelected_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("numSelected() requires zero arguments");
    return QScriptValue(_mainWin->nativeNumSelected());
}

pointer
SelectAll_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("selectAll() requires zero arguments");

    _mainWin->nativeSelectAll();
    return sc->NIL;
}

pointer
AddToSelection_f(scheme* sc, pointer args)
{
    //TODO: finish
    return sc->NIL;
}

pointer
ClearSelection_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("clearSelection() requires zero arguments");

    _mainWin->nativeClearSelection();
    return sc->NIL;
}

pointer
DeleteSelected_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) return debug("deleteSelected() requires zero arguments");

    _mainWin->nativeDeleteSelected();
    return sc->NIL;
}

pointer
CutSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return debug("cutSelected() requires two arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "cutSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "cutSelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return debug(TypeError, "cutSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug(TypeError, "cutSelected(): second argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeCutSelected(x, y);
    return sc->NIL;
}

pointer
CopySelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return debug("copySelected() requires two arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "copySelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "copySelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return debug(TypeError, "copySelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug(TypeError, "copySelected(): second argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeCopySelected(x, y);
    return sc->NIL;
}

pointer
PasteSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return debug("pasteSelected() requires two arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "pasteSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "pasteSelected(): second argument is not a number");

    qreal x = context->argument(0).toNumber();
    qreal y = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(x)) return debug(TypeError, "pasteSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y)) return debug(TypeError, "pasteSelected(): second argument failed isNaN check. There is an error in your code.");

    _mainWin->nativePasteSelected(x, y);
    return sc->NIL;
}

pointer
MoveSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 2)    return debug("moveSelected() requires two arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "moveSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "moveSelected(): second argument is not a number");

    qreal dx = context->argument(0).toNumber();
    qreal dy = context->argument(1).toNumber();

    //isNaN check
    if (qIsNaN(dx)) return debug(TypeError, "moveSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(dy)) return debug(TypeError, "moveSelected(): second argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeMoveSelected(dx, dy);
    return sc->NIL;
}

pointer
ScaleSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return debug("scaleSelected() requires three arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "scaleSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "scaleSelected(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "scaleSelected(): third argument is not a number");

    qreal x      = context->argument(0).toNumber();
    qreal y      = context->argument(1).toNumber();
    qreal factor = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x))      return debug(TypeError, "scaleSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))      return debug(TypeError, "scaleSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(factor)) return debug(TypeError, "scaleSelected(): third argument failed isNaN check. There is an error in your code.");

    if (factor <= 0.0) return debug(UnknownError, "scaleSelected(): scale factor must be greater than zero");

    _mainWin->nativeScaleSelected(x, y, factor);
    return sc->NIL;
}

pointer
RotateSelected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 3)    return debug("rotateSelected() requires three arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "rotateSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "rotateSelected(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "rotateSelected(): third argument is not a number");

    qreal x   = context->argument(0).toNumber();
    qreal y   = context->argument(1).toNumber();
    qreal rot = context->argument(2).toNumber();

    //isNaN check
    if (qIsNaN(x))   return debug(TypeError, "rotateSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y))   return debug(TypeError, "rotateSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(rot)) return debug(TypeError, "rotateSelected(): third argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeRotateSelected(x, y, rot);
    return sc->NIL;
}

pointer
mirror_selected_f(scheme* sc, pointer args)
{
    if (context->argumentCount() != 4)    return debug("mirrorSelected() requires four arguments");
    if (!context->argument(0).isNumber()) return debug(TypeError, "mirrorSelected(): first argument is not a number");
    if (!context->argument(1).isNumber()) return debug(TypeError, "mirrorSelected(): second argument is not a number");
    if (!context->argument(2).isNumber()) return debug(TypeError, "mirrorSelected(): third argument is not a number");
    if (!context->argument(3).isNumber()) return debug(TypeError, "mirrorSelected(): fourth argument is not a number");

    qreal x1 = context->argument(0).toNumber();
    qreal y1 = context->argument(1).toNumber();
    qreal x2 = context->argument(2).toNumber();
    qreal y2 = context->argument(3).toNumber();

    //isNaN check
    if (qIsNaN(x1))  return debug(TypeError, "mirrorSelected(): first argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y1))  return debug(TypeError, "mirrorSelected(): second argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(x2))  return debug(TypeError, "mirrorSelected(): third argument failed isNaN check. There is an error in your code.");
    if (qIsNaN(y2))  return debug(TypeError, "mirrorSelected(): fourth argument failed isNaN check. There is an error in your code.");

    _mainWin->nativeMirrorSelected(x1, y1, x2, y2);
    return sc->NIL;
}
#endif

/* Report the current x-position of the mouse and return it. */
pointer
mouse_x_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("mouseX() requires zero arguments");
    }
    QGraphicsScene* scene = activeScene();
    double result = 0.0;
    if (scene) {
        qDebug("mouseX: %.50f", scene->property(SCENE_MOUSE_POINT).toPointF().x());
        result = scene->property(SCENE_MOUSE_POINT).toPointF().x();
    }
    return mk_real(sc, result);
}

/* Report the current y-position of the mouse and return it. */
pointer
mouse_y_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("mouseY() requires zero arguments");
    }
    QGraphicsScene* scene = activeScene();
    double result = 0.0;
    if (scene) {
        qDebug("mouseY: %.50f", -scene->property(SCENE_MOUSE_POINT).toPointF().y());
        result = -scene->property(SCENE_MOUSE_POINT).toPointF().y();
    }
    return mk_real(sc, result);
}

/* Return the current x-position of the quicksnap position. */
pointer
qsnap_x_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("qsnapX() requires zero arguments");
    }
    QGraphicsScene* scene = activeScene();
    double result = 0.0;
    if (scene) {
        result = scene->property(SCENE_QSNAP_POINT).toPointF().x();
    }
    return mk_real(sc, result);
}

/* Return the current y-position of the quicksnap position. */
pointer
qsnap_y_f(scheme* sc, pointer args)
{
    if (args != sc->NIL) {
        debug("qsnapY() requires zero arguments");
    }
    QGraphicsScene* scene = activeScene();
    double result = 0.0;
    if (scene) {
        result = -scene->property(SCENE_QSNAP_POINT).toPointF().y();
    }
    return mk_real(sc, result);
}

/* Scheme in Embroidermodder 2 uses a 3 stage boot process.
 *
 * 1. Standard scheme only functions that are used by scripts like `cddr`.
 * 2. Built-in style hookups to Embroidermodder2 features which are
 *    `scheme_registerable`s.
 * 3. Commands written in scheme only which represent the QActions used by
 *    the interface in the menus, toolbars and command line.
 *
 * Each command loaded in boot step 3 has up to 5 contexts
 * (see the list of defines in the headers: grep for "CONTEXT_") and will switch
 * depending on what context it is called in. This is enabled by a global
 * variable packed into `sc` called `context` which is checked during the
 * execution of the command's main.
 *
 * FIXME: loading of initation for each command needs a replacement in the
 * scheme files.
 */
bool
MainWindow::scheme_boot(void)
{
    /* NOTE: The mainWin pointer can't be passed to scheme_registerable functions
     * NOTE: so we make it file-scope.
     */
    _mainWin = this;

    root = scheme_init_new();
    FILE *f = fopen("scripts/boot.scm", "r");
    if (!f) {
        qDebug("Failed to boot scripting environment.");
        return false;
    }
    scheme_load_file(root, f);
    fclose(f);

    for (int i=0; command_map[i].icon[0] != '^'; i++) {
        printf("Loading %s\n", command_map[i].icon);
        if (command_map[i].function == do_nothing_f) {
            /* This allows us to have commands that are defined as scheme
             * scripts rather than built-in C functions, while we use the
             * same table for both.
             */
            continue;
        }
        scheme_define(root, root->global_env,
            mk_symbol(root, command_map[i].command),
            mk_foreign_func(root, command_map[i].function));
    }

    f = fopen("scripts/commands.scm", "r");
    if (!f) {
        qDebug("Failed to load commands in the scripting environment.");
        return false;
    }
    scheme_load_file(root, f);
    fclose(f);

#if 0
void
MainWindow::load_command(const QString& cmdName)
{
    qDebug("javaLoadCommand(%s)", qPrintable(cmdName));
    QString appDir = qApp->applicationDirPath();
    QFile file(appDir + "/commands/" + cmdName + "/" + cmdName + ".js");
    file.open(QIODevice::ReadOnly);
    QString script(file.readAll());
    file.close();

    QSettings settings(appDir + "/commands/" + cmdName + "/" + cmdName + ".ini", QSettings::IniFormat);
    QString menuName = settings.value("Menu/Name",    "Lost & Found").toString();
    int menuPos = settings.value("Menu/Position", 0).toInt();
    QString toolbarName = settings.value("ToolBar/Name", "Lost & Found").toString();
    int toolbarPos = settings.value("ToolBar/Position", 0).toInt();
    QString toolTip = settings.value("Tips/ToolTip", "").toString();
    QString statusTip = settings.value("Tips/StatusTip", "").toString();
    QStringList aliases = settings.value("Prompt/Alias").toStringList();

    QAction* ACTION = createAction(cmdName, toolTip, statusTip, true);

    if (toolbarName.toUpper() != "NONE") {
        //If the toolbar doesn't exist, create it.
        if (!toolbarHash.value(toolbarName)) {
            QToolBar* tb = new QToolBar(toolbarName, this);
            tb->setObjectName("toolbar" + toolbarName);
            connect(tb, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
            addToolBar(Qt::LeftToolBarArea, tb);
            addToolBarBreak(Qt::LeftToolBarArea);
            toolbarHash.insert(toolbarName, tb);
        }

        //TODO: order actions position in toolbar based on .ini setting
        toolbarHash.value(toolbarName)->addAction(ACTION);
    }

    if (menuName.toUpper() != "NONE") {
        //If the menu doesn't exist, create it.
        if (!menuHash.value(menuName)) {
            QMenu* menu = new QMenu(menuName, this);
            menu->setTearOffEnabled(true);
            menuBar()->addMenu(menu);
            menuHash.insert(menuName, menu);
        }

        //TODO: order actions position in menu based on .ini setting
        menuHash.value(menuName)->addAction(ACTION);
    }

    foreach(QString alias, aliases) {
        prompt->addCommand(alias, cmdName);
    }
}
#endif

    return true;
}

void
MainWindow::scheme_free(void)
{
    scheme_deinit(root);
}

