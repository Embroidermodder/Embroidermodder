/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Settings Dialog
 */

#include "embroidermodder.h"

QStringList extensions = {
    "100",
    "10o",
    "ART",
    "BMC",
    "BRO",
    "CND",
    "COL",
    "CSD",
    "CSV",
    "DAT",
    "DEM",
    "DSB",
    "DST",
    "DSZ",
    "DXF",
    "EDR",
    "EMD",
    "EXP",
    "EXY",
    "EYS",
    "FXY",
    "GNC",
    "GT",
    "HUS",
    "INB",
    "JEF",
    "KSM",
    "PCD",
    "PCM",
    "PCQ",
    "PCS",
    "PEC",
    "PEL",
    "PEM",
    "PES",
    "PHB",
    "PHC",
    "RGB",
    "SEW",
    "SHV",
    "SST",
    "STX",
    "SVG",
    "T09",
    "TAP",
    "THR",
    "TXT",
    "U00",
    "U01",
    "VIP",
    "VP3",
    "XXX",
    "ZSK"
};

QIcon
create_swatch(int32_t color)
{
    QPixmap pixmap(16, 16);
    pixmap.fill(QColor(color));
    return QIcon(pixmap); 
}

QPushButton *
choose_color_button(QGroupBox* groupbox, IntSetting* color_setting)
{
    QPushButton* button = new QPushButton(_main->tr("Choose"), groupbox);
    color_setting->dialog = color_setting->setting;
    color_setting->preview = color_setting->dialog;
    color_setting->accept = color_setting->dialog;
    button->setIcon(create_swatch(color_setting->preview));
    return button;
}

Settings_Dialog::Settings_Dialog(MainWindow* mw, const QString& showTab, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    tabWidget = new QTabWidget(this);

    //TODO: Add icons to tabs
    tabWidget->addTab(createTabGeneral(), tr("General"));
    tabWidget->addTab(createTabFilesPaths(), tr("Files/Paths"));
    tabWidget->addTab(createTabDisplay(), tr("Display"));
    tabWidget->addTab(createTabPrompt(), tr("Prompt"));
    tabWidget->addTab(createTabOpenSave(), tr("Open/Save"));
    tabWidget->addTab(createTabPrinting(), tr("Printing"));
    tabWidget->addTab(createTabSnap(), tr("Snap"));
    tabWidget->addTab(createTabGridRuler(), tr("Grid/Ruler"));
    tabWidget->addTab(createTabOrthoPolar(), tr("Ortho/Polar"));
    tabWidget->addTab(createTabQuickSnap(), tr("QuickSnap"));
    tabWidget->addTab(createTabQuickTrack(), tr("QuickTrack"));
    tabWidget->addTab(createTabLineWeight(), tr("LineWeight"));
    tabWidget->addTab(createTabSelection(), tr("Selection"));

    if     (showTab == "General")     tabWidget->setCurrentIndex( 0);
    else if (showTab == "Files/Path")  tabWidget->setCurrentIndex( 1);
    else if (showTab == "Display")     tabWidget->setCurrentIndex( 2);
    else if (showTab == "Prompt")      tabWidget->setCurrentIndex( 3);
    else if (showTab == "Open/Save")   tabWidget->setCurrentIndex( 4);
    else if (showTab == "Printing")    tabWidget->setCurrentIndex( 5);
    else if (showTab == "Snap")        tabWidget->setCurrentIndex( 6);
    else if (showTab == "Grid/Ruler")  tabWidget->setCurrentIndex( 7);
    else if (showTab == "Ortho/Polar") tabWidget->setCurrentIndex( 8);
    else if (showTab == "QuickSnap")   tabWidget->setCurrentIndex( 9);
    else if (showTab == "QuickTrack")  tabWidget->setCurrentIndex(10);
    else if (showTab == "LineWeight")  tabWidget->setCurrentIndex(11);
    else if (showTab == "Selection")   tabWidget->setCurrentIndex(12);

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

QWidget* Settings_Dialog::createTabGeneral()
{
    QWidget* widget = new QWidget(this);

    //Language
    QGroupBox* groupBoxLanguage = new QGroupBox(tr("Language"), widget);

    QLabel* labelLanguage = new QLabel(tr("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    strcpy(general_language.dialog,
        qPrintable(QString(general_language.setting).toLower()));
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
    QString current = general_language.dialog;
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxLanguageCurrentIndexChanged(const QString&)));

    QVBoxLayout* vboxLayoutLanguage = new QVBoxLayout(groupBoxLanguage);
    vboxLayoutLanguage->addWidget(labelLanguage);
    vboxLayoutLanguage->addWidget(comboBoxLanguage);
    groupBoxLanguage->setLayout(vboxLayoutLanguage);

    //Icons
    QGroupBox* groupBoxIcon = new QGroupBox(tr("Icons"), widget);

    QLabel* labelIconTheme = new QLabel(tr("Icon Theme"), groupBoxIcon);
    QComboBox* comboBoxIconTheme = new QComboBox(groupBoxIcon);
    QDir dir(qApp->applicationDirPath());
    dir.cd("icons");
    strcpy(general_icon_theme.dialog, general_icon_theme.setting);
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/" + "theme" + ".png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(general_icon_theme.dialog));
    connect(comboBoxIconTheme, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxIconThemeCurrentIndexChanged(const QString&)));

    QLabel* labelIconSize = new QLabel(tr("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    QString dialog_icon("icons/");
    dialog_icon += general_icon_theme.dialog;
    dialog_icon += "/";
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon16"  + ".png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon24"  + ".png"), "Small", 24);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon32"  + ".png"), "Medium", 32);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon48"  + ".png"), "Large", 48);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon64"  + ".png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon128" + ".png"), "I'm Blind", 128);
    general_icon_size.dialog = general_icon_size.setting;
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(general_icon_size.dialog));
    connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIconSizeCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutIcon = new QVBoxLayout(groupBoxIcon);
    vboxLayoutIcon->addWidget(labelIconTheme);
    vboxLayoutIcon->addWidget(comboBoxIconTheme);
    vboxLayoutIcon->addWidget(labelIconSize);
    vboxLayoutIcon->addWidget(comboBoxIconSize);
    groupBoxIcon->setLayout(vboxLayoutIcon);

    /* Mdi Background */
    QGroupBox* groupBoxMdiBG = new QGroupBox(tr("Background"), widget);

    QCheckBox* checkBoxMdiBGUseLogo = new QCheckBox(tr("Use Logo"), groupBoxMdiBG);
    general_mdi_bg_use_logo.dialog = general_mdi_bg_use_logo.setting;
    general_mdi_bg_use_logo.preview = general_mdi_bg_use_logo.dialog;
    checkBoxMdiBGUseLogo->setChecked(general_mdi_bg_use_logo.preview);
    connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)));

    QPushButton* buttonMdiBGLogo = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(general_mdi_bg_use_logo.dialog);
    strcpy(general_mdi_bg_logo.dialog, general_mdi_bg_logo.setting);
    strcpy(general_mdi_bg_logo.accept, general_mdi_bg_logo.dialog);
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseTexture = new QCheckBox(tr("Use Texture"), groupBoxMdiBG);
    general_mdi_bg_use_texture.dialog = general_mdi_bg_use_texture.setting;
    general_mdi_bg_use_texture.preview = general_mdi_bg_use_texture.dialog;
    checkBoxMdiBGUseTexture->setChecked(general_mdi_bg_use_texture.preview);
    connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)));

    QPushButton* buttonMdiBGTexture = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(general_mdi_bg_use_texture.dialog);
    strcpy(general_mdi_bg_texture.dialog, general_mdi_bg_texture.setting);
    strcpy(general_mdi_bg_texture.accept, general_mdi_bg_texture.dialog);
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseColor = new QCheckBox(tr("Use Color"), groupBoxMdiBG);
    general_mdi_bg_use_color.dialog = general_mdi_bg_color.setting;
    general_mdi_bg_use_color.preview = general_mdi_bg_use_color.dialog;
    checkBoxMdiBGUseColor->setChecked(general_mdi_bg_use_color.preview);
    connect(checkBoxMdiBGUseColor, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseColorStateChanged(int)));

    QPushButton* buttonMdiBGColor = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setEnabled(general_mdi_bg_use_color.dialog);
    general_mdi_bg_color.dialog  = general_mdi_bg_color.setting;
    general_mdi_bg_color.preview = general_mdi_bg_color.dialog;
    general_mdi_bg_color.accept  = general_mdi_bg_color.dialog;
    buttonMdiBGColor->setIcon(create_swatch(general_mdi_bg_color.preview));
    connect(buttonMdiBGColor, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundColor()));
    connect(checkBoxMdiBGUseColor, SIGNAL(toggled(bool)), buttonMdiBGColor, SLOT(setEnabled(bool)));

    QGridLayout* gridLayoutMdiBG = new QGridLayout(widget);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseLogo, 0, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGLogo, 0, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseTexture, 1, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGTexture, 1, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseColor, 2, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGColor, 2, 1, Qt::AlignRight);
    groupBoxMdiBG->setLayout(gridLayoutMdiBG);

    /* Tips */
    QGroupBox* groupBoxTips = new QGroupBox(tr("Tips"), widget);

    QCheckBox* checkBoxTipOfTheDay = new QCheckBox(tr("Show Tip of the Day on startup"), groupBoxTips);
    general_tip_of_the_day.dialog = general_tip_of_the_day.setting;
    checkBoxTipOfTheDay->setChecked(general_tip_of_the_day.dialog);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    //Help Browser
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(tr("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(tr("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(general_system_help_browser.setting);
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(tr("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!general_system_help_browser.setting);
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

    QCheckBox* checkBoxUseOpenGL = new QCheckBox(tr("Use OpenGL"), groupBoxRender);
    display_use_opengl.dialog = display_use_opengl.setting;
    checkBoxUseOpenGL->setChecked(display_use_opengl.dialog);
    connect(checkBoxUseOpenGL, SIGNAL(stateChanged(int)), this, SLOT(checkBoxUseOpenGLStateChanged(int)));

    QCheckBox* checkBoxRenderHintAA = new QCheckBox(tr("Antialias"), groupBoxRender);
    display_renderhint_aa.dialog = display_renderhint_aa.setting;
    checkBoxRenderHintAA->setChecked(display_renderhint_aa.dialog);
    connect(checkBoxRenderHintAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintTextAA = new QCheckBox(tr("Antialias Text"), groupBoxRender);
    display_renderhint_text_aa.dialog = display_renderhint_text_aa.setting;
    checkBoxRenderHintTextAA->setChecked(display_renderhint_text_aa.dialog);
    connect(checkBoxRenderHintTextAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintTextAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintSmoothPix = new QCheckBox(tr("Smooth Pixmap"), groupBoxRender);
    display_renderhint_smooth_pix.dialog = display_renderhint_smooth_pix.setting;
    checkBoxRenderHintSmoothPix->setChecked(display_renderhint_smooth_pix.dialog);
    connect(checkBoxRenderHintSmoothPix, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintSmoothPixStateChanged(int)));

    QCheckBox* checkBoxRenderHintHighAA = new QCheckBox(tr("High Quality Antialiasing (OpenGL)"), groupBoxRender);
    display_renderhint_high_aa.dialog = display_renderhint_high_aa.setting;
    checkBoxRenderHintHighAA->setChecked(display_renderhint_high_aa.dialog);
    connect(checkBoxRenderHintHighAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintHighAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintNonCosmetic = new QCheckBox(tr("Non Cosmetic"), groupBoxRender);
    display_renderhint_noncosmetic.dialog = display_renderhint_noncosmetic.setting;
    checkBoxRenderHintNonCosmetic->setChecked(display_renderhint_noncosmetic.dialog);
    connect(checkBoxRenderHintNonCosmetic, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintNonCosmeticStateChanged(int)));

    QVBoxLayout* vboxLayoutRender = new QVBoxLayout(groupBoxRender);
    vboxLayoutRender->addWidget(checkBoxUseOpenGL);
    vboxLayoutRender->addWidget(checkBoxRenderHintAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintTextAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintSmoothPix);
    vboxLayoutRender->addWidget(checkBoxRenderHintHighAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintNonCosmetic);
    groupBoxRender->setLayout(vboxLayoutRender);

    //ScrollBars
    QGroupBox* groupBoxScrollBars = new QGroupBox(tr("ScrollBars"), widget);

    QCheckBox* checkBoxShowScrollBars = new QCheckBox(tr("Show ScrollBars"), groupBoxScrollBars);
    display_show_scrollbars.dialog = display_show_scrollbars.setting;
    display_show_scrollbars.preview = display_show_scrollbars.dialog;
    checkBoxShowScrollBars->setChecked(display_show_scrollbars.preview);
    connect(checkBoxShowScrollBars, SIGNAL(stateChanged(int)), this, SLOT(checkBoxShowScrollBarsStateChanged(int)));

    QLabel* labelScrollBarWidget = new QLabel(tr("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    display_scrollbar_widget_num.dialog = display_scrollbar_widget_num.setting;
    int numActions = actionHash.size();
    for (int i = 0; i < numActions; i++) {
        QAction* action = actionHash.value(i);
        if (action) {
            comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        }
    }
    comboBoxScrollBarWidget->setCurrentIndex(display_scrollbar_widget_num.dialog);
    connect(comboBoxScrollBarWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxScrollBarWidgetCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutScrollBars = new QVBoxLayout(groupBoxScrollBars);
    vboxLayoutScrollBars->addWidget(checkBoxShowScrollBars);
    vboxLayoutScrollBars->addWidget(labelScrollBarWidget);
    vboxLayoutScrollBars->addWidget(comboBoxScrollBarWidget);
    groupBoxScrollBars->setLayout(vboxLayoutScrollBars);

    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(tr("Colors"), widget);

    QLabel* labelCrossHairColor = new QLabel(tr("Crosshair Color"), groupBoxColor);
    QPushButton* buttonCrossHairColor = choose_color_button(groupBoxColor,
        &display_crosshair_color);
    connect(buttonCrossHairColor, SIGNAL(clicked()), this,
        SLOT(chooseDisplayCrossHairColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = choose_color_button(groupBoxColor,
        &display_bg_color);
    connect(buttonBGColor, SIGNAL(clicked()), this,
        SLOT(chooseDisplayBackgroundColor()));

    QLabel* labelSelectBoxLeftColor = new QLabel(tr("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = choose_color_button(groupBoxColor,
        &display_selectbox_left_color);
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this,
        SLOT(chooseDisplaySelectBoxLeftColor()));

    QLabel* labelSelectBoxLeftFill = new QLabel(tr("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = choose_color_button(groupBoxColor,
        &display_selectbox_left_fill);
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this,
        SLOT(chooseDisplaySelectBoxLeftFill()));

    QLabel* labelSelectBoxRightColor = new QLabel(tr("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = choose_color_button(groupBoxColor,
        &display_selectbox_right_color);
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this,
        SLOT(chooseDisplaySelectBoxRightColor()));

    QLabel* labelSelectBoxRightFill = new QLabel(tr("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = choose_color_button(groupBoxColor,
        &display_selectbox_right_fill);
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this,
        SLOT(chooseDisplaySelectBoxRightFill()));

    QLabel* labelSelectBoxAlpha = new QLabel(tr("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    display_selectbox_alpha.dialog = display_selectbox_alpha.setting;
    display_selectbox_alpha.preview = display_selectbox_alpha.dialog;
    spinBoxSelectBoxAlpha->setValue(display_selectbox_alpha.preview);
    connect(spinBoxSelectBoxAlpha, SIGNAL(valueChanged(int)), this, SLOT(spinBoxDisplaySelectBoxAlphaValueChanged(int)));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelCrossHairColor, 0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonCrossHairColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor, 1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor, 1, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxLeftColor, 2, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxLeftColor, 2, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxLeftFill, 3, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxLeftFill, 3, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxRightColor, 4, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxRightColor, 4, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxRightFill, 5, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonSelectBoxRightFill, 5, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelSelectBoxAlpha, 6, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(spinBoxSelectBoxAlpha, 6, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    //Zoom
    QGroupBox* groupBoxZoom = new QGroupBox(tr("Zoom"), widget);

    QLabel* labelZoomScaleIn = new QLabel(tr("Zoom In Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleIn = new QDoubleSpinBox(groupBoxZoom);
    display_zoomscale_in.dialog = display_zoomscale_in.setting;
    spinBoxZoomScaleIn->setValue(display_zoomscale_in.dialog);
    spinBoxZoomScaleIn->setSingleStep(0.01);
    spinBoxZoomScaleIn->setRange(1.01, 10.00);
    connect(spinBoxZoomScaleIn, SIGNAL(valueChanged(double)), this, SLOT(spinBoxZoomScaleInValueChanged(double)));

    QLabel* labelZoomScaleOut = new QLabel(tr("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = new QDoubleSpinBox(groupBoxZoom);
    display_zoomscale_out.dialog = display_zoomscale_out.setting;
    spinBoxZoomScaleOut->setValue(display_zoomscale_out.dialog);
    spinBoxZoomScaleOut->setSingleStep(0.01);
    spinBoxZoomScaleOut->setRange(0.01, 0.99);
    connect(spinBoxZoomScaleOut, SIGNAL(valueChanged(double)), this, SLOT(spinBoxZoomScaleOutValueChanged(double)));

    QGridLayout* gridLayoutZoom = new QGridLayout(groupBoxZoom);
    gridLayoutZoom->addWidget(labelZoomScaleIn, 0, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleIn, 0, 1, Qt::AlignRight);
    gridLayoutZoom->addWidget(labelZoomScaleOut, 1, 0, Qt::AlignLeft);
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
    QPushButton* buttonTextColor = choose_color_button(groupBoxColor,
        &prompt_text_color);
    connect(buttonTextColor, SIGNAL(clicked()), this, SLOT(choosePromptTextColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = choose_color_button(groupBoxColor,
        &prompt_bg_color);
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(choosePromptBackgroundColor()));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelTextColor, 0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonTextColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor, 1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor, 1, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    //TODO: Tweak the Prompt Font ComboBoxes so they work properly
    //Font
    QGroupBox* groupBoxFont = new QGroupBox(tr("Font"), widget);

    QLabel* labelFontFamily = new QLabel(tr("Font Family"), groupBoxFont);
    QFontComboBox* comboBoxFontFamily = new QFontComboBox(groupBoxFont);
    strcpy(prompt_font_family.dialog, prompt_font_family.setting);
    strcpy(prompt_font_family.preview, prompt_font_family.dialog);
    comboBoxFontFamily->setCurrentFont(QFont(prompt_font_family.preview));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(const QString&)));
    QLabel* labelFontStyle = new QLabel(tr("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(prompt_font_style.preview);
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(const QString&)));
    QLabel* labelFontSize = new QLabel(tr("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    prompt_font_size.dialog = prompt_font_size.setting;
    prompt_font_size.preview = prompt_font_size.dialog;
    spinBoxFontSize->setValue(prompt_font_size.preview);
    connect(spinBoxFontSize, SIGNAL(valueChanged(int)), this, SLOT(spinBoxPromptFontSizeValueChanged(int)));

    QGridLayout* gridLayoutFont = new QGridLayout(groupBoxFont);
    gridLayoutFont->addWidget(labelFontFamily, 0, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontFamily, 0, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontStyle, 1, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontStyle, 1, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontSize, 2, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(spinBoxFontSize, 2, 1, Qt::AlignRight);
    groupBoxFont->setLayout(gridLayoutFont);

    //History
    QGroupBox* groupBoxHistory = new QGroupBox(tr("History"), widget);

    QCheckBox* checkBoxPromptSaveHistory = new QCheckBox(tr("Save History"), groupBoxHistory);
    prompt_save_history.dialog = prompt_save_history.setting;
    checkBoxPromptSaveHistory->setChecked(prompt_save_history.dialog);
    connect(checkBoxPromptSaveHistory, SIGNAL(stateChanged(int)), this, SLOT(checkBoxPromptSaveHistoryStateChanged(int)));

    QCheckBox* checkBoxPromptSaveHistoryAsHtml = new QCheckBox(tr("Save As HTML"), groupBoxHistory);
    prompt_save_history_as_html.dialog = prompt_save_history_as_html.setting;
    checkBoxPromptSaveHistoryAsHtml->setChecked(prompt_save_history_as_html.dialog);
    connect(checkBoxPromptSaveHistoryAsHtml, SIGNAL(stateChanged(int)), this, SLOT(checkBoxPromptSaveHistoryAsHtmlStateChanged(int)));

    QVBoxLayout* vboxLayoutHistory = new QVBoxLayout(groupBoxHistory);
    vboxLayoutHistory->addWidget(checkBoxPromptSaveHistory);
    vboxLayoutHistory->addWidget(checkBoxPromptSaveHistoryAsHtml);
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

    strcpy(opensave_custom_filter.dialog, opensave_custom_filter.setting);

    QHash<QString, QCheckBox*> custom_filter;

    foreach (QString extension, extensions) {
        custom_filter[extension] = new QCheckBox(extension, groupBoxCustomFilter);
        custom_filter[extension]->setChecked(QString(opensave_custom_filter.dialog).contains("*." + extension, Qt::CaseInsensitive));
        connect(custom_filter[extension], SIGNAL(stateChanged(int)), this,
            SLOT(checkBoxCustomFilterStateChanged(int)));
    }

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(tr("Select All"), widget);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()));
    foreach (QString extension, extensions) {
        connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)),
            custom_filter[extension], SLOT(setChecked(bool)));
    }

    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", widget);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()));
    foreach (QString extension, extensions) {
        connect(this, SIGNAL(buttonCustomFilterClearAll(bool)),
            custom_filter[extension], SLOT(setChecked(bool)));
    }

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    int row = 0;
    int column = 0;
    foreach (QString extension, extensions) {
        gridLayoutCustomFilter->addWidget(custom_filter[extension], row, column, Qt::AlignLeft);
        row++;
        if (row == 10) {
            row = 0;
            column++;
        }
    }
    gridLayoutCustomFilter->addWidget(buttonCustomFilterClearAll);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterSelectAll);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->setLayout(gridLayoutCustomFilter);

    if (QString(opensave_custom_filter.dialog).contains("supported", Qt::CaseInsensitive)) {
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
    opensave_recent_max_files.dialog = opensave_recent_max_files.setting;
    spinBoxRecentMaxFiles->setValue(opensave_recent_max_files.dialog);
    connect(spinBoxRecentMaxFiles, SIGNAL(valueChanged(int)), this, SLOT(spinBoxRecentMaxFilesValueChanged(int)));

    QFrame* frameRecent = new QFrame(groupBoxOpening);
    QGridLayout* gridLayoutRecent = new QGridLayout(frameRecent);
    gridLayoutRecent->addWidget(labelRecentMaxFiles, 0, 0, Qt::AlignLeft);
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
    opensave_trim_dst_num_jumps.dialog = opensave_trim_dst_num_jumps.setting;
    spinBoxTrimDstNumJumps->setValue(opensave_trim_dst_num_jumps.dialog);
    connect(spinBoxTrimDstNumJumps, SIGNAL(valueChanged(int)), this, SLOT(spinBoxTrimDstNumJumpsValueChanged(int)));

    QFrame* frameTrimDstNumJumps = new QFrame(groupBoxTrim);
    QGridLayout* gridLayoutTrimDstNumJumps = new QGridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps->addWidget(labelTrimDstNumJumps, 0, 0, Qt::AlignLeft);
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
    radioButtonUseSame->setChecked(!printing_use_last_device.setting);
    QRadioButton* radioButtonUseLast = new QRadioButton(tr("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(printing_use_last_device.setting);

    QComboBox* comboBoxDefaultDevice = new QComboBox(groupBoxDefaultPrinter);
    QList<QPrinterInfo> listAvailPrinters = QPrinterInfo::availablePrinters();
    foreach (QPrinterInfo info, listAvailPrinters) {
        comboBoxDefaultDevice->addItem(create_icon("print"), info.printerName());
    }

    QVBoxLayout* vboxLayoutDefaultPrinter = new QVBoxLayout(groupBoxDefaultPrinter);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter->addWidget(comboBoxDefaultDevice);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseLast);
    groupBoxDefaultPrinter->setLayout(vboxLayoutDefaultPrinter);

    //Save Ink
    QGroupBox* groupBoxSaveInk = new QGroupBox(tr("Save Ink"), widget);

    QCheckBox* checkBoxDisableBG = new QCheckBox(tr("Disable Background"), groupBoxSaveInk);
    printing_disable_bg.dialog = printing_disable_bg.setting;
    checkBoxDisableBG->setChecked(printing_disable_bg.dialog);
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
    grid_show_on_load.dialog = grid_show_on_load.setting;
    checkBoxGridShowOnLoad->setChecked(grid_show_on_load.dialog);
    connect(checkBoxGridShowOnLoad, &QCheckBox::stateChanged, this,
        [=](int x) { grid_show_on_load.dialog = x; });

    QCheckBox* checkBoxGridShowOrigin = new QCheckBox(tr("Show the origin when the grid is enabled"), groupBoxGridMisc);
    grid_show_origin.dialog = grid_show_origin.setting;
    checkBoxGridShowOrigin->setChecked(grid_show_origin.dialog);
    connect(checkBoxGridShowOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOriginStateChanged(int)));

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = new QGroupBox(tr("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(tr("Match grid color to crosshair color"), groupBoxGridColor);
    grid_color_match_crosshair.dialog = grid_color_match_crosshair.setting;
    checkBoxGridColorMatchCrossHair->setChecked(grid_color_match_crosshair.dialog);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(tr("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(tr("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (grid_color_match_crosshair.dialog) {
        grid_color.dialog = display_crosshair_color.setting;
    }
    else {
        grid_color.dialog = grid_color.setting;
    }
    grid_color.preview = grid_color.dialog;
    grid_color.accept  = grid_color.dialog;
    QPixmap gridPix(16,16);
    gridPix.fill(QColor(grid_color.preview));
    buttonGridColor->setIcon(QIcon(gridPix));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor->setEnabled(!grid_color_match_crosshair.dialog);
    buttonGridColor->setEnabled(!grid_color_match_crosshair.dialog);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = new QGroupBox(tr("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(tr("Set grid size from opened file"), groupBoxGridGeom);
    grid_load_from_file.dialog = grid_load_from_file.setting;
    checkBoxGridLoadFromFile->setChecked(grid_load_from_file.dialog);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(tr("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    strcpy(grid_type.dialog, grid_type.setting);
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(grid_type.dialog));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(const QString&)));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(tr("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    grid_center_on_origin.dialog = grid_center_on_origin.setting;
    checkBoxGridCenterOnOrigin->setChecked(grid_center_on_origin.dialog);
    connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)));

    QLabel* labelGridCenterX = new QLabel(tr("Grid Center X"), groupBoxGridGeom);
    labelGridCenterX->setObjectName("labelGridCenterX");
    QDoubleSpinBox* spinBoxGridCenterX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridCenterX->setObjectName("spinBoxGridCenterX");
    grid_center_x.dialog = grid_center_x.setting;
    spinBoxGridCenterX->setSingleStep(1.000);
    spinBoxGridCenterX->setRange(-1000.000, 1000.000);
    spinBoxGridCenterX->setValue(grid_center_x.dialog);
    connect(spinBoxGridCenterX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridCenterXValueChanged(double)));

    QLabel* labelGridCenterY = new QLabel(tr("Grid Center Y"), groupBoxGridGeom);
    labelGridCenterY->setObjectName("labelGridCenterY");
    QDoubleSpinBox* spinBoxGridCenterY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridCenterY->setObjectName("spinBoxGridCenterY");
    grid_center_y.dialog = grid_center_y.setting;
    spinBoxGridCenterY->setSingleStep(1.000);
    spinBoxGridCenterY->setRange(-1000.000, 1000.000);
    spinBoxGridCenterY->setValue(grid_center_y.dialog);
    connect(spinBoxGridCenterY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridCenterYValueChanged(double)));

    QLabel* labelGridSizeX = new QLabel(tr("Grid Size X"), groupBoxGridGeom);
    labelGridSizeX->setObjectName("labelGridSizeX");
    QDoubleSpinBox* spinBoxGridSizeX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeX->setObjectName("spinBoxGridSizeX");
    grid_size_x.dialog = grid_size_x.setting;
    spinBoxGridSizeX->setSingleStep(1.000);
    spinBoxGridSizeX->setRange(1.000, 1000.000);
    spinBoxGridSizeX->setValue(grid_size_x.dialog);
    connect(spinBoxGridSizeX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeXValueChanged(double)));

    QLabel* labelGridSizeY = new QLabel(tr("Grid Size Y"), groupBoxGridGeom);
    labelGridSizeY->setObjectName("labelGridSizeY");
    QDoubleSpinBox* spinBoxGridSizeY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeY->setObjectName("spinBoxGridSizeY");
    grid_size_y.dialog = grid_size_y.setting;
    spinBoxGridSizeY->setSingleStep(1.000);
    spinBoxGridSizeY->setRange(1.000, 1000.000);
    spinBoxGridSizeY->setValue(grid_size_y.dialog);
    connect(spinBoxGridSizeY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeYValueChanged(double)));

    QLabel* labelGridSpacingX = new QLabel(tr("Grid Spacing X"), groupBoxGridGeom);
    labelGridSpacingX->setObjectName("labelGridSpacingX");
    QDoubleSpinBox* spinBoxGridSpacingX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingX->setObjectName("spinBoxGridSpacingX");
    grid_spacing_x.dialog = grid_spacing_x.setting;
    spinBoxGridSpacingX->setSingleStep(1.000);
    spinBoxGridSpacingX->setRange(0.001, 1000.000);
    spinBoxGridSpacingX->setValue(grid_spacing_x.dialog);
    connect(spinBoxGridSpacingX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingXValueChanged(double)));

    QLabel* labelGridSpacingY = new QLabel(tr("Grid Spacing Y"), groupBoxGridGeom);
    labelGridSpacingY->setObjectName("labelGridSpacingY");
    QDoubleSpinBox* spinBoxGridSpacingY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingY->setObjectName("spinBoxGridSpacingY");
    grid_spacing_y.dialog = grid_spacing_y.setting;
    spinBoxGridSpacingY->setSingleStep(1.000);
    spinBoxGridSpacingY->setRange(0.001, 1000.000);
    spinBoxGridSpacingY->setValue(grid_spacing_y.dialog);
    connect(spinBoxGridSpacingY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingYValueChanged(double)));

    QLabel* labelGridSizeRadius = new QLabel(tr("Grid Size Radius"), groupBoxGridGeom);
    labelGridSizeRadius->setObjectName("labelGridSizeRadius");
    QDoubleSpinBox* spinBoxGridSizeRadius = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeRadius->setObjectName("spinBoxGridSizeRadius");
    grid_size_radius.dialog = grid_size_radius.setting;
    spinBoxGridSizeRadius->setSingleStep(1.000);
    spinBoxGridSizeRadius->setRange(1.000, 1000.000);
    spinBoxGridSizeRadius->setValue(grid_size_radius.dialog);
    connect(spinBoxGridSizeRadius, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeRadiusValueChanged(double)));

    QLabel* labelGridSpacingRadius = new QLabel(tr("Grid Spacing Radius"), groupBoxGridGeom);
    labelGridSpacingRadius->setObjectName("labelGridSpacingRadius");
    QDoubleSpinBox* spinBoxGridSpacingRadius = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingRadius->setObjectName("spinBoxGridSpacingRadius");
    grid_spacing_radius.dialog = grid_spacing_radius.setting;
    spinBoxGridSpacingRadius->setSingleStep(1.000);
    spinBoxGridSpacingRadius->setRange(0.001, 1000.000);
    spinBoxGridSpacingRadius->setValue(grid_spacing_radius.dialog);
    connect(spinBoxGridSpacingRadius, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingRadiusValueChanged(double)));

    QLabel* labelGridSpacingAngle = new QLabel(tr("Grid Spacing Angle"), groupBoxGridGeom);
    labelGridSpacingAngle->setObjectName("labelGridSpacingAngle");
    QDoubleSpinBox* spinBoxGridSpacingAngle = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingAngle->setObjectName("spinBoxGridSpacingAngle");
    grid_spacing_angle.dialog = grid_spacing_angle.setting;
    spinBoxGridSpacingAngle->setSingleStep(1.000);
    spinBoxGridSpacingAngle->setRange(0.001, 1000.000);
    spinBoxGridSpacingAngle->setValue(grid_spacing_angle.dialog);
    connect(spinBoxGridSpacingAngle, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingAngleValueChanged(double)));

    labelGridType->setEnabled(!grid_load_from_file.dialog);
    comboBoxGridType->setEnabled(!grid_load_from_file.dialog);
    checkBoxGridCenterOnOrigin->setEnabled(!grid_load_from_file.dialog);
    labelGridCenterX->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridCenterX->setEnabled(!grid_load_from_file.dialog);
    labelGridCenterY->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridCenterY->setEnabled(!grid_load_from_file.dialog);
    labelGridSizeX->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridSizeX->setEnabled(!grid_load_from_file.dialog);
    labelGridSizeY->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridSizeY->setEnabled(!grid_load_from_file.dialog);
    labelGridSpacingX->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridSpacingX->setEnabled(!grid_load_from_file.dialog);
    labelGridSpacingY->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridSpacingY->setEnabled(!grid_load_from_file.dialog);
    labelGridSizeRadius->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridSizeRadius->setEnabled(!grid_load_from_file.dialog);
    labelGridSpacingRadius->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridSpacingRadius->setEnabled(!grid_load_from_file.dialog);
    labelGridSpacingAngle->setEnabled(!grid_load_from_file.dialog);
    spinBoxGridSpacingAngle->setEnabled(!grid_load_from_file.dialog);

    bool visibility = false;
    if (grid_type.dialog == "Circular") {
        visibility = true;
    }
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
    gridLayoutGridGeom->addWidget(checkBoxGridLoadFromFile, 0, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(labelGridType, 1, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(comboBoxGridType, 1, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(checkBoxGridCenterOnOrigin, 2, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(labelGridCenterX, 3, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridCenterX, 3, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridCenterY, 4, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridCenterY, 4, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeX, 5, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeX, 5, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeY, 6, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeY, 6, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingX, 7, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingX, 7, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingY, 8, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingY, 8, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSizeRadius, 9, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSizeRadius, 9, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingRadius, 10, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingRadius, 10, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(labelGridSpacingAngle, 11, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(spinBoxGridSpacingAngle, 11, 1, Qt::AlignRight);
    groupBoxGridGeom->setLayout(gridLayoutGridGeom);

    //Ruler Misc
    QGroupBox* groupBoxRulerMisc = new QGroupBox(tr("Ruler Misc"), widget);

    QCheckBox* checkBoxRulerShowOnLoad = new QCheckBox(tr("Initially show ruler when loading a file"), groupBoxRulerMisc);
    ruler_show_on_load.dialog = ruler_show_on_load.setting;
    checkBoxRulerShowOnLoad->setChecked(ruler_show_on_load.dialog);
    connect(checkBoxRulerShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRulerShowOnLoadStateChanged(int)));

    QLabel* labelRulerMetric = new QLabel(tr("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric", true);
    ruler_metric.dialog = ruler_metric.setting;
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(ruler_metric.dialog));
    connect(comboBoxRulerMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRulerMetricCurrentIndexChanged(int)));

    QGridLayout* gridLayoutRulerMisc = new QGridLayout(widget);
    gridLayoutRulerMisc->addWidget(checkBoxRulerShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(labelRulerMetric, 1, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(comboBoxRulerMetric, 1, 1, Qt::AlignRight);
    groupBoxRulerMisc->setLayout(gridLayoutRulerMisc);

    //Ruler Color
    QGroupBox* groupBoxRulerColor = new QGroupBox(tr("Ruler Color"), widget);

    QLabel* labelRulerColor = new QLabel(tr("Ruler Color"), groupBoxRulerColor);
    labelRulerColor->setObjectName("labelRulerColor");
    QPushButton* buttonRulerColor = new QPushButton(tr("Choose"), groupBoxRulerColor);
    buttonRulerColor->setObjectName("buttonRulerColor");
    ruler_color.dialog = ruler_color.setting;
    ruler_color.preview = ruler_color.dialog;
    ruler_color.accept  = ruler_color.dialog;
    QPixmap rulerPix(16,16);
    rulerPix.fill(QColor(ruler_color.preview));
    buttonRulerColor->setIcon(QIcon(rulerPix));
    connect(buttonRulerColor, SIGNAL(clicked()), this, SLOT(chooseRulerColor()));

    QGridLayout* gridLayoutRulerColor = new QGridLayout(widget);
    gridLayoutRulerColor->addWidget(labelRulerColor, 1, 0, Qt::AlignLeft);
    gridLayoutRulerColor->addWidget(buttonRulerColor, 1, 1, Qt::AlignRight);
    groupBoxRulerColor->setLayout(gridLayoutRulerColor);

    //Ruler Geometry
    QGroupBox* groupBoxRulerGeom = new QGroupBox(tr("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = new QLabel(tr("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QDoubleSpinBox* spinBoxRulerPixelSize = new QDoubleSpinBox(groupBoxRulerGeom);
    spinBoxRulerPixelSize->setObjectName("spinBoxRulerPixelSize");
    ruler_pixel_size.dialog = ruler_pixel_size.setting;
    spinBoxRulerPixelSize->setSingleStep(1.000);
    spinBoxRulerPixelSize->setRange(20.000, 100.000);
    spinBoxRulerPixelSize->setValue(ruler_pixel_size.dialog);
    connect(spinBoxRulerPixelSize, SIGNAL(valueChanged(double)), this, SLOT(spinBoxRulerPixelSizeValueChanged(double)));

    QGridLayout* gridLayoutRulerGeom = new QGridLayout(groupBoxRulerGeom);
    gridLayoutRulerGeom->addWidget(labelRulerPixelSize, 0, 0, Qt::AlignLeft);
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

    QCheckBox* checkBoxQSnapEndPoint = new QCheckBox(tr("Endpoint"), groupBoxQSnapLoc);
    qsnap_endpoint.dialog = qsnap_endpoint.setting;
    checkBoxQSnapEndPoint->setChecked(qsnap_endpoint.dialog);
    checkBoxQSnapEndPoint->setIcon(create_icon("locator-snaptoendpoint"));
    connect(checkBoxQSnapEndPoint, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapEndPointStateChanged(int)));

    QCheckBox* checkBoxQSnapMidPoint = new QCheckBox(tr("Midpoint"), groupBoxQSnapLoc);
    qsnap_midpoint.dialog = qsnap_midpoint.setting;
    checkBoxQSnapMidPoint->setChecked(qsnap_midpoint.dialog);
    checkBoxQSnapMidPoint->setIcon(create_icon("locator-snaptomidpoint"));
    connect(checkBoxQSnapMidPoint, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapMidPointStateChanged(int)));

    QCheckBox* checkBoxQSnapCenter = new QCheckBox(tr("Center"), groupBoxQSnapLoc);
    qsnap_center.dialog = qsnap_center.setting;
    checkBoxQSnapCenter->setChecked(qsnap_center.dialog);
    checkBoxQSnapCenter->setIcon(create_icon("locator-snaptocenter"));
    connect(checkBoxQSnapCenter, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapCenterStateChanged(int)));

    QCheckBox* checkBoxQSnapNode = new QCheckBox(tr("Node"), groupBoxQSnapLoc);
    qsnap_node.dialog = qsnap_node.setting;
    checkBoxQSnapNode->setChecked(qsnap_node.dialog);
    checkBoxQSnapNode->setIcon(create_icon("locator-snaptonode"));
    connect(checkBoxQSnapNode, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapNodeStateChanged(int)));

    QCheckBox* checkBoxQSnapQuadrant = new QCheckBox(tr("Quadrant"), groupBoxQSnapLoc);
    qsnap_quadrant.dialog = qsnap_quadrant.setting;
    checkBoxQSnapQuadrant->setChecked(qsnap_quadrant.dialog);
    checkBoxQSnapQuadrant->setIcon(create_icon("locator-snaptoquadrant"));
    connect(checkBoxQSnapQuadrant, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapQuadrantStateChanged(int)));

    QCheckBox* checkBoxQSnapIntersection = new QCheckBox(tr("Intersection"), groupBoxQSnapLoc);
    qsnap_intersection.dialog = qsnap_intersection.setting;
    checkBoxQSnapIntersection->setChecked(qsnap_intersection.dialog);
    checkBoxQSnapIntersection->setIcon(create_icon("locator-snaptointersection"));
    connect(checkBoxQSnapIntersection, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapIntersectionStateChanged(int)));

    QCheckBox* checkBoxQSnapExtension = new QCheckBox(tr("Extension"), groupBoxQSnapLoc);
    qsnap_extension.dialog = qsnap_extension.setting;
    checkBoxQSnapExtension->setChecked(qsnap_extension.dialog);
    checkBoxQSnapExtension->setIcon(create_icon("locator-snaptoextension"));
    connect(checkBoxQSnapExtension, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapExtensionStateChanged(int)));

    QCheckBox* checkBoxQSnapInsertion = new QCheckBox(tr("Insertion"), groupBoxQSnapLoc);
    qsnap_insertion.dialog = qsnap_insertion.setting;
    checkBoxQSnapInsertion->setChecked(qsnap_insertion.dialog);
    checkBoxQSnapInsertion->setIcon(create_icon("locator-snaptoinsert"));
    connect(checkBoxQSnapInsertion, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapInsertionStateChanged(int)));

    QCheckBox* checkBoxQSnapPerpendicular = new QCheckBox(tr("Perpendicular"), groupBoxQSnapLoc);
    qsnap_perpendicular.dialog = qsnap_perpendicular.setting;
    checkBoxQSnapPerpendicular->setChecked(qsnap_perpendicular.dialog);
    checkBoxQSnapPerpendicular->setIcon(create_icon("locator-snaptoperpendicular"));
    connect(checkBoxQSnapPerpendicular, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapPerpendicularStateChanged(int)));

    QCheckBox* checkBoxQSnapTangent = new QCheckBox(tr("Tangent"), groupBoxQSnapLoc);
    qsnap_tangent.dialog = qsnap_tangent.setting;
    checkBoxQSnapTangent->setChecked(qsnap_tangent.dialog);
    checkBoxQSnapTangent->setIcon(create_icon("locator-snaptotangent"));
    connect(checkBoxQSnapTangent, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapTangentStateChanged(int)));

    QCheckBox* checkBoxQSnapNearest = new QCheckBox(tr("Nearest"), groupBoxQSnapLoc);
    qsnap_nearest.dialog = qsnap_nearest.setting;
    checkBoxQSnapNearest->setChecked(qsnap_nearest.dialog);
    checkBoxQSnapNearest->setIcon(create_icon("locator-snaptonearest"));
    connect(checkBoxQSnapNearest, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapNearestStateChanged(int)));

    QCheckBox* checkBoxQSnapApparent = new QCheckBox(tr("Apparent Intersection"), groupBoxQSnapLoc);
    qsnap_apparent.dialog = qsnap_apparent.setting;
    checkBoxQSnapApparent->setChecked(qsnap_apparent.dialog);
    checkBoxQSnapApparent->setIcon(create_icon("locator-snaptoapparentintersection"));
    connect(checkBoxQSnapApparent, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapApparentStateChanged(int)));

    QCheckBox* checkBoxQSnapParallel = new QCheckBox(tr("Parallel"), groupBoxQSnapLoc);
    qsnap_parallel.dialog = qsnap_parallel.setting;
    checkBoxQSnapParallel->setChecked(qsnap_parallel.dialog);
    checkBoxQSnapParallel->setIcon(create_icon("locator-snaptoparallel"));
    connect(checkBoxQSnapParallel, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapParallelStateChanged(int)));

    QPushButton* buttonQSnapSelectAll = new QPushButton(tr("Select All"), groupBoxQSnapLoc);
    connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(buttonQSnapSelectAllClicked()));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapEndPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapMidPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapCenter, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapNode, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapQuadrant, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapIntersection, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapExtension, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapInsertion, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapPerpendicular, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapTangent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapNearest, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapApparent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkBoxQSnapParallel, SLOT(setChecked(bool)));

    QPushButton* buttonQSnapClearAll = new QPushButton(tr("Clear All"), groupBoxQSnapLoc);
    connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapEndPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapMidPoint, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapCenter, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapNode, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapQuadrant, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapIntersection, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapExtension, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapInsertion, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapPerpendicular, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapTangent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapNearest, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapApparent, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkBoxQSnapParallel, SLOT(setChecked(bool)));

    QGridLayout* gridLayoutQSnap = new QGridLayout(groupBoxQSnapLoc);
    gridLayoutQSnap->addWidget(checkBoxQSnapEndPoint, 0, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapMidPoint, 1, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapCenter, 2, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapNode, 3, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapQuadrant, 4, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapIntersection, 5, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapExtension, 6, 0, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapInsertion, 0, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapPerpendicular, 1, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapTangent, 2, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapNearest, 3, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapApparent, 4, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(checkBoxQSnapParallel, 5, 1, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapSelectAll, 0, 2, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapClearAll, 1, 2, Qt::AlignLeft);
    gridLayoutQSnap->setColumnStretch(2,1);
    groupBoxQSnapLoc->setLayout(gridLayoutQSnap);

    //QSnap Visual Config
    QGroupBox* groupBoxQSnapVisual = new QGroupBox(tr("Visual Configuration"), widget);

    QLabel* labelQSnapLocColor = new QLabel(tr("Locator Color"), groupBoxQSnapVisual);
    QComboBox* comboBoxQSnapLocColor = new QComboBox(groupBoxQSnapVisual);
    addColorsToComboBox(comboBoxQSnapLocColor);
    qsnap_locator_color.dialog = qsnap_locator_color.setting;
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(qsnap_locator_color.dialog));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(tr("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    qsnap_locator_size.dialog = qsnap_locator_size.setting;
    sliderQSnapLocSize->setValue(qsnap_locator_size.dialog);
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
    qsnap_aperture_size.dialog = qsnap_aperture_size.setting;
    sliderQSnapApertureSize->setValue(qsnap_aperture_size.dialog);
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

    QCheckBox* checkBoxShowLwt = new QCheckBox(tr("Show LineWeight"), groupBoxLwtMisc);
    if (s) {
        lwt_show_lwt.dialog = s->property("ENABLE_LWT").toBool();
    }
    else {
        lwt_show_lwt.dialog = lwt_show_lwt.setting;
    }
    lwt_show_lwt.preview = lwt_show_lwt.dialog;
    checkBoxShowLwt->setChecked(lwt_show_lwt.preview);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(tr("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if (s) {
        lwt_real_render.dialog = s->property("ENABLE_REAL").toBool();
    }
    else {
        lwt_real_render.dialog = lwt_real_render.setting;
    }
    lwt_real_render.preview = lwt_real_render.dialog;
    checkBoxRealRender->setChecked(lwt_real_render.preview);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(lwt_show_lwt.dialog);

    QLabel* labelDefaultLwt = new QLabel(tr("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //TODO: remove later
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    lwt_default_lwt.dialog = lwt_default_lwt.setting;
    //TODO: populate the comboBox and set the initial value
    comboBoxDefaultLwt->addItem(QString().setNum(lwt_default_lwt.dialog, 'F', 2).append(" mm"), lwt_default_lwt.dialog);
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
    selection_mode_pickfirst.dialog = selection_mode_pickfirst.setting;
    checkBoxSelectionModePickFirst->setChecked(selection_mode_pickfirst.dialog);
    checkBoxSelectionModePickFirst->setChecked(true); checkBoxSelectionModePickFirst->setEnabled(false); //TODO: Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickFirstStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickAdd = new QCheckBox(tr("Add to Selection (PickAdd)"), groupBoxSelectionModes);
    selection_mode_pickadd.dialog = selection_mode_pickadd.setting;
    checkBoxSelectionModePickAdd->setChecked(selection_mode_pickadd.dialog);
    connect(checkBoxSelectionModePickAdd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickAddStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickDrag = new QCheckBox(tr("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    selection_mode_pickdrag.dialog = selection_mode_pickdrag.setting;
    checkBoxSelectionModePickDrag->setChecked(selection_mode_pickdrag.dialog);
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
    selection_coolgrip_color.dialog = selection_coolgrip_color.setting;
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(selection_coolgrip_color.dialog));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionCoolGripColorCurrentIndexChanged(int)));

    QLabel* labelHotGripColor = new QLabel(tr("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    selection_hotgrip_color.dialog = selection_hotgrip_color.setting;
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(selection_hotgrip_color.dialog));
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
    selection_grip_size.dialog = selection_grip_size.setting;
    sliderSelectionGripSize->setValue(selection_grip_size.dialog);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionGripSizeValueChanged(int)));

    QLabel* labelSelectionPickBoxSize = new QLabel(tr("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    selection_pickbox_size.dialog = selection_pickbox_size.setting;
    sliderSelectionPickBoxSize->setValue(selection_pickbox_size.dialog);
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

void
Settings_Dialog::addColorsToComboBox(QComboBox* comboBox)
{
    comboBox->addItem(create_icon("colorred"), tr("Red"), qRgb(255, 0, 0));
    comboBox->addItem(create_icon("coloryellow"), tr("Yellow"), qRgb(255, 255, 0));
    comboBox->addItem(create_icon("colorgreen"), tr("Green"), qRgb(0, 255, 0));
    comboBox->addItem(create_icon("colorcyan"), tr("Cyan"), qRgb(0, 255, 255));
    comboBox->addItem(create_icon("colorblue"), tr("Blue"), qRgb(0, 0, 255));
    comboBox->addItem(create_icon("colormagenta"), tr("Magenta"), qRgb(255, 0, 255));
    comboBox->addItem(create_icon("colorwhite"), tr("White"), qRgb(255, 255, 255));
    //TODO: Add Other... so the user can select custom colors
}

void
Settings_Dialog::comboBoxLanguageCurrentIndexChanged(const QString& lang)
{
    strcpy(general_language.dialog, qPrintable(lang.toLower()));
}

void
Settings_Dialog::comboBoxIconThemeCurrentIndexChanged(const QString& theme)
{
    strcpy(general_icon_theme.dialog, qPrintable(theme));
}

void
Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        general_icon_size.dialog = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            general_icon_size.dialog = 16;
        }
    }
    else {
        general_icon_size.dialog = 16;
    }
}

void
Settings_Dialog::checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    general_mdi_bg_use_logo.preview = checked;
    _main->mdiArea->useBackgroundLogo(checked);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), //Qt5
                        tr("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            strcpy(general_mdi_bg_logo.accept, qPrintable(selectedImage));
        }

        //Update immediately so it can be previewed
        _main->mdiArea->setBackgroundLogo(general_mdi_bg_logo.accept);
    }
}

void
Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    general_mdi_bg_use_texture.preview = checked;
    _main->mdiArea->useBackgroundTexture(checked);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), //Qt5
            tr("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            strcpy(general_mdi_bg_texture.accept, qPrintable(selectedImage));
        }

        //Update immediately so it can be previewed
        _main->mdiArea->setBackgroundTexture(general_mdi_bg_texture.accept);
    }
}

void
Settings_Dialog::checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    general_mdi_bg_use_color.preview = checked;
    _main->mdiArea->useBackgroundColor(checked);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(general_mdi_bg_color.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGeneralMdiBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            general_mdi_bg_color.accept = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(general_mdi_bg_color.accept));
            button->setIcon(QIcon(pix));
            _main->mdiArea->setBackgroundColor(QColor(general_mdi_bg_color.accept));
        }
        else
        {
            _main->mdiArea->setBackgroundColor(QColor(general_mdi_bg_color.dialog));
        }
    }
}

void
Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    general_mdi_bg_color.preview = color.rgb();
    _main->mdiArea->setBackgroundColor(QColor(general_mdi_bg_color.preview));
}

void
Settings_Dialog::checkBoxTipOfTheDayStateChanged(int checked)
{
    general_tip_of_the_day.dialog = checked;
}

void
Settings_Dialog::checkBoxUseOpenGLStateChanged(int checked)
{
    display_use_opengl.dialog = checked;
}

void
Settings_Dialog::checkBoxRenderHintAAStateChanged(int checked)
{
    display_renderhint_aa.dialog = checked;
}

void
Settings_Dialog::checkBoxRenderHintTextAAStateChanged(int checked)
{
    display_renderhint_text_aa.dialog = checked;
}

void
Settings_Dialog::checkBoxRenderHintSmoothPixStateChanged(int checked)
{
    display_renderhint_smooth_pix.dialog = checked;
}

void
Settings_Dialog::checkBoxRenderHintHighAAStateChanged(int checked)
{
    display_renderhint_high_aa.dialog = checked;
}

void
Settings_Dialog::checkBoxRenderHintNonCosmeticStateChanged(int checked)
{
    display_renderhint_noncosmetic.dialog = checked;
}

void
Settings_Dialog::checkBoxShowScrollBarsStateChanged(int checked)
{
    display_show_scrollbars.preview = checked;
    _main->updateAllViewScrollBars(display_show_scrollbars.preview);
}

void
Settings_Dialog::comboBoxScrollBarWidgetCurrentIndexChanged(int index)
{
    display_scrollbar_widget_num.dialog = index;
}

void
Settings_Dialog::spinBoxZoomScaleInValueChanged(double value)
{
    display_zoomscale_in.dialog = value;
}

void
Settings_Dialog::spinBoxZoomScaleOutValueChanged(double value)
{
    display_zoomscale_out.dialog = value;
}

void
Settings_Dialog::checkBoxDisableBGStateChanged(int checked)
{
    printing_disable_bg.dialog = checked;
}

void
Settings_Dialog::chooseDisplayCrossHairColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(display_crosshair_color.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayCrossHairColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            display_crosshair_color.accept = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(display_crosshair_color.accept));
            button->setIcon(QIcon(pix));
            _main->updateAllViewCrossHairColors(display_crosshair_color.accept);
        }
        else {
            _main->updateAllViewCrossHairColors(display_crosshair_color.dialog);
        }
    }
}

void
Settings_Dialog::currentDisplayCrossHairColorChanged(const QColor& color)
{
    display_crosshair_color.preview = color.rgb();
    _main->updateAllViewCrossHairColors(display_crosshair_color.preview);
}

void
Settings_Dialog::chooseDisplayBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(display_bg_color.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            display_bg_color.accept = colorDialog->selectedColor().rgb();
            button->setIcon(create_swatch(display_bg_color.accept));
            _main->updateAllViewBackgroundColors(display_bg_color.accept);
        }
        else {
            _main->updateAllViewBackgroundColors(display_bg_color.dialog);
        }
    }
}

void
Settings_Dialog::currentDisplayBackgroundColorChanged(const QColor& color)
{
    display_bg_color.preview = color.rgb();
    _main->updateAllViewBackgroundColors(display_bg_color.preview);
}

void
Settings_Dialog::chooseDisplaySelectBoxLeftColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(display_selectbox_left_color.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            display_selectbox_left_color.accept = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(display_selectbox_left_color.accept));
            button->setIcon(QIcon(pix));
            _main->updateAllViewSelectBoxColors(display_selectbox_left_color.accept,
                                                  display_selectbox_left_fill.accept,
                                                  display_selectbox_right_color.accept,
                                                  display_selectbox_right_fill.accept,
                                                  display_selectbox_alpha.preview);
        }
        else {
            _main->updateAllViewSelectBoxColors(display_selectbox_left_color.dialog,
                                                  display_selectbox_left_fill.dialog,
                                                  display_selectbox_right_color.dialog,
                                                  display_selectbox_right_fill.dialog,
                                                  display_selectbox_alpha.preview);
        }
    }
}

void
Settings_Dialog::currentDisplaySelectBoxLeftColorChanged(const QColor& color)
{
    display_selectbox_left_color.preview = color.rgb();
    _main->updateAllViewSelectBoxColors(
        display_selectbox_left_color.preview,
        display_selectbox_left_fill.preview,
        display_selectbox_right_color.preview,
        display_selectbox_right_fill.preview,
        display_selectbox_alpha.preview);
}

void
Settings_Dialog::chooseDisplaySelectBoxLeftFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
    QColorDialog* colorDialog = new QColorDialog(QColor(display_selectbox_left_fill.accept), this);
    connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftFillChanged(const QColor&)));
    colorDialog->exec();

    if (colorDialog->result() == QDialog::Accepted) {
        display_selectbox_left_fill.accept = colorDialog->selectedColor().rgb();
        QPixmap pix(16,16);
        pix.fill(QColor(display_selectbox_left_fill.accept));
        button->setIcon(QIcon(pix));
        _main->updateAllViewSelectBoxColors(
            display_selectbox_left_color.accept,
            display_selectbox_left_fill.accept,
            display_selectbox_right_color.accept,
            display_selectbox_right_fill.accept,
            display_selectbox_alpha.preview);
    }
    else {
        _main->updateAllViewSelectBoxColors(
            display_selectbox_left_color.dialog,
            display_selectbox_left_fill.dialog,
            display_selectbox_right_color.dialog,
            display_selectbox_right_fill.dialog,
            display_selectbox_alpha.preview);
    }
}

void
Settings_Dialog::currentDisplaySelectBoxLeftFillChanged(const QColor& color)
{
    display_selectbox_left_fill.preview = color.rgb();
    _main->updateAllViewSelectBoxColors(
        display_selectbox_left_color.preview,
        display_selectbox_left_fill.preview,
        display_selectbox_right_color.preview,
        display_selectbox_right_fill.preview,
        display_selectbox_alpha.preview);
}

void
Settings_Dialog::chooseDisplaySelectBoxRightColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
    QColorDialog* colorDialog = new QColorDialog(QColor(display_selectbox_right_color.accept), this);
    connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightColorChanged(const QColor&)));
    colorDialog->exec();

    if (colorDialog->result() == QDialog::Accepted) {
        display_selectbox_right_color.accept = colorDialog->selectedColor().rgb();
        button->setIcon(create_swatch(display_selectbox_right_color.accept));
        _main->updateAllViewSelectBoxColors(
            display_selectbox_left_color.accept,
            display_selectbox_left_fill.accept,
            display_selectbox_right_color.accept,
            display_selectbox_right_fill.accept,
            display_selectbox_alpha.preview);
    }
    else {
        _main->updateAllViewSelectBoxColors(
            display_selectbox_left_color.dialog,
            display_selectbox_left_fill.dialog,
            display_selectbox_right_color.dialog,
            display_selectbox_right_fill.dialog,
            display_selectbox_alpha.preview);
    }
}

void
Settings_Dialog::currentDisplaySelectBoxRightColorChanged(const QColor& color)
{
    display_selectbox_right_color.preview = color.rgb();
    _main->updateAllViewSelectBoxColors(
        display_selectbox_left_color.preview,
        display_selectbox_left_fill.preview,
        display_selectbox_right_color.preview,
        display_selectbox_right_fill.preview,
        display_selectbox_alpha.preview);
}

void
Settings_Dialog::chooseDisplaySelectBoxRightFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(display_selectbox_right_fill.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightFillChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            display_selectbox_right_fill.accept = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(display_selectbox_right_fill.accept));
            button->setIcon(QIcon(pix));
            _main->updateAllViewSelectBoxColors(display_selectbox_left_color.accept,
                                                  display_selectbox_left_fill.accept,
                                                  display_selectbox_right_color.accept,
                                                  display_selectbox_right_fill.accept,
                                                  display_selectbox_alpha.preview);
        }
        else {
            _main->updateAllViewSelectBoxColors(display_selectbox_left_color.dialog,
                                                  display_selectbox_left_fill.dialog,
                                                  display_selectbox_right_color.dialog,
                                                  display_selectbox_right_fill.dialog,
                                                  display_selectbox_alpha.preview);
        }
    }
}

void
Settings_Dialog::currentDisplaySelectBoxRightFillChanged(const QColor& color)
{
    display_selectbox_right_fill.preview = color.rgb();
    _main->updateAllViewSelectBoxColors(display_selectbox_left_color.preview,
                                          display_selectbox_left_fill.preview,
                                          display_selectbox_right_color.preview,
                                          display_selectbox_right_fill.preview,
                                          display_selectbox_alpha.preview);
}

void
Settings_Dialog::spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    display_selectbox_alpha.preview = value;
    _main->updateAllViewSelectBoxColors(display_selectbox_left_color.accept,
                                          display_selectbox_left_fill.accept,
                                          display_selectbox_right_color.accept,
                                          display_selectbox_right_fill.accept,
                                          display_selectbox_alpha.preview);
}

void
Settings_Dialog::choosePromptTextColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(prompt_text_color.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentPromptTextColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            prompt_text_color.accept = colorDialog->selectedColor().rgb();
            button->setIcon(create_swatch(prompt_text_color.accept));
            _main->prompt->setPromptTextColor(QColor(prompt_text_color.accept));
        }
        else {
            _main->prompt->setPromptTextColor(QColor(prompt_text_color.dialog));
        }
    }
}

void
Settings_Dialog::currentPromptTextColorChanged(const QColor& color)
{
    prompt_text_color.preview = color.rgb();
    _main->prompt->setPromptTextColor(QColor(prompt_text_color.preview));
}

void
Settings_Dialog::choosePromptBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(prompt_bg_color.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentPromptBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            prompt_bg_color.accept = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(prompt_bg_color.accept));
            button->setIcon(QIcon(pix));
            _main->prompt->setPromptBackgroundColor(QColor(prompt_bg_color.accept));
        }
        else {
            _main->prompt->setPromptBackgroundColor(QColor(prompt_bg_color.dialog));
        }
    }
}

void
Settings_Dialog::currentPromptBackgroundColorChanged(const QColor& color)
{
    prompt_bg_color.preview = color.rgb();
    _main->prompt->setPromptBackgroundColor(QColor(prompt_bg_color.preview));
}

void
Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(const QString& family)
{
    strcpy(prompt_font_family.preview, qPrintable(family));
    _main->prompt->setPromptFontFamily(prompt_font_family.preview);
}

void
Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(const QString& style)
{
    strcpy(prompt_font_style.preview, qPrintable(style));
    _main->prompt->setPromptFontStyle(prompt_font_style.preview);
}

void
Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    prompt_font_size.preview = value;
    _main->prompt->setPromptFontSize(prompt_font_size.preview);
}

void
Settings_Dialog::checkBoxPromptSaveHistoryStateChanged(int checked)
{
    prompt_save_history.dialog = checked;
}

void
Settings_Dialog::checkBoxPromptSaveHistoryAsHtmlStateChanged(int checked)
{
    prompt_save_history_as_html.dialog = checked;
}

void
Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        QString format = checkBox->text();
        qDebug("CustomFilter: %s %d", qPrintable(format), checked);
        if (checked) {
            strcat(opensave_custom_filter.dialog,
                qPrintable(" *." + format.toLower()));
        }
        else {
            QString s;
            s = QString(opensave_custom_filter.dialog).remove("*." + format, Qt::CaseInsensitive);
            strcpy(opensave_custom_filter.dialog, qPrintable(s));
        }
        //opensave_custom_filter.dialog = checked; //TODO
    }
}

void
Settings_Dialog::buttonCustomFilterSelectAllClicked()
{
    emit buttonCustomFilterSelectAll(true);
    strcpy(opensave_custom_filter.dialog, "supported");
}

void
Settings_Dialog::buttonCustomFilterClearAllClicked()
{
    emit buttonCustomFilterClearAll(false);
    strcpy(opensave_custom_filter.dialog, "");
}

void
Settings_Dialog::spinBoxRecentMaxFilesValueChanged(int value)
{
    opensave_recent_max_files.dialog = value;
}

void
Settings_Dialog::spinBoxTrimDstNumJumpsValueChanged(int value)
{
    opensave_trim_dst_num_jumps.dialog = value;
}

void
Settings_Dialog::checkBoxGridShowOriginStateChanged(int checked)
{
    grid_show_origin.dialog = checked;
}

void
Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    grid_color_match_crosshair.dialog = checked;
    if (grid_color_match_crosshair.dialog) {
        _main->updateAllViewGridColors(display_crosshair_color.accept);
    }
    else {
        _main->updateAllViewGridColors(grid_color.accept);
    }

    QObject* senderObj = sender();
    if (senderObj)
    {
        QObject* parent = senderObj->parent();
        if (parent)
        {
            QLabel* labelGridColor = parent->findChild<QLabel*>("labelGridColor");
            if (labelGridColor) labelGridColor->setEnabled(!grid_color_match_crosshair.dialog);
            QPushButton* buttonGridColor = parent->findChild<QPushButton*>("buttonGridColor");
            if (buttonGridColor) buttonGridColor->setEnabled(!grid_color_match_crosshair.dialog);
        }
    }
}

void
Settings_Dialog::chooseGridColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(grid_color.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGridColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            grid_color.accept = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(grid_color.accept));
            button->setIcon(QIcon(pix));
            _main->updateAllViewGridColors(grid_color.accept);
        }
        else {
            _main->updateAllViewGridColors(grid_color.dialog);
        }
    }
}

void
Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    grid_color.preview = color.rgb();
    _main->updateAllViewGridColors(grid_color.preview);
}

void
Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    grid_load_from_file.dialog = checked;

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (!parent) {
        return;
    }
    QLabel* labelGridType = parent->findChild<QLabel*>("labelGridType");
    if (labelGridType) {
        labelGridType->setEnabled(!grid_load_from_file.dialog);
    }
    QComboBox* comboBoxGridType = parent->findChild<QComboBox*>("comboBoxGridType");
    if (comboBoxGridType) {
        comboBoxGridType->setEnabled(!grid_load_from_file.dialog);
    }
    QCheckBox* checkBoxGridCenterOnOrigin = parent->findChild<QCheckBox*>("checkBoxGridCenterOnOrigin");
    if (checkBoxGridCenterOnOrigin) {
        checkBoxGridCenterOnOrigin->setEnabled(!grid_load_from_file.dialog);
    }
    QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
    if (labelGridCenterX) {
        labelGridCenterX->setEnabled(!grid_load_from_file.dialog && !grid_center_on_origin.dialog);
    }
    QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
    if (spinBoxGridCenterX) {
        spinBoxGridCenterX->setEnabled(!grid_load_from_file.dialog && !grid_center_on_origin.dialog);
    }
    QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
    if (labelGridCenterY) {
        labelGridCenterY->setEnabled(!grid_load_from_file.dialog && !grid_center_on_origin.dialog);
    }
    QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
    if (spinBoxGridCenterY) {
        spinBoxGridCenterY->setEnabled(!grid_load_from_file.dialog && !grid_center_on_origin.dialog);
    }
    QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
    if (labelGridSizeX) {
        labelGridSizeX->setEnabled(!grid_load_from_file.dialog);
    }
    QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
    if (spinBoxGridSizeX) {
        spinBoxGridSizeX->setEnabled(!grid_load_from_file.dialog);
    }
    QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
    if (labelGridSizeY) {
        labelGridSizeY->setEnabled(!grid_load_from_file.dialog);
    }
    QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
    if (spinBoxGridSizeY) {
        spinBoxGridSizeY->setEnabled(!grid_load_from_file.dialog);
    }
    QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
    if (labelGridSpacingX) {
        labelGridSpacingX->setEnabled(!grid_load_from_file.dialog);
    }
    QDoubleSpinBox* spinBoxGridSpacingX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingX");
    if (spinBoxGridSpacingX) {
        spinBoxGridSpacingX->setEnabled(!grid_load_from_file.dialog);
    }
    QLabel* labelGridSpacingY = parent->findChild<QLabel*>("labelGridSpacingY");
    if (labelGridSpacingY) {
        labelGridSpacingY->setEnabled(!grid_load_from_file.dialog);
    }
    QDoubleSpinBox* spinBoxGridSpacingY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingY");
    if (spinBoxGridSpacingY) {
        spinBoxGridSpacingY->setEnabled(!grid_load_from_file.dialog);
    }
    QLabel* labelGridSizeRadius = parent->findChild<QLabel*>("labelGridSizeRadius");
    if (labelGridSizeRadius) {
        labelGridSizeRadius->setEnabled(!grid_load_from_file.dialog);
    }
    QDoubleSpinBox* spinBoxGridSizeRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeRadius");
    if (spinBoxGridSizeRadius) {
        spinBoxGridSizeRadius->setEnabled(!grid_load_from_file.dialog);
    }
    QLabel* labelGridSpacingRadius = parent->findChild<QLabel*>("labelGridSpacingRadius");
    if (labelGridSpacingRadius) {
        labelGridSpacingRadius->setEnabled(!grid_load_from_file.dialog);
    }
    QDoubleSpinBox* spinBoxGridSpacingRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingRadius");
    if (spinBoxGridSpacingRadius) {
        spinBoxGridSpacingRadius->setEnabled(!grid_load_from_file.dialog);
    }
    QLabel* labelGridSpacingAngle = parent->findChild<QLabel*>("labelGridSpacingAngle");
    if (labelGridSpacingAngle) {
        labelGridSpacingAngle->setEnabled(!grid_load_from_file.dialog);
    }
    QDoubleSpinBox* spinBoxGridSpacingAngle = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingAngle");
    if (spinBoxGridSpacingAngle) {
        spinBoxGridSpacingAngle->setEnabled(!grid_load_from_file.dialog);
    }
}

void
Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(const QString& type)
{
    strcpy(grid_type.dialog, qPrintable(type));

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (!parent) {
        return;
    }
    bool visibility = false;
    if (type == "Circular") visibility = true;

    QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
    if (labelGridSizeX) labelGridSizeX->setVisible(!visibility);
    QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
    if (spinBoxGridSizeX) spinBoxGridSizeX->setVisible(!visibility);
    QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
    if (labelGridSizeY) labelGridSizeY->setVisible(!visibility);
    QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
    if (spinBoxGridSizeY) spinBoxGridSizeY->setVisible(!visibility);
    QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
    if (labelGridSpacingX) labelGridSpacingX->setVisible(!visibility);
    QDoubleSpinBox* spinBoxGridSpacingX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingX");
    if (spinBoxGridSpacingX) spinBoxGridSpacingX->setVisible(!visibility);
    QLabel* labelGridSpacingY = parent->findChild<QLabel*>("labelGridSpacingY");
    if (labelGridSpacingY) labelGridSpacingY->setVisible(!visibility);
    QDoubleSpinBox* spinBoxGridSpacingY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingY");
    if (spinBoxGridSpacingY) spinBoxGridSpacingY->setVisible(!visibility);
    QLabel* labelGridSizeRadius = parent->findChild<QLabel*>("labelGridSizeRadius");
    if (labelGridSizeRadius) labelGridSizeRadius->setVisible(visibility);
    QDoubleSpinBox* spinBoxGridSizeRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeRadius");
    if (spinBoxGridSizeRadius) spinBoxGridSizeRadius->setVisible(visibility);
    QLabel* labelGridSpacingRadius = parent->findChild<QLabel*>("labelGridSpacingRadius");
    if (labelGridSpacingRadius) labelGridSpacingRadius->setVisible(visibility);
    QDoubleSpinBox* spinBoxGridSpacingRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingRadius");
    if (spinBoxGridSpacingRadius) spinBoxGridSpacingRadius->setVisible(visibility);
    QLabel* labelGridSpacingAngle = parent->findChild<QLabel*>("labelGridSpacingAngle");
    if (labelGridSpacingAngle) labelGridSpacingAngle->setVisible(visibility);
    QDoubleSpinBox* spinBoxGridSpacingAngle = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingAngle");
    if (spinBoxGridSpacingAngle) spinBoxGridSpacingAngle->setVisible(visibility);
}

void
Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    grid_center_on_origin.dialog = checked;

    QObject* senderObj = sender();
    if (senderObj)
    {
        QObject* parent = senderObj->parent();
        if (parent)
        {
            QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
            if (labelGridCenterX) labelGridCenterX->setEnabled(!grid_center_on_origin.dialog);
            QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
            if (spinBoxGridCenterX) spinBoxGridCenterX->setEnabled(!grid_center_on_origin.dialog);
            QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
            if (labelGridCenterY) labelGridCenterY->setEnabled(!grid_center_on_origin.dialog);
            QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
            if (spinBoxGridCenterY) spinBoxGridCenterY->setEnabled(!grid_center_on_origin.dialog);
        }
    }
}

void
Settings_Dialog::spinBoxGridCenterXValueChanged(double value)
{
    grid_center_x.dialog = value;
}

void
Settings_Dialog::spinBoxGridCenterYValueChanged(double value)
{
    grid_center_y.dialog = value;
}

void
Settings_Dialog::spinBoxGridSizeXValueChanged(double value)
{
    grid_size_x.dialog = value;
}

void
Settings_Dialog::spinBoxGridSizeYValueChanged(double value)
{
    grid_size_y.dialog = value;
}

void
Settings_Dialog::spinBoxGridSpacingXValueChanged(double value)
{
    grid_spacing_x.dialog = value;
}

void
Settings_Dialog::spinBoxGridSpacingYValueChanged(double value)
{
    grid_spacing_y.dialog = value;
}

void
Settings_Dialog::spinBoxGridSizeRadiusValueChanged(double value)
{
    grid_size_radius.dialog = value;
}

void
Settings_Dialog::spinBoxGridSpacingRadiusValueChanged(double value)
{
    grid_spacing_radius.dialog = value;
}

void
Settings_Dialog::spinBoxGridSpacingAngleValueChanged(double value)
{
    grid_spacing_angle.dialog = value;
}

void
Settings_Dialog::checkBoxRulerShowOnLoadStateChanged(int checked)
{
    ruler_show_on_load.dialog = checked;
}

void
Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox)
    {
        bool ok = 0;
        ruler_metric.dialog = comboBox->itemData(index).toBool();
    }
    else
        ruler_metric.dialog = true;
}

void
Settings_Dialog::chooseRulerColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(ruler_color.accept), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentRulerColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            ruler_color.accept = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(ruler_color.accept));
            button->setIcon(QIcon(pix));
            _main->updateAllViewRulerColors(ruler_color.accept);
        }
        else {
            _main->updateAllViewRulerColors(ruler_color.dialog);
        }
    }
}

void
Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    ruler_color.preview = color.rgb();
    _main->updateAllViewRulerColors(ruler_color.preview);
}

void
Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    ruler_pixel_size.dialog = value;
}

void
Settings_Dialog::checkBoxQSnapEndPointStateChanged(int checked)
{
    qsnap_endpoint.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapMidPointStateChanged(int checked)
{
    qsnap_midpoint.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapCenterStateChanged(int checked)
{
    qsnap_center.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapNodeStateChanged(int checked)
{
    qsnap_node.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapQuadrantStateChanged(int checked)
{
    qsnap_quadrant.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapIntersectionStateChanged(int checked)
{
    qsnap_intersection.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapExtensionStateChanged(int checked)
{
    qsnap_extension.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapInsertionStateChanged(int checked)
{
    qsnap_insertion.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapPerpendicularStateChanged(int checked)
{
    qsnap_perpendicular.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapTangentStateChanged(int checked)
{
    qsnap_tangent.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapNearestStateChanged(int checked)
{
    qsnap_nearest.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapApparentStateChanged(int checked)
{
    qsnap_apparent.dialog = checked;
}

void
Settings_Dialog::checkBoxQSnapParallelStateChanged(int checked)
{
    qsnap_parallel.dialog = checked;
}

void
Settings_Dialog::buttonQSnapSelectAllClicked()
{
    emit buttonQSnapSelectAll(true);
}

void
Settings_Dialog::buttonQSnapClearAllClicked()
{
    emit buttonQSnapClearAll(false);
}

/*
//TODO: Figure out how to abstract the slot in a way that it can be used for comboBoxes in general
        Currently comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
                  comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
                  comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
        are all similar except the dialog_ variable being worked on and the QVariant.
*/

void
Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,255,0); //Yellow
    if (comboBox) {
        bool ok = 0;
        qsnap_locator_color.dialog = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            qsnap_locator_color.dialog = defaultColor;
        }
    }
    else {
        qsnap_locator_color.dialog = defaultColor;
    }
}

void
Settings_Dialog::sliderQSnapLocatorSizeValueChanged(int value)
{
    qsnap_locator_size.dialog = value;
}

void
Settings_Dialog::sliderQSnapApertureSizeValueChanged(int value)
{
    qsnap_aperture_size.dialog = value;
}

void
Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    lwt_show_lwt.preview = checked;
    if (lwt_show_lwt.preview) { _main->statusbar->statusBarLwtButton->enableLwt(); }
    else                     { _main->statusbar->statusBarLwtButton->disableLwt(); }

    QObject* senderObj = sender();
    if (senderObj)
    {
        QObject* parent = senderObj->parent();
        if (parent)
        {
            QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
            if (checkBoxRealRender) checkBoxRealRender->setEnabled(lwt_show_lwt.preview);
        }
    }
}

void
Settings_Dialog::checkBoxLwtRealRenderStateChanged(int checked)
{
    lwt_real_render.preview = checked;
    if (lwt_real_render.preview) {
        _main->statusbar->statusBarLwtButton->enableReal();
    }
    else {
        _main->statusbar->statusBarLwtButton->disableReal();
    }
}

void
Settings_Dialog::checkBoxSelectionModePickFirstStateChanged(int checked)
{
    selection_mode_pickfirst.dialog = checked;
}

void
Settings_Dialog::checkBoxSelectionModePickAddStateChanged(int checked)
{
    selection_mode_pickadd.dialog = checked;
}

void
Settings_Dialog::checkBoxSelectionModePickDragStateChanged(int checked)
{
    selection_mode_pickdrag.dialog = checked;
}

void
Settings_Dialog::sliderSelectionGripSizeValueChanged(int value)
{
    selection_grip_size.dialog = value;
}

void
Settings_Dialog::sliderSelectionPickBoxSizeValueChanged(int value)
{
    selection_pickbox_size.dialog = value;
}

void
Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(0,0,255); //Blue
    if (comboBox)
    {
        bool ok = 0;
        selection_coolgrip_color.dialog = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            selection_coolgrip_color.dialog = defaultColor;
    }
    else {
        selection_coolgrip_color.dialog = defaultColor;
    }
}

void
Settings_Dialog::comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,0,0); //Red
    if (comboBox) {
        bool ok = 0;
        selection_hotgrip_color.dialog = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            selection_hotgrip_color.dialog = defaultColor;
        }
    }
    else {
        selection_hotgrip_color.dialog = defaultColor;
    }
}

void
Settings_Dialog::acceptChanges()
{
    general_mdi_bg_use_logo.dialog = general_mdi_bg_use_logo.preview;
    general_mdi_bg_use_texture.dialog = general_mdi_bg_use_texture.preview;
    general_mdi_bg_use_color.dialog = general_mdi_bg_use_color.preview;
    strcpy(general_mdi_bg_logo.dialog, general_mdi_bg_logo.accept);
    strcpy(general_mdi_bg_texture.dialog, general_mdi_bg_texture.accept);
    general_mdi_bg_color.dialog = general_mdi_bg_color.accept;

    display_show_scrollbars.dialog = display_show_scrollbars.preview;
    display_crosshair_color.dialog = display_crosshair_color.accept;
    display_bg_color.dialog = display_bg_color.accept;
    display_selectbox_left_color.dialog = display_selectbox_left_color.accept;
    display_selectbox_left_fill.dialog = display_selectbox_left_fill.accept;
    display_selectbox_right_color.dialog = display_selectbox_right_color.accept;
    display_selectbox_right_fill.dialog = display_selectbox_right_fill.accept;
    display_selectbox_alpha.dialog = display_selectbox_alpha.preview;

    prompt_text_color.dialog = prompt_text_color.accept;
    prompt_bg_color.dialog = prompt_bg_color.accept;
    strcpy(prompt_font_family.dialog, prompt_font_family.preview);
    strcpy(prompt_font_style.dialog, prompt_font_style.preview);
    prompt_font_size.dialog = prompt_font_size.preview;
    if (grid_color_match_crosshair.dialog) {
        grid_color.dialog = display_crosshair_color.accept;
    }
    else {
        grid_color.dialog = grid_color.accept;
    }
    ruler_color.dialog = ruler_color.accept;
    lwt_show_lwt.dialog = lwt_show_lwt.preview;
    lwt_real_render.dialog = lwt_real_render.preview;

    strcpy(general_language.setting, general_language.dialog);
    strcpy(general_icon_theme.setting, general_icon_theme.dialog);
    general_icon_size.setting = general_icon_size.dialog;
    general_mdi_bg_use_logo.setting = general_mdi_bg_use_logo.dialog;
    general_mdi_bg_use_texture.setting = general_mdi_bg_use_texture.dialog;
    general_mdi_bg_use_color.setting = general_mdi_bg_use_color.dialog;
    strcpy(general_mdi_bg_logo.setting, general_mdi_bg_logo.dialog);
    strcpy(general_mdi_bg_texture.setting, general_mdi_bg_texture.dialog);
    general_mdi_bg_color.setting = general_mdi_bg_color.dialog;
    general_tip_of_the_day.setting = general_tip_of_the_day.dialog;
    //TODO: general_system_help_browser.setting = general_system_help_browser.dialog;

    display_use_opengl.setting = display_use_opengl.dialog;
    display_renderhint_aa.setting = display_renderhint_aa.dialog;
    display_renderhint_text_aa.setting = display_renderhint_text_aa.dialog;
    display_renderhint_smooth_pix.setting = display_renderhint_smooth_pix.dialog;
    display_renderhint_high_aa.setting = display_renderhint_high_aa.dialog;
    display_renderhint_noncosmetic.setting = display_renderhint_noncosmetic.dialog;
    display_show_scrollbars.setting = display_show_scrollbars.dialog;
    display_scrollbar_widget_num.setting = display_scrollbar_widget_num.dialog;
    display_crosshair_color.setting = display_crosshair_color.dialog;
    display_bg_color.setting = display_bg_color.dialog;
    display_selectbox_left_color.setting = display_selectbox_left_color.dialog;
    display_selectbox_left_fill.setting = display_selectbox_left_fill.dialog;
    display_selectbox_right_color.setting = display_selectbox_right_color.dialog;
    display_selectbox_right_fill.setting = display_selectbox_right_fill.dialog;
    display_selectbox_alpha.setting = display_selectbox_alpha.dialog;
    display_zoomscale_in.setting = display_zoomscale_in.dialog;
    display_zoomscale_out.setting = display_zoomscale_out.dialog;
    //TODO: display_crosshair_percent.setting = dialog_display_crosshair_percent;
    //TODO: display_units.setting = dialog_display_units;

    prompt_text_color.setting = prompt_text_color.dialog;
    prompt_bg_color.setting = prompt_bg_color.dialog;
    strcpy(prompt_font_family.setting, prompt_font_family.dialog);
    strcpy(prompt_font_style.setting, prompt_font_style.dialog);
    prompt_font_size.setting = prompt_font_size.dialog;
    prompt_save_history.setting = prompt_save_history.dialog;
    prompt_save_history_as_html.setting = prompt_save_history_as_html.dialog;
    //TODO: prompt_save_history_filename.setting = prompt_save_history_filename.dialog;
    strcpy(opensave_custom_filter.setting, opensave_custom_filter.dialog);
    //TODO: opensave_open_format.setting = dialog_opensave_open_format;
    //TODO: opensave_open_thumbnail.setting = dialog_opensave_open_thumbnail;
    //TODO: opensave_save_format.setting = dialog_opensave_save_format;
    //TODO: opensave_save_thumbnail.setting = dialog_opensave_save_thumbnail;
    opensave_recent_max_files.setting = opensave_recent_max_files.dialog;
    opensave_trim_dst_num_jumps.setting = opensave_trim_dst_num_jumps.dialog;
    //TODO: printing_default_device.setting = dialog_printing_default_device;
    //TODO: printing_use_last_device.setting = dialog_printing_use_last_device;
    printing_disable_bg.setting = printing_disable_bg.dialog;
    grid_show_on_load.setting = grid_show_on_load.dialog;
    grid_show_origin.setting = grid_show_origin.dialog;
    grid_color_match_crosshair.setting = grid_color_match_crosshair.dialog;
    grid_color.setting = grid_color.dialog;
    //TODO: grid_load_from_file.setting = grid_load_from_file.dialog;
    strcpy(grid_type.setting, grid_type.dialog);
    grid_center_on_origin.setting = grid_center_on_origin.dialog;
    grid_center_x.setting = grid_center_x.dialog;
    grid_center_y.setting = grid_center_y.dialog;
    grid_size_x.setting = grid_size_x.dialog;
    grid_size_y.setting = grid_size_y.dialog;
    grid_spacing_x.setting = grid_spacing_x.dialog;
    grid_spacing_y.setting = grid_spacing_y.dialog;
    grid_size_radius.setting = grid_size_radius.dialog;
    grid_spacing_radius.setting = grid_spacing_radius.dialog;
    grid_spacing_angle.setting = grid_spacing_angle.dialog;
    ruler_show_on_load.setting = ruler_show_on_load.dialog;
    ruler_metric.setting = ruler_metric.dialog;
    ruler_color.setting = ruler_color.dialog;
    ruler_pixel_size.setting = ruler_pixel_size.dialog;
    //TODO: qsnap_enabled.setting = dialog_qsnap_enabled;
    qsnap_locator_color.setting = qsnap_locator_color.dialog;
    qsnap_locator_size.setting = qsnap_locator_size.dialog;
    qsnap_aperture_size.setting = qsnap_aperture_size.dialog;
    qsnap_endpoint.setting = qsnap_endpoint.dialog;
    qsnap_midpoint.setting = qsnap_midpoint.dialog;
    qsnap_center.setting = qsnap_center.dialog;
    qsnap_node.setting = qsnap_node.dialog;
    qsnap_quadrant.setting = qsnap_quadrant.dialog;
    qsnap_intersection.setting = qsnap_intersection.dialog;
    qsnap_extension.setting = qsnap_extension.dialog;
    qsnap_insertion.setting = qsnap_insertion.dialog;
    qsnap_perpendicular.setting = qsnap_perpendicular.dialog;
    qsnap_tangent.setting = qsnap_tangent.dialog;
    qsnap_nearest.setting = qsnap_nearest.dialog;
    qsnap_apparent.setting = qsnap_apparent.dialog;
    qsnap_parallel.setting = qsnap_parallel.dialog;
    lwt_show_lwt.setting = lwt_show_lwt.dialog;
    lwt_real_render.setting = lwt_real_render.dialog;
    selection_mode_pickfirst.setting = selection_mode_pickfirst.dialog;
    selection_mode_pickadd.setting = selection_mode_pickadd.dialog;
    selection_mode_pickdrag.setting = selection_mode_pickdrag.dialog;
    selection_coolgrip_color.setting = selection_coolgrip_color.dialog;
    selection_hotgrip_color.setting = selection_hotgrip_color.dialog;
    selection_grip_size.setting = selection_grip_size.dialog;
    selection_pickbox_size.setting = selection_pickbox_size.dialog;

    //Make sure the user sees the changes applied immediately
    _main->mdiArea->useBackgroundLogo(general_mdi_bg_use_logo.dialog);
    _main->mdiArea->useBackgroundTexture(general_mdi_bg_use_texture.dialog);
    _main->mdiArea->useBackgroundColor(general_mdi_bg_use_color.dialog);
    _main->mdiArea->setBackgroundLogo(general_mdi_bg_logo.dialog);
    _main->mdiArea->setBackgroundTexture(general_mdi_bg_texture.dialog);
    _main->mdiArea->setBackgroundColor(general_mdi_bg_color.dialog);
    _main->iconResize(general_icon_size.dialog);
    _main->updateAllViewScrollBars(display_show_scrollbars.dialog);
    _main->updateAllViewCrossHairColors(display_crosshair_color.dialog);
    _main->updateAllViewBackgroundColors(display_bg_color.dialog);
    _main->updateAllViewSelectBoxColors(
        display_selectbox_left_color.dialog,
        display_selectbox_left_fill.dialog,
        display_selectbox_right_color.dialog,
        display_selectbox_right_fill.dialog,
        display_selectbox_alpha.dialog);
    _main->prompt->setPromptTextColor(QColor(prompt_text_color.dialog));
    _main->prompt->setPromptBackgroundColor(QColor(prompt_bg_color.dialog));
    _main->prompt->setPromptFontFamily(prompt_font_family.dialog);
    _main->prompt->setPromptFontStyle(prompt_font_style.dialog);
    _main->prompt->setPromptFontSize(prompt_font_size.dialog);
    _main->updateAllViewGridColors(grid_color.dialog);
    _main->updateAllViewRulerColors(ruler_color.dialog);
    if (lwt_show_lwt.dialog) {
        _main->statusbar->statusBarLwtButton->enableLwt();
    }
    else {
        _main->statusbar->statusBarLwtButton->disableLwt();
    }
    if (lwt_real_render.dialog) {
        _main->statusbar->statusBarLwtButton->enableReal();
    }
    else {
        _main->statusbar->statusBarLwtButton->disableReal();
    }
    _main->updatePickAddMode(selection_mode_pickadd.dialog);

    _main->writeSettings();
    accept();
}

void
Settings_Dialog::rejectChanges()
{
    //TODO: inform the user if they have changed settings

    //Update the view since the user must accept the preview
    _main->mdiArea->useBackgroundLogo(general_mdi_bg_use_logo.dialog);
    _main->mdiArea->useBackgroundTexture(general_mdi_bg_use_texture.dialog);
    _main->mdiArea->useBackgroundColor(general_mdi_bg_use_color.dialog);
    _main->mdiArea->setBackgroundLogo(general_mdi_bg_logo.dialog);
    _main->mdiArea->setBackgroundTexture(general_mdi_bg_texture.dialog);
    _main->mdiArea->setBackgroundColor(general_mdi_bg_color.dialog);
    _main->updateAllViewScrollBars(display_show_scrollbars.dialog);
    _main->updateAllViewCrossHairColors(display_crosshair_color.dialog);
    _main->updateAllViewBackgroundColors(display_bg_color.dialog);
    _main->updateAllViewSelectBoxColors(
        display_selectbox_left_color.dialog,
        display_selectbox_left_fill.dialog,
        display_selectbox_right_color.dialog,
        display_selectbox_right_fill.dialog,
        display_selectbox_alpha.dialog);
    _main->prompt->setPromptTextColor(QColor(prompt_text_color.dialog));
    _main->prompt->setPromptBackgroundColor(QColor(prompt_bg_color.dialog));
    _main->prompt->setPromptFontFamily(prompt_font_family.dialog);
    _main->prompt->setPromptFontStyle(prompt_font_style.dialog);
    _main->prompt->setPromptFontSize(prompt_font_size.dialog);
    _main->updateAllViewGridColors(grid_color.dialog);
    _main->updateAllViewRulerColors(ruler_color.dialog);
    if (lwt_show_lwt.dialog) {
        _main->statusbar->statusBarLwtButton->enableLwt();
    }
    else {
        _main->statusbar->statusBarLwtButton->disableLwt();
    }
    if (lwt_real_render.dialog) {
        _main->statusbar->statusBarLwtButton->enableReal();
    }
    else {
        _main->statusbar->statusBarLwtButton->disableReal();
    }

    reject();
}
