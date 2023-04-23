#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include "mainwindow.h"

class MainWindow;

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QFileInfo;
class QTabWidget;
QT_END_NAMESPACE

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

    //Temporary for instant preview
    bool    preview_general_mdi_bg_use_logo;
    bool    preview_general_mdi_bg_use_texture;
    bool    preview_general_mdi_bg_use_color;

    QString accept_general_mdi_bg_logo;
    QString accept_general_mdi_bg_texture;
    QRgb    preview_general_mdi_bg_color;
    QRgb    accept_general_mdi_bg_color;

    bool    preview_display_show_scrollbars;

    QRgb    preview_display_crosshair_color;
    QRgb    accept_display_crosshair_color;
    QRgb    preview_display_bg_color;
    QRgb    accept_display_bg_color;

    QRgb    preview_display_selectbox_left_color;
    QRgb    accept_display_selectbox_left_color;
    QRgb    preview_display_selectbox_left_fill;
    QRgb    accept_display_selectbox_left_fill;
    QRgb    preview_display_selectbox_right_color;
    QRgb    accept_display_selectbox_right_color;
    QRgb    preview_display_selectbox_right_fill;
    QRgb    accept_display_selectbox_right_fill;
    quint8  preview_display_selectbox_alpha;

    QRgb    preview_prompt_text_color;
    QRgb    accept_prompt_text_color;

    QRgb    preview_prompt_bg_color;
    QRgb    accept_prompt_bg_color;

    QString preview_prompt_font_family;
    QString preview_prompt_font_style;
    quint8  preview_prompt_font_size;

    QRgb    preview_grid_color;
    QRgb    accept_grid_color;

    QRgb    preview_ruler_color;
    QRgb    accept_ruler_color;

    bool    preview_lwt_show_lwt;
    bool    preview_lwt_real_render;

    //Temporary until changes are accepted
    QString dialog_general_language;
    QString dialog_general_icon_theme;
    int     dialog_general_icon_size;
    bool    dialog_general_mdi_bg_use_logo;
    bool    dialog_general_mdi_bg_use_texture;
    bool    dialog_general_mdi_bg_use_color;
    QString dialog_general_mdi_bg_logo;
    QString dialog_general_mdi_bg_texture;
    QRgb    dialog_general_mdi_bg_color;
    bool    dialog_general_tip_of_the_day;
    bool    dialog_general_system_help_browser;
    bool    dialog_display_use_opengl;
    bool    dialog_display_renderhint_aa;
    bool    dialog_display_renderhint_text_aa;
    bool    dialog_display_renderhint_smooth_pix;
    bool    dialog_display_renderhint_high_aa;
    bool    dialog_display_renderhint_noncosmetic;
    bool    dialog_display_show_scrollbars;
    int     dialog_display_scrollbar_widget_num;
    QRgb    dialog_display_crosshair_color;
    QRgb    dialog_display_bg_color;
    QRgb    dialog_display_selectbox_left_color;
    QRgb    dialog_display_selectbox_left_fill;
    QRgb    dialog_display_selectbox_right_color;
    QRgb    dialog_display_selectbox_right_fill;
    quint8  dialog_display_selectbox_alpha;
    EmbReal   dialog_display_zoomscale_in;
    EmbReal   dialog_display_zoomscale_out;
    quint8  dialog_display_crosshair_percent;
    QString dialog_display_units;
    QRgb    dialog_prompt_text_color;
    QRgb    dialog_prompt_bg_color;
    QString dialog_prompt_font_family;
    QString dialog_prompt_font_style;
    quint8  dialog_prompt_font_size;
    bool    dialog_prompt_save_history;
    bool    dialog_prompt_save_history_as_html;
    QString dialog_prompt_save_history_filename;
    QString dialog_opensave_custom_filter;
    QString dialog_opensave_open_format;
    bool    dialog_opensave_open_thumbnail;
    QString dialog_opensave_save_format;
    bool    dialog_opensave_save_thumbnail;
    quint8  dialog_opensave_recent_max_files;
    quint8  dialog_opensave_trim_dst_num_jumps;
    QString dialog_printing_default_device;
    bool    dialog_printing_use_last_device;
    bool    dialog_printing_disable_bg;
    bool    dialog_grid_show_on_load;
    bool    dialog_grid_show_origin;
    bool    dialog_grid_color_match_crosshair;
    QRgb    dialog_grid_color;
    bool    dialog_grid_load_from_file;
    QString dialog_grid_type;
    bool    dialog_grid_center_on_origin;
    EmbReal   dialog_grid_center_x;
    EmbReal   dialog_grid_center_y;
    EmbReal   dialog_grid_size_x;
    EmbReal   dialog_grid_size_y;
    EmbReal   dialog_grid_spacing_x;
    EmbReal   dialog_grid_spacing_y;
    EmbReal   dialog_grid_size_radius;
    EmbReal   dialog_grid_spacing_radius;
    EmbReal   dialog_grid_spacing_angle;
    bool    dialog_ruler_show_on_load;
    bool    dialog_ruler_metric;
    QRgb    dialog_ruler_color;
    quint8  dialog_ruler_pixel_size;
    bool    dialog_qsnap_enabled;
    QRgb    dialog_qsnap_locator_color;
    quint8  dialog_qsnap_locator_size;
    quint8  dialog_qsnap_aperture_size;
    bool    dialog_qsnap_endpoint;
    bool    dialog_qsnap_midpoint;
    bool    dialog_qsnap_center;
    bool    dialog_qsnap_node;
    bool    dialog_qsnap_quadrant;
    bool    dialog_qsnap_intersection;
    bool    dialog_qsnap_extension;
    bool    dialog_qsnap_insertion;
    bool    dialog_qsnap_perpendicular;
    bool    dialog_qsnap_tangent;
    bool    dialog_qsnap_nearest;
    bool    dialog_qsnap_apparent;
    bool    dialog_qsnap_parallel;
    bool    dialog_lwt_show_lwt;
    bool    dialog_lwt_real_render;
    EmbReal   dialog_lwt_default_lwt;
    bool    dialog_selection_mode_pickfirst;
    bool    dialog_selection_mode_pickadd;
    bool    dialog_selection_mode_pickdrag;
    QRgb    dialog_selection_coolgrip_color;
    QRgb    dialog_selection_hotgrip_color;
    quint8  dialog_selection_grip_size;
    quint8  dialog_selection_pickbox_size;

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

#endif
