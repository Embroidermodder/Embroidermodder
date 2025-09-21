/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Settings Dialog: a dialog to offer a GUI for altering settings variables.
 */

#include "embroidermodder.h"

#include <QtGui>
#include <QStandardPaths>

typedef struct Settings_ {
    QString general_mdi_bg_logo;
    QString general_mdi_bg_texture;
    QRgb general_mdi_bg_color;
    uint16_t general_current_tip;
    int display_scrollbar_widget_num;
    QRgb display_crosshair_color;
    QRgb display_bg_color;
    QRgb display_selectbox_left_color;
    QRgb display_selectbox_left_fill;
    QRgb display_selectbox_right_color;
    QRgb display_selectbox_right_fill;
    quint8 display_selectbox_alpha;
    qreal display_zoomscale_in;
    qreal display_zoomscale_out;
    quint8 display_crosshair_percent;
    QString display_units;
    QRgb prompt_text_color;
    QRgb prompt_bg_color;
    QString prompt_font_family;
    QString prompt_font_style;
    quint8 prompt_font_size;
    QString prompt_save_history_filename;
    QString opensave_custom_filter;
    QString opensave_open_format;
    QString opensave_save_format;
    quint8 opensave_recent_max_files;
    QStringList opensave_recent_list_of_files;
    QString opensave_recent_directory;
    quint8 opensave_trim_dst_num_jumps;
    QString printing_default_device;
    QRgb grid_color;
    QString grid_type;
    QRgb ruler_color;
    quint8 ruler_pixel_size;
    QRgb qsnap_locator_color;
    quint8 qsnap_locator_size;
    quint8 qsnap_aperture_size;
    qreal lwt_default_lwt;
    QRgb selection_coolgrip_color;
    QRgb selection_hotgrip_color;
    quint8 selection_grip_size;
    quint8 selection_pickbox_size;
    QString text_font;
    qreal text_size;
    qreal text_angle;
    bool grid_color_match_crosshair;
    bool grid_center_on_origin;
    bool grid_load_from_file;
    bool lwt_real_render;
    bool lwt_show_lwt;
    bool selection_mode_pickadd;
    bool selection_mode_pickdrag;
    qreal grid_size_radius;
    qreal grid_spacing_radius;
    qreal grid_spacing_angle;
    bool grid_show_origin;
    bool general_mdi_bg_use_logo;
    bool general_mdi_bg_use_texture;
    bool general_mdi_bg_use_color;
    bool selection_mode_pickfirst;
    bool ruler_metric;
    bool grid_show_on_load;
    bool printing_disable_bg;
    bool display_show_scrollbars;
    bool ruler_show_on_load;
    bool general_tip_of_the_day;
    bool display_use_opengl;
} Settings;

ScriptValue st[N_SETTINGS];
/* Temporary until changes are accepted. */
ScriptValue st_dialog[N_SETTINGS];
ScriptValue st_accept[N_SETTINGS];
/* Temporary for instant preview. */
ScriptValue st_preview[N_SETTINGS];

Settings dialog, accept_, preview;

QTabWidget* tabWidget;
QDialogButtonBox* buttonBox;

void
set_label_visible(QObject* parent, const char *key, bool visibility)
{
    QLabel* label = parent->findChild<QLabel*>(key);
    if (label) {
        label->setVisible(visibility);
    }
    else {
        qDebug("ERROR: Failed to find label with the key \"%s\".", key);
    }
}

void
set_double_spinbox_visible(QObject* parent, const char *key, bool visibility)
{
    QDoubleSpinBox* spinbox = parent->findChild<QDoubleSpinBox*>(key);
    if (spinbox) {
        spinbox->setVisible(visibility);
    }
    else {
        qDebug("ERROR: Failed to find spinbox with the key \"%s\".", key);
    }
}

Settings_Dialog::Settings_Dialog(const QString& showTab, QWidget* parent) : QDialog(parent)
{
    for (int i=0; i<N_SETTINGS; i++) {
        st_dialog[i] = st[i];
        st_accept[i] = st[i];
        st_preview[i] = st[i];
    }

    setMinimumSize(750,550);

    tabWidget = new QTabWidget(this);

    //TODO: Add icons to tabs
    tabWidget->addTab(createTabGeneral(),    tr("General"));
    tabWidget->addTab(createTabFilesPaths(), tr("Files/Paths"));
    tabWidget->addTab(createTabDisplay(),    tr("Display"));
    tabWidget->addTab(createTabPrompt(),     tr("Prompt"));
    tabWidget->addTab(createTabOpenSave(),   tr("Open/Save"));
    tabWidget->addTab(createTabPrinting(),   tr("Printing"));
    tabWidget->addTab(createTabSnap(),       tr("Snap"));
    tabWidget->addTab(createTabGridRuler(),  tr("Grid/Ruler"));
    tabWidget->addTab(createTabOrthoPolar(), tr("Ortho/Polar"));
    tabWidget->addTab(createTabQuickSnap(),  tr("QuickSnap"));
    tabWidget->addTab(createTabQuickTrack(), tr("QuickTrack"));
    tabWidget->addTab(createTabLineWeight(), tr("LineWeight"));
    tabWidget->addTab(createTabSelection(),  tr("Selection"));

    if     (showTab == "General")     tabWidget->setCurrentIndex( 0);
    else if(showTab == "Files/Path")  tabWidget->setCurrentIndex( 1);
    else if(showTab == "Display")     tabWidget->setCurrentIndex( 2);
    else if(showTab == "Prompt")      tabWidget->setCurrentIndex( 3);
    else if(showTab == "Open/Save")   tabWidget->setCurrentIndex( 4);
    else if(showTab == "Printing")    tabWidget->setCurrentIndex( 5);
    else if(showTab == "Snap")        tabWidget->setCurrentIndex( 6);
    else if(showTab == "Grid/Ruler")  tabWidget->setCurrentIndex( 7);
    else if(showTab == "Ortho/Polar") tabWidget->setCurrentIndex( 8);
    else if(showTab == "QuickSnap")   tabWidget->setCurrentIndex( 9);
    else if(showTab == "QuickTrack")  tabWidget->setCurrentIndex(10);
    else if(showTab == "LineWeight")  tabWidget->setCurrentIndex(11);
    else if(showTab == "Selection")   tabWidget->setCurrentIndex(12);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));

    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(this);
    vboxLayoutMain->addWidget(tabWidget);
    vboxLayoutMain->addWidget(buttonBox);
    setLayout(vboxLayoutMain);

    setWindowTitle(tr("Settings"));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

Settings_Dialog::~Settings_Dialog()
{
    QApplication::restoreOverrideCursor();
}

/*! Create a checkbox that only needs the basic features. */
QCheckBox *
Settings_Dialog::basic_checkbox(QGroupBox *gb, int id)
{
    QCheckBox* cb = new QCheckBox(tr(settings_table[id].description.c_str()), gb);
    cb->setChecked(st_dialog[id].b);
    cb->setEnabled(settings_table[id].enabled);
    if (settings_table[id].icon != "") {
        cb->setIcon(_mainWin->createIcon(settings_table[id].icon.c_str()));
    }
    connect(cb, SIGNAL(stateChanged(int)), this, SLOT(
        [=](int checked) {
            st_dialog[id].b = checked;
        }));
    return cb;
}

/*! Create a double spinbox that only needs the basic features. */
QDoubleSpinBox *
Settings_Dialog::basic_double_spinbox(QGroupBox *gb, int id)
{
    QDoubleSpinBox* sb = new QDoubleSpinBox(gb);
    sb->setSingleStep(settings_table[id].single_step);
    sb->setRange(settings_table[id].lower, settings_table[id].upper);
    sb->setValue(st_dialog[id].r);
    connect(sb, SIGNAL(valueChanged(double)), this,
        SLOT([=](double value) { st_dialog[id].r = value; }));
    sb->setObjectName(QString("spinBox") + settings_table[id].key.c_str());
    return sb;
}

/*! Create an integer spinbox that only needs the basic features. */
QSpinBox *
Settings_Dialog::basic_spinbox(QGroupBox *gb, int id)
{
    QSpinBox* sb = new QSpinBox(gb);
    return sb;
}

QWidget*
Settings_Dialog::createTabGeneral()
{
    QWidget* widget = new QWidget(this);

    //Language
    QGroupBox* groupBoxLanguage = new QGroupBox(tr("Language"), widget);

    QLabel* labelLanguage = new QLabel(tr("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    comboBoxLanguage->addItem("Default");
    comboBoxLanguage->addItem("System");
    comboBoxLanguage->insertSeparator(2);
    QDir trDir(qApp->applicationDirPath());
    trDir.cd("translations");
    foreach(QString dirName, trDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        dirName[0] = dirName[0].toUpper();
        comboBoxLanguage->addItem(dirName);
    }
    QString current = st_dialog[ST_LANGUAGE].s.c_str();
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxLanguageCurrentIndexChanged(const QString&)));

    QVBoxLayout* vboxLayoutLanguage = new QVBoxLayout(groupBoxLanguage);
    vboxLayoutLanguage->addWidget(labelLanguage);
    vboxLayoutLanguage->addWidget(comboBoxLanguage);
    groupBoxLanguage->setLayout(vboxLayoutLanguage);

    //Icons
    QGroupBox* groupBoxIcon = new QGroupBox(tr("Icons"), widget);

    QString theme = st_dialog[ST_ICON_THEME].s.c_str();
    QLabel* labelIconTheme = new QLabel(tr("Icon Theme"), groupBoxIcon);
    QComboBox* comboBoxIconTheme = new QComboBox(groupBoxIcon);
    QDir dir(qApp->applicationDirPath());
    dir.cd("icons");
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/" + "theme" + ".png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(theme));
    connect(comboBoxIconTheme, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxIconThemeCurrentIndexChanged(const QString&)));

    QLabel* labelIconSize = new QLabel(tr("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon16"  + ".png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon24"  + ".png"), "Small",      24);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon32"  + ".png"), "Medium",     32);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon48"  + ".png"), "Large",      48);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon64"  + ".png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon128" + ".png"), "I'm Blind", 128);
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(st_dialog[ST_ICON_SIZE].i));
    connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIconSizeCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutIcon = new QVBoxLayout(groupBoxIcon);
    vboxLayoutIcon->addWidget(labelIconTheme);
    vboxLayoutIcon->addWidget(comboBoxIconTheme);
    vboxLayoutIcon->addWidget(labelIconSize);
    vboxLayoutIcon->addWidget(comboBoxIconSize);
    groupBoxIcon->setLayout(vboxLayoutIcon);

    //Mdi Background
    QGroupBox* groupBoxMdiBG = new QGroupBox(tr("Background"), widget);


    QCheckBox* checkBoxMdiBGUseLogo = new QCheckBox(tr("Use Logo"), groupBoxMdiBG);
    checkBoxMdiBGUseLogo->setChecked(preview.general_mdi_bg_use_logo);
    connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)));

    QPushButton* buttonMdiBGLogo = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(dialog.general_mdi_bg_use_logo);
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseTexture = new QCheckBox(tr("Use Texture"), groupBoxMdiBG);
    checkBoxMdiBGUseTexture->setChecked(preview.general_mdi_bg_use_texture);
    connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)));

    QPushButton* buttonMdiBGTexture = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(dialog.general_mdi_bg_use_texture);
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseColor = new QCheckBox(tr("Use Color"), groupBoxMdiBG);
    checkBoxMdiBGUseColor->setChecked(preview.general_mdi_bg_use_color);
    connect(checkBoxMdiBGUseColor, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseColorStateChanged(int)));

    QPushButton* buttonMdiBGColor = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setEnabled(dialog.general_mdi_bg_use_color);
    QPixmap mdiBGPix(16,16);
    mdiBGPix.fill(QColor(preview.general_mdi_bg_color));
    buttonMdiBGColor->setIcon(QIcon(mdiBGPix));
    connect(buttonMdiBGColor, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundColor()));
    connect(checkBoxMdiBGUseColor, SIGNAL(toggled(bool)), buttonMdiBGColor, SLOT(setEnabled(bool)));

    QGridLayout* gridLayoutMdiBG = new QGridLayout(widget);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseLogo,    0, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGLogo,         0, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseTexture, 1, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGTexture,      1, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseColor,   2, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGColor,        2, 1, Qt::AlignRight);
    groupBoxMdiBG->setLayout(gridLayoutMdiBG);

    //Tips
    QGroupBox* groupBoxTips = new QGroupBox(tr("Tips"), widget);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("Show Tip of the Day on startup"), groupBoxTips);
    checkBoxTipOfTheDay->setChecked(st_dialog[ST_TIP_OF_THE_DAY].b);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    //Help Browser
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(tr("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(tr("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(st[ST_SYSTEM_HELP_BROWSER].b);
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(tr("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!st[ST_SYSTEM_HELP_BROWSER].b);
    radioButtonCustomHelpBrowser->setEnabled(false); //TODO: finish this

    QVBoxLayout* vboxLayoutHelpBrowser = new QVBoxLayout(groupBoxHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonSystemHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonCustomHelpBrowser);
    groupBoxHelpBrowser->setLayout(vboxLayoutHelpBrowser);

    //Widget Layout
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxLanguage);
    vboxLayoutMain->addWidget(groupBoxIcon);
    vboxLayoutMain->addWidget(groupBoxMdiBG);
    vboxLayoutMain->addWidget(groupBoxTips);
    vboxLayoutMain->addWidget(groupBoxHelpBrowser);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabFilesPaths()
{
    QWidget* widget = new QWidget(this);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabDisplay()
{
    QWidget* widget = new QWidget(this);

    //Rendering
    //TODO: Review OpenGL and Rendering settings for future inclusion
    QGroupBox* groupBoxRender = new QGroupBox(tr("Rendering"), widget);

    QVBoxLayout* vboxLayoutRender = new QVBoxLayout(groupBoxRender);
    vboxLayoutRender->addWidget(basic_checkbox(groupBoxRender, ST_USE_OPENGL));
    vboxLayoutRender->addWidget(basic_checkbox(groupBoxRender, ST_RENDERHINT_AA));
    vboxLayoutRender->addWidget(basic_checkbox(groupBoxRender, ST_RENDERHINT_TEXT_AA));
    vboxLayoutRender->addWidget(basic_checkbox(groupBoxRender, ST_RENDERHINT_SMOOTHPIX));
    vboxLayoutRender->addWidget(basic_checkbox(groupBoxRender, ST_RENDERHINT_HIGH_AA));
    vboxLayoutRender->addWidget(basic_checkbox(groupBoxRender, ST_RENDERHINT_NONCOSMETIC));
    groupBoxRender->setLayout(vboxLayoutRender);

    //ScrollBars
    QGroupBox* groupBoxScrollBars = new QGroupBox(tr("ScrollBars"), widget);

    QCheckBox* checkBoxShowScrollBars = new QCheckBox(tr("Show ScrollBars"), groupBoxScrollBars);
    checkBoxShowScrollBars->setChecked(preview.display_show_scrollbars);
    connect(checkBoxShowScrollBars, SIGNAL(stateChanged(int)), this, SLOT(checkBoxShowScrollBarsStateChanged(int)));

    QLabel* labelScrollBarWidget = new QLabel(tr("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    int numActions = actionHash.size();
    for(int i = 0; i < numActions; i++)
    {
        /* FIXME:
        QAction* action = actionHash.value(i);
        if(action) comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        */
    }
    comboBoxScrollBarWidget->setCurrentIndex(dialog.display_scrollbar_widget_num);
    connect(comboBoxScrollBarWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxScrollBarWidgetCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutScrollBars = new QVBoxLayout(groupBoxScrollBars);
    vboxLayoutScrollBars->addWidget(checkBoxShowScrollBars);
    vboxLayoutScrollBars->addWidget(labelScrollBarWidget);
    vboxLayoutScrollBars->addWidget(comboBoxScrollBarWidget);
    groupBoxScrollBars->setLayout(vboxLayoutScrollBars);

    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(tr("Colors"), widget);

    QLabel* labelCrossHairColor = new QLabel(tr("Crosshair Color"), groupBoxColor);
    QPushButton* buttonCrossHairColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap crosshairPix(16,16);
    crosshairPix.fill(QColor(preview.display_crosshair_color));
    buttonCrossHairColor->setIcon(QIcon(crosshairPix));
    connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap bgPix(16,16);
    bgPix.fill(QColor(preview.display_bg_color));
    buttonBGColor->setIcon(QIcon(bgPix));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()));

    QLabel* labelSelectBoxLeftColor = new QLabel(tr("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap sBoxLCPix(16,16);
    sBoxLCPix.fill(QColor(preview.display_selectbox_left_color));
    buttonSelectBoxLeftColor->setIcon(QIcon(sBoxLCPix));
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()));

    QLabel* labelSelectBoxLeftFill = new QLabel(tr("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap sBoxLFPix(16,16);
    sBoxLFPix.fill(QColor(preview.display_selectbox_left_fill));
    buttonSelectBoxLeftFill->setIcon(QIcon(sBoxLFPix));
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()));

    QLabel* labelSelectBoxRightColor = new QLabel(tr("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap sBoxRCPix(16,16);
    sBoxRCPix.fill(QColor(preview.display_selectbox_right_color));
    buttonSelectBoxRightColor->setIcon(QIcon(sBoxRCPix));
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()));

    QLabel* labelSelectBoxRightFill = new QLabel(tr("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap sBoxRFPix(16,16);
    sBoxRFPix.fill(QColor(preview.display_selectbox_right_fill));
    buttonSelectBoxRightFill->setIcon(QIcon(sBoxRFPix));
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()));

    QLabel* labelSelectBoxAlpha = new QLabel(tr("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    spinBoxSelectBoxAlpha->setValue(preview.display_selectbox_alpha);
    connect(spinBoxSelectBoxAlpha, SIGNAL(valueChanged(int)), this, SLOT(spinBoxDisplaySelectBoxAlphaValueChanged(int)));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelCrossHairColor,       0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonCrossHairColor,      0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor,              1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor,             1, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxLeftColor,   2, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxLeftColor,  2, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxLeftFill,    3, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxLeftFill,   3, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxRightColor,  4, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxRightColor, 4, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxRightFill,   5, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxRightFill,  5, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxAlpha,       6, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(spinBoxSelectBoxAlpha,     6, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    //Zoom
    QGroupBox* groupBoxZoom = new QGroupBox(tr("Zoom"), widget);

    QLabel* labelZoomScaleIn = new QLabel(tr("Zoom In Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleIn = basic_double_spinbox(groupBoxZoom, ST_ZOOMSCALE_IN);

    QLabel* labelZoomScaleOut = new QLabel(tr("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = basic_double_spinbox(groupBoxZoom, ST_ZOOMSCALE_OUT);

    QGridLayout* gridLayoutZoom = new QGridLayout(groupBoxZoom);
    gridLayoutZoom->addWidget(labelZoomScaleIn,    0, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleIn,  0, 1, Qt::AlignRight);
    gridLayoutZoom->addWidget(labelZoomScaleOut,   1, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleOut, 1, 1, Qt::AlignRight);
    groupBoxZoom->setLayout(gridLayoutZoom);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    //vboxLayoutMain->addWidget(groupBoxRender); //TODO: Review OpenGL and Rendering settings for future inclusion
    vboxLayoutMain->addWidget(groupBoxScrollBars);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxZoom);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

//TODO: finish prompt options
QWidget* Settings_Dialog::createTabPrompt()
{
    QWidget* widget = new QWidget(this);

    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(tr("Colors"), widget);

    QLabel* labelTextColor = new QLabel(tr("Text Color"), groupBoxColor);
    QPushButton* buttonTextColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap pix(16,16);
    pix.fill(QColor(preview.prompt_text_color));
    buttonTextColor->setIcon(QIcon(pix));
    connect(buttonTextColor, SIGNAL(clicked()), this, SLOT(choosePromptTextColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap pixx(16,16);
    pixx.fill(QColor(preview.prompt_bg_color));
    buttonBGColor->setIcon(QIcon(pixx));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(choosePromptBackgroundColor()));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelTextColor,  0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonTextColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor,    1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor,   1, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    //TODO: Tweak the Prompt Font ComboBoxes so they work properly
    //Font
    QGroupBox* groupBoxFont = new QGroupBox(tr("Font"), widget);

    QLabel* labelFontFamily = new QLabel(tr("Font Family"), groupBoxFont);
    QFontComboBox* comboBoxFontFamily = new QFontComboBox(groupBoxFont);
    comboBoxFontFamily->setCurrentFont(QFont(preview.prompt_font_family));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(const QString&)));
    QLabel* labelFontStyle = new QLabel(tr("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(preview.prompt_font_style);
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(const QString&)));
    QLabel* labelFontSize = new QLabel(tr("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    spinBoxFontSize->setValue(preview.prompt_font_size);
    connect(spinBoxFontSize, SIGNAL(valueChanged(int)), this, SLOT(spinBoxPromptFontSizeValueChanged(int)));

    QGridLayout* gridLayoutFont = new QGridLayout(groupBoxFont);
    gridLayoutFont->addWidget(labelFontFamily,    0, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontFamily, 0, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontStyle,     1, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontStyle,  1, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontSize,      2, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(spinBoxFontSize,    2, 1, Qt::AlignRight);
    groupBoxFont->setLayout(gridLayoutFont);

    //History
    QGroupBox* groupBoxHistory = new QGroupBox(tr("History"), widget);

    QVBoxLayout* vboxLayoutHistory = new QVBoxLayout(groupBoxHistory);
    vboxLayoutHistory->addWidget(basic_checkbox(groupBoxHistory, ST_PROMPT_SAVE_HISTORY));
    vboxLayoutHistory->addWidget(basic_checkbox(groupBoxHistory, ST_PROMPT_SAVE_AS_HTML));
    groupBoxHistory->setLayout(vboxLayoutHistory);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxFont);
    vboxLayoutMain->addWidget(groupBoxHistory);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

//TODO: finish open/save options
QWidget* Settings_Dialog::createTabOpenSave()
{
    QWidget* widget = new QWidget(this);

    //Custom Filter
    QGroupBox* groupBoxCustomFilter = new QGroupBox(tr("Custom Filter"), widget);
    groupBoxCustomFilter->setEnabled(false); //TODO: Fixup custom filter

    StringList ext = string_tables["extensions"];
    QCheckBox* checkBoxCustomFilter[100];
    for (size_t i=0; i<ext.size(); i++) {
        checkBoxCustomFilter[i] = new QCheckBox(ext[i].c_str(),
            groupBoxCustomFilter);
        checkBoxCustomFilter[i]->setChecked(
            dialog.opensave_custom_filter.contains(QString("*.") + ext[i].c_str(),
                Qt::CaseInsensitive));
        connect(checkBoxCustomFilter[i], SIGNAL(stateChanged(int)), this,
            SLOT(checkBoxCustomFilterStateChanged(int)));
    }

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(tr("Select All"), groupBoxCustomFilter);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()));
    for (size_t i=0; i<ext.size(); i++) {
        connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilter[i],
            SLOT(setChecked(bool)));
    }

    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", groupBoxCustomFilter);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()));
    for (size_t i=0; i<ext.size(); i++) {
        connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilter[i],
            SLOT(setChecked(bool)));
    }

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    for (size_t i=0; i<ext.size(); i++) {
        gridLayoutCustomFilter->addWidget(checkBoxCustomFilter[i], i%10, i/10, Qt::AlignLeft);
    }
    gridLayoutCustomFilter->addWidget(buttonCustomFilterSelectAll, 0, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterClearAll,  1, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->setLayout(gridLayoutCustomFilter);

    if (dialog.opensave_custom_filter.contains("supported", Qt::CaseInsensitive)) {
        buttonCustomFilterSelectAllClicked();
    }

    //Opening
    QGroupBox* groupBoxOpening = new QGroupBox(tr("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(tr("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    //TODO: Add a button to clear the recent history.

    QLabel* labelRecentMaxFiles = new QLabel(tr("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    spinBoxRecentMaxFiles->setValue(dialog.opensave_recent_max_files);
    connect(spinBoxRecentMaxFiles, SIGNAL(valueChanged(int)), this, SLOT(spinBoxRecentMaxFilesValueChanged(int)));

    QFrame* frameRecent = new QFrame(groupBoxOpening);
    QGridLayout* gridLayoutRecent = new QGridLayout(frameRecent);
    gridLayoutRecent->addWidget(labelRecentMaxFiles,   0, 0, Qt::AlignLeft);
    gridLayoutRecent->addWidget(spinBoxRecentMaxFiles, 0, 1, Qt::AlignRight);
    frameRecent->setLayout(gridLayoutRecent);

    QVBoxLayout* vboxLayoutOpening = new QVBoxLayout(groupBoxOpening);
    vboxLayoutOpening->addWidget(comboBoxOpenFormat);
    vboxLayoutOpening->addWidget(checkBoxOpenThumbnail);
    vboxLayoutOpening->addWidget(frameRecent);
    groupBoxOpening->setLayout(vboxLayoutOpening);

    //Saving
    QGroupBox* groupBoxSaving = new QGroupBox(tr("File Save"), widget);

    QComboBox* comboBoxSaveFormat = new QComboBox(groupBoxSaving);

    QCheckBox* checkBoxSaveThumbnail = new QCheckBox(tr("Save Thumbnails"), groupBoxSaving);
    checkBoxSaveThumbnail->setChecked(false);

    QCheckBox* checkBoxAutoSave = new QCheckBox(tr("AutoSave"), groupBoxSaving);
    checkBoxAutoSave->setChecked(false);

    QVBoxLayout* vboxLayoutSaving = new QVBoxLayout(groupBoxSaving);
    vboxLayoutSaving->addWidget(comboBoxSaveFormat);
    vboxLayoutSaving->addWidget(checkBoxSaveThumbnail);
    vboxLayoutSaving->addWidget(checkBoxAutoSave);
    groupBoxSaving->setLayout(vboxLayoutSaving);

    //Trimming
    QGroupBox* groupBoxTrim = new QGroupBox(tr("Trimming"), widget);

    QLabel* labelTrimDstNumJumps = new QLabel(tr("DST Only: Minimum number of jumps to trim"), groupBoxTrim);
    QSpinBox* spinBoxTrimDstNumJumps = new QSpinBox(groupBoxTrim);
    spinBoxTrimDstNumJumps->setRange(1, 20);
    spinBoxTrimDstNumJumps->setValue(dialog.opensave_trim_dst_num_jumps);
    connect(spinBoxTrimDstNumJumps, SIGNAL(valueChanged(int)), this, SLOT(spinBoxTrimDstNumJumpsValueChanged(int)));

    QFrame* frameTrimDstNumJumps = new QFrame(groupBoxTrim);
    QGridLayout* gridLayoutTrimDstNumJumps = new QGridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps->addWidget(labelTrimDstNumJumps,   0, 0, Qt::AlignLeft);
    gridLayoutTrimDstNumJumps->addWidget(spinBoxTrimDstNumJumps, 0, 1, Qt::AlignRight);
    frameTrimDstNumJumps->setLayout(gridLayoutTrimDstNumJumps);

    QVBoxLayout* vboxLayoutTrim = new QVBoxLayout(groupBoxTrim);
    vboxLayoutTrim->addWidget(frameTrimDstNumJumps);
    groupBoxTrim->setLayout(vboxLayoutTrim);

    //Widget Layout
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxCustomFilter);
    vboxLayoutMain->addWidget(groupBoxOpening);
    vboxLayoutMain->addWidget(groupBoxSaving);
    vboxLayoutMain->addWidget(groupBoxTrim);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabPrinting()
{
    QWidget* widget = new QWidget(this);

    //Default Printer
    QGroupBox* groupBoxDefaultPrinter = new QGroupBox(tr("Default Printer"), widget);

    QRadioButton* radioButtonUseSame = new QRadioButton(tr("Use as default device"), groupBoxDefaultPrinter);
    radioButtonUseSame->setChecked(!st[ST_PRINTING_USE_LAST_DEVICE].b);
    QRadioButton* radioButtonUseLast = new QRadioButton(tr("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(st[ST_PRINTING_USE_LAST_DEVICE].b);

    QComboBox* comboBoxDefaultDevice = new QComboBox(groupBoxDefaultPrinter);
    QList<QPrinterInfo> listAvailPrinters = QPrinterInfo::availablePrinters();
    foreach(QPrinterInfo info, listAvailPrinters) {
        comboBoxDefaultDevice->addItem(_mainWin->createIcon("print"), info.printerName());
    }

    QVBoxLayout* vboxLayoutDefaultPrinter = new QVBoxLayout(groupBoxDefaultPrinter);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter->addWidget(comboBoxDefaultDevice);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseLast);
    groupBoxDefaultPrinter->setLayout(vboxLayoutDefaultPrinter);

    //Save Ink
    QGroupBox* groupBoxSaveInk = new QGroupBox(tr("Save Ink"), widget);

    QCheckBox* checkBoxDisableBG = new QCheckBox(tr("Disable Background"), groupBoxSaveInk);
    checkBoxDisableBG->setChecked(dialog.printing_disable_bg);
    connect(checkBoxDisableBG, SIGNAL(stateChanged(int)), this, SLOT(checkBoxDisableBGStateChanged(int)));

    QVBoxLayout* vboxLayoutSaveInk = new QVBoxLayout(groupBoxSaveInk);
    vboxLayoutSaveInk->addWidget(checkBoxDisableBG);
    groupBoxSaveInk->setLayout(vboxLayoutSaveInk);

    //Widget Layout
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxDefaultPrinter);
    vboxLayoutMain->addWidget(groupBoxSaveInk);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabSnap()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabGridRuler()
{
    QWidget* widget = new QWidget(this);

    //Grid Misc
    QGroupBox* groupBoxGridMisc = new QGroupBox(tr("Grid Misc"), widget);

    QCheckBox* checkBoxGridShowOnLoad = new QCheckBox(tr("Initially show grid when loading a file"), groupBoxGridMisc);
    checkBoxGridShowOnLoad->setChecked(dialog.grid_show_on_load);
    connect(checkBoxGridShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOnLoadStateChanged(int)));

    QCheckBox* checkBoxGridShowOrigin = new QCheckBox(tr("Show the origin when the grid is enabled"), groupBoxGridMisc);
    checkBoxGridShowOrigin->setChecked(dialog.grid_show_origin);
    connect(checkBoxGridShowOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOriginStateChanged(int)));

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = new QGroupBox(tr("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(tr("Match grid color to crosshair color"), groupBoxGridColor);
    checkBoxGridColorMatchCrossHair->setChecked(dialog.grid_color_match_crosshair);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(tr("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(tr("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b) {
        st_dialog[ST_GRID_COLOR].u = st[ST_CROSSHAIR_COLOR].u;
        st_preview[ST_GRID_COLOR].u = st[ST_CROSSHAIR_COLOR].u;
        st_accept[ST_GRID_COLOR].u  = st[ST_CROSSHAIR_COLOR].u;
    }
    QPixmap gridPix(16,16);
    gridPix.fill(QColor(preview.grid_color));
    buttonGridColor->setIcon(QIcon(gridPix));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor->setEnabled(!dialog.grid_color_match_crosshair);
    buttonGridColor->setEnabled(!dialog.grid_color_match_crosshair);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = new QGroupBox(tr("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(tr("Set grid size from opened file"), groupBoxGridGeom);
    checkBoxGridLoadFromFile->setChecked(dialog.grid_load_from_file);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(tr("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(dialog.grid_type));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(const QString&)));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(tr("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    checkBoxGridCenterOnOrigin->setChecked(dialog.grid_center_on_origin);
    connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)));

    QLabel* labelGridCenterX = new QLabel(tr("Grid Center X"), groupBoxGridGeom);
    labelGridCenterX->setObjectName("labelGridCenterX");
    QDoubleSpinBox* spinBoxGridCenterX = basic_double_spinbox(groupBoxGridGeom, ST_GRID_CENTER_X);

    QLabel* labelGridCenterY = new QLabel(tr("Grid Center Y"), groupBoxGridGeom);
    labelGridCenterY->setObjectName("labelGridCenterY");
    QDoubleSpinBox* spinBoxGridCenterY = basic_double_spinbox(groupBoxGridGeom, ST_GRID_CENTER_Y);

    QLabel* labelGridSizeX = new QLabel(tr("Grid Size X"), groupBoxGridGeom);
    labelGridSizeX->setObjectName("labelGridSizeX");
    QDoubleSpinBox* spinBoxGridSizeX = basic_double_spinbox(groupBoxGridGeom, ST_GRID_SIZE_X);

    QLabel* labelGridSizeY = new QLabel(tr("Grid Size Y"), groupBoxGridGeom);
    labelGridSizeY->setObjectName("labelGridSizeY");
    QDoubleSpinBox* spinBoxGridSizeY = basic_double_spinbox(groupBoxGridGeom, ST_GRID_SIZE_Y);

    QLabel* labelGridSpacingX = new QLabel(tr("Grid Spacing X"), groupBoxGridGeom);
    labelGridSpacingX->setObjectName("labelGridSpacingX");
    QDoubleSpinBox* spinBoxGridSpacingX = basic_double_spinbox(groupBoxGridGeom,
        ST_GRID_SPACING_X);

    QLabel* labelGridSpacingY = new QLabel(tr("Grid Spacing Y"), groupBoxGridGeom);
    labelGridSpacingY->setObjectName("labelGridSpacingY");
    QDoubleSpinBox* spinBoxGridSpacingY = basic_double_spinbox(groupBoxGridGeom,
        ST_GRID_SPACING_Y);

    QLabel* labelGridSizeRadius = new QLabel(tr("Grid Size Radius"), groupBoxGridGeom);
    labelGridSizeRadius->setObjectName("labelGridSizeRadius");
    QDoubleSpinBox* spinBoxGridSizeRadius = basic_double_spinbox(groupBoxGridGeom,
        ST_GRID_SIZE_RADIUS);

    QLabel* labelGridSpacingRadius = new QLabel(tr("Grid Spacing Radius"), groupBoxGridGeom);
    labelGridSpacingRadius->setObjectName("labelGridSpacingRadius");
    QDoubleSpinBox* spinBoxGridSpacingRadius = basic_double_spinbox(groupBoxGridGeom,
        ST_GRID_SPACING_RADIUS);

    QLabel* labelGridSpacingAngle = new QLabel(tr("Grid Spacing Angle"), groupBoxGridGeom);
    labelGridSpacingAngle->setObjectName("labelGridSpacingAngle");
    QDoubleSpinBox* spinBoxGridSpacingAngle = basic_double_spinbox(groupBoxGridGeom,
        ST_GRID_SPACING_ANGLE);

    labelGridType->setEnabled(!dialog.grid_load_from_file);
    comboBoxGridType->setEnabled(!dialog.grid_load_from_file);
    checkBoxGridCenterOnOrigin->setEnabled(!dialog.grid_load_from_file);
    labelGridCenterX->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridCenterX->setEnabled(!dialog.grid_load_from_file);
    labelGridCenterY->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridCenterY->setEnabled(!dialog.grid_load_from_file);
    labelGridSizeX->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSizeX->setEnabled(!dialog.grid_load_from_file);
    labelGridSizeY->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSizeY->setEnabled(!dialog.grid_load_from_file);
    labelGridSpacingX->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSpacingX->setEnabled(!dialog.grid_load_from_file);
    labelGridSpacingY->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSpacingY->setEnabled(!dialog.grid_load_from_file);
    labelGridSizeRadius->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSizeRadius->setEnabled(!dialog.grid_load_from_file);
    labelGridSpacingRadius->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSpacingRadius->setEnabled(!dialog.grid_load_from_file);
    labelGridSpacingAngle->setEnabled(!dialog.grid_load_from_file);
    spinBoxGridSpacingAngle->setEnabled(!dialog.grid_load_from_file);

    bool visibility = false;
    if(dialog.grid_type == "Circular") visibility = true;
    labelGridSizeX->setVisible(!visibility);
    spinBoxGridSizeX->setVisible(!visibility);
    labelGridSizeY->setVisible(!visibility);
    spinBoxGridSizeY->setVisible(!visibility);
    labelGridSpacingX->setVisible(!visibility);
    spinBoxGridSpacingX->setVisible(!visibility);
    labelGridSpacingY->setVisible(!visibility);
    spinBoxGridSpacingY->setVisible(!visibility);
    labelGridSizeRadius->setVisible(visibility);
    spinBoxGridSizeRadius->setVisible(visibility);
    labelGridSpacingRadius->setVisible(visibility);
    spinBoxGridSpacingRadius->setVisible(visibility);
    labelGridSpacingAngle->setVisible(visibility);
    spinBoxGridSpacingAngle->setVisible(visibility);

    QGridLayout* gridLayoutGridGeom = new QGridLayout(groupBoxGridGeom);
    gridLayoutGridGeom->addWidget(checkBoxGridLoadFromFile,   0, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(labelGridType,              1, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(comboBoxGridType,           1, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(checkBoxGridCenterOnOrigin, 2, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(labelGridCenterX,           3, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridCenterX,         3, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridCenterY,           4, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridCenterY,         4, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeX,             5, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeX,           5, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeY,             6, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeY,           6, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingX,          7, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingX,        7, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingY,          8, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingY,        8, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeRadius,        9, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeRadius,      9, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingRadius,    10, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingRadius,  10, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingAngle,     11, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingAngle,   11, 1, Qt::AlignRight);
    groupBoxGridGeom->setLayout(gridLayoutGridGeom);

    //Ruler Misc
    QGroupBox* groupBoxRulerMisc = new QGroupBox(tr("Ruler Misc"), widget);

    QCheckBox* checkBoxRulerShowOnLoad = new QCheckBox(tr("Initially show ruler when loading a file"), groupBoxRulerMisc);
    checkBoxRulerShowOnLoad->setChecked(dialog.ruler_show_on_load);
    connect(checkBoxRulerShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRulerShowOnLoadStateChanged(int)));

    QLabel* labelRulerMetric = new QLabel(tr("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric",   true);
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(dialog.ruler_metric));
    connect(comboBoxRulerMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRulerMetricCurrentIndexChanged(int)));

    QGridLayout* gridLayoutRulerMisc = new QGridLayout(widget);
    gridLayoutRulerMisc->addWidget(checkBoxRulerShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(labelRulerMetric,        1, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(comboBoxRulerMetric,     1, 1, Qt::AlignRight);
    groupBoxRulerMisc->setLayout(gridLayoutRulerMisc);

    //Ruler Color
    QGroupBox* groupBoxRulerColor = new QGroupBox(tr("Ruler Color"), widget);

    QLabel* labelRulerColor = new QLabel(tr("Ruler Color"), groupBoxRulerColor);
    labelRulerColor->setObjectName("labelRulerColor");
    QPushButton* buttonRulerColor = new QPushButton(tr("Choose"), groupBoxRulerColor);
    buttonRulerColor->setObjectName("buttonRulerColor");
    QPixmap rulerPix(16,16);
    rulerPix.fill(QColor(preview.ruler_color));
    buttonRulerColor->setIcon(QIcon(rulerPix));
    connect(buttonRulerColor, SIGNAL(clicked()), this, SLOT(chooseRulerColor()));

    QGridLayout* gridLayoutRulerColor = new QGridLayout(widget);
    gridLayoutRulerColor->addWidget(labelRulerColor,  1, 0, Qt::AlignLeft);
    gridLayoutRulerColor->addWidget(buttonRulerColor, 1, 1, Qt::AlignRight);
    groupBoxRulerColor->setLayout(gridLayoutRulerColor);

    //Ruler Geometry
    QGroupBox* groupBoxRulerGeom = new QGroupBox(tr("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = new QLabel(tr("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QDoubleSpinBox* spinBoxRulerPixelSize = new QDoubleSpinBox(groupBoxRulerGeom);
    spinBoxRulerPixelSize->setObjectName("spinBoxRulerPixelSize");
    spinBoxRulerPixelSize->setSingleStep(1.000);
    spinBoxRulerPixelSize->setRange(20.000, 100.000);
    spinBoxRulerPixelSize->setValue(dialog.ruler_pixel_size);
    connect(spinBoxRulerPixelSize, SIGNAL(valueChanged(double)), this, SLOT(spinBoxRulerPixelSizeValueChanged(double)));

    QGridLayout* gridLayoutRulerGeom = new QGridLayout(groupBoxRulerGeom);
    gridLayoutRulerGeom->addWidget(labelRulerPixelSize,   0, 0, Qt::AlignLeft);
    gridLayoutRulerGeom->addWidget(spinBoxRulerPixelSize, 0, 1, Qt::AlignRight);
    groupBoxRulerGeom->setLayout(gridLayoutRulerGeom);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxGridMisc);
    vboxLayoutMain->addWidget(groupBoxGridColor);
    vboxLayoutMain->addWidget(groupBoxGridGeom);
    vboxLayoutMain->addWidget(groupBoxRulerMisc);
    vboxLayoutMain->addWidget(groupBoxRulerColor);
    vboxLayoutMain->addWidget(groupBoxRulerGeom);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabOrthoPolar()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabQuickSnap()
{
    QWidget* widget = new QWidget(this);

    //QSnap Locators
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(tr("Locators Used"), widget);

    QCheckBox* checkBoxQSnapEndPoint = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_ENDPOINT);
    QCheckBox* checkBoxQSnapMidPoint = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_MIDPOINT);
    QCheckBox* checkBoxQSnapCenter = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_CENTER);
    QCheckBox* checkBoxQSnapNode = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_NODE);
    QCheckBox* checkBoxQSnapQuadrant = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_QUADRANT);
    QCheckBox* checkBoxQSnapIntersection = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_INTERSECTION);
    QCheckBox* checkBoxQSnapExtension = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_EXTENSION);
    QCheckBox* checkBoxQSnapInsertion = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_INSERTION);
    QCheckBox* checkBoxQSnapPerpendicular = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_PERPENDICULAR);
    QCheckBox* checkBoxQSnapTangent = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_TANGENT);
    QCheckBox* checkBoxQSnapNearest = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_NEAREST);
    QCheckBox* checkBoxQSnapApparent = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_APPARENT);
    QCheckBox* checkBoxQSnapParallel = basic_checkbox(groupBoxQSnapLoc, ST_QSNAP_PARALLEL);

    QPushButton* buttonQSnapSelectAll = new QPushButton(tr("Select All"), groupBoxQSnapLoc);
    connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(buttonQSnapSelectAllClicked()));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapEndPoint,      SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapMidPoint,      SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapCenter,        SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapNode,          SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapQuadrant,      SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapIntersection,  SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapExtension,     SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapInsertion,     SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapPerpendicular, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapTangent,       SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapNearest,       SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapApparent,      SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapParallel,      SLOT(setChecked(bool)));

    QPushButton* buttonQSnapClearAll = new QPushButton(tr("Clear All"), groupBoxQSnapLoc);
    connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapEndPoint,      SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapMidPoint,      SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapCenter,        SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapNode,          SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapQuadrant,      SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapIntersection,  SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapExtension,     SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapInsertion,     SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapPerpendicular, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapTangent,       SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapNearest,       SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapApparent,      SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapParallel,      SLOT(setChecked(bool)));

    QGridLayout* gridLayoutQSnap = new QGridLayout(groupBoxQSnapLoc);
    gridLayoutQSnap->addWidget(checkBoxQSnapEndPoint,      0, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapMidPoint,      1, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapCenter,        2, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapNode,          3, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapQuadrant,      4, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapIntersection,  5, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapExtension,     6, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapInsertion,     0, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapPerpendicular, 1, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapTangent,       2, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapNearest,       3, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapApparent,      4, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapParallel,      5, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapSelectAll,       0, 2, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapClearAll,        1, 2, Qt::AlignLeft);
    gridLayoutQSnap->setColumnStretch(2,1);
    groupBoxQSnapLoc->setLayout(gridLayoutQSnap);

    //QSnap Visual Config
    QGroupBox* groupBoxQSnapVisual = new QGroupBox(tr("Visual Configuration"), widget);

    QLabel* labelQSnapLocColor = new QLabel(tr("Locator Color"), groupBoxQSnapVisual);
    QComboBox* comboBoxQSnapLocColor = new QComboBox(groupBoxQSnapVisual);
    addColorsToComboBox(comboBoxQSnapLocColor);
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(dialog.qsnap_locator_color));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(tr("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    sliderQSnapLocSize->setValue(dialog.qsnap_locator_size);
    connect(sliderQSnapLocSize, SIGNAL(valueChanged(int)), this, SLOT(sliderQSnapLocatorSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutQSnapVisual = new QVBoxLayout(groupBoxQSnapVisual);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(comboBoxQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocSize);
    vboxLayoutQSnapVisual->addWidget(sliderQSnapLocSize);
    groupBoxQSnapVisual->setLayout(vboxLayoutQSnapVisual);

    //QSnap Sensitivity Config
    QGroupBox* groupBoxQSnapSensitivity = new QGroupBox(tr("Sensitivity"), widget);

    QLabel* labelQSnapApertureSize = new QLabel(tr("Aperture Size"), groupBoxQSnapSensitivity);
    QSlider* sliderQSnapApertureSize = new QSlider(Qt::Horizontal, groupBoxQSnapSensitivity);
    sliderQSnapApertureSize->setRange(1,20);
    sliderQSnapApertureSize->setValue(dialog.qsnap_aperture_size);
    connect(sliderQSnapApertureSize, SIGNAL(valueChanged(int)), this, SLOT(sliderQSnapApertureSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutQSnapSensitivity = new QVBoxLayout(groupBoxQSnapSensitivity);
    vboxLayoutQSnapSensitivity->addWidget(labelQSnapApertureSize);
    vboxLayoutQSnapSensitivity->addWidget(sliderQSnapApertureSize);
    groupBoxQSnapSensitivity->setLayout(vboxLayoutQSnapSensitivity);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxQSnapLoc);
    vboxLayoutMain->addWidget(groupBoxQSnapVisual);
    vboxLayoutMain->addWidget(groupBoxQSnapSensitivity);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabQuickTrack()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabLineWeight()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    //Misc
    QGroupBox* groupBoxLwtMisc = new QGroupBox(tr("LineWeight Misc"), widget);

    QGraphicsScene* s = activeScene();
    if (s) {
        dialog.lwt_show_lwt = s->property(ENABLE_LWT).toBool();
        preview.lwt_show_lwt = dialog.lwt_show_lwt;

        dialog.lwt_real_render = s->property(ENABLE_REAL).toBool();
        preview.lwt_real_render = dialog.lwt_real_render;
    }

    QCheckBox* checkBoxShowLwt = new QCheckBox(tr("Show LineWeight"), groupBoxLwtMisc);
    checkBoxShowLwt->setChecked(preview.lwt_show_lwt);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(tr("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    checkBoxRealRender->setChecked(preview.lwt_real_render);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(dialog.lwt_show_lwt);

    QLabel* labelDefaultLwt = new QLabel(tr("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //TODO: remove later
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    //TODO: populate the comboBox and set the initial value
    comboBoxDefaultLwt->addItem(QString().setNum(dialog.lwt_default_lwt, 'F', 2).append(" mm"), dialog.lwt_default_lwt);
    comboBoxDefaultLwt->setEnabled(false); //TODO: remove later

    QVBoxLayout* vboxLayoutLwtMisc = new QVBoxLayout(groupBoxLwtMisc);
    vboxLayoutLwtMisc->addWidget(checkBoxShowLwt);
    vboxLayoutLwtMisc->addWidget(checkBoxRealRender);
    vboxLayoutLwtMisc->addWidget(labelDefaultLwt);
    vboxLayoutLwtMisc->addWidget(comboBoxDefaultLwt);
    groupBoxLwtMisc->setLayout(vboxLayoutLwtMisc);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxLwtMisc);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget* Settings_Dialog::createTabSelection()
{
    QWidget* widget = new QWidget(this);

    //Selection Modes
    QGroupBox* groupBoxSelectionModes = new QGroupBox(tr("Modes"), widget);

    QCheckBox* checkBoxSelectionModePickFirst = new QCheckBox(tr("Allow Preselection (PickFirst)"), groupBoxSelectionModes);
    checkBoxSelectionModePickFirst->setChecked(dialog.selection_mode_pickfirst);
    checkBoxSelectionModePickFirst->setChecked(true); checkBoxSelectionModePickFirst->setEnabled(false); //TODO: Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickFirstStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickAdd = new QCheckBox(tr("Add to Selection (PickAdd)"), groupBoxSelectionModes);
    checkBoxSelectionModePickAdd->setChecked(dialog.selection_mode_pickadd);
    connect(checkBoxSelectionModePickAdd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickAddStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickDrag = new QCheckBox(tr("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    checkBoxSelectionModePickDrag->setChecked(dialog.selection_mode_pickdrag);
    checkBoxSelectionModePickDrag->setChecked(false); checkBoxSelectionModePickDrag->setEnabled(false); //TODO: Remove this line when this functionality is available
    connect(checkBoxSelectionModePickDrag, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickDragStateChanged(int)));

    QVBoxLayout* vboxLayoutSelectionModes = new QVBoxLayout(groupBoxSelectionModes);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickFirst);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickAdd);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickDrag);
    groupBoxSelectionModes->setLayout(vboxLayoutSelectionModes);

    //Selection Colors
    QGroupBox* groupBoxSelectionColors = new QGroupBox(tr("Colors"), widget);

    QLabel* labelCoolGripColor = new QLabel(tr("Cool Grip (Unselected)"), groupBoxSelectionColors);
    QComboBox* comboBoxCoolGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxCoolGripColor);
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(dialog.selection_coolgrip_color));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionCoolGripColorCurrentIndexChanged(int)));

    QLabel* labelHotGripColor = new QLabel(tr("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(dialog.selection_hotgrip_color));
    connect(comboBoxHotGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionHotGripColorCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutSelectionColors = new QVBoxLayout(groupBoxSelectionColors);
    vboxLayoutSelectionColors->addWidget(labelCoolGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxCoolGripColor);
    vboxLayoutSelectionColors->addWidget(labelHotGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxHotGripColor);
    groupBoxSelectionColors->setLayout(vboxLayoutSelectionColors);

    //Selection Sizes
    QGroupBox* groupBoxSelectionSizes = new QGroupBox(tr("Sizes"), widget);

    QLabel* labelSelectionGripSize = new QLabel(tr("Grip Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionGripSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionGripSize->setRange(1,20);
    sliderSelectionGripSize->setValue(dialog.selection_grip_size);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionGripSizeValueChanged(int)));

    QLabel* labelSelectionPickBoxSize = new QLabel(tr("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    sliderSelectionPickBoxSize->setValue(dialog.selection_pickbox_size);
    connect(sliderSelectionPickBoxSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionPickBoxSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutSelectionSizes = new QVBoxLayout(groupBoxSelectionSizes);
    vboxLayoutSelectionSizes->addWidget(labelSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(labelSelectionPickBoxSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionPickBoxSize);
    groupBoxSelectionSizes->setLayout(vboxLayoutSelectionSizes);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxSelectionModes);
    vboxLayoutMain->addWidget(groupBoxSelectionColors);
    vboxLayoutMain->addWidget(groupBoxSelectionSizes);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/* Combobox callback for selecting colors.
 *
 * @todo Add Other... so the user can select custom colors.
 */
void
Settings_Dialog::addColorsToComboBox(QComboBox* comboBox)
{
    comboBox->addItem(_mainWin->createIcon("colorred"), tr("Red"), qRgb(255, 0, 0));
    comboBox->addItem(_mainWin->createIcon("coloryellow"), tr("Yellow"), qRgb(255, 255, 0));
    comboBox->addItem(_mainWin->createIcon("colorgreen"), tr("Green"), qRgb(0, 255, 0));
    comboBox->addItem(_mainWin->createIcon("colorcyan"), tr("Cyan"), qRgb(0, 255, 255));
    comboBox->addItem(_mainWin->createIcon("colorblue"), tr("Blue"), qRgb(0, 0, 255));
    comboBox->addItem(_mainWin->createIcon("colormagenta"), tr("Magenta"), qRgb(255, 0, 255));
    comboBox->addItem(_mainWin->createIcon("colorwhite"), tr("White"), qRgb(255, 255, 255));
}

/* Combobox callback for the general interface language */
void
Settings_Dialog::comboBoxLanguageCurrentIndexChanged(const QString& lang)
{
    st_dialog[ST_LANGUAGE].s = qPrintable(lang.toLower());
}

/* Combobox callback for the general interface icon theme. */
void
Settings_Dialog::comboBoxIconThemeCurrentIndexChanged(const QString& theme)
{
    st_dialog[ST_ICON_THEME].s = qPrintable(theme);
}

/* Combobox callback for the general interface icon size.
 *
 * Defaults to 16 pixels wide.
 */
void
Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        st_dialog[ST_ICON_SIZE].i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            st_dialog[ST_ICON_SIZE].i = 16;
        }
    }
    else {
        st_dialog[ST_ICON_SIZE].i = 16;
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    preview.general_mdi_bg_use_logo = checked;
    mdiArea->useBackgroundLogo(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        tr("Images (*.bmp *.png *.jpg)"));

        if(!selectedImage.isNull())
            accept_.general_mdi_bg_logo = selectedImage;

        //Update immediately so it can be previewed
        mdiArea->setBackgroundLogo(accept_.general_mdi_bg_logo);
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    preview.general_mdi_bg_use_texture = checked;
    mdiArea->useBackgroundTexture(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        tr("Images (*.bmp *.png *.jpg)"));

        if(!selectedImage.isNull())
            accept_.general_mdi_bg_texture = selectedImage;

        //Update immediately so it can be previewed
        mdiArea->setBackgroundTexture(accept_.general_mdi_bg_texture);
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    preview.general_mdi_bg_use_color = checked;
    mdiArea->useBackgroundColor(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.general_mdi_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGeneralMdiBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.general_mdi_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.general_mdi_bg_color));
            button->setIcon(QIcon(pix));
            mdiArea->setBackgroundColor(QColor(accept_.general_mdi_bg_color));
        }
        else
        {
            mdiArea->setBackgroundColor(QColor(dialog.general_mdi_bg_color));
        }
    }
}

void Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    preview.general_mdi_bg_color = color.rgb();
    mdiArea->setBackgroundColor(QColor(preview.general_mdi_bg_color));
}

void Settings_Dialog::checkBoxTipOfTheDayStateChanged(int checked)
{
    dialog.general_tip_of_the_day = checked;
}

void Settings_Dialog::checkBoxShowScrollBarsStateChanged(int checked)
{
    preview.display_show_scrollbars = checked;
    _mainWin->updateAllViewScrollBars(preview.display_show_scrollbars);
}

void Settings_Dialog::comboBoxScrollBarWidgetCurrentIndexChanged(int index)
{
    dialog.display_scrollbar_widget_num = index;
}

void Settings_Dialog::spinBoxZoomScaleInValueChanged(double value)
{
    dialog.display_zoomscale_in = value;
}

void Settings_Dialog::spinBoxZoomScaleOutValueChanged(double value)
{
    dialog.display_zoomscale_out = value;
}

void Settings_Dialog::checkBoxDisableBGStateChanged(int checked)
{
    dialog.printing_disable_bg = checked;
}

void Settings_Dialog::chooseDisplayCrossHairColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_crosshair_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayCrossHairColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.display_crosshair_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_crosshair_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewCrossHairColors(accept_.display_crosshair_color);
        }
        else
        {
            _mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
        }
    }
}

void Settings_Dialog::currentDisplayCrossHairColorChanged(const QColor& color)
{
    preview.display_crosshair_color = color.rgb();
    _mainWin->updateAllViewCrossHairColors(preview.display_crosshair_color);
}

void Settings_Dialog::chooseDisplayBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.display_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_bg_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewBackgroundColors(accept_.display_bg_color);
        }
        else
        {
            _mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
        }
    }
}

void Settings_Dialog::currentDisplayBackgroundColorChanged(const QColor& color)
{
    preview.display_bg_color = color.rgb();
    _mainWin->updateAllViewBackgroundColors(preview.display_bg_color);
}

void Settings_Dialog::chooseDisplaySelectBoxLeftColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_left_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.display_selectbox_left_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_selectbox_left_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                                  accept_.display_selectbox_left_fill,
                                                  accept_.display_selectbox_right_color,
                                                  accept_.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
        else
        {
            _mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                                  dialog.display_selectbox_left_fill,
                                                  dialog.display_selectbox_right_color,
                                                  dialog.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftColorChanged(const QColor& color)
{
    preview.display_selectbox_left_color = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(preview.display_selectbox_left_color,
                                          preview.display_selectbox_left_fill,
                                          preview.display_selectbox_right_color,
                                          preview.display_selectbox_right_fill,
                                          preview.display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxLeftFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button == NULL) {
        return;
    }

    QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_left_fill), this);
    connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftFillChanged(const QColor&)));
    colorDialog->exec();

    if (colorDialog->result() == QDialog::Accepted) {
        accept_.display_selectbox_left_fill = colorDialog->selectedColor().rgb();
        QPixmap pix(16,16);
        pix.fill(QColor(accept_.display_selectbox_left_fill));
        button->setIcon(QIcon(pix));
        _mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                              accept_.display_selectbox_left_fill,
                                              accept_.display_selectbox_right_color,
                                              accept_.display_selectbox_right_fill,
                                              preview.display_selectbox_alpha);
    }
    else {
        _mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                              dialog.display_selectbox_left_fill,
                                              dialog.display_selectbox_right_color,
                                              dialog.display_selectbox_right_fill,
                                              preview.display_selectbox_alpha);
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftFillChanged(const QColor& color)
{
    preview.display_selectbox_left_fill = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(preview.display_selectbox_left_color,
                                          preview.display_selectbox_left_fill,
                                          preview.display_selectbox_right_color,
                                          preview.display_selectbox_right_fill,
                                          preview.display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxRightColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_right_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.display_selectbox_right_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_selectbox_right_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                                  accept_.display_selectbox_left_fill,
                                                  accept_.display_selectbox_right_color,
                                                  accept_.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
        else
        {
            _mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                                  dialog.display_selectbox_left_fill,
                                                  dialog.display_selectbox_right_color,
                                                  dialog.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxRightColorChanged(const QColor& color)
{
    preview.display_selectbox_right_color = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(preview.display_selectbox_left_color,
                                          preview.display_selectbox_left_fill,
                                          preview.display_selectbox_right_color,
                                          preview.display_selectbox_right_fill,
                                          preview.display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxRightFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_right_fill), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightFillChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.display_selectbox_right_fill = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_selectbox_right_fill));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                                  accept_.display_selectbox_left_fill,
                                                  accept_.display_selectbox_right_color,
                                                  accept_.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
        else
        {
            _mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                                  dialog.display_selectbox_left_fill,
                                                  dialog.display_selectbox_right_color,
                                                  dialog.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxRightFillChanged(const QColor& color)
{
    preview.display_selectbox_right_fill = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(preview.display_selectbox_left_color,
                                          preview.display_selectbox_left_fill,
                                          preview.display_selectbox_right_color,
                                          preview.display_selectbox_right_fill,
                                          preview.display_selectbox_alpha);
}

void Settings_Dialog::spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    preview.display_selectbox_alpha = value;
    _mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                          accept_.display_selectbox_left_fill,
                                          accept_.display_selectbox_right_color,
                                          accept_.display_selectbox_right_fill,
                                          preview.display_selectbox_alpha);
}

void Settings_Dialog::choosePromptTextColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.prompt_text_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentPromptTextColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.prompt_text_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.prompt_text_color));
            button->setIcon(QIcon(pix));
            prompt->setPromptTextColor(QColor(accept_.prompt_text_color));
        }
        else
        {
            prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
        }
    }
}

void Settings_Dialog::currentPromptTextColorChanged(const QColor& color)
{
    preview.prompt_text_color = color.rgb();
    prompt->setPromptTextColor(QColor(preview.prompt_text_color));
}

void Settings_Dialog::choosePromptBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.prompt_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentPromptBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.prompt_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.prompt_bg_color));
            button->setIcon(QIcon(pix));
            prompt->setPromptBackgroundColor(QColor(accept_.prompt_bg_color));
        }
        else
        {
            prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
        }
    }
}

void Settings_Dialog::currentPromptBackgroundColorChanged(const QColor& color)
{
    preview.prompt_bg_color = color.rgb();
    prompt->setPromptBackgroundColor(QColor(preview.prompt_bg_color));
}

void Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(const QString& family)
{
    preview.prompt_font_family = family;
    prompt->setPromptFontFamily(preview.prompt_font_family);
}

void Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(const QString& style)
{
    preview.prompt_font_style = style;
    prompt->setPromptFontStyle(preview.prompt_font_style);
}

void Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    preview.prompt_font_size = value;
    prompt->setPromptFontSize(preview.prompt_font_size);
}

void Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if(checkBox)
    {
        QString format = checkBox->text();
        qDebug("CustomFilter: %s %d", qPrintable(format), checked);
        if(checked)
            dialog.opensave_custom_filter.append(" *." + format.toLower());
        else
            dialog.opensave_custom_filter.remove("*." + format, Qt::CaseInsensitive);
        //dialog.opensave_custom_filter = checked; //TODO
    }
}

void Settings_Dialog::buttonCustomFilterSelectAllClicked()
{
    emit buttonCustomFilterSelectAll(true);
    dialog.opensave_custom_filter = "supported";
}

void Settings_Dialog::buttonCustomFilterClearAllClicked()
{
    emit buttonCustomFilterClearAll(false);
    dialog.opensave_custom_filter.clear();
}

void Settings_Dialog::spinBoxRecentMaxFilesValueChanged(int value)
{
    dialog.opensave_recent_max_files = value;
}

void Settings_Dialog::spinBoxTrimDstNumJumpsValueChanged(int value)
{
    dialog.opensave_trim_dst_num_jumps = value;
}

void Settings_Dialog::checkBoxGridShowOnLoadStateChanged(int checked)
{
    dialog.grid_show_on_load = checked;
}

void Settings_Dialog::checkBoxGridShowOriginStateChanged(int checked)
{
    dialog.grid_show_origin = checked;
}

void Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    dialog.grid_color_match_crosshair = checked;
    if(dialog.grid_color_match_crosshair) { _mainWin->updateAllViewGridColors(accept_.display_crosshair_color); }
    else                                  { _mainWin->updateAllViewGridColors(accept_.grid_color);              }

    QObject* senderObj = sender();
    if(senderObj)
    {
        QObject* parent = senderObj->parent();
        if(parent)
        {
            QLabel* labelGridColor = parent->findChild<QLabel*>("labelGridColor");
            if(labelGridColor) labelGridColor->setEnabled(!dialog.grid_color_match_crosshair);
            QPushButton* buttonGridColor = parent->findChild<QPushButton*>("buttonGridColor");
            if(buttonGridColor) buttonGridColor->setEnabled(!dialog.grid_color_match_crosshair);
        }
    }
}

void Settings_Dialog::chooseGridColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.grid_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGridColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.grid_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.grid_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewGridColors(accept_.grid_color);
        }
        else
        {
            _mainWin->updateAllViewGridColors(dialog.grid_color);
        }
    }
}

void Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    preview.grid_color = color.rgb();
    _mainWin->updateAllViewGridColors(preview.grid_color);
}

void Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    dialog.grid_load_from_file = checked;

    QObject* senderObj = sender();
    if(senderObj)
    {
        QObject* parent = senderObj->parent();
        if(parent)
        {
            QLabel* labelGridType = parent->findChild<QLabel*>("labelGridType");
            if(labelGridType) labelGridType->setEnabled(!dialog.grid_load_from_file);
            QComboBox* comboBoxGridType = parent->findChild<QComboBox*>("comboBoxGridType");
            if(comboBoxGridType) comboBoxGridType->setEnabled(!dialog.grid_load_from_file);
            QCheckBox* checkBoxGridCenterOnOrigin = parent->findChild<QCheckBox*>("checkBoxGridCenterOnOrigin");
            if(checkBoxGridCenterOnOrigin) checkBoxGridCenterOnOrigin->setEnabled(!dialog.grid_load_from_file);
            QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
            if(labelGridCenterX) labelGridCenterX->setEnabled(!dialog.grid_load_from_file && !dialog.grid_center_on_origin);
            QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
            if(spinBoxGridCenterX) spinBoxGridCenterX->setEnabled(!dialog.grid_load_from_file && !dialog.grid_center_on_origin);
            QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
            if(labelGridCenterY) labelGridCenterY->setEnabled(!dialog.grid_load_from_file && !dialog.grid_center_on_origin);
            QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
            if(spinBoxGridCenterY) spinBoxGridCenterY->setEnabled(!dialog.grid_load_from_file && !dialog.grid_center_on_origin);
            QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
            if(labelGridSizeX) labelGridSizeX->setEnabled(!dialog.grid_load_from_file);
            QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
            if(spinBoxGridSizeX) spinBoxGridSizeX->setEnabled(!dialog.grid_load_from_file);
            QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
            if(labelGridSizeY) labelGridSizeY->setEnabled(!dialog.grid_load_from_file);
            QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
            if(spinBoxGridSizeY) spinBoxGridSizeY->setEnabled(!dialog.grid_load_from_file);
            QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
            if(labelGridSpacingX) labelGridSpacingX->setEnabled(!dialog.grid_load_from_file);
            QDoubleSpinBox* spinBoxGridSpacingX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingX");
            if(spinBoxGridSpacingX) spinBoxGridSpacingX->setEnabled(!dialog.grid_load_from_file);
            QLabel* labelGridSpacingY = parent->findChild<QLabel*>("labelGridSpacingY");
            if(labelGridSpacingY) labelGridSpacingY->setEnabled(!dialog.grid_load_from_file);
            QDoubleSpinBox* spinBoxGridSpacingY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingY");
            if(spinBoxGridSpacingY) spinBoxGridSpacingY->setEnabled(!dialog.grid_load_from_file);
            QLabel* labelGridSizeRadius = parent->findChild<QLabel*>("labelGridSizeRadius");
            if(labelGridSizeRadius) labelGridSizeRadius->setEnabled(!dialog.grid_load_from_file);
            QDoubleSpinBox* spinBoxGridSizeRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeRadius");
            if(spinBoxGridSizeRadius) spinBoxGridSizeRadius->setEnabled(!dialog.grid_load_from_file);
            QLabel* labelGridSpacingRadius = parent->findChild<QLabel*>("labelGridSpacingRadius");
            if(labelGridSpacingRadius) labelGridSpacingRadius->setEnabled(!dialog.grid_load_from_file);
            QDoubleSpinBox* spinBoxGridSpacingRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingRadius");
            if(spinBoxGridSpacingRadius) spinBoxGridSpacingRadius->setEnabled(!dialog.grid_load_from_file);
            QLabel* labelGridSpacingAngle = parent->findChild<QLabel*>("labelGridSpacingAngle");
            if(labelGridSpacingAngle) labelGridSpacingAngle->setEnabled(!dialog.grid_load_from_file);
            QDoubleSpinBox* spinBoxGridSpacingAngle = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingAngle");
            if(spinBoxGridSpacingAngle) spinBoxGridSpacingAngle->setEnabled(!dialog.grid_load_from_file);
        }
    }
}

void Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(const QString& type)
{
    dialog.grid_type = type;

    QObject* senderObj = sender();
    if (senderObj == NULL) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (parent == NULL) {
        return;
    }
    bool visibility = false;
    if (type == "Circular") {
        visibility = true;
    }

    set_label_visible(parent, "labelGridSizeX", !visibility);
    set_double_spinbox_visible(parent, "spinBoxGridSizeX", !visibility);
    set_label_visible(parent, "labelGridSizeY", !visibility);
    set_double_spinbox_visible(parent, "spinBoxGridSizeY", !visibility);
    set_label_visible(parent, "labelGridSpacingX", !visibility);
    set_double_spinbox_visible(parent, "spinBoxGridSpacingX", !visibility);
    set_label_visible(parent, "labelGridSpacingY", !visibility);
    set_double_spinbox_visible(parent, "spinBoxGridSpacingY", !visibility);

    set_label_visible(parent, "labelGridSizeRadius", visibility);
    set_double_spinbox_visible(parent, "spinBoxGridSizeRadius", visibility);
    set_label_visible(parent, "labelGridSpacingRadius", visibility);
    set_double_spinbox_visible(parent, "spinBoxGridSpacingRadius", visibility);
    set_label_visible(parent, "labelGridSpacingAngle", visibility);
    set_double_spinbox_visible(parent, "spinBoxGridSpacingAngle", visibility);
}

void Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    dialog.grid_center_on_origin = checked;

    QObject* senderObj = sender();
    if (senderObj == NULL) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (parent == NULL) {
        return;
    }

    QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
    if(labelGridCenterX) labelGridCenterX->setEnabled(!dialog.grid_center_on_origin);
    QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
    if(spinBoxGridCenterX) spinBoxGridCenterX->setEnabled(!dialog.grid_center_on_origin);
    QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
    if(labelGridCenterY) labelGridCenterY->setEnabled(!dialog.grid_center_on_origin);
    QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
    if(spinBoxGridCenterY) spinBoxGridCenterY->setEnabled(!dialog.grid_center_on_origin);
}

void Settings_Dialog::checkBoxRulerShowOnLoadStateChanged(int checked)
{
    dialog.ruler_show_on_load = checked;
}

void Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if(comboBox)
    {
        bool ok = 0;
        dialog.ruler_metric = comboBox->itemData(index).toBool();
    }
    else
        dialog.ruler_metric = true;
}

void Settings_Dialog::chooseRulerColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.ruler_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentRulerColorChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.ruler_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.ruler_color));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewRulerColors(accept_.ruler_color);
        }
        else
        {
            _mainWin->updateAllViewRulerColors(dialog.ruler_color);
        }
    }
}

void Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    preview.ruler_color = color.rgb();
    _mainWin->updateAllViewRulerColors(preview.ruler_color);
}

void Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    dialog.ruler_pixel_size = value;
}

void Settings_Dialog::buttonQSnapSelectAllClicked()
{
    emit buttonQSnapSelectAll(true);
}

void Settings_Dialog::buttonQSnapClearAllClicked()
{
    emit buttonQSnapClearAll(false);
}

void Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,255,0); //Yellow
    if(comboBox)
    {
        bool ok = 0;
        dialog.qsnap_locator_color = comboBox->itemData(index).toUInt(&ok);
        if(!ok)
            dialog.qsnap_locator_color = defaultColor;
    }
    else
        dialog.qsnap_locator_color = defaultColor;
}

void Settings_Dialog::sliderQSnapLocatorSizeValueChanged(int value)
{
    dialog.qsnap_locator_size = value;
}

void Settings_Dialog::sliderQSnapApertureSizeValueChanged(int value)
{
    dialog.qsnap_aperture_size = value;
}

void Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    preview.lwt_show_lwt = checked;
    if(preview.lwt_show_lwt) { statusbar->statusBarLwtButton->enableLwt(); }
    else                     { statusbar->statusBarLwtButton->disableLwt(); }

    QObject* senderObj = sender();
    if(senderObj)
    {
        QObject* parent = senderObj->parent();
        if(parent)
        {
            QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
            if(checkBoxRealRender) checkBoxRealRender->setEnabled(preview.lwt_show_lwt);
        }
    }
}

void Settings_Dialog::checkBoxLwtRealRenderStateChanged(int checked)
{
    preview.lwt_real_render = checked;
    if(preview.lwt_real_render) { statusbar->statusBarLwtButton->enableReal(); }
    else                        { statusbar->statusBarLwtButton->disableReal(); }
}

void Settings_Dialog::checkBoxSelectionModePickFirstStateChanged(int checked)
{
    dialog.selection_mode_pickfirst = checked;
}

void Settings_Dialog::checkBoxSelectionModePickAddStateChanged(int checked)
{
    dialog.selection_mode_pickadd = checked;
}

void Settings_Dialog::checkBoxSelectionModePickDragStateChanged(int checked)
{
    dialog.selection_mode_pickdrag = checked;
}

void Settings_Dialog::sliderSelectionGripSizeValueChanged(int value)
{
    dialog.selection_grip_size = value;
}

void Settings_Dialog::sliderSelectionPickBoxSizeValueChanged(int value)
{
    dialog.selection_pickbox_size = value;
}

void Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(0,0,255); //Blue
    if(comboBox)
    {
        bool ok = 0;
        dialog.selection_coolgrip_color = comboBox->itemData(index).toUInt(&ok);
        if(!ok)
            dialog.selection_coolgrip_color = defaultColor;
    }
    else
        dialog.selection_coolgrip_color = defaultColor;
}

void Settings_Dialog::comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,0,0); //Red
    if(comboBox)
    {
        bool ok = 0;
        dialog.selection_hotgrip_color = comboBox->itemData(index).toUInt(&ok);
        if(!ok)
            dialog.selection_hotgrip_color = defaultColor;
    }
    else
        dialog.selection_hotgrip_color = defaultColor;
}

/* Update the interface settings to the dialog state. */
void
Settings_Dialog::acceptChanges(void)
{
    dialog.general_mdi_bg_use_logo = preview.general_mdi_bg_use_logo;
    dialog.general_mdi_bg_use_texture = preview.general_mdi_bg_use_texture;
    dialog.general_mdi_bg_use_color = preview.general_mdi_bg_use_color;
    dialog.general_mdi_bg_logo = accept_.general_mdi_bg_logo;
    dialog.general_mdi_bg_texture = accept_.general_mdi_bg_texture;
    dialog.general_mdi_bg_color = accept_.general_mdi_bg_color;
    dialog.display_show_scrollbars = preview.display_show_scrollbars;
    dialog.display_crosshair_color = accept_.display_crosshair_color;
    dialog.display_bg_color = accept_.display_bg_color;
    dialog.display_selectbox_left_color = accept_.display_selectbox_left_color;
    dialog.display_selectbox_left_fill = accept_.display_selectbox_left_fill;
    dialog.display_selectbox_right_color = accept_.display_selectbox_right_color;
    dialog.display_selectbox_right_fill = accept_.display_selectbox_right_fill;
    dialog.display_selectbox_alpha = preview.display_selectbox_alpha;
    dialog.prompt_text_color = accept_.prompt_text_color;
    dialog.prompt_bg_color = accept_.prompt_bg_color;
    dialog.prompt_font_family = preview.prompt_font_family;
    dialog.prompt_font_style = preview.prompt_font_style;
    dialog.prompt_font_size = preview.prompt_font_size;
    if(dialog.grid_color_match_crosshair) dialog.grid_color = accept_.display_crosshair_color;
    else                                  dialog.grid_color = accept_.grid_color;
    dialog.ruler_color = accept_.ruler_color;
    dialog.lwt_show_lwt = preview.lwt_show_lwt;
    dialog.lwt_real_render = preview.lwt_real_render;

    for (int i=0; i<N_SETTINGS; i++) {
        st[i] = st_dialog[i];
    }

    //Make sure the user sees the changes applied immediately
    mdiArea->useBackgroundLogo(dialog.general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(dialog.general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(dialog.general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(dialog.general_mdi_bg_logo);
    mdiArea->setBackgroundTexture(dialog.general_mdi_bg_texture);
    mdiArea->setBackgroundColor(dialog.general_mdi_bg_color);
    _mainWin->iconResize(st_dialog[ST_ICON_SIZE].i);
    _mainWin->updateAllViewScrollBars(dialog.display_show_scrollbars);
    _mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
    _mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
    _mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                          dialog.display_selectbox_left_fill,
                                          dialog.display_selectbox_right_color,
                                          dialog.display_selectbox_right_fill,
                                          dialog.display_selectbox_alpha);
    prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
    prompt->setPromptFontFamily(dialog.prompt_font_family);
    prompt->setPromptFontStyle(dialog.prompt_font_style);
    prompt->setPromptFontSize(dialog.prompt_font_size);
    _mainWin->updateAllViewGridColors(dialog.grid_color);
    _mainWin->updateAllViewRulerColors(dialog.ruler_color);
    if(dialog.lwt_show_lwt) { statusbar->statusBarLwtButton->enableLwt(); }
    else                    { statusbar->statusBarLwtButton->disableLwt(); }
    if(dialog.lwt_real_render) { statusbar->statusBarLwtButton->enableReal(); }
    else                       { statusbar->statusBarLwtButton->disableReal(); }
    _mainWin->updatePickAddMode(dialog.selection_mode_pickadd);

    _mainWin->writeSettings();
    accept();
}

/* Reset all fo the interface settings to ensure everything is displayed as before. */
void
Settings_Dialog::rejectChanges(void)
{
    //TODO: inform the user if they have changed settings

    //Update the view since the user must accept the preview
    mdiArea->useBackgroundLogo(dialog.general_mdi_bg_use_logo);
    mdiArea->useBackgroundTexture(dialog.general_mdi_bg_use_texture);
    mdiArea->useBackgroundColor(dialog.general_mdi_bg_use_color);
    mdiArea->setBackgroundLogo(dialog.general_mdi_bg_logo);
    mdiArea->setBackgroundTexture(dialog.general_mdi_bg_texture);
    mdiArea->setBackgroundColor(dialog.general_mdi_bg_color);
    _mainWin->updateAllViewScrollBars(dialog.display_show_scrollbars);
    _mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
    _mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
    _mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                          dialog.display_selectbox_left_fill,
                                          dialog.display_selectbox_right_color,
                                          dialog.display_selectbox_right_fill,
                                          dialog.display_selectbox_alpha);
    prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
    prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
    prompt->setPromptFontFamily(dialog.prompt_font_family);
    prompt->setPromptFontStyle(dialog.prompt_font_style);
    prompt->setPromptFontSize(dialog.prompt_font_size);
    _mainWin->updateAllViewGridColors(dialog.grid_color);
    _mainWin->updateAllViewRulerColors(dialog.ruler_color);
    if (dialog.lwt_show_lwt) {
        statusbar->statusBarLwtButton->enableLwt();
    }
    else {
        statusbar->statusBarLwtButton->disableLwt();
    }
    if (dialog.lwt_real_render) {
        statusbar->statusBarLwtButton->enableReal();
    }
    else {
        statusbar->statusBarLwtButton->disableReal();
    }

    reject();
}

