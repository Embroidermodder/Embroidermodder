/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "embroidermodder.h"

#include "imgui.h"

void create_tab_general(void);
void create_tab_files_paths(void);
void create_tab_display(void);
void create_tab_prompt(void);
void create_tab_open_save(void);
void create_tab_printing(void);
void create_tab_snap(void);
void create_tab_grid_ruler(void);
void create_tab_ortho_polar(void);
void create_tab_quicksnap(void);
void create_tab_quicktrack(void);
void create_tab_lineweight(void);
void create_tab_selection(void);

void
settings_editor(void)
{
    // ImGuiTabItemFlags_SetSelected   
    ImGui::SetNextWindowSize(ImVec2(750, 550));
    ImGui::Begin("Settings");
    ImGui::SetWindowFontScale(1.5);
    if (ImGui:: BeginTabBar("Tab Bar")) {
        if (ImGui::BeginTabItem("General")) {
            create_tab_general();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Files/Paths")) {
            create_tab_files_paths();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Display")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Prompt")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Open/Save")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Printing")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Snap")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Grid/Ruler")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ortho/Polar")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("QuickSnap")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("QuickTrack")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("LineWeight")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Selection")) {
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    if (ImGui::Button("Accept")) {
        
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        settings.show_settings_editor = false;
    }
    ImGui::End();
}

std::vector<std::string> languages = {
    "default",
    "spanish",
    "french"
};

void create_tab_general(void)
{
    ImGui::Text("Language");

    if (ImGui::BeginCombo("Language", settings.language.c_str())) {
        for (std::string lang : languages) {
            bool current_language = (settings.language == lang);
            if (ImGui::Selectable(lang.c_str(), current_language)) {
                settings.language = lang;
            }
        }
        ImGui::EndCombo();
    }
    /*
    QLabel* labelLanguage = new QLabel(tr("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    dialog_general_language = settings_general_language;
    comboBoxLanguage->addItem("Default");
    comboBoxLanguage->addItem("System");
    comboBoxLanguage->insertSeparator(2);
    QDir trDir(qApp->applicationDirPath());
    trDir.cd("translations");
    foreach(std::string dirName, trDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        dirName[0] = dirName[0].toUpper();
        comboBoxLanguage->addItem(dirName);
    }
    std::string current = dialog_general_language;
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(const std::string&)), this, SLOT(comboBoxLanguageCurrentIndexChanged(const std::string&)));

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
    dialog_general_icon_theme.string_value = settings_general_icon_theme;
    foreach (std::string dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/" + "theme" + ".png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(dialog_general_icon_theme.string_value));
    connect(comboBoxIconTheme, SIGNAL(currentIndexChanged(const std::string&)), this, SLOT(comboBoxIconThemeCurrentIndexChanged(const std::string&)));

    QLabel* labelIconSize = new QLabel(tr("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    std::string s = "icons/" + dialog_general_icon_theme.string_value + "/icon16.png";
    comboBoxIconSize->addItem(QIcon(s), "Very Small", 16);
    s = "icons/" + dialog_general_icon_theme.string_value + "/icon24.png";
    comboBoxIconSize->addItem(QIcon(s), "Small", 24);
    s = "icons/" + dialog_general_icon_theme.string_value + "/icon32.png";
    comboBoxIconSize->addItem(QIcon(s), "Medium",     32);
    s = "icons/" + dialog_general_icon_theme.string_value + "/icon48.png";
    comboBoxIconSize->addItem(QIcon(s), "Large",      48);
    s = "icons/" + dialog_general_icon_theme.string_value + "/icon64.png";
    comboBoxIconSize->addItem(QIcon(s), "Very Large", 64);
    s = "icons/" + dialog_general_icon_theme.string_value + "/icon128.png";
    comboBoxIconSize->addItem(QIcon(s), "I'm Blind", 128);
    dialog_general_icon_size = settings_general_icon_size;
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(dialog_general_icon_size));
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
    dialog_general_mdi_bg_use_logo = settings_general_mdi_bg_use_logo;
    preview_general_mdi_bg_use_logo = dialog_general_mdi_bg_use_logo;
    checkBoxMdiBGUseLogo->setChecked(preview_general_mdi_bg_use_logo);
    connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)));

    QPushButton* buttonMdiBGLogo = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(dialog_general_mdi_bg_use_logo);
    dialog_general_mdi_bg_logo = settings_general_mdi_bg_logo;
    accept_general_mdi_bg_logo = dialog_general_mdi_bg_logo;
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseTexture = new QCheckBox(tr("Use Texture"), groupBoxMdiBG);
    dialog_general_mdi_bg_use_texture = settings_general_mdi_bg_use_texture;
    preview_general_mdi_bg_use_texture = dialog_general_mdi_bg_use_texture;
    checkBoxMdiBGUseTexture->setChecked(preview_general_mdi_bg_use_texture);
    connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)));

    QPushButton* buttonMdiBGTexture = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(dialog_general_mdi_bg_use_texture);
    dialog_general_mdi_bg_texture  = settings_general_mdi_bg_texture;
    accept_general_mdi_bg_texture  = dialog_general_mdi_bg_texture;
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseColor = new QCheckBox(tr("Use Color"), groupBoxMdiBG);
    dialog_general_mdi_bg_use_color = settings_general_mdi_bg_use_color;
    preview_general_mdi_bg_use_color = dialog_general_mdi_bg_use_color;
    checkBoxMdiBGUseColor->setChecked(preview_general_mdi_bg_use_color);
    connect(checkBoxMdiBGUseColor, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseColorStateChanged(int)));

    QPushButton* buttonMdiBGColor = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setEnabled(dialog_general_mdi_bg_use_color);
    dialog_general_mdi_bg_color = settings_general_mdi_bg_color;
    preview_general_mdi_bg_color = dialog_general_mdi_bg_color;
    accept_general_mdi_bg_color = dialog_general_mdi_bg_color;
    QPixmap mdiBGPix(16,16);
    mdiBGPix.fill(QColor(preview_general_mdi_bg_color));
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
    dialog_general_tip_of_the_day = settings_general_tip_of_the_day;
    checkBoxTipOfTheDay->setChecked(dialog_general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    //Help Browser
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(tr("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(tr("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(settings_general_system_help_browser);
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(tr("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!settings_general_system_help_browser);
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
    */
}

void create_tab_files_paths(void)
{
    /*
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    */ 
}

void create_tab_display(void)
{
    /*
    //Rendering
    //TODO: Review OpenGL and Rendering settings for future inclusion
    QGroupBox* groupBoxRender = new QGroupBox(tr("Rendering"), widget);

    QCheckBox* checkBoxUseOpenGL = new QCheckBox(tr("Use OpenGL"), groupBoxRender);
    dialog_display_use_opengl = getSettingsDisplayUseOpenGL();
    checkBoxUseOpenGL->setChecked(dialog_display_use_opengl);
    connect(checkBoxUseOpenGL, SIGNAL(stateChanged(int)), this, SLOT(checkBoxUseOpenGLStateChanged(int)));

    QCheckBox* checkBoxRenderHintAA = new QCheckBox(tr("Antialias"), groupBoxRender);
    dialog_display_renderhint_aa = getSettingsDisplayRenderHintAA();
    checkBoxRenderHintAA->setChecked(dialog_display_renderhint_aa);
    connect(checkBoxRenderHintAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintTextAA = new QCheckBox(tr("Antialias Text"), groupBoxRender);
    dialog_display_renderhint_text_aa = getSettingsDisplayRenderHintTextAA();
    checkBoxRenderHintTextAA->setChecked(dialog_display_renderhint_text_aa);
    connect(checkBoxRenderHintTextAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintTextAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintSmoothPix = new QCheckBox(tr("Smooth Pixmap"), groupBoxRender);
    dialog_display_renderhint_smooth_pix = getSettingsDisplayRenderHintSmoothPix();
    checkBoxRenderHintSmoothPix->setChecked(dialog_display_renderhint_smooth_pix);
    connect(checkBoxRenderHintSmoothPix, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintSmoothPixStateChanged(int)));

    QCheckBox* checkBoxRenderHintHighAA = new QCheckBox(tr("High Quality Antialiasing (OpenGL)"), groupBoxRender);
    dialog_display_renderhint_high_aa = getSettingsDisplayRenderHintHighAA();
    checkBoxRenderHintHighAA->setChecked(dialog_display_renderhint_high_aa);
    connect(checkBoxRenderHintHighAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintHighAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintNonCosmetic = new QCheckBox(tr("Non Cosmetic"), groupBoxRender);
    dialog_display_renderhint_noncosmetic = getSettingsDisplayRenderHintNonCosmetic();
    checkBoxRenderHintNonCosmetic->setChecked(dialog_display_renderhint_noncosmetic);
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
    dialog_display_show_scrollbars = settings_display_show_scrollbars;
    preview_display_show_scrollbars = dialog_display_show_scrollbars;
    checkBoxShowScrollBars->setChecked(preview_display_show_scrollbars);
    connect(checkBoxShowScrollBars, SIGNAL(stateChanged(int)), this, SLOT(checkBoxShowScrollBarsStateChanged(int)));

    QLabel* labelScrollBarWidget = new QLabel(tr("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    dialog_display_scrollbar_widget_num = settings_display_scrollbar_widget_num;
    int numActions = mainWin->actionHash.size();
    for(int i = 0; i < numActions; i++)
    {
        QAction* action = mainWin->actionHash.value(i);
        if (action) comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
    }
    comboBoxScrollBarWidget->setCurrentIndex(dialog_display_scrollbar_widget_num);
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
    dialog_display_crosshair_color = settings_display_crosshair_color;
    preview_display_crosshair_color = dialog_display_crosshair_color;
    accept_display_crosshair_color = dialog_display_crosshair_color;
    QPixmap crosshairPix(16,16);
    crosshairPix.fill(QColor(preview_display_crosshair_color));
    buttonCrossHairColor->setIcon(QIcon(crosshairPix));
    connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog_display_bg_color = settings_display_bg_color;
    preview_display_bg_color = dialog_display_bg_color;
    accept_display_bg_color = dialog_display_bg_color;
    QPixmap bgPix(16,16);
    bgPix.fill(QColor(preview_display_bg_color));
    buttonBGColor->setIcon(QIcon(bgPix));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()));

    QLabel* labelSelectBoxLeftColor = new QLabel(tr("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog_display_selectbox_left_color = settings_display_selectbox_left_color;
    preview_display_selectbox_left_color = dialog_display_selectbox_left_color;
    accept_display_selectbox_left_color = dialog_display_selectbox_left_color;
    QPixmap sBoxLCPix(16,16);
    sBoxLCPix.fill(QColor(preview_display_selectbox_left_color));
    buttonSelectBoxLeftColor->setIcon(QIcon(sBoxLCPix));
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()));

    QLabel* labelSelectBoxLeftFill = new QLabel(tr("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = new QPushButton(tr("Choose"), groupBoxColor);
    dialog_display_selectbox_left_fill = settings_display_selectbox_left_fill;
    preview_display_selectbox_left_fill = dialog_display_selectbox_left_fill;
    accept_display_selectbox_left_fill = dialog_display_selectbox_left_fill;
    QPixmap sBoxLFPix(16,16);
    sBoxLFPix.fill(QColor(preview_display_selectbox_left_fill));
    buttonSelectBoxLeftFill->setIcon(QIcon(sBoxLFPix));
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()));

    QLabel* labelSelectBoxRightColor = new QLabel(tr("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog_display_selectbox_right_color = settings_display_selectbox_right_color;
    preview_display_selectbox_right_color = dialog_display_selectbox_right_color;
    accept_display_selectbox_right_color = dialog_display_selectbox_right_color;
    QPixmap sBoxRCPix(16,16);
    sBoxRCPix.fill(QColor(preview_display_selectbox_right_color));
    buttonSelectBoxRightColor->setIcon(QIcon(sBoxRCPix));
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()));

    QLabel* labelSelectBoxRightFill = new QLabel(tr("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = new QPushButton(tr("Choose"), groupBoxColor);
    dialog_display_selectbox_right_fill = settings_display_selectbox_right_fill;
    preview_display_selectbox_right_fill = dialog_display_selectbox_right_fill;
    accept_display_selectbox_right_fill = dialog_display_selectbox_right_fill;
    QPixmap sBoxRFPix(16,16);
    sBoxRFPix.fill(QColor(preview_display_selectbox_right_fill));
    buttonSelectBoxRightFill->setIcon(QIcon(sBoxRFPix));
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()));

    QLabel* labelSelectBoxAlpha = new QLabel(tr("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    dialog_display_selectbox_alpha = settings_display_selectbox_alpha;
    preview_display_selectbox_alpha = dialog_display_selectbox_alpha;
    spinBoxSelectBoxAlpha->setValue(preview_display_selectbox_alpha);
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
    QDoubleSpinBox* spinBoxZoomScaleIn = new QDoubleSpinBox(groupBoxZoom);
    dialog_display_zoomscale_in = settings_display_zoomscale_in;
    spinBoxZoomScaleIn->setValue(dialog_display_zoomscale_in);
    spinBoxZoomScaleIn->setSingleStep(0.01);
    spinBoxZoomScaleIn->setRange(1.01, 10.00);
    connect(spinBoxZoomScaleIn, SIGNAL(valueChanged(double)), this, SLOT(spinBoxZoomScaleInValueChanged(double)));

    QLabel* labelZoomScaleOut = new QLabel(tr("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = new QDoubleSpinBox(groupBoxZoom);
    dialog_display_zoomscale_out = settings_display_zoomscale_out;
    spinBoxZoomScaleOut->setValue(dialog_display_zoomscale_out);
    spinBoxZoomScaleOut->setSingleStep(0.01);
    spinBoxZoomScaleOut->setRange(0.01, 0.99);
    connect(spinBoxZoomScaleOut, SIGNAL(valueChanged(double)), this, SLOT(spinBoxZoomScaleOutValueChanged(double)));

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
    */
}

void create_tab_prompt(void)
{
    /*
    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(tr("Colors"), widget);

    QLabel* labelTextColor = new QLabel(tr("Text Color"), groupBoxColor);
    QPushButton* buttonTextColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog_prompt_text_color = settings_prompt_text_color;
    preview_prompt_text_color = dialog_prompt_text_color;
    accept_prompt_text_color = dialog_prompt_text_color;
    QPixmap pix(16,16);
    pix.fill(QColor(preview_prompt_text_color));
    buttonTextColor->setIcon(QIcon(pix));
    connect(buttonTextColor, SIGNAL(clicked()), this, SLOT(choosePromptTextColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog_prompt_bg_color = settings_prompt_bg_color;
    preview_prompt_bg_color = dialog_prompt_bg_color;
    accept_prompt_bg_color = dialog_prompt_bg_color;
    QPixmap pixx(16,16);
    pixx.fill(QColor(preview_prompt_bg_color));
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
    dialog_prompt_font_family = settings_prompt_font_family;
    preview_prompt_font_family = dialog_prompt_font_family;
    comboBoxFontFamily->setCurrentFont(QFont(preview_prompt_font_family));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(const std::string&)), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(const std::string&)));
    QLabel* labelFontStyle = new QLabel(tr("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(preview_prompt_font_style);
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(const std::string&)), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(const std::string&)));
    QLabel* labelFontSize = new QLabel(tr("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    dialog_prompt_font_size = settings_prompt_font_size;
    preview_prompt_font_size = dialog_prompt_font_size;
    spinBoxFontSize->setValue(preview_prompt_font_size);
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

    QCheckBox* checkBoxPromptSaveHistory = new QCheckBox(tr("Save History"), groupBoxHistory);
    dialog_prompt_save_history = settings_prompt_save_history;
    checkBoxPromptSaveHistory->setChecked(dialog_prompt_save_history);
    connect(checkBoxPromptSaveHistory, SIGNAL(stateChanged(int)), this, SLOT(checkBoxPromptSaveHistoryStateChanged(int)));

    QCheckBox* checkBoxPromptSaveHistoryAsHtml = new QCheckBox(tr("Save As HTML"), groupBoxHistory);
    dialog_prompt_save_history_as_html = settings_prompt_save_history_as_html;
    checkBoxPromptSaveHistoryAsHtml->setChecked(dialog_prompt_save_history_as_html);
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
    */
}

void create_tab_open_save(void)
{
    std::vector<std::string> extensions = {
        "100", "10o", "ART", "BMC", "BRO", "CND", "COL", "CSD",
        "CSV", "DAT", "DEM", "DSB", "DST", "DSZ", "DXF", "EDR",
        "EMD", "EXP", "EXY", "EYS", "FXY", "GNC", "GT", "HUS",
        "INB", "JEF", "KSM", "PCD", "PCM", "PCQ", "PCS", "PEC",
        "PEL", "PEM", "PES", "PHB", "PHC", "RGB", "SEW", "SHV",
        "SST", "STX", "SVG", "T09", "TAP", "THR", "TXT",
        "U00", "U01", "VIP", "VP3", "XXX", "ZSK"
    };

    /*
    //Custom Filter
    QGroupBox* groupBoxCustomFilter = new QGroupBox(tr("Custom Filter"), widget);
    groupBoxCustomFilter->setEnabled(false); //TODO: Fixup custom filter

    dialog_opensave_custom_filter = settings_opensave_custom_filter;

    QCheckBox* checkBoxCustomFilter[200];

    for (std::vector<std::string>::size_type i=0; i<extensions.size(); i++) {
        checkBoxCustomFilter[i] = new QCheckBox(extensions[i],
                                                groupBoxCustomFilter);
        bool checked = dialog_opensave_custom_filter.contains("*."+extensions[i],
                                               Qt::CaseInsensitive);
        checkBoxCustomFilter[i]->setChecked(checked);
        connect(checkBoxCustomFilter[i], SIGNAL(stateChanged(int)), this,
                SLOT(checkBoxCustomFilterStateChanged(int)));
    }

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(tr("Select All"), groupBoxCustomFilter);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()));
    for (std::vector<std::string>::size_type i=0; i<extensions.size(); i++) {
        connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilter[i], SLOT(setChecked(bool)));
    }

    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", groupBoxCustomFilter);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()));
    for (std::vector<std::string>::size_type i=0; i<extensions.size(); i++) {
        connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilter[i], SLOT(setChecked(bool)));
    }

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    for (int i=0; i<10; i++)
    for (int j=0; j<10; j++) {
        if (10*i+j >= extensions.size()) {
            break;
        }
        gridLayoutCustomFilter->addWidget(checkBoxCustomFilter[10*i+j], j, i, Qt::AlignLeft);
    }
    gridLayoutCustomFilter->addWidget(buttonCustomFilterSelectAll, 0, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterClearAll,  1, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->setLayout(gridLayoutCustomFilter);

    if (dialog_opensave_custom_filter.contains("supported", Qt::CaseInsensitive)) buttonCustomFilterSelectAllClicked();

    //Opening
    QGroupBox* groupBoxOpening = new QGroupBox(tr("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(tr("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    //TODO: Add a button to clear the recent history.

    QLabel* labelRecentMaxFiles = new QLabel(tr("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    dialog_opensave_recent_max_files = settings_opensave_recent_max_files;
    spinBoxRecentMaxFiles->setValue(dialog_opensave_recent_max_files);
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
    dialog_opensave_trim_dst_num_jumps = settings_opensave_trim_dst_num_jumps;
    spinBoxTrimDstNumJumps->setValue(dialog_opensave_trim_dst_num_jumps);
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
    */
}

void create_tab_printing(void)
{
    /*
    //Default Printer
    QGroupBox* groupBoxDefaultPrinter = new QGroupBox(tr("Default Printer"), widget);

    QRadioButton* radioButtonUseSame = new QRadioButton(tr("Use as default device"), groupBoxDefaultPrinter);
    radioButtonUseSame->setChecked(!settings_printing_use_last_device);
    QRadioButton* radioButtonUseLast = new QRadioButton(tr("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(settings_printing_use_last_device);

    QComboBox* comboBoxDefaultDevice = new QComboBox(groupBoxDefaultPrinter);
    std::vector<QPrinterInfo> listAvailPrinters = QPrinterInfo::availablePrinters();
    foreach(QPrinterInfo info, listAvailPrinters)
    {
        comboBoxDefaultDevice->addItem(mainWin->load_icon("print"), info.printerName());
    }

    QVBoxLayout* vboxLayoutDefaultPrinter = new QVBoxLayout(groupBoxDefaultPrinter);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter->addWidget(comboBoxDefaultDevice);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseLast);
    groupBoxDefaultPrinter->setLayout(vboxLayoutDefaultPrinter);

    //Save Ink
    QGroupBox* groupBoxSaveInk = new QGroupBox(tr("Save Ink"), widget);

    QCheckBox* checkBoxDisableBG = new QCheckBox(tr("Disable Background"), groupBoxSaveInk);
    dialog_printing_disable_bg = settings_printing_disable_bg;
    checkBoxDisableBG->setChecked(dialog_printing_disable_bg);
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
    */
}

void create_tab_snap(void)
{
    /*
    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    */   
}

void create_tab_grid_ruler(void)
{
    /*
    //Grid Misc
    QGroupBox* groupBoxGridMisc = new QGroupBox(tr("Grid Misc"), widget);

    QCheckBox* checkBoxGridShowOnLoad = new QCheckBox(tr("Initially show grid when loading a file"), groupBoxGridMisc);
    dialog_grid_show_on_load = settings_grid_show_on_load;
    checkBoxGridShowOnLoad->setChecked(dialog_grid_show_on_load);
    connect(checkBoxGridShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOnLoadStateChanged(int)));

    QCheckBox* checkBoxGridShowOrigin = new QCheckBox(tr("Show the origin when the grid is enabled"), groupBoxGridMisc);
    dialog_grid_show_origin = settings_grid_show_origin;
    checkBoxGridShowOrigin->setChecked(dialog_grid_show_origin);
    connect(checkBoxGridShowOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOriginStateChanged(int)));

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = new QGroupBox(tr("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(tr("Match grid color to crosshair color"), groupBoxGridColor);
    dialog_grid_color_match_crosshair = settings_grid_color_match_crosshair;
    checkBoxGridColorMatchCrossHair->setChecked(dialog_grid_color_match_crosshair);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(tr("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(tr("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (dialog_grid_color_match_crosshair) {
        dialog_grid_color = settings_display_crosshair_color;
    }
    else {
        dialog_grid_color = settings_grid_color;
    }
    preview_grid_color = dialog_grid_color;
    accept_grid_color = dialog_grid_color;
    QPixmap gridPix(16,16);
    gridPix.fill(QColor(preview_grid_color));
    buttonGridColor->setIcon(QIcon(gridPix));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor->setEnabled(!dialog_grid_color_match_crosshair);
    buttonGridColor->setEnabled(!dialog_grid_color_match_crosshair);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor,                  1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor,                 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = new QGroupBox(tr("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(tr("Set grid size from opened file"), groupBoxGridGeom);
    dialog_grid_load_from_file = settings_grid_load_from_file;
    checkBoxGridLoadFromFile->setChecked(dialog_grid_load_from_file);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(tr("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    dialog_grid_type = settings_grid_type;
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(dialog_grid_type));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(const std::string&)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(const std::string&)));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(tr("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    dialog_grid_center_on_origin = settings_grid_center_on_origin;
    checkBoxGridCenterOnOrigin->setChecked(dialog_grid_center_on_origin);
    connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)));

    QLabel* labelGridCenterX = new QLabel(tr("Grid Center X"), groupBoxGridGeom);
    labelGridCenterX->setObjectName("labelGridCenterX");
    QDoubleSpinBox* spinBoxGridCenterX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridCenterX->setObjectName("spinBoxGridCenterX");
    dialog_grid_center_x = settings_grid_center_x;
    spinBoxGridCenterX->setSingleStep(1.000);
    spinBoxGridCenterX->setRange(-1000.000, 1000.000);
    spinBoxGridCenterX->setValue(dialog_grid_center_x);
    connect(spinBoxGridCenterX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridCenterXValueChanged(double)));

    QLabel* labelGridCenterY = new QLabel(tr("Grid Center Y"), groupBoxGridGeom);
    labelGridCenterY->setObjectName("labelGridCenterY");
    QDoubleSpinBox* spinBoxGridCenterY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridCenterY->setObjectName("spinBoxGridCenterY");
    dialog_grid_center_y = settings_grid_center_y;
    spinBoxGridCenterY->setSingleStep(1.000);
    spinBoxGridCenterY->setRange(-1000.000, 1000.000);
    spinBoxGridCenterY->setValue(dialog_grid_center_y);
    connect(spinBoxGridCenterY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridCenterYValueChanged(double)));

    QLabel* labelGridSizeX = new QLabel(tr("Grid Size X"), groupBoxGridGeom);
    labelGridSizeX->setObjectName("labelGridSizeX");
    QDoubleSpinBox* spinBoxGridSizeX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeX->setObjectName("spinBoxGridSizeX");
    dialog_grid_size_x = settings_grid_size_x;
    spinBoxGridSizeX->setSingleStep(1.000);
    spinBoxGridSizeX->setRange(1.000, 1000.000);
    spinBoxGridSizeX->setValue(dialog_grid_size_x);
    connect(spinBoxGridSizeX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeXValueChanged(double)));

    QLabel* labelGridSizeY = new QLabel(tr("Grid Size Y"), groupBoxGridGeom);
    labelGridSizeY->setObjectName("labelGridSizeY");
    QDoubleSpinBox* spinBoxGridSizeY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeY->setObjectName("spinBoxGridSizeY");
    dialog_grid_size_y = settings_grid_size_y;
    spinBoxGridSizeY->setSingleStep(1.000);
    spinBoxGridSizeY->setRange(1.000, 1000.000);
    spinBoxGridSizeY->setValue(dialog_grid_size_y);
    connect(spinBoxGridSizeY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeYValueChanged(double)));

    QLabel* labelGridSpacingX = new QLabel(tr("Grid Spacing X"), groupBoxGridGeom);
    labelGridSpacingX->setObjectName("labelGridSpacingX");
    QDoubleSpinBox* spinBoxGridSpacingX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingX->setObjectName("spinBoxGridSpacingX");
    dialog_grid_spacing_x = settings_grid_spacing_x;
    spinBoxGridSpacingX->setSingleStep(1.000);
    spinBoxGridSpacingX->setRange(0.001, 1000.000);
    spinBoxGridSpacingX->setValue(dialog_grid_spacing_x);
    connect(spinBoxGridSpacingX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingXValueChanged(double)));

    QLabel* labelGridSpacingY = new QLabel(tr("Grid Spacing Y"), groupBoxGridGeom);
    labelGridSpacingY->setObjectName("labelGridSpacingY");
    QDoubleSpinBox* spinBoxGridSpacingY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingY->setObjectName("spinBoxGridSpacingY");
    dialog_grid_spacing_y = settings_grid_spacing_y;
    spinBoxGridSpacingY->setSingleStep(1.000);
    spinBoxGridSpacingY->setRange(0.001, 1000.000);
    spinBoxGridSpacingY->setValue(dialog_grid_spacing_y);
    connect(spinBoxGridSpacingY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingYValueChanged(double)));

    QLabel* labelGridSizeRadius = new QLabel(tr("Grid Size Radius"), groupBoxGridGeom);
    labelGridSizeRadius->setObjectName("labelGridSizeRadius");
    QDoubleSpinBox* spinBoxGridSizeRadius = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeRadius->setObjectName("spinBoxGridSizeRadius");
    dialog_grid_size_radius = settings_grid_size_radius;
    spinBoxGridSizeRadius->setSingleStep(1.000);
    spinBoxGridSizeRadius->setRange(1.000, 1000.000);
    spinBoxGridSizeRadius->setValue(dialog_grid_size_radius);
    connect(spinBoxGridSizeRadius, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeRadiusValueChanged(double)));

    QLabel* labelGridSpacingRadius = new QLabel(tr("Grid Spacing Radius"), groupBoxGridGeom);
    labelGridSpacingRadius->setObjectName("labelGridSpacingRadius");
    QDoubleSpinBox* spinBoxGridSpacingRadius = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingRadius->setObjectName("spinBoxGridSpacingRadius");
    dialog_grid_spacing_radius = settings_grid_spacing_radius;
    spinBoxGridSpacingRadius->setSingleStep(1.000);
    spinBoxGridSpacingRadius->setRange(0.001, 1000.000);
    spinBoxGridSpacingRadius->setValue(dialog_grid_spacing_radius);
    connect(spinBoxGridSpacingRadius, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingRadiusValueChanged(double)));

    QLabel* labelGridSpacingAngle = new QLabel(tr("Grid Spacing Angle"), groupBoxGridGeom);
    labelGridSpacingAngle->setObjectName("labelGridSpacingAngle");
    QDoubleSpinBox* spinBoxGridSpacingAngle = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingAngle->setObjectName("spinBoxGridSpacingAngle");
    dialog_grid_spacing_angle = settings_grid_spacing_angle;
    spinBoxGridSpacingAngle->setSingleStep(1.000);
    spinBoxGridSpacingAngle->setRange(0.001, 1000.000);
    spinBoxGridSpacingAngle->setValue(dialog_grid_spacing_angle);
    connect(spinBoxGridSpacingAngle, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingAngleValueChanged(double)));

    labelGridType->setEnabled(!dialog_grid_load_from_file);
    comboBoxGridType->setEnabled(!dialog_grid_load_from_file);
    checkBoxGridCenterOnOrigin->setEnabled(!dialog_grid_load_from_file);
    labelGridCenterX->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridCenterX->setEnabled(!dialog_grid_load_from_file);
    labelGridCenterY->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridCenterY->setEnabled(!dialog_grid_load_from_file);
    labelGridSizeX->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridSizeX->setEnabled(!dialog_grid_load_from_file);
    labelGridSizeY->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridSizeY->setEnabled(!dialog_grid_load_from_file);
    labelGridSpacingX->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridSpacingX->setEnabled(!dialog_grid_load_from_file);
    labelGridSpacingY->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridSpacingY->setEnabled(!dialog_grid_load_from_file);
    labelGridSizeRadius->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridSizeRadius->setEnabled(!dialog_grid_load_from_file);
    labelGridSpacingRadius->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridSpacingRadius->setEnabled(!dialog_grid_load_from_file);
    labelGridSpacingAngle->setEnabled(!dialog_grid_load_from_file);
    spinBoxGridSpacingAngle->setEnabled(!dialog_grid_load_from_file);

    bool visibility = false;
    if (dialog_grid_type == "Circular") visibility = true;
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
    dialog_ruler_show_on_load = settings_ruler_show_on_load;
    checkBoxRulerShowOnLoad->setChecked(dialog_ruler_show_on_load);
    connect(checkBoxRulerShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRulerShowOnLoadStateChanged(int)));

    QLabel* labelRulerMetric = new QLabel(tr("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric",   true);
    dialog_ruler_metric = settings_ruler_metric;
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(dialog_ruler_metric));
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
    dialog_ruler_color = settings_ruler_color;
    preview_ruler_color = dialog_ruler_color;
    accept_ruler_color  = dialog_ruler_color;
    QPixmap rulerPix(16,16);
    rulerPix.fill(QColor(preview_ruler_color));
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
    dialog_ruler_pixel_size = settings_ruler_pixel_size;
    spinBoxRulerPixelSize->setSingleStep(1.000);
    spinBoxRulerPixelSize->setRange(20.000, 100.000);
    spinBoxRulerPixelSize->setValue(dialog_ruler_pixel_size);
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
    */
}

void create_tab_ortho_polar(void)
{
    /*
    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    */
}

void create_tab_quicksnap(void)
{
    /*
    std::string iconTheme = settings_general_icon_theme;

    //QSnap Locators
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(tr("Locators Used"), widget);

    QCheckBox* checkBoxQSnapEndPoint = new QCheckBox(tr("Endpoint"), groupBoxQSnapLoc);
    dialog_qsnap_endpoint = settings_qsnap_endpoint;
    checkBoxQSnapEndPoint->setChecked(dialog_qsnap_endpoint);
    checkBoxQSnapEndPoint->setIcon(mainWin->load_icon("locator-snaptoendpoint"));
    connect(checkBoxQSnapEndPoint, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapEndPointStateChanged(int)));

    QCheckBox* checkBoxQSnapMidPoint = new QCheckBox(tr("Midpoint"), groupBoxQSnapLoc);
    dialog_qsnap_midpoint = settings_qsnap_midpoint;
    checkBoxQSnapMidPoint->setChecked(dialog_qsnap_midpoint);
    checkBoxQSnapMidPoint->setIcon(mainWin->load_icon("locator-snaptomidpoint"));
    connect(checkBoxQSnapMidPoint, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapMidPointStateChanged(int)));

    QCheckBox* checkBoxQSnapCenter = new QCheckBox(tr("Center"), groupBoxQSnapLoc);
    dialog_qsnap_center = settings_qsnap_center;
    checkBoxQSnapCenter->setChecked(dialog_qsnap_center);
    checkBoxQSnapCenter->setIcon(mainWin->load_icon("locator-snaptocenter"));
    connect(checkBoxQSnapCenter, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapCenterStateChanged(int)));

    QCheckBox* checkBoxQSnapNode = new QCheckBox(tr("Node"), groupBoxQSnapLoc);
    dialog_qsnap_node = settings_qsnap_node;
    checkBoxQSnapNode->setChecked(dialog_qsnap_node);
    checkBoxQSnapNode->setIcon(mainWin->load_icon("locator-snaptonode"));
    connect(checkBoxQSnapNode, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapNodeStateChanged(int)));

    QCheckBox* checkBoxQSnapQuadrant = new QCheckBox(tr("Quadrant"), groupBoxQSnapLoc);
    dialog_qsnap_quadrant = settings_qsnap_quadrant;
    checkBoxQSnapQuadrant->setChecked(dialog_qsnap_quadrant);
    checkBoxQSnapQuadrant->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoquadrant" + ".png"));
    connect(checkBoxQSnapQuadrant, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapQuadrantStateChanged(int)));

    QCheckBox* checkBoxQSnapIntersection = new QCheckBox(tr("Intersection"), groupBoxQSnapLoc);
    dialog_qsnap_intersection = settings_qsnap_intersection;
    checkBoxQSnapIntersection->setChecked(dialog_qsnap_intersection);
    checkBoxQSnapIntersection->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptointersection" + ".png"));
    connect(checkBoxQSnapIntersection, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapIntersectionStateChanged(int)));

    QCheckBox* checkBoxQSnapExtension = new QCheckBox(tr("Extension"), groupBoxQSnapLoc);
    dialog_qsnap_extension = settings_qsnap_extension;
    checkBoxQSnapExtension->setChecked(dialog_qsnap_extension);
    checkBoxQSnapExtension->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoextension" + ".png"));
    connect(checkBoxQSnapExtension, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapExtensionStateChanged(int)));

    QCheckBox* checkBoxQSnapInsertion = new QCheckBox(tr("Insertion"), groupBoxQSnapLoc);
    dialog_qsnap_insertion = settings_qsnap_insertion;
    checkBoxQSnapInsertion->setChecked(dialog_qsnap_insertion);
    checkBoxQSnapInsertion->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoinsert" + ".png"));
    connect(checkBoxQSnapInsertion, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapInsertionStateChanged(int)));

    QCheckBox* checkBoxQSnapPerpendicular = new QCheckBox(tr("Perpendicular"), groupBoxQSnapLoc);
    dialog_qsnap_perpendicular = settings_qsnap_perpendicular;
    checkBoxQSnapPerpendicular->setChecked(dialog_qsnap_perpendicular);
    checkBoxQSnapPerpendicular->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoperpendicular" + ".png"));
    connect(checkBoxQSnapPerpendicular, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapPerpendicularStateChanged(int)));

    QCheckBox* checkBoxQSnapTangent = new QCheckBox(tr("Tangent"), groupBoxQSnapLoc);
    dialog_qsnap_tangent = settings_qsnap_tangent;
    checkBoxQSnapTangent->setChecked(dialog_qsnap_tangent);
    checkBoxQSnapTangent->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptotangent" + ".png"));
    connect(checkBoxQSnapTangent, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapTangentStateChanged(int)));

    QCheckBox* checkBoxQSnapNearest = new QCheckBox(tr("Nearest"), groupBoxQSnapLoc);
    dialog_qsnap_nearest = settings_qsnap_nearest;
    checkBoxQSnapNearest->setChecked(dialog_qsnap_nearest);
    checkBoxQSnapNearest->setIcon(mainWin->load_icon("locator-snaptonearest"));
    connect(checkBoxQSnapNearest, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapNearestStateChanged(int)));

    QCheckBox* checkBoxQSnapApparent = new QCheckBox(tr("Apparent Intersection"), groupBoxQSnapLoc);
    dialog_qsnap_apparent = settings_qsnap_apparent;
    checkBoxQSnapApparent->setChecked(dialog_qsnap_apparent);
    checkBoxQSnapApparent->setIcon(mainWin->load_icon("locator-snaptoapparentintersection"));
    connect(checkBoxQSnapApparent, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapApparentStateChanged(int)));

    QCheckBox* checkBoxQSnapParallel = new QCheckBox(tr("Parallel"), groupBoxQSnapLoc);
    dialog_qsnap_parallel = settings_qsnap_parallel;
    checkBoxQSnapParallel->setChecked(dialog_qsnap_parallel);
    checkBoxQSnapParallel->setIcon(mainWin->load_icon("locator-snaptoparallel"));
    connect(checkBoxQSnapParallel, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapParallelStateChanged(int)));

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
    dialog_qsnap_locator_color = settings_qsnap_locator_color;
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(dialog_qsnap_locator_color));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(tr("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    dialog_qsnap_locator_size = settings_qsnap_locator_size;
    sliderQSnapLocSize->setValue(dialog_qsnap_locator_size);
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
    dialog_qsnap_aperture_size = settings_qsnap_aperture_size;
    sliderQSnapApertureSize->setValue(dialog_qsnap_aperture_size);
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
    */
}

void create_tab_quicktrack(void)
{
    /*
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
    */
}

void create_tab_lineweight(void)
{
    /*
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    //Misc
    QGroupBox* groupBoxLwtMisc = new QGroupBox(tr("LineWeight Misc"), widget);

    QGraphicsScene* s = mainWin->activeScene();

    QCheckBox* checkBoxShowLwt = new QCheckBox(tr("Show LineWeight"), groupBoxLwtMisc);
    if (s) {
        dialog_lwt_show_lwt = s->property(ENABLE_LWT).toBool();
    }
    else {
        dialog_lwt_show_lwt = settings_lwt_show_lwt;
    }
    preview_lwt_show_lwt = dialog_lwt_show_lwt;
    checkBoxShowLwt->setChecked(preview_lwt_show_lwt);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(tr("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if (s) {
        dialog_lwt_real_render = s->property(ENABLE_REAL).toBool();
    }
    else {
        dialog_lwt_real_render = settings_lwt_real_render;
    }
    preview_lwt_real_render = dialog_lwt_real_render;
    checkBoxRealRender->setChecked(preview_lwt_real_render);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(dialog_lwt_show_lwt);

    QLabel* labelDefaultLwt = new QLabel(tr("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //TODO: remove later
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    dialog_lwt_default_lwt = settings_lwt_default_lwt;
    //TODO: populate the comboBox and set the initial value
    comboBoxDefaultLwt->addItem(std::string().setNum(dialog_lwt_default_lwt, 'F', 2).append(" mm"), dialog_lwt_default_lwt);
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
    */
}

void create_tab_selection(void)
{
    /*
    QWidget* widget = new QWidget(this);

    //Selection Modes
    QGroupBox* groupBoxSelectionModes = new QGroupBox(tr("Modes"), widget);

    QCheckBox* checkBoxSelectionModePickFirst = new QCheckBox(tr("Allow Preselection (PickFirst)"), groupBoxSelectionModes);
    dialog_selection_mode_pickfirst = settings_selection_mode_pickfirst;
    checkBoxSelectionModePickFirst->setChecked(dialog_selection_mode_pickfirst);
    checkBoxSelectionModePickFirst->setChecked(true); checkBoxSelectionModePickFirst->setEnabled(false); //TODO: Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickFirstStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickAdd = new QCheckBox(tr("Add to Selection (PickAdd)"), groupBoxSelectionModes);
    dialog_selection_mode_pickadd = settings_selection_mode_pickadd;
    checkBoxSelectionModePickAdd->setChecked(dialog_selection_mode_pickadd);
    connect(checkBoxSelectionModePickAdd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickAddStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickDrag = new QCheckBox(tr("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    dialog_selection_mode_pickdrag = settings_selection_mode_pickdrag;
    checkBoxSelectionModePickDrag->setChecked(dialog_selection_mode_pickdrag);
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
    dialog_selection_coolgrip_color = settings_selection_coolgrip_color;
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(dialog_selection_coolgrip_color));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionCoolGripColorCurrentIndexChanged(int)));

    QLabel* labelHotGripColor = new QLabel(tr("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    dialog_selection_hotgrip_color = settings_selection_hotgrip_color;
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(dialog_selection_hotgrip_color));
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
    dialog_selection_grip_size = settings_selection_grip_size;
    sliderSelectionGripSize->setValue(dialog_selection_grip_size);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionGripSizeValueChanged(int)));

    QLabel* labelSelectionPickBoxSize = new QLabel(tr("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    dialog_selection_pickbox_size = settings_selection_pickbox_size;
    sliderSelectionPickBoxSize->setValue(dialog_selection_pickbox_size);
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
    */
}

#if 0
void Settings_Dialog::addColorsToComboBox(QComboBox* comboBox)
{
    comboBox->addItem(mainWin->load_icon("colorred"), translate("Red"), qRgb(255, 0, 0));
    comboBox->addItem(mainWin->load_icon("coloryellow"), translate("Yellow"),  qRgb(255,255,  0));
    comboBox->addItem(mainWin->load_icon("colorgreen"),   translate("Green"),   qRgb(  0,255,  0));
    comboBox->addItem(mainWin->load_icon("colorcyan"),    translate("Cyan"),    qRgb(  0,255,255));
    comboBox->addItem(mainWin->load_icon("colorblue"),    translate("Blue"),    qRgb(  0,  0,255));
    comboBox->addItem(mainWin->load_icon("colormagenta"), translate("Magenta"), qRgb(255,  0,255));
    comboBox->addItem(mainWin->load_icon("colorwhite"),   translate("White"),   qRgb(255,255,255));
    //TODO: Add Other... so the user can select custom colors
}

void Settings_Dialog::comboBoxLanguageCurrentIndexChanged(const std::string& lang)
{
    dialog_general_language = lang.toLower();
}

void Settings_Dialog::comboBoxIconThemeCurrentIndexChanged(const std::string& theme)
{
    dialog_general_icon_theme.string_value = theme.toLocal8Bit();
}

void Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        dialog_general_icon_size = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog_general_icon_size = 16;
        }
    }
    else {
        dialog_general_icon_size = 16;
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    preview_general_mdi_bg_use_logo = checked;
    mainWin->mdiArea->useBackgroundLogo(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        std::string selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, translate("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        translate("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            accept_general_mdi_bg_logo = selectedImage;
        }

        //Update immediately so it can be previewed
        mainWin->mdiArea->setBackgroundLogo(accept_general_mdi_bg_logo);
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    preview_general_mdi_bg_use_texture = checked;
    mainWin->mdiArea->useBackgroundTexture(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        std::string selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, translate("Open File"),
                        #if QT_VERSION >= 0x050000
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), //Qt5
                        #else
                        QDesktopServices::storageLocation(QDesktopServices::PicturesLocation), //Qt4
                        #endif
                        translate("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            accept_general_mdi_bg_texture = selectedImage;
        }

        //Update immediately so it can be previewed
        mainWin->mdiArea->setBackgroundTexture(accept_general_mdi_bg_texture);
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    preview_general_mdi_bg_use_color = checked;
    mainWin->mdiArea->useBackgroundColor(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_general_mdi_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGeneralMdiBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_general_mdi_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_general_mdi_bg_color));
            button->setIcon(QIcon(pix));
            mainWin->mdiArea->setBackgroundColor(QColor(accept_general_mdi_bg_color));
        }
        else {
            mainWin->mdiArea->setBackgroundColor(QColor(dialog_general_mdi_bg_color));
        }
    }
}

void Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    preview_general_mdi_bg_color = color.rgb();
    mainWin->mdiArea->setBackgroundColor(QColor(preview_general_mdi_bg_color));
}

void Settings_Dialog::checkBoxTipOfTheDayStateChanged(int checked)
{
    dialog_general_tip_of_the_day = checked;
}

void Settings_Dialog::checkBoxUseOpenGLStateChanged(int checked)
{
    dialog_display_use_opengl = checked;
}

void Settings_Dialog::checkBoxRenderHintAAStateChanged(int checked)
{
    dialog_display_renderhint_aa = checked;
}

void Settings_Dialog::checkBoxRenderHintTextAAStateChanged(int checked)
{
    dialog_display_renderhint_text_aa = checked;
}

void Settings_Dialog::checkBoxRenderHintSmoothPixStateChanged(int checked)
{
    dialog_display_renderhint_smooth_pix = checked;
}

void Settings_Dialog::checkBoxRenderHintHighAAStateChanged(int checked)
{
    dialog_display_renderhint_high_aa = checked;
}

void Settings_Dialog::checkBoxRenderHintNonCosmeticStateChanged(int checked)
{
    dialog_display_renderhint_noncosmetic = checked;
}

void Settings_Dialog::checkBoxShowScrollBarsStateChanged(int checked)
{
    preview_display_show_scrollbars = checked;
    mainWin->updateAllViewScrollBars(preview_display_show_scrollbars);
}

void Settings_Dialog::comboBoxScrollBarWidgetCurrentIndexChanged(int index)
{
    dialog_display_scrollbar_widget_num = index;
}

void Settings_Dialog::spinBoxZoomScaleInValueChanged(double value)
{
    dialog_display_zoomscale_in = value;
}

void Settings_Dialog::spinBoxZoomScaleOutValueChanged(double value)
{
    dialog_display_zoomscale_out = value;
}

void Settings_Dialog::checkBoxDisableBGStateChanged(int checked)
{
    dialog_printing_disable_bg = checked;
}

void Settings_Dialog::chooseDisplayCrossHairColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_display_crosshair_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayCrossHairColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_display_crosshair_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_display_crosshair_color));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewCrossHairColors(accept_display_crosshair_color);
        }
        else {
            mainWin->updateAllViewCrossHairColors(dialog_display_crosshair_color);
        }
    }
}

void Settings_Dialog::currentDisplayCrossHairColorChanged(const QColor& color)
{
    preview_display_crosshair_color = color.rgb();
    mainWin->updateAllViewCrossHairColors(preview_display_crosshair_color);
}

void Settings_Dialog::chooseDisplayBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_display_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_display_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_display_bg_color));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewBackgroundColors(accept_display_bg_color);
        }
        else {
            mainWin->updateAllViewBackgroundColors(dialog_display_bg_color);
        }
    }
}

void Settings_Dialog::currentDisplayBackgroundColorChanged(const QColor& color)
{
    preview_display_bg_color = color.rgb();
    mainWin->updateAllViewBackgroundColors(preview_display_bg_color);
}

void Settings_Dialog::chooseDisplaySelectBoxLeftColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_display_selectbox_left_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_display_selectbox_left_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_display_selectbox_left_color));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewSelectBoxColors(accept_display_selectbox_left_color,
                                                  accept_display_selectbox_left_fill,
                                                  accept_display_selectbox_right_color,
                                                  accept_display_selectbox_right_fill,
                                                  preview_display_selectbox_alpha);
        }
        else {
            mainWin->updateAllViewSelectBoxColors(dialog_display_selectbox_left_color,
                                                  dialog_display_selectbox_left_fill,
                                                  dialog_display_selectbox_right_color,
                                                  dialog_display_selectbox_right_fill,
                                                  preview_display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftColorChanged(const QColor& color)
{
    preview_display_selectbox_left_color = color.rgb();
    mainWin->updateAllViewSelectBoxColors(preview_display_selectbox_left_color,
                                          preview_display_selectbox_left_fill,
                                          preview_display_selectbox_right_color,
                                          preview_display_selectbox_right_fill,
                                          preview_display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxLeftFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_display_selectbox_left_fill), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftFillChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_display_selectbox_left_fill = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_display_selectbox_left_fill));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewSelectBoxColors(accept_display_selectbox_left_color,
                                                  accept_display_selectbox_left_fill,
                                                  accept_display_selectbox_right_color,
                                                  accept_display_selectbox_right_fill,
                                                  preview_display_selectbox_alpha);
        }
        else {
            mainWin->updateAllViewSelectBoxColors(dialog_display_selectbox_left_color,
                                                  dialog_display_selectbox_left_fill,
                                                  dialog_display_selectbox_right_color,
                                                  dialog_display_selectbox_right_fill,
                                                  preview_display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftFillChanged(const QColor& color)
{
    preview_display_selectbox_left_fill = color.rgb();
    mainWin->updateAllViewSelectBoxColors(preview_display_selectbox_left_color,
                                          preview_display_selectbox_left_fill,
                                          preview_display_selectbox_right_color,
                                          preview_display_selectbox_right_fill,
                                          preview_display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxRightColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_display_selectbox_right_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_display_selectbox_right_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_display_selectbox_right_color));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewSelectBoxColors(accept_display_selectbox_left_color,
                                                  accept_display_selectbox_left_fill,
                                                  accept_display_selectbox_right_color,
                                                  accept_display_selectbox_right_fill,
                                                  preview_display_selectbox_alpha);
        }
        else {
            mainWin->updateAllViewSelectBoxColors(dialog_display_selectbox_left_color,
                                                  dialog_display_selectbox_left_fill,
                                                  dialog_display_selectbox_right_color,
                                                  dialog_display_selectbox_right_fill,
                                                  preview_display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxRightColorChanged(const QColor& color)
{
    preview_display_selectbox_right_color = color.rgb();
    mainWin->updateAllViewSelectBoxColors(preview_display_selectbox_left_color,
                                          preview_display_selectbox_left_fill,
                                          preview_display_selectbox_right_color,
                                          preview_display_selectbox_right_fill,
                                          preview_display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxRightFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_display_selectbox_right_fill), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightFillChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_display_selectbox_right_fill = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_display_selectbox_right_fill));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewSelectBoxColors(accept_display_selectbox_left_color,
                                                  accept_display_selectbox_left_fill,
                                                  accept_display_selectbox_right_color,
                                                  accept_display_selectbox_right_fill,
                                                  preview_display_selectbox_alpha);
        }
        else {
            mainWin->updateAllViewSelectBoxColors(dialog_display_selectbox_left_color,
                                                  dialog_display_selectbox_left_fill,
                                                  dialog_display_selectbox_right_color,
                                                  dialog_display_selectbox_right_fill,
                                                  preview_display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxRightFillChanged(const QColor& color)
{
    preview_display_selectbox_right_fill = color.rgb();
    mainWin->updateAllViewSelectBoxColors(preview_display_selectbox_left_color,
                                          preview_display_selectbox_left_fill,
                                          preview_display_selectbox_right_color,
                                          preview_display_selectbox_right_fill,
                                          preview_display_selectbox_alpha);
}

void Settings_Dialog::spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    preview_display_selectbox_alpha = value;
    mainWin->updateAllViewSelectBoxColors(accept_display_selectbox_left_color,
                                          accept_display_selectbox_left_fill,
                                          accept_display_selectbox_right_color,
                                          accept_display_selectbox_right_fill,
                                          preview_display_selectbox_alpha);
}

void Settings_Dialog::choosePromptTextColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_prompt_text_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentPromptTextColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_prompt_text_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_prompt_text_color));
            button->setIcon(QIcon(pix));
            mainWin->prompt->setPromptTextColor(QColor(accept_prompt_text_color));
        }
        else {
            mainWin->prompt->setPromptTextColor(QColor(dialog_prompt_text_color));
        }
    }
}

void Settings_Dialog::currentPromptTextColorChanged(const QColor& color)
{
    preview_prompt_text_color = color.rgb();
    mainWin->prompt->setPromptTextColor(QColor(preview_prompt_text_color));
}

void Settings_Dialog::choosePromptBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_prompt_bg_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentPromptBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_prompt_bg_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_prompt_bg_color));
            button->setIcon(QIcon(pix));
            mainWin->prompt->setPromptBackgroundColor(QColor(accept_prompt_bg_color));
        }
        else {
            mainWin->prompt->setPromptBackgroundColor(QColor(dialog_prompt_bg_color));
        }
    }
}

void Settings_Dialog::currentPromptBackgroundColorChanged(const QColor& color)
{
    preview_prompt_bg_color = color.rgb();
    mainWin->prompt->setPromptBackgroundColor(QColor(preview_prompt_bg_color));
}

void Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(const std::string& family)
{
    preview_prompt_font_family = family;
    mainWin->prompt->setPromptFontFamily(preview_prompt_font_family);
}

void Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(const std::string& style)
{
    preview_prompt_font_style = style;
    mainWin->prompt->setPromptFontStyle(preview_prompt_font_style);
}

void Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    preview_prompt_font_size = value;
    mainWin->prompt->setPromptFontSize(preview_prompt_font_size);
}

void Settings_Dialog::checkBoxPromptSaveHistoryStateChanged(int checked)
{
    dialog_prompt_save_history = checked;
}

void Settings_Dialog::checkBoxPromptSaveHistoryAsHtmlStateChanged(int checked)
{
    dialog_prompt_save_history_as_html = checked;
}

void Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        std::string format = checkBox->text();
        debug_message("CustomFilter: %s %d", qPrintable(format), checked);
        if (checked)
            dialog_opensave_custom_filter.append(" *." + format.toLower());
        else
            dialog_opensave_custom_filter.remove("*." + format, Qt::CaseInsensitive);
        //dialog_opensave_custom_filter = checked; //TODO
    }
}

void Settings_Dialog::buttonCustomFilterSelectAllClicked()
{
    emit buttonCustomFilterSelectAll(true);
    dialog_opensave_custom_filter = "supported";
}

void Settings_Dialog::buttonCustomFilterClearAllClicked()
{
    emit buttonCustomFilterClearAll(false);
    dialog_opensave_custom_filter.clear();
}

void Settings_Dialog::spinBoxRecentMaxFilesValueChanged(int value)
{
    dialog_opensave_recent_max_files = value;
}

void Settings_Dialog::spinBoxTrimDstNumJumpsValueChanged(int value)
{
    dialog_opensave_trim_dst_num_jumps = value;
}

void Settings_Dialog::checkBoxGridShowOnLoadStateChanged(int checked)
{
    dialog_grid_show_on_load = checked;
}

void Settings_Dialog::checkBoxGridShowOriginStateChanged(int checked)
{
    dialog_grid_show_origin = checked;
}

void Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    dialog_grid_color_match_crosshair = checked;
    if (dialog_grid_color_match_crosshair) {
        mainWin->updateAllViewGridColors(accept_display_crosshair_color);
    }
    else {
        mainWin->updateAllViewGridColors(accept_grid_color);
    }

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QLabel* labelGridColor = parent->findChild<QLabel*>("labelGridColor");
            if (labelGridColor) labelGridColor->setEnabled(!dialog_grid_color_match_crosshair);
            QPushButton* buttonGridColor = parent->findChild<QPushButton*>("buttonGridColor");
            if (buttonGridColor) buttonGridColor->setEnabled(!dialog_grid_color_match_crosshair);
        }
    }
}

void Settings_Dialog::chooseGridColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_grid_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGridColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_grid_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_grid_color));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewGridColors(accept_grid_color);
        }
        else {
            mainWin->updateAllViewGridColors(dialog_grid_color);
        }
    }
}

void Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    preview_grid_color = color.rgb();
    mainWin->updateAllViewGridColors(preview_grid_color);
}

void Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    dialog_grid_load_from_file = checked;

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
        labelGridType->setEnabled(!dialog_grid_load_from_file);
    }
    QComboBox* comboBoxGridType = parent->findChild<QComboBox*>("comboBoxGridType");
    if (comboBoxGridType) {
        comboBoxGridType->setEnabled(!dialog_grid_load_from_file);
    }
    QCheckBox* checkBoxGridCenterOnOrigin = parent->findChild<QCheckBox*>("checkBoxGridCenterOnOrigin");
    if (checkBoxGridCenterOnOrigin)
        checkBoxGridCenterOnOrigin->setEnabled(!dialog_grid_load_from_file);
    QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
    if (labelGridCenterX)
        labelGridCenterX->setEnabled(!dialog_grid_load_from_file && !dialog_grid_center_on_origin);
    QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
    if (spinBoxGridCenterX)
        spinBoxGridCenterX->setEnabled(!dialog_grid_load_from_file && !dialog_grid_center_on_origin);
    QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
    if (labelGridCenterY)
        labelGridCenterY->setEnabled(!dialog_grid_load_from_file && !dialog_grid_center_on_origin);
    QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
    if (spinBoxGridCenterY)
        spinBoxGridCenterY->setEnabled(!dialog_grid_load_from_file && !dialog_grid_center_on_origin);
    QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
    if (labelGridSizeX)
        labelGridSizeX->setEnabled(!dialog_grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
    if (spinBoxGridSizeX)
        spinBoxGridSizeX->setEnabled(!dialog_grid_load_from_file);
    QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
    if (labelGridSizeY)
        labelGridSizeY->setEnabled(!dialog_grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
    if (spinBoxGridSizeY)
        spinBoxGridSizeY->setEnabled(!dialog_grid_load_from_file);
    QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
    if (labelGridSpacingX)
        labelGridSpacingX->setEnabled(!dialog_grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSpacingX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingX");
    if (spinBoxGridSpacingX)
        spinBoxGridSpacingX->setEnabled(!dialog_grid_load_from_file);
    QLabel* labelGridSpacingY = parent->findChild<QLabel*>("labelGridSpacingY");
    if (labelGridSpacingY)
        labelGridSpacingY->setEnabled(!dialog_grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSpacingY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingY");
    if (spinBoxGridSpacingY)
        spinBoxGridSpacingY->setEnabled(!dialog_grid_load_from_file);
    QLabel* labelGridSizeRadius = parent->findChild<QLabel*>("labelGridSizeRadius");
    if (labelGridSizeRadius)
        labelGridSizeRadius->setEnabled(!dialog_grid_load_from_file);
    QDoubleSpinBox* spinBoxGridSizeRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeRadius");
    if (spinBoxGridSizeRadius) {
        spinBoxGridSizeRadius->setEnabled(!dialog_grid_load_from_file);
    }
    QLabel* labelGridSpacingRadius = parent->findChild<QLabel*>("labelGridSpacingRadius");
    if (labelGridSpacingRadius) {
        labelGridSpacingRadius->setEnabled(!dialog_grid_load_from_file);
    }
    QDoubleSpinBox* spinBoxGridSpacingRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingRadius");
    if (spinBoxGridSpacingRadius) {
        spinBoxGridSpacingRadius->setEnabled(!dialog_grid_load_from_file);
    }
    QLabel* labelGridSpacingAngle = parent->findChild<QLabel*>("labelGridSpacingAngle");
    if (labelGridSpacingAngle) {
        labelGridSpacingAngle->setEnabled(!dialog_grid_load_from_file);
    }
    QDoubleSpinBox* spinBoxGridSpacingAngle = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingAngle");
    if (spinBoxGridSpacingAngle) {
        spinBoxGridSpacingAngle->setEnabled(!dialog_grid_load_from_file);
    }
}

void Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(const std::string& type)
{
    dialog_grid_type = type;

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (!parent) {
        return;
    }
    bool visibility = false;
    if (type == "Circular") {
        visibility = true;
    }

    QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
    if (labelGridSizeX) {
        labelGridSizeX->setVisible(!visibility);
    }
    QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
    if (spinBoxGridSizeX) {
        spinBoxGridSizeX->setVisible(!visibility);
    }
    QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
    if (labelGridSizeY)
        labelGridSizeY->setVisible(!visibility);
    QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
    if (spinBoxGridSizeY)
        spinBoxGridSizeY->setVisible(!visibility);
    QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
    if (labelGridSpacingX)
        labelGridSpacingX->setVisible(!visibility);
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

void Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    dialog_grid_center_on_origin = checked;

    QObject* senderObj = sender();
    if (senderObj)
    {
        QObject* parent = senderObj->parent();
        if (parent)
        {
            QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
            if (labelGridCenterX) labelGridCenterX->setEnabled(!dialog_grid_center_on_origin);
            QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
            if (spinBoxGridCenterX) spinBoxGridCenterX->setEnabled(!dialog_grid_center_on_origin);
            QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
            if (labelGridCenterY) labelGridCenterY->setEnabled(!dialog_grid_center_on_origin);
            QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
            if (spinBoxGridCenterY) spinBoxGridCenterY->setEnabled(!dialog_grid_center_on_origin);
        }
    }
}

void Settings_Dialog::spinBoxGridCenterXValueChanged(double value)
{
    dialog_grid_center_x = value;
}

void Settings_Dialog::spinBoxGridCenterYValueChanged(double value)
{
    dialog_grid_center_y = value;
}

void Settings_Dialog::spinBoxGridSizeXValueChanged(double value)
{
    dialog_grid_size_x = value;
}

void Settings_Dialog::spinBoxGridSizeYValueChanged(double value)
{
    dialog_grid_size_y = value;
}

void Settings_Dialog::spinBoxGridSpacingXValueChanged(double value)
{
    dialog_grid_spacing_x = value;
}

void Settings_Dialog::spinBoxGridSpacingYValueChanged(double value)
{
    dialog_grid_spacing_y = value;
}

void Settings_Dialog::spinBoxGridSizeRadiusValueChanged(double value)
{
    dialog_grid_size_radius = value;
}

void Settings_Dialog::spinBoxGridSpacingRadiusValueChanged(double value)
{
    dialog_grid_spacing_radius = value;
}

void Settings_Dialog::spinBoxGridSpacingAngleValueChanged(double value)
{
    dialog_grid_spacing_angle = value;
}

void Settings_Dialog::checkBoxRulerShowOnLoadStateChanged(int checked)
{
    dialog_ruler_show_on_load = checked;
}

void Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        dialog_ruler_metric = comboBox->itemData(index).toBool();
    }
    else {
        dialog_ruler_metric = true;
    }
}

void Settings_Dialog::chooseRulerColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_ruler_color), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentRulerColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted)
        {
            accept_ruler_color = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_ruler_color));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewRulerColors(accept_ruler_color);
        }
        else
        {
            mainWin->updateAllViewRulerColors(dialog_ruler_color);
        }
    }
}

void Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    preview_ruler_color = color.rgb();
    mainWin->updateAllViewRulerColors(preview_ruler_color);
}

void Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    dialog_ruler_pixel_size = value;
}

void Settings_Dialog::checkBoxQSnapEndPointStateChanged(int checked)
{
    dialog_qsnap_endpoint = checked;
}

void Settings_Dialog::checkBoxQSnapMidPointStateChanged(int checked)
{
    dialog_qsnap_midpoint = checked;
}

void Settings_Dialog::checkBoxQSnapCenterStateChanged(int checked)
{
    dialog_qsnap_center = checked;
}

void Settings_Dialog::checkBoxQSnapNodeStateChanged(int checked)
{
    dialog_qsnap_node = checked;
}

void Settings_Dialog::checkBoxQSnapQuadrantStateChanged(int checked)
{
    dialog_qsnap_quadrant = checked;
}

void Settings_Dialog::checkBoxQSnapIntersectionStateChanged(int checked)
{
    dialog_qsnap_intersection = checked;
}

void Settings_Dialog::checkBoxQSnapExtensionStateChanged(int checked)
{
    dialog_qsnap_extension = checked;
}

void Settings_Dialog::checkBoxQSnapInsertionStateChanged(int checked)
{
    dialog_qsnap_insertion = checked;
}

void Settings_Dialog::checkBoxQSnapPerpendicularStateChanged(int checked)
{
    dialog_qsnap_perpendicular = checked;
}

void Settings_Dialog::checkBoxQSnapTangentStateChanged(int checked)
{
    dialog_qsnap_tangent = checked;
}

void Settings_Dialog::checkBoxQSnapNearestStateChanged(int checked)
{
    dialog_qsnap_nearest = checked;
}

void Settings_Dialog::checkBoxQSnapApparentStateChanged(int checked)
{
    dialog_qsnap_apparent = checked;
}

void Settings_Dialog::checkBoxQSnapParallelStateChanged(int checked)
{
    dialog_qsnap_parallel = checked;
}

void Settings_Dialog::buttonQSnapSelectAllClicked()
{
    emit buttonQSnapSelectAll(true);
}

void Settings_Dialog::buttonQSnapClearAllClicked()
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

void Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    unsigned int defaultColor = qRgb(255,255,0); //Yellow
    if (comboBox) {
        bool ok = 0;
        dialog_qsnap_locator_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog_qsnap_locator_color = defaultColor;
        }
    }
    else {
        dialog_qsnap_locator_color = defaultColor;
    }
}

void Settings_Dialog::sliderQSnapLocatorSizeValueChanged(int value)
{
    dialog_qsnap_locator_size = value;
}

void Settings_Dialog::sliderQSnapApertureSizeValueChanged(int value)
{
    dialog_qsnap_aperture_size = value;
}

void Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    preview_lwt_show_lwt = checked;
    if (preview_lwt_show_lwt) {
        mainWin->statusbar->statusBarLwtButton->enableLwt();
    }
    else {
        mainWin->statusbar->statusBarLwtButton->disableLwt();
    }

    QObject* senderObj = sender();
    if (senderObj)
    {
        QObject* parent = senderObj->parent();
        if (parent)
        {
            QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
            if (checkBoxRealRender) checkBoxRealRender->setEnabled(preview_lwt_show_lwt);
        }
    }
}

void Settings_Dialog::checkBoxLwtRealRenderStateChanged(int checked)
{
    preview_lwt_real_render = checked;
    if (preview_lwt_real_render) {
        mainWin->statusbar->statusBarLwtButton->enableReal();
    }
    else {
        mainWin->statusbar->statusBarLwtButton->disableReal();
    }
}

void Settings_Dialog::checkBoxSelectionModePickFirstStateChanged(int checked)
{
    dialog_selection_mode_pickfirst = checked;
}

void Settings_Dialog::checkBoxSelectionModePickAddStateChanged(int checked)
{
    dialog_selection_mode_pickadd = checked;
}

void Settings_Dialog::checkBoxSelectionModePickDragStateChanged(int checked)
{
    dialog_selection_mode_pickdrag = checked;
}

void Settings_Dialog::sliderSelectionGripSizeValueChanged(int value)
{
    dialog_selection_grip_size = value;
}

void Settings_Dialog::sliderSelectionPickBoxSizeValueChanged(int value)
{
    dialog_selection_pickbox_size = value;
}

void Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    unsigned int defaultColor = qRgb(0,0,255); //Blue
    if (comboBox)
    {
        bool ok = 0;
        dialog_selection_coolgrip_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            dialog_selection_coolgrip_color = defaultColor;
    }
    else
        dialog_selection_coolgrip_color = defaultColor;
}

void Settings_Dialog::comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    unsigned int defaultColor = qRgb(255,0,0); //Red
    if (comboBox)
    {
        bool ok = 0;
        dialog_selection_hotgrip_color = comboBox->itemData(index).toUInt(&ok);
        if (!ok)
            dialog_selection_hotgrip_color = defaultColor;
    }
    else
        dialog_selection_hotgrip_color = defaultColor;
}

void Settings_Dialog::acceptChanges()
{
    dialog_general_mdi_bg_use_logo = preview_general_mdi_bg_use_logo;
    dialog_general_mdi_bg_use_texture = preview_general_mdi_bg_use_texture;
    dialog_general_mdi_bg_use_color = preview_general_mdi_bg_use_color;
    dialog_general_mdi_bg_logo = accept_general_mdi_bg_logo;
    dialog_general_mdi_bg_texture = accept_general_mdi_bg_texture;
    dialog_general_mdi_bg_color = accept_general_mdi_bg_color;
    dialog_display_show_scrollbars = preview_display_show_scrollbars;
    dialog_display_crosshair_color = accept_display_crosshair_color;
    dialog_display_bg_color = accept_display_bg_color;
    dialog_display_selectbox_left_color = accept_display_selectbox_left_color;
    dialog_display_selectbox_left_fill = accept_display_selectbox_left_fill;
    dialog_display_selectbox_right_color = accept_display_selectbox_right_color;
    dialog_display_selectbox_right_fill = accept_display_selectbox_right_fill;
    dialog_display_selectbox_alpha = preview_display_selectbox_alpha;
    dialog_prompt_text_color = accept_prompt_text_color;
    dialog_prompt_bg_color = accept_prompt_bg_color;
    dialog_prompt_font_family = preview_prompt_font_family;
    dialog_prompt_font_style = preview_prompt_font_style;
    dialog_prompt_font_size = preview_prompt_font_size;
    if (dialog_grid_color_match_crosshair) {
        dialog_grid_color = accept_display_crosshair_color;
    }
    else {
        dialog_grid_color = accept_grid_color;
    }
    dialog_ruler_color = accept_ruler_color;
    dialog_lwt_show_lwt = preview_lwt_show_lwt;
    dialog_lwt_real_render = preview_lwt_real_render;

    settings_general_language = dialog_general_language;
    settings_general_icon_theme = dialog_general_icon_theme.string_value;
    settings_general_icon_size = dialog_general_icon_size;
    settings_general_mdi_bg_use_logo = dialog_general_mdi_bg_use_logo;
    settings_general_mdi_bg_use_texture = dialog_general_mdi_bg_use_texture;
    settings_general_mdi_bg_use_color = dialog_general_mdi_bg_use_color;
    settings_general_mdi_bg_logo = dialog_general_mdi_bg_logo;
    settings_general_mdi_bg_texture = dialog_general_mdi_bg_texture;
    settings_general_mdi_bg_color = dialog_general_mdi_bg_color;
    settings_general_tip_of_the_day = dialog_general_tip_of_the_day;
    //TODO: settings_general_system_help_browser = dialog_general_system_help_browser;
    settings_display_use_opengl = dialog_display_use_opengl;
    settings_display_renderhint_aa = dialog_display_renderhint_aa;
    settings_display_renderhint_text_aa = dialog_display_renderhint_text_aa;
    settings_display_renderhint_smooth_pix = dialog_display_renderhint_smooth_pix;
    settings_display_renderhint_high_aa = dialog_display_renderhint_high_aa;
    settings_display_renderhint_noncosmetic = dialog_display_renderhint_noncosmetic;
    settings_display_show_scrollbars = dialog_display_show_scrollbars;
    settings_display_scrollbar_widget_num = dialog_display_scrollbar_widget_num;
    settings_display_crosshair_color = dialog_display_crosshair_color;
    settings_display_bg_color = dialog_display_bg_color;
    settings_display_selectbox_left_color = dialog_display_selectbox_left_color;
    settings_display_selectbox_left_fill = dialog_display_selectbox_left_fill;
    settings_display_selectbox_right_color = dialog_display_selectbox_right_color;
    settings_display_selectbox_right_fill = dialog_display_selectbox_right_fill;
    settings_display_selectbox_alpha = dialog_display_selectbox_alpha;
    settings_display_zoomscale_in = dialog_display_zoomscale_in;
    settings_display_zoomscale_out = dialog_display_zoomscale_out;
    //TODO: setSettingsDisplayCrossHairPercent(dialog_display_crosshair_percent);
    //TODO: setSettingsDisplayUnits(dialog_display_units);
    settings_prompt_text_color = dialog_prompt_text_color;
    settings_prompt_bg_color = dialog_prompt_bg_color;
    settings_prompt_font_family = dialog_prompt_font_family;
    settings_prompt_font_style = dialog_prompt_font_style;
    settings_prompt_font_size = dialog_prompt_font_size;
    settings_prompt_save_history = dialog_prompt_save_history;
    settings_prompt_save_history_as_html = dialog_prompt_save_history_as_html;
    //TODO: setSettingsPromptSaveHistoryFilename(dialog_prompt_save_history_filename);
    settings_opensave_custom_filter = dialog_opensave_custom_filter;
    //TODO: setSettingsOpenFormat(dialog_opensave_open_format);
    //TODO: setSettingsOpenThumbnail(dialog_opensave_open_thumbnail);
    //TODO: setSettingsSaveFormat(dialog_opensave_save_format);
    //TODO: setSettingsSaveThumbnail(dialog_opensave_save_thumbnail);
    settings_opensave_recent_max_files = dialog_opensave_recent_max_files;
    settings_opensave_trim_dst_num_jumps = dialog_opensave_trim_dst_num_jumps;
    //TODO: setSettingsPrintingDefaultDevice(dialog_printing_default_device);
    //TODO: setSettingsPrintingUseLastDevice(dialog_printing_use_last_device);
    settings_printing_disable_bg = dialog_printing_disable_bg;
    settings_grid_show_on_load = dialog_grid_show_on_load;
    settings_grid_show_origin = dialog_grid_show_origin;
    settings_grid_color_match_crosshair = dialog_grid_color_match_crosshair;
    settings_grid_color = dialog_grid_color;
    //TODO: setSettingsGridLoadFromFile(dialog_grid_load_from_file);
    settings_grid_type = dialog_grid_type;
    settings_grid_center_on_origin = dialog_grid_center_on_origin;
    settings_grid_center_x = dialog_grid_center_x;
    settings_grid_center_y = dialog_grid_center_y;
    settings_grid_size_x = dialog_grid_size_x;
    settings_grid_size_y = dialog_grid_size_y;
    settings_grid_spacing_x = dialog_grid_spacing_x;
    settings_grid_spacing_y = dialog_grid_spacing_y;
    settings_grid_size_radius = dialog_grid_size_radius;
    settings_grid_spacing_radius = dialog_grid_spacing_radius;
    settings_grid_spacing_angle = dialog_grid_spacing_angle;
    settings_ruler_show_on_load = dialog_ruler_show_on_load;
    settings_ruler_metric = dialog_ruler_metric;
    settings_ruler_color = dialog_ruler_color;
    settings_ruler_pixel_size = dialog_ruler_pixel_size;
    //TODO: settings_qsnap_enabled = dialog_qsnap_enabled;
    settings_qsnap_locator_color = dialog_qsnap_locator_color;
    settings_qsnap_locator_size = dialog_qsnap_locator_size;
    settings_qsnap_aperture_size = dialog_qsnap_aperture_size;
    settings_qsnap_endpoint = dialog_qsnap_endpoint;
    settings_qsnap_midpoint = dialog_qsnap_midpoint;
    settings_qsnap_center = dialog_qsnap_center;
    settings_qsnap_node = dialog_qsnap_node;
    settings_qsnap_quadrant = dialog_qsnap_quadrant;
    settings_qsnap_intersection = dialog_qsnap_intersection;
    settings_qsnap_extension = dialog_qsnap_extension;
    settings_qsnap_insertion = dialog_qsnap_insertion;
    settings_qsnap_perpendicular = dialog_qsnap_perpendicular;
    settings_qsnap_tangent = dialog_qsnap_tangent;
    settings_qsnap_nearest = dialog_qsnap_nearest;
    settings_qsnap_apparent = dialog_qsnap_apparent;
    settings_qsnap_parallel = dialog_qsnap_parallel;
    settings_lwt_show_lwt = dialog_lwt_show_lwt;
    settings_lwt_real_render = dialog_lwt_real_render;
    settings_selection_mode_pickfirst = dialog_selection_mode_pickfirst;
    settings_selection_mode_pickadd = dialog_selection_mode_pickadd;
    settings_selection_mode_pickdrag = dialog_selection_mode_pickdrag;
    settings_selection_coolgrip_color = dialog_selection_coolgrip_color;
    settings_selection_hotgrip_color = dialog_selection_hotgrip_color;
    settings_selection_grip_size = dialog_selection_grip_size;
    settings_selection_pickbox_size = dialog_selection_pickbox_size;

    //Make sure the user sees the changes applied immediately
    mainWin->mdiArea->useBackgroundLogo(dialog_general_mdi_bg_use_logo);
    mainWin->mdiArea->useBackgroundTexture(dialog_general_mdi_bg_use_texture);
    mainWin->mdiArea->useBackgroundColor(dialog_general_mdi_bg_use_color);
    mainWin->mdiArea->setBackgroundLogo(dialog_general_mdi_bg_logo);
    mainWin->mdiArea->setBackgroundTexture(dialog_general_mdi_bg_texture);
    mainWin->mdiArea->setBackgroundColor(dialog_general_mdi_bg_color);
    mainWin->iconResize(dialog_general_icon_size);
    mainWin->updateAllViewScrollBars(dialog_display_show_scrollbars);
    mainWin->updateAllViewCrossHairColors(dialog_display_crosshair_color);
    mainWin->updateAllViewBackgroundColors(dialog_display_bg_color);
    mainWin->updateAllViewSelectBoxColors(dialog_display_selectbox_left_color,
                                          dialog_display_selectbox_left_fill,
                                          dialog_display_selectbox_right_color,
                                          dialog_display_selectbox_right_fill,
                                          dialog_display_selectbox_alpha);
    mainWin->prompt->setPromptTextColor(QColor(dialog_prompt_text_color));
    mainWin->prompt->setPromptBackgroundColor(QColor(dialog_prompt_bg_color));
    mainWin->prompt->setPromptFontFamily(dialog_prompt_font_family);
    mainWin->prompt->setPromptFontStyle(dialog_prompt_font_style);
    mainWin->prompt->setPromptFontSize(dialog_prompt_font_size);
    mainWin->updateAllViewGridColors(dialog_grid_color);
    mainWin->updateAllViewRulerColors(dialog_ruler_color);
    if (dialog_lwt_show_lwt) {
        mainWin->statusbar->statusBarLwtButton->enableLwt();
    }
    else {
        mainWin->statusbar->statusBarLwtButton->disableLwt();
    }
    if (dialog_lwt_real_render) {
        mainWin->statusbar->statusBarLwtButton->enableReal();
    }
    else {
        mainWin->statusbar->statusBarLwtButton->disableReal();
    }
    mainWin->updatePickAddMode(dialog_selection_mode_pickadd);

    mainWin->writeSettings();
    accept();
}

void Settings_Dialog::rejectChanges()
{
    //TODO: inform the user if they have changed settings

    //Update the view since the user must accept the preview
    mainWin->mdiArea->useBackgroundLogo(dialog_general_mdi_bg_use_logo);
    mainWin->mdiArea->useBackgroundTexture(dialog_general_mdi_bg_use_texture);
    mainWin->mdiArea->useBackgroundColor(dialog_general_mdi_bg_use_color);
    mainWin->mdiArea->setBackgroundLogo(dialog_general_mdi_bg_logo);
    mainWin->mdiArea->setBackgroundTexture(dialog_general_mdi_bg_texture);
    mainWin->mdiArea->setBackgroundColor(dialog_general_mdi_bg_color);
    mainWin->updateAllViewScrollBars(dialog_display_show_scrollbars);
    mainWin->updateAllViewCrossHairColors(dialog_display_crosshair_color);
    mainWin->updateAllViewBackgroundColors(dialog_display_bg_color);
    mainWin->updateAllViewSelectBoxColors(dialog_display_selectbox_left_color,
                                          dialog_display_selectbox_left_fill,
                                          dialog_display_selectbox_right_color,
                                          dialog_display_selectbox_right_fill,
                                          dialog_display_selectbox_alpha);
    mainWin->prompt->setPromptTextColor(QColor(dialog_prompt_text_color));
    mainWin->prompt->setPromptBackgroundColor(QColor(dialog_prompt_bg_color));
    mainWin->prompt->setPromptFontFamily(dialog_prompt_font_family);
    mainWin->prompt->setPromptFontStyle(dialog_prompt_font_style);
    mainWin->prompt->setPromptFontSize(dialog_prompt_font_size);
    mainWin->updateAllViewGridColors(dialog_grid_color);
    mainWin->updateAllViewRulerColors(dialog_ruler_color);
    if (dialog_lwt_show_lwt) {
        mainWin->statusbar->statusBarLwtButton->enableLwt();
    }
    else {
        mainWin->statusbar->statusBarLwtButton->disableLwt();
    }
    if (dialog_lwt_real_render) {
        mainWin->statusbar->statusBarLwtButton->enableReal();
    }
    else {
        mainWin->statusbar->statusBarLwtButton->disableReal();
    }

    reject();
}
#endif
