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

/* . */
ScriptValue *
setting_ptr(int key, int mode)
{
    ScriptValue *set_mode;
    switch (mode) {
    case SETTING_DIALOG: {
        set_mode = &(setting[key].dialog);
        break;
    }
    case SETTING_PREVIEW: {
        set_mode = &(setting[key].preview);
        break;
    }
    case SETTING_ACCEPT: {
        set_mode = &(setting[key].dialog);
        break;
    }
    default:
    case SETTING_SETTING: {
        set_mode = &(setting[key].setting);
        break;
    }
    }
    return set_mode;
}

/* . */
void
copy_setting(int key, int dst, int src)
{
    ScriptValue *dst_set = setting_ptr(key, dst);
    ScriptValue *src_set = setting_ptr(key, src);
    switch (setting[src].setting.type) {
    case SCRIPT_INT:
        dst_set->i = src_set->i;
        break;
    case SCRIPT_REAL:
        dst_set->r = src_set->r;
        break;
    case SCRIPT_STRING:
        strcpy(dst_set->s, src_set->s);
        break;
    case SCRIPT_BOOL:
        dst_set->b = src_set->b;
        break;
    default:
        break;
    }
}

/* . */
Settings_Dialog::Settings_Dialog(MainWindow* mw, QString showTab, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    tabWidget = new QTabWidget(this);

    for (int i=0; i<N_SETTINGS; i++) {
        copy_setting(i, SETTING_DIALOG, SETTING_SETTING);
        copy_setting(i, SETTING_PREVIEW, SETTING_SETTING);
        copy_setting(i, SETTING_ACCEPT, SETTING_SETTING);
    }

    /* TODO: Add icons to tabs */
    tabWidget->addTab(createTabGeneral(), translate("General"));
    tabWidget->addTab(createTabFilesPaths(), translate("Files/Paths"));
    tabWidget->addTab(createTabDisplay(), translate("Display"));
    tabWidget->addTab(createTabPrompt(), translate("Prompt"));
    tabWidget->addTab(createTabOpenSave(), translate("Open/Save"));
    tabWidget->addTab(createTabPrinting(), translate("Printing"));
    tabWidget->addTab(createTabSnap(), translate("Snap"));
    tabWidget->addTab(createTabGridRuler(), translate("Grid/Ruler"));
    tabWidget->addTab(createTabOrthoPolar(), translate("Ortho/Polar"));
    tabWidget->addTab(createTabQuickSnap(), translate("QuickSnap"));
    tabWidget->addTab(createTabQuickTrack(), translate("QuickTrack"));
    tabWidget->addTab(createTabLineWeight(), translate("LineWeight"));
    tabWidget->addTab(createTabSelection(), translate("Selection"));

    if (showTab == "General") {
        tabWidget->setCurrentIndex(0);
    }
    else if (showTab == "Files/Path") {
        tabWidget->setCurrentIndex(1);
    }
    else if (showTab == "Display") {
        tabWidget->setCurrentIndex(2);
    }
    else if (showTab == "Prompt") {
        tabWidget->setCurrentIndex(3);
    }
    else if (showTab == "Open/Save") {
        tabWidget->setCurrentIndex(4);
    }
    else if (showTab == "Printing") {
        tabWidget->setCurrentIndex( 5);
    }
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

    setWindowTitle(translate("Settings"));

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/* . */
Settings_Dialog::~Settings_Dialog()
{
    QApplication::restoreOverrideCursor();
}

/* . */
QWidget*
Settings_Dialog::createTabGeneral()
{
    QWidget* widget = new QWidget(this);

    /* Language */
    QGroupBox* groupBoxLanguage = new QGroupBox(translate("Language"), widget);

    QLabel* labelLanguage = new QLabel(translate("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    strcpy(setting[GENERAL_LANGUAGE].dialog.s,
        qPrintable(QString(get_str(GENERAL_LANGUAGE)).toLower()));
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
    QString current = setting[GENERAL_LANGUAGE].dialog.s;
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(QString )), this, SLOT(comboBoxLanguageCurrentIndexChanged(QString )));

    QVBoxLayout* vboxLayoutLanguage = new QVBoxLayout(groupBoxLanguage);
    vboxLayoutLanguage->addWidget(labelLanguage);
    vboxLayoutLanguage->addWidget(comboBoxLanguage);
    groupBoxLanguage->setLayout(vboxLayoutLanguage);

    /* Icons */
    QGroupBox* groupBoxIcon = new QGroupBox(translate("Icons"), widget);

    QLabel* labelIconTheme = new QLabel(translate("Icon Theme"), groupBoxIcon);
    QComboBox* comboBoxIconTheme = new QComboBox(groupBoxIcon);
    QDir dir(qApp->applicationDirPath());
    dir.cd("icons");
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/theme.png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(setting[GENERAL_ICON_THEME].dialog.s));
    connect(comboBoxIconTheme, SIGNAL(currentIndexChanged(QString )), this, SLOT(comboBoxIconThemeCurrentIndexChanged(QString )));

    QLabel* labelIconSize = new QLabel(translate("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    QString dialog_icon("icons/");
    dialog_icon += setting[GENERAL_ICON_THEME].dialog.s;
    dialog_icon += "/";
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon16.png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon24.png"), "Small", 24);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon32.png"), "Medium", 32);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon48.png"), "Large", 48);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon64.png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon(dialog_icon + "icon128.png"), "I'm Blind", 128);
    setting[GENERAL_ICON_SIZE].dialog.i = get_int(GENERAL_ICON_SIZE);
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(setting[GENERAL_ICON_SIZE].dialog.i));
    connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIconSizeCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutIcon = new QVBoxLayout(groupBoxIcon);
    vboxLayoutIcon->addWidget(labelIconTheme);
    vboxLayoutIcon->addWidget(comboBoxIconTheme);
    vboxLayoutIcon->addWidget(labelIconSize);
    vboxLayoutIcon->addWidget(comboBoxIconSize);
    groupBoxIcon->setLayout(vboxLayoutIcon);

    /* Mdi Background */
    QGroupBox* groupBoxMdiBG = new QGroupBox(translate("Background"), widget);

    QCheckBox* checkBoxMdiBGUseLogo = create_checkbox(groupBoxMdiBG, "Use Logo",
        GENERAL_MDI_BG_USE_LOGO, "");
    QCheckBox* checkBoxMdiBGUseTexture = create_checkbox(groupBoxMdiBG, "Use Texture",
        GENERAL_MDI_BG_USE_TEXTURE, "");
    QCheckBox* checkBoxMdiBGUseColor = create_checkbox(groupBoxMdiBG, "Use Color",
        GENERAL_MDI_BG_USE_COLOR, "");

    QPushButton* buttonMdiBGLogo = new QPushButton(translate("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(setting[GENERAL_MDI_BG_USE_LOGO].dialog.b);
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QPushButton* buttonMdiBGTexture = new QPushButton(translate("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(setting[GENERAL_MDI_BG_USE_TEXTURE].dialog.b);
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QPushButton* buttonMdiBGColor = choose_color_button(groupBoxMdiBG,
        GENERAL_MDI_BG_COLOR);
    buttonMdiBGColor->setEnabled(setting[GENERAL_MDI_BG_USE_COLOR].dialog.b);
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
    QGroupBox* groupBoxTips = new QGroupBox(translate("Tips"), widget);

    QCheckBox* checkBoxTipOfTheDay = create_checkbox(groupBoxTips,
        "Show Tip of the Day on startup", GENERAL_TIP_OF_THE_DAY, "");

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    /* Help Browser */
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(translate("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(translate("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(get_bool(GENERAL_SYSTEM_HELP_BROWSER));
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(translate("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!get_bool(GENERAL_SYSTEM_HELP_BROWSER));
    radioButtonCustomHelpBrowser->setEnabled(false); /* TODO: finish this. */

    QVBoxLayout* vboxLayoutHelpBrowser = new QVBoxLayout(groupBoxHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonSystemHelpBrowser);
    vboxLayoutHelpBrowser->addWidget(radioButtonCustomHelpBrowser);
    groupBoxHelpBrowser->setLayout(vboxLayoutHelpBrowser);

    /* Widget Layout */
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

/* . */
QWidget*
Settings_Dialog::createTabFilesPaths()
{
    QWidget* widget = new QWidget(this);

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

/* . */
QGroupBox*
create_group_box(QWidget* widget, const char *label, int data[])
{
    QGroupBox* groupbox = new QGroupBox(translate(label), widget);
    QVBoxLayout* vboxLayoutRender = new QVBoxLayout(groupbox);
    for (int i=0; data[i] != TERMINATOR_SYMBOL; i++) {
        QCheckBox* checkBox = create_checkbox(groupbox,
            settings_data[data[i]].label,
            settings_data[data[i]].id,
            settings_data[data[i]].icon);
        vboxLayoutRender->addWidget(checkBox);
    }
    groupbox->setLayout(vboxLayoutRender);
    return groupbox;
}

/* . */
QWidget*
Settings_Dialog::createTabDisplay()
{
    QWidget* widget = new QWidget(this);

    /* Rendering */
    /* TODO: Review OpenGL and Rendering settings for future inclusion */
    QGroupBox* groupBoxRender = create_group_box(widget, "Rendering", render_hints);

    /* ScrollBars */
    QGroupBox* groupBoxScrollBars = new QGroupBox(translate("ScrollBars"), widget);

    QCheckBox* checkBoxShowScrollBars = create_checkbox(groupBoxScrollBars,
        "Show ScrollBars", DISPLAY_SHOW_SCROLLBARS, "");

    QLabel* labelScrollBarWidget = new QLabel(translate("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    int numActions = actionHash.size();
    for (int i = 0; i < numActions; i++) {
        QAction* action = actionHash[i];
        if (action) {
            comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        }
    }
    comboBoxScrollBarWidget->setCurrentIndex(setting[DISPLAY_SCROLLBAR_WIDGET_NUM].dialog.i);
    connect(comboBoxScrollBarWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxScrollBarWidgetCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutScrollBars = new QVBoxLayout(groupBoxScrollBars);
    vboxLayoutScrollBars->addWidget(checkBoxShowScrollBars);
    vboxLayoutScrollBars->addWidget(labelScrollBarWidget);
    vboxLayoutScrollBars->addWidget(comboBoxScrollBarWidget);
    groupBoxScrollBars->setLayout(vboxLayoutScrollBars);

    /* Colors */
    QGroupBox* groupBoxColor = new QGroupBox(translate("Colors"), widget);

    QLabel* labelCrossHairColor = new QLabel(translate("Crosshair Color"), groupBoxColor);
    QPushButton* buttonCrossHairColor = choose_color_button(groupBoxColor,
        DISPLAY_CROSSHAIR_COLOR);
    connect(buttonCrossHairColor, SIGNAL(clicked()), this,
        SLOT(chooseDisplayCrossHairColor()));

    QLabel* labelBGColor = new QLabel(translate("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = choose_color_button(groupBoxColor,
        DISPLAY_BG_COLOR);
    connect(buttonBGColor, SIGNAL(clicked()), this,
        SLOT(chooseDisplayBackgroundColor()));

    QLabel* labelSelectBoxLeftColor = new QLabel(translate("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = choose_color_button(groupBoxColor,
        DISPLAY_SELECTBOX_LEFT_COLOR);
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this,
        SLOT(chooseDisplaySelectBoxLeftColor()));

    QLabel* labelSelectBoxLeftFill = new QLabel(translate("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = choose_color_button(groupBoxColor,
        DISPLAY_SELECTBOX_LEFT_FILL);
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this,
        SLOT(chooseDisplaySelectBoxLeftFill()));

    QLabel* labelSelectBoxRightColor = new QLabel(translate("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = choose_color_button(groupBoxColor,
        DISPLAY_SELECTBOX_RIGHT_COLOR);
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this,
        SLOT(chooseDisplaySelectBoxRightColor()));

    QLabel* labelSelectBoxRightFill = new QLabel(translate("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = choose_color_button(groupBoxColor,
        DISPLAY_SELECTBOX_RIGHT_FILL);
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this,
        SLOT(chooseDisplaySelectBoxRightFill()));

    QLabel* labelSelectBoxAlpha = new QLabel(translate("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    spinBoxSelectBoxAlpha->setValue(setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
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

    /* Zoom */
    QGroupBox* groupBoxZoom = new QGroupBox(translate("Zoom"), widget);

    QLabel* labelZoomScaleIn = new QLabel(translate("Zoom In Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleIn = create_spinbox(
        groupBoxZoom, "spinBoxZoomInScale", DISPLAY_ZOOMSCALE_IN, 0.01, 1.01, 10.00);

    QLabel* labelZoomScaleOut = new QLabel(translate("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = create_spinbox(
        groupBoxZoom, "spinBoxZoomOutScale", DISPLAY_ZOOMSCALE_OUT, 0.01, 0.01, 0.99);

    QGridLayout* gridLayoutZoom = new QGridLayout(groupBoxZoom);
    gridLayoutZoom->addWidget(labelZoomScaleIn, 0, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleIn, 0, 1, Qt::AlignRight);
    gridLayoutZoom->addWidget(labelZoomScaleOut, 1, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleOut, 1, 1, Qt::AlignRight);
    groupBoxZoom->setLayout(gridLayoutZoom);

    /* Widget Layout */
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    /* TODO: Review OpenGL and Rendering settings for future inclusion. */
    vboxLayoutMain->addWidget(groupBoxRender);
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

/* TODO: finish prompt options */
QWidget* Settings_Dialog::createTabPrompt()
{
    QWidget* widget = new QWidget(this);

    /* Colors */
    QGroupBox* groupBoxColor = new QGroupBox(translate("Colors"), widget);

    QLabel* labelTextColor = new QLabel(translate("Text Color"), groupBoxColor);
    QPushButton* buttonTextColor = choose_color_button(groupBoxColor, PROMPT_TEXT_COLOR);
    connect(buttonTextColor, SIGNAL(clicked()), this, SLOT(choosePromptTextColor()));

    QLabel* labelBGColor = new QLabel(translate("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = choose_color_button(groupBoxColor, PROMPT_BG_COLOR);
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(choosePromptBackgroundColor()));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelTextColor, 0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonTextColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor, 1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor, 1, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    /* TODO: Tweak the Prompt Font ComboBoxes so they work properly */
    /* Font */
    QGroupBox* groupBoxFont = new QGroupBox(translate("Font"), widget);

    QLabel* labelFontFamily = new QLabel(translate("Font Family"), groupBoxFont);
    QFontComboBox* comboBoxFontFamily = new QFontComboBox(groupBoxFont);
    comboBoxFontFamily->setCurrentFont(QFont(setting[PROMPT_FONT_FAMILY].preview.s));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(QString )), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(QString )));

    QLabel* labelFontStyle = new QLabel(translate("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(setting[PROMPT_FONT_STYLE].preview.s);
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(QString )), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(QString )));

    QLabel* labelFontSize = new QLabel(translate("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    spinBoxFontSize->setValue(setting[PROMPT_FONT_SIZE].preview.i);
    connect(spinBoxFontSize, SIGNAL(valueChanged(int)), this, SLOT(spinBoxPromptFontSizeValueChanged(int)));

    QGridLayout* gridLayoutFont = new QGridLayout(groupBoxFont);
    gridLayoutFont->addWidget(labelFontFamily, 0, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontFamily, 0, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontStyle, 1, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(comboBoxFontStyle, 1, 1, Qt::AlignRight);
    gridLayoutFont->addWidget(labelFontSize, 2, 0, Qt::AlignLeft);
    gridLayoutFont->addWidget(spinBoxFontSize, 2, 1, Qt::AlignRight);
    groupBoxFont->setLayout(gridLayoutFont);

    /* History */
    QGroupBox* groupBoxHistory = new QGroupBox(translate("History"), widget);

    QVBoxLayout* vboxLayoutHistory = new QVBoxLayout(groupBoxHistory);
    vboxLayoutHistory->addWidget(create_checkbox(groupBoxHistory,
        "Save History", PROMPT_SAVE_HISTORY, ""));
    vboxLayoutHistory->addWidget(create_checkbox(groupBoxHistory,
        "Save As HTML", PROMPT_SAVE_HISTORY_AS_HTML, ""));
    groupBoxHistory->setLayout(vboxLayoutHistory);

    /* Widget Layout */
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

/* TODO: finish open/save options */
QWidget* Settings_Dialog::createTabOpenSave()
{
    QWidget* widget = new QWidget(this);

    /* Custom Filter */
    QGroupBox* groupBoxCustomFilter = new QGroupBox(translate("Custom Filter"), widget);
    groupBoxCustomFilter->setEnabled(false); /* TODO: Fixup custom filter */

    QHash<QString, QCheckBox*> custom_filter;

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(translate("Select All"), widget);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()));
    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", widget);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()));

    int i;
    int n_extensions = string_array_length(extensions);
    for (i=0; i<n_extensions; i++) {
        const char *extension = extensions[i];
        custom_filter[extension] = new QCheckBox(extension, groupBoxCustomFilter);
        custom_filter[extension]->setChecked(QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).contains("*." + QString(extension), Qt::CaseInsensitive));
        connect(custom_filter[extension], SIGNAL(stateChanged(int)), this,
            SLOT(checkBoxCustomFilterStateChanged(int)));

        connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)),
            custom_filter[extension], SLOT(setChecked(bool)));

        connect(this, SIGNAL(buttonCustomFilterClearAll(bool)),
            custom_filter[extension], SLOT(setChecked(bool)));
    }

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    int row = 0;
    int column = 0;
    for (i=0; i<n_extensions; i++) {
        const char *extension = extensions[i];
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

    if (QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).contains("supported", Qt::CaseInsensitive)) {
        buttonCustomFilterSelectAllClicked();
    }

    /* Opening */
    QGroupBox* groupBoxOpening = new QGroupBox(translate("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(translate("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    /* TODO: Add a button to clear the recent history. */

    QLabel* labelRecentMaxFiles = new QLabel(translate("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    spinBoxRecentMaxFiles->setValue(setting[OPENSAVE_RECENT_MAX_FILES].dialog.b);
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

    /* Saving */
    QGroupBox* groupBoxSaving = new QGroupBox(translate("File Save"), widget);

    QComboBox* comboBoxSaveFormat = new QComboBox(groupBoxSaving);

    QCheckBox* checkBoxSaveThumbnail = new QCheckBox(translate("Save Thumbnails"), groupBoxSaving);
    checkBoxSaveThumbnail->setChecked(false);

    QCheckBox* checkBoxAutoSave = new QCheckBox(translate("AutoSave"), groupBoxSaving);
    checkBoxAutoSave->setChecked(false);

    QVBoxLayout* vboxLayoutSaving = new QVBoxLayout(groupBoxSaving);
    vboxLayoutSaving->addWidget(comboBoxSaveFormat);
    vboxLayoutSaving->addWidget(checkBoxSaveThumbnail);
    vboxLayoutSaving->addWidget(checkBoxAutoSave);
    groupBoxSaving->setLayout(vboxLayoutSaving);

    /* Trimming */
    QGroupBox* groupBoxTrim = new QGroupBox(translate("Trimming"), widget);

    QLabel* labelTrimDstNumJumps = new QLabel(translate("DST Only: Minimum number of jumps to trim"), groupBoxTrim);
    QSpinBox* spinBoxTrimDstNumJumps = new QSpinBox(groupBoxTrim);
    spinBoxTrimDstNumJumps->setRange(1, 20);
    spinBoxTrimDstNumJumps->setValue(setting[OPENSAVE_TRIM_DST_NUM_JUMPS].dialog.i);
    connect(spinBoxTrimDstNumJumps, SIGNAL(valueChanged(int)), this, SLOT(spinBoxTrimDstNumJumpsValueChanged(int)));

    QFrame* frameTrimDstNumJumps = new QFrame(groupBoxTrim);
    QGridLayout* gridLayoutTrimDstNumJumps = new QGridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps->addWidget(labelTrimDstNumJumps, 0, 0, Qt::AlignLeft);
    gridLayoutTrimDstNumJumps->addWidget(spinBoxTrimDstNumJumps, 0, 1, Qt::AlignRight);
    frameTrimDstNumJumps->setLayout(gridLayoutTrimDstNumJumps);

    QVBoxLayout* vboxLayoutTrim = new QVBoxLayout(groupBoxTrim);
    vboxLayoutTrim->addWidget(frameTrimDstNumJumps);
    groupBoxTrim->setLayout(vboxLayoutTrim);

    /* Widget Layout */
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

    /* Default Printer */
    QGroupBox* groupBoxDefaultPrinter = new QGroupBox(translate("Default Printer"), widget);

    QRadioButton* radioButtonUseSame = new QRadioButton(translate("Use as default device"), groupBoxDefaultPrinter);
    radioButtonUseSame->setChecked(!get_bool(PRINTING_USE_LAST_DEVICE));
    QRadioButton* radioButtonUseLast = new QRadioButton(translate("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(get_bool(PRINTING_USE_LAST_DEVICE));

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

    /* Save Ink */
    QGroupBox* groupBoxSaveInk = new QGroupBox(translate("Save Ink"), widget);

    QVBoxLayout* vboxLayoutSaveInk = new QVBoxLayout(groupBoxSaveInk);
    vboxLayoutSaveInk->addWidget(create_checkbox(groupBoxSaveInk,
        "Disable Background", PRINTING_DISABLE_BG, ""));
    groupBoxSaveInk->setLayout(vboxLayoutSaveInk);

    /* Widget Layout */
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

    /* TODO: finish this */

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget*
Settings_Dialog::createTabGridRuler()
{
    QWidget* widget = new QWidget(this);

    /* Grid Misc */
    QGroupBox* groupBoxGridMisc = new QGroupBox(translate("Grid Misc"), widget);

    QCheckBox* checkBoxGridShowOnLoad = create_checkbox(groupBoxGridMisc,
        "Initially show grid when loading a file", GRID_SHOW_ON_LOAD, "");
    QCheckBox* checkBoxGridShowOrigin = create_checkbox(groupBoxGridMisc,
        "Show the origin when the grid is enabled", GRID_SHOW_ORIGIN, "");

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    /* Grid Color */
    QGroupBox* groupBoxGridColor = new QGroupBox(translate("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(
        translate("Match grid color to crosshair color"), groupBoxGridColor);
    checkBoxGridColorMatchCrossHair->setChecked(setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(translate("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(translate("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        setting[GRID_COLOR].dialog.i = setting[DISPLAY_CROSSHAIR_COLOR].setting.i;
        setting[GRID_COLOR].preview.i = setting[GRID_COLOR].dialog.i;
        setting[GRID_COLOR].accept.i = setting[GRID_COLOR].dialog.i;
    }
    buttonGridColor->setIcon(QIcon(create_swatch(setting[GRID_COLOR].preview.i)));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
    buttonGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    /* Grid Geometry */
    QGroupBox* groupBoxGridGeom = new QGroupBox(translate("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(translate("Set grid size from opened file"), groupBoxGridGeom);
    checkBoxGridLoadFromFile->setChecked(setting[GRID_LOAD_FROM_FILE].dialog.b);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(translate("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(setting[GRID_TYPE].dialog.s));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(QString )));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(translate("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    checkBoxGridCenterOnOrigin->setChecked(setting[GRID_CENTER_ON_ORIGIN].dialog.b);
    connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)));

    QLabel* labelGridCenterX = new QLabel(translate("Grid Center X"), groupBoxGridGeom);
    labelGridCenterX->setObjectName("labelGridCenterX");
    QDoubleSpinBox* spinBoxGridCenterX = create_spinbox(
        groupBoxGridGeom, "spinBoxGridCenterX", GRID_CENTER_X,
        1.000, -1000.000, 1000.000);

    QLabel* labelGridCenterY = new QLabel(translate("Grid Center Y"), groupBoxGridGeom);
    labelGridCenterY->setObjectName("labelGridCenterY");
    QDoubleSpinBox* spinBoxGridCenterY = create_spinbox(
        groupBoxGridGeom, "spinBoxGridCenterY", GRID_CENTER_Y,
        1.000, -1000.000, 1000.000);

    QLabel* labelGridSizeX = new QLabel(translate("Grid Size X"), groupBoxGridGeom);
    labelGridSizeX->setObjectName("labelGridSizeX");
    QDoubleSpinBox* spinBoxGridSizeX = create_spinbox(
        groupBoxGridGeom, "spinBoxGridSizeX", GRID_SIZE_X,
        1.000, 1.000, 1000.000);

    QLabel* labelGridSizeY = new QLabel(translate("Grid Size Y"), groupBoxGridGeom);
    labelGridSizeY->setObjectName("labelGridSizeY");
    QDoubleSpinBox* spinBoxGridSizeY = create_spinbox(
        groupBoxGridGeom, "spinBoxGridSizeY", GRID_SIZE_Y,
        1.000, 1.000, 1000.000);

    QLabel* labelGridSpacingX = new QLabel(translate("Grid Spacing X"), groupBoxGridGeom);
    labelGridSpacingX->setObjectName("labelGridSpacingX");
    QDoubleSpinBox* spinBoxGridSpacingX = create_spinbox(
        groupBoxGridGeom, "spinBoxGridSpacingX", GRID_SPACING_X,
        1.000, 0.001, 1000.000);

    QLabel* labelGridSpacingY = new QLabel(translate("Grid Spacing Y"), groupBoxGridGeom);
    labelGridSpacingY->setObjectName("labelGridSpacingY");
    QDoubleSpinBox* spinBoxGridSpacingY = create_spinbox(
        groupBoxGridGeom, "spinBoxGridSpacingY", GRID_SPACING_Y,
        1.000, 0.001, 1000.000);

    QLabel* labelGridSizeRadius = new QLabel(translate("Grid Size Radius"), groupBoxGridGeom);
    labelGridSizeRadius->setObjectName("labelGridSizeRadius");
    QDoubleSpinBox* spinBoxGridSizeRadius = create_spinbox(
        groupBoxGridGeom, "spinBoxGridSizeRadius", GRID_SIZE_RADIUS,
        1.000, 1.000, 1000.000);

    QLabel* labelGridSpacingRadius = new QLabel(translate("Grid Spacing Radius"), groupBoxGridGeom);
    labelGridSpacingRadius->setObjectName("labelGridSpacingRadius");
    QDoubleSpinBox* spinBoxGridSpacingRadius = create_spinbox(
        groupBoxGridGeom, "spinBoxGridSpacingRadius", GRID_SPACING_RADIUS,
        1.000, 0.001, 1000.000);

    QLabel* labelGridSpacingAngle = new QLabel(translate("Grid Spacing Angle"), groupBoxGridGeom);
    labelGridSpacingAngle->setObjectName("labelGridSpacingAngle");
    QDoubleSpinBox* spinBoxGridSpacingAngle = create_spinbox(
        groupBoxGridGeom, "spinBoxGridSpacingAngle", GRID_SPACING_ANGLE,
        1.000, 0.001, 1000.000);

    bool disable = setting[GRID_LOAD_FROM_FILE].dialog.b;
    labelGridType->setEnabled(disable);
    comboBoxGridType->setEnabled(disable);
    checkBoxGridCenterOnOrigin->setEnabled(disable);
    labelGridCenterX->setEnabled(disable);
    spinBoxGridCenterX->setEnabled(disable);
    labelGridCenterY->setEnabled(disable);
    spinBoxGridCenterY->setEnabled(disable);
    labelGridSizeX->setEnabled(disable);
    spinBoxGridSizeX->setEnabled(disable);
    labelGridSizeY->setEnabled(disable);
    spinBoxGridSizeY->setEnabled(disable);
    labelGridSpacingX->setEnabled(disable);
    spinBoxGridSpacingX->setEnabled(disable);
    labelGridSpacingY->setEnabled(disable);
    spinBoxGridSpacingY->setEnabled(disable);
    labelGridSizeRadius->setEnabled(disable);
    spinBoxGridSizeRadius->setEnabled(disable);
    labelGridSpacingRadius->setEnabled(disable);
    spinBoxGridSpacingRadius->setEnabled(disable);
    labelGridSpacingAngle->setEnabled(disable);
    spinBoxGridSpacingAngle->setEnabled(disable);

    bool visibility = false;
    if (QString(setting[GRID_TYPE].dialog.s) == "Circular") {
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

    /* Ruler Misc */
    QGroupBox* groupBoxRulerMisc = new QGroupBox(translate("Ruler Misc"), widget);

    QCheckBox* checkBoxRulerShowOnLoad = create_checkbox(groupBoxRulerMisc,
        "Initially show ruler when loading a file", RULER_SHOW_ON_LOAD, "");

    QLabel* labelRulerMetric = new QLabel(translate("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric", true);
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(setting[RULER_METRIC].dialog.i));
    connect(comboBoxRulerMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRulerMetricCurrentIndexChanged(int)));

    QGridLayout* gridLayoutRulerMisc = new QGridLayout(widget);
    gridLayoutRulerMisc->addWidget(checkBoxRulerShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(labelRulerMetric, 1, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(comboBoxRulerMetric, 1, 1, Qt::AlignRight);
    groupBoxRulerMisc->setLayout(gridLayoutRulerMisc);

    /* Ruler Color */
    QGroupBox* groupBoxRulerColor = new QGroupBox(translate("Ruler Color"), widget);

    QLabel* labelRulerColor = new QLabel(translate("Ruler Color"), groupBoxRulerColor);
    labelRulerColor->setObjectName("labelRulerColor");
    QPushButton* buttonRulerColor = new QPushButton(translate("Choose"), groupBoxRulerColor);
    buttonRulerColor->setObjectName("buttonRulerColor");
    buttonRulerColor->setIcon(create_swatch(setting[RULER_COLOR].preview.i));
    connect(buttonRulerColor, SIGNAL(clicked()), this, SLOT(chooseRulerColor()));

    QGridLayout* gridLayoutRulerColor = new QGridLayout(widget);
    gridLayoutRulerColor->addWidget(labelRulerColor, 1, 0, Qt::AlignLeft);
    gridLayoutRulerColor->addWidget(buttonRulerColor, 1, 1, Qt::AlignRight);
    groupBoxRulerColor->setLayout(gridLayoutRulerColor);

    /* Ruler Geometry */
    QGroupBox* groupBoxRulerGeom = new QGroupBox(translate("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = new QLabel(translate("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QSpinBox* spinBoxRulerPixelSize = create_int_spinbox(
        groupBoxRulerGeom, "spinBoxRulerPixelSize", RULER_PIXEL_SIZE, 1, 20, 100);

    QGridLayout* gridLayoutRulerGeom = new QGridLayout(groupBoxRulerGeom);
    gridLayoutRulerGeom->addWidget(labelRulerPixelSize, 0, 0, Qt::AlignLeft);
    gridLayoutRulerGeom->addWidget(spinBoxRulerPixelSize, 0, 1, Qt::AlignRight);
    groupBoxRulerGeom->setLayout(gridLayoutRulerGeom);

    /* Widget Layout */
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

    /* TODO: finish this */

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget*
Settings_Dialog::createTabQuickSnap()
{
    QWidget* widget = new QWidget(this);

    /* QSnap Locators */
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(translate("Locators Used"), widget);

    QCheckBox* checkBoxQSnapEndPoint = create_checkbox(groupBoxQSnapLoc,
        "Endpoint", QSNAP_ENDPOINT, "locator-snaptoendpoint");
    QCheckBox* checkBoxQSnapMidPoint = create_checkbox(groupBoxQSnapLoc,
        "Midpoint", QSNAP_MIDPOINT, "locator-snaptomidpoint");
    QCheckBox* checkBoxQSnapCenter = create_checkbox(groupBoxQSnapLoc,
        "Center", QSNAP_CENTER, "locator-snaptocenter");
    QCheckBox* checkBoxQSnapNode = create_checkbox(groupBoxQSnapLoc,
        "Node", QSNAP_NODE, "locator-snaptonode");
    QCheckBox* checkBoxQSnapQuadrant = create_checkbox(groupBoxQSnapLoc,
        "Quadrant", QSNAP_QUADRANT, "locator-snaptoquadrant");
    QCheckBox* checkBoxQSnapIntersection = create_checkbox(groupBoxQSnapLoc,
        "Intersection", QSNAP_INTERSECTION, "locator-snaptointersection");
    QCheckBox* checkBoxQSnapExtension = create_checkbox(groupBoxQSnapLoc,
        "Extension", QSNAP_EXTENSION, "locator-snaptoextension");
    QCheckBox* checkBoxQSnapInsertion = create_checkbox(groupBoxQSnapLoc,
        "Insertion", QSNAP_INSERTION, "locator-snaptoinsert");
    QCheckBox* checkBoxQSnapPerpendicular = create_checkbox(groupBoxQSnapLoc,
        "Perpendicular", QSNAP_PERPENDICULAR, "locator-snaptoperpendicular");
    QCheckBox* checkBoxQSnapTangent = create_checkbox(groupBoxQSnapLoc,
        "Tangent", QSNAP_TANGENT, "locator-snaptotangent");
    QCheckBox* checkBoxQSnapNearest = create_checkbox(groupBoxQSnapLoc,
        "Nearest", QSNAP_NEAREST, "locator-snaptonearest");
    QCheckBox* checkBoxQSnapApparent = create_checkbox(groupBoxQSnapLoc,
        "Apparent Intersection", QSNAP_APPARENT, "locator-snaptoapparentintersection");
    QCheckBox* checkBoxQSnapParallel = create_checkbox(groupBoxQSnapLoc,
        "Parallel", QSNAP_PARALLEL, "locator-snaptoparallel");

    QPushButton* buttonQSnapSelectAll = new QPushButton(translate("Select All"), groupBoxQSnapLoc);
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

    QPushButton* buttonQSnapClearAll = new QPushButton(translate("Clear All"), groupBoxQSnapLoc);
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

    /* QSnap Visual Config */
    QGroupBox* groupBoxQSnapVisual = new QGroupBox(translate("Visual Configuration"), widget);

    QLabel* labelQSnapLocColor = new QLabel(translate("Locator Color"), groupBoxQSnapVisual);
    QComboBox* comboBoxQSnapLocColor = new QComboBox(groupBoxQSnapVisual);
    addColorsToComboBox(comboBoxQSnapLocColor);
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(setting[QSNAP_LOCATOR_COLOR].dialog.i));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(translate("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    sliderQSnapLocSize->setValue(setting[QSNAP_LOCATOR_SIZE].dialog.i);
    connect(sliderQSnapLocSize, SIGNAL(valueChanged(int)), this, SLOT(sliderQSnapLocatorSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutQSnapVisual = new QVBoxLayout(groupBoxQSnapVisual);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(comboBoxQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocSize);
    vboxLayoutQSnapVisual->addWidget(sliderQSnapLocSize);
    groupBoxQSnapVisual->setLayout(vboxLayoutQSnapVisual);

    /* QSnap Sensitivity Config */
    QGroupBox* groupBoxQSnapSensitivity = new QGroupBox(translate("Sensitivity"), widget);

    QLabel* labelQSnapApertureSize = new QLabel(translate("Aperture Size"), groupBoxQSnapSensitivity);
    QSlider* sliderQSnapApertureSize = new QSlider(Qt::Horizontal, groupBoxQSnapSensitivity);
    sliderQSnapApertureSize->setRange(1,20);
    sliderQSnapApertureSize->setValue(setting[QSNAP_APERTURE_SIZE].dialog.i);
    connect(sliderQSnapApertureSize, SIGNAL(valueChanged(int)), this, SLOT(sliderQSnapApertureSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutQSnapSensitivity = new QVBoxLayout(groupBoxQSnapSensitivity);
    vboxLayoutQSnapSensitivity->addWidget(labelQSnapApertureSize);
    vboxLayoutQSnapSensitivity->addWidget(sliderQSnapApertureSize);
    groupBoxQSnapSensitivity->setLayout(vboxLayoutQSnapSensitivity);

    /* Widget Layout */
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

QWidget*
Settings_Dialog::createTabQuickTrack()
{
    QWidget* widget = new QWidget(this);

    /* TODO: finish this */

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget*
Settings_Dialog::createTabLineWeight()
{
    QWidget* widget = new QWidget(this);

    /* TODO: finish this */

    /* Misc */
    QGroupBox* groupBoxLwtMisc = new QGroupBox(translate("LineWeight Misc"), widget);

    QGraphicsScene* s = activeScene();

    QCheckBox* checkBoxShowLwt = new QCheckBox(translate("Show LineWeight"), groupBoxLwtMisc);
    if (s) {
        setting[LWT_SHOW_LWT].dialog.b = s->property("ENABLE_LWT").toBool();
    }
    else {
        setting[LWT_SHOW_LWT].dialog.b = setting[LWT_SHOW_LWT].setting.b;
    }
    setting[LWT_SHOW_LWT].preview.b = setting[LWT_SHOW_LWT].dialog.b;
    checkBoxShowLwt->setChecked(setting[LWT_SHOW_LWT].preview.b);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(translate("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if (s) {
        setting[LWT_REAL_RENDER].dialog.b = s->property("ENABLE_REAL").toBool();
    }
    else {
        setting[LWT_REAL_RENDER].dialog.b = setting[LWT_REAL_RENDER].setting.b;
    }
    setting[LWT_REAL_RENDER].preview.b = setting[LWT_REAL_RENDER].dialog.b;
    checkBoxRealRender->setChecked(setting[LWT_REAL_RENDER].preview.b);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(setting[LWT_SHOW_LWT].dialog.b);

    QLabel* labelDefaultLwt = new QLabel(translate("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); /* TODO: remove later. */
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    /* TODO: populate the comboBox and set the initial value. */
    comboBoxDefaultLwt->addItem(QString().setNum(setting[LWT_DEFAULT_LWT].dialog.r, 'F', 2).append(" mm"), setting[LWT_DEFAULT_LWT].dialog.r);
    comboBoxDefaultLwt->setEnabled(false); /* TODO: remove later */

    QVBoxLayout* vboxLayoutLwtMisc = new QVBoxLayout(groupBoxLwtMisc);
    vboxLayoutLwtMisc->addWidget(checkBoxShowLwt);
    vboxLayoutLwtMisc->addWidget(checkBoxRealRender);
    vboxLayoutLwtMisc->addWidget(labelDefaultLwt);
    vboxLayoutLwtMisc->addWidget(comboBoxDefaultLwt);
    groupBoxLwtMisc->setLayout(vboxLayoutLwtMisc);

    /* Widget Layout. */
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

    /* Selection Modes */
    QGroupBox* groupBoxSelectionModes = new QGroupBox(translate("Modes"), widget);

    QCheckBox* checkBoxSelectionModePickFirst = create_checkbox(groupBoxSelectionModes,
        "Allow Preselection (PickFirst)", SELECTION_MODE_PICKFIRST, "");
    checkBoxSelectionModePickFirst->setEnabled(false); /* TODO: Remove this line when Post-selection is available. */

    QCheckBox* checkBoxSelectionModePickAdd = create_checkbox(groupBoxSelectionModes,
        "Add to Selection (PickAdd)", SELECTION_MODE_PICKADD, "");

    QCheckBox* checkBoxSelectionModePickDrag = create_checkbox(groupBoxSelectionModes,
        "Drag to Select (PickDrag)", SELECTION_MODE_PICKDRAG, "");
    checkBoxSelectionModePickDrag->setEnabled(false); /* TODO: Remove this line when this functionality is available. */

    QVBoxLayout* vboxLayoutSelectionModes = new QVBoxLayout(groupBoxSelectionModes);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickFirst);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickAdd);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickDrag);
    groupBoxSelectionModes->setLayout(vboxLayoutSelectionModes);

    /* Selection Colors */
    QGroupBox* groupBoxSelectionColors = new QGroupBox(translate("Colors"), widget);

    QLabel* labelCoolGripColor = new QLabel(translate("Cool Grip (Unselected)"), groupBoxSelectionColors);
    QComboBox* comboBoxCoolGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxCoolGripColor);
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(setting[SELECTION_COOLGRIP_COLOR].dialog.i));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionCoolGripColorCurrentIndexChanged(int)));

    QLabel* labelHotGripColor = new QLabel(translate("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(setting[SELECTION_HOTGRIP_COLOR].dialog.i));
    connect(comboBoxHotGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionHotGripColorCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutSelectionColors = new QVBoxLayout(groupBoxSelectionColors);
    vboxLayoutSelectionColors->addWidget(labelCoolGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxCoolGripColor);
    vboxLayoutSelectionColors->addWidget(labelHotGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxHotGripColor);
    groupBoxSelectionColors->setLayout(vboxLayoutSelectionColors);

    /* Selection Sizes */
    QGroupBox* groupBoxSelectionSizes = new QGroupBox(translate("Sizes"), widget);

    QLabel* labelSelectionGripSize = new QLabel(translate("Grip Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionGripSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionGripSize->setRange(1,20);
    sliderSelectionGripSize->setValue(setting[SELECTION_GRIP_SIZE].dialog.i);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionGripSizeValueChanged(int)));

    QLabel* labelSelectionPickBoxSize = new QLabel(translate("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    sliderSelectionPickBoxSize->setValue(setting[SELECTION_PICKBOX_SIZE].dialog.i);
    connect(sliderSelectionPickBoxSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionPickBoxSizeValueChanged(int)));

    QVBoxLayout* vboxLayoutSelectionSizes = new QVBoxLayout(groupBoxSelectionSizes);
    vboxLayoutSelectionSizes->addWidget(labelSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionGripSize);
    vboxLayoutSelectionSizes->addWidget(labelSelectionPickBoxSize);
    vboxLayoutSelectionSizes->addWidget(sliderSelectionPickBoxSize);
    groupBoxSelectionSizes->setLayout(vboxLayoutSelectionSizes);

    /* Widget Layout */
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
    comboBox->addItem(create_icon("colorred"), translate("Red"), qRgb(255, 0, 0));
    comboBox->addItem(create_icon("coloryellow"), translate("Yellow"), qRgb(255, 255, 0));
    comboBox->addItem(create_icon("colorgreen"), translate("Green"), qRgb(0, 255, 0));
    comboBox->addItem(create_icon("colorcyan"), translate("Cyan"), qRgb(0, 255, 255));
    comboBox->addItem(create_icon("colorblue"), translate("Blue"), qRgb(0, 0, 255));
    comboBox->addItem(create_icon("colormagenta"), translate("Magenta"), qRgb(255, 0, 255));
    comboBox->addItem(create_icon("colorwhite"), translate("White"), qRgb(255, 255, 255));
    /* TODO: Add Other... so the user can select custom colors */
}

void
Settings_Dialog::comboBoxLanguageCurrentIndexChanged(QString lang)
{
    strcpy(setting[GENERAL_LANGUAGE].dialog.s, qPrintable(lang.toLower()));
}

void
Settings_Dialog::comboBoxIconThemeCurrentIndexChanged(QString theme)
{
    strcpy(setting[GENERAL_ICON_THEME].dialog.s, qPrintable(theme));
}

void
Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        setting[GENERAL_ICON_SIZE].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[GENERAL_ICON_SIZE].dialog.i = 16;
        }
    }
    else {
        setting[GENERAL_ICON_SIZE].dialog.i = 16;
    }
}

void
preview_update(void)
{
    mdiArea->useBackgroundLogo(setting[GENERAL_MDI_BG_USE_LOGO].preview.b);
    mdiArea->useBackgroundColor(setting[GENERAL_MDI_BG_USE_COLOR].preview.b);
    mdiArea->useBackgroundTexture(setting[GENERAL_MDI_BG_USE_TEXTURE].preview.b);
    updateAllViewScrollBars(setting[DISPLAY_SHOW_SCROLLBARS].preview.b);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage = QFileDialog::getOpenFileName(this,
            translate("Open File"),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            translate("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            strcpy(setting[GENERAL_MDI_BG_LOGO].accept.s, qPrintable(selectedImage));
        }

        /* Update immediately so it can be previewed */
        mdiArea->setBackgroundLogo(setting[GENERAL_MDI_BG_LOGO].accept.s);
    }
}

void
Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, translate("Open File"),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
            translate("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            strcpy(setting[GENERAL_MDI_BG_TEXTURE].accept.s, qPrintable(selectedImage));
        }

        /* Update immediately so it can be previewed */
        mdiArea->setBackgroundTexture(setting[GENERAL_MDI_BG_TEXTURE].accept.s);
    }
}

void
Settings_Dialog::chooseGeneralMdiBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, GENERAL_MDI_BG_COLOR);
    }
}

/* . */
void
Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    setting[GENERAL_MDI_BG_COLOR].preview.i = color.rgb();
    mdiArea->setBackgroundColor(QColor(setting[GENERAL_MDI_BG_COLOR].preview.i));
}

/* . */
void
Settings_Dialog::comboBoxScrollBarWidgetCurrentIndexChanged(int index)
{
    setting[DISPLAY_SCROLLBAR_WIDGET_NUM].dialog.i = index;
}

/* . */
void
Settings_Dialog::color_dialog(QPushButton *button, int key)
{
    QColorDialog* colorDialog = new QColorDialog(QColor(setting[key].accept.i), this);
    connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayCrossHairColorChanged(const QColor&)));
    colorDialog->exec();

    if (colorDialog->result() == QDialog::Accepted) {
        setting[key].accept.i = colorDialog->selectedColor().rgb();
        button->setIcon(create_swatch(setting[key].accept.i));
        switch (key) {
        case DISPLAY_CROSSHAIR_COLOR:
            updateAllViewCrossHairColors(setting[key].accept.i);
            break;
        case DISPLAY_BG_COLOR:
            updateAllViewBackgroundColors(setting[key].accept.i);
            break;
        case DISPLAY_SELECTBOX_LEFT_COLOR:
        case DISPLAY_SELECTBOX_LEFT_FILL:
        case DISPLAY_SELECTBOX_RIGHT_COLOR:
        case DISPLAY_SELECTBOX_RIGHT_FILL:
            updateAllViewSelectBoxColors(
                setting[DISPLAY_SELECTBOX_LEFT_COLOR].accept.i,
                setting[DISPLAY_SELECTBOX_LEFT_FILL].accept.i,
                setting[DISPLAY_SELECTBOX_RIGHT_COLOR].accept.i,
                setting[DISPLAY_SELECTBOX_RIGHT_FILL].accept.i,
                setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
            break;
        case PROMPT_TEXT_COLOR:
            setPromptTextColor(QColor(setting[key].accept.i));
            break;
        case PROMPT_BG_COLOR:
            setPromptBackgroundColor(QColor(setting[key].accept.i));
            break;
        case GRID_COLOR:
            updateAllViewGridColors(setting[GRID_COLOR].accept.i);
            break;
        case RULER_COLOR:
            updateAllViewRulerColors(setting[key].accept.i);
            break;
        case GENERAL_MDI_BG_COLOR:
            mdiArea->setBackgroundColor(QColor(setting[key].accept.i));
            break;
        default:
            break;
        }
    }
    else {
        switch (key) {
        case DISPLAY_CROSSHAIR_COLOR:
            updateAllViewCrossHairColors(setting[key].dialog.i);
            break;
        case DISPLAY_BG_COLOR:
            updateAllViewBackgroundColors(setting[key].dialog.i);
            break;
        case DISPLAY_SELECTBOX_LEFT_COLOR:
        case DISPLAY_SELECTBOX_LEFT_FILL:
        case DISPLAY_SELECTBOX_RIGHT_COLOR:
        case DISPLAY_SELECTBOX_RIGHT_FILL:
            updateAllViewSelectBoxColors(
                setting[DISPLAY_SELECTBOX_LEFT_COLOR].dialog.i,
                setting[DISPLAY_SELECTBOX_LEFT_FILL].dialog.i,
                setting[DISPLAY_SELECTBOX_RIGHT_COLOR].dialog.i,
                setting[DISPLAY_SELECTBOX_RIGHT_FILL].dialog.i,
                setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
            break;
        case PROMPT_TEXT_COLOR:
            setPromptTextColor(QColor(setting[key].dialog.i));
            break;
        case PROMPT_BG_COLOR:
            setPromptBackgroundColor(QColor(setting[key].dialog.i));
            break;
        case GRID_COLOR:
            updateAllViewGridColors(setting[GRID_COLOR].dialog.i);
            break;
        case RULER_COLOR:
            updateAllViewRulerColors(setting[key].dialog.i);
            break;
        case GENERAL_MDI_BG_COLOR:
            mdiArea->setBackgroundColor(QColor(setting[key].dialog.i));
            break;
        default:
            break;
        }
    }
}

/* . */
void
Settings_Dialog::chooseDisplayCrossHairColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, DISPLAY_CROSSHAIR_COLOR);
    }
}

/* . */
void
Settings_Dialog::currentDisplayCrossHairColorChanged(const QColor& color)
{
    setting[DISPLAY_CROSSHAIR_COLOR].preview.i = color.rgb();
    updateAllViewCrossHairColors(setting[DISPLAY_CROSSHAIR_COLOR].preview.i);
}

/* . */
void
Settings_Dialog::chooseDisplayBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, DISPLAY_BG_COLOR);
    }
}

void
Settings_Dialog::currentDisplayBackgroundColorChanged(const QColor& color)
{
    setting[DISPLAY_BG_COLOR].preview.i = color.rgb();
    updateAllViewBackgroundColors(setting[DISPLAY_BG_COLOR].preview.i);
}

void
Settings_Dialog::chooseDisplaySelectBoxLeftColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, DISPLAY_SELECTBOX_LEFT_COLOR);
    }
}

void
Settings_Dialog::currentDisplaySelectBoxLeftColorChanged(const QColor& color)
{
    setting[DISPLAY_SELECTBOX_LEFT_COLOR].preview.i = color.rgb();
    updateAllViewSelectBoxColors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].preview.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].preview.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].preview.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].preview.i,
        setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
}

void
Settings_Dialog::chooseDisplaySelectBoxLeftFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, DISPLAY_SELECTBOX_LEFT_FILL);
    }
}

void
Settings_Dialog::currentDisplaySelectBoxLeftFillChanged(const QColor& color)
{
    setting[DISPLAY_SELECTBOX_LEFT_FILL].preview.i = color.rgb();
    updateAllViewSelectBoxColors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].preview.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].preview.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].preview.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].preview.i,
        setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
}

void
Settings_Dialog::chooseDisplaySelectBoxRightColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, DISPLAY_SELECTBOX_RIGHT_COLOR);
    }
}

void
Settings_Dialog::currentDisplaySelectBoxRightColorChanged(const QColor& color)
{
    setting[DISPLAY_SELECTBOX_RIGHT_COLOR].preview.i = color.rgb();
    updateAllViewSelectBoxColors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].preview.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].preview.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].preview.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].preview.i,
        setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
}

void
Settings_Dialog::chooseDisplaySelectBoxRightFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, DISPLAY_SELECTBOX_RIGHT_FILL);
    }
}

void
Settings_Dialog::currentDisplaySelectBoxRightFillChanged(const QColor& color)
{
    setting[DISPLAY_SELECTBOX_RIGHT_FILL].preview.i = color.rgb();
    updateAllViewSelectBoxColors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].preview.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].preview.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].preview.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].preview.i,
        setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
}

void
Settings_Dialog::spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    setting[DISPLAY_SELECTBOX_ALPHA].preview.i = value;
    updateAllViewSelectBoxColors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].accept.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].accept.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].accept.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].accept.i,
        setting[DISPLAY_SELECTBOX_ALPHA].preview.i);
}

void
Settings_Dialog::choosePromptTextColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, PROMPT_TEXT_COLOR);
    }
}

/* . */
void
Settings_Dialog::currentPromptTextColorChanged(const QColor& color)
{
    setting[PROMPT_TEXT_COLOR].preview.i = color.rgb();
    setPromptTextColor(QColor(setting[PROMPT_TEXT_COLOR].preview.i));
}

/* . */
void
Settings_Dialog::choosePromptBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, PROMPT_BG_COLOR);
    }
}

/* . */
void
Settings_Dialog::currentPromptBackgroundColorChanged(const QColor& color)
{
    setting[PROMPT_BG_COLOR].preview.i = color.rgb();
    setPromptBackgroundColor(QColor(setting[PROMPT_BG_COLOR].preview.i));
}

/* . */
void
Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(QString family)
{
    strcpy(setting[PROMPT_FONT_FAMILY].preview.s, qPrintable(family));
    setPromptFontFamily(setting[PROMPT_FONT_FAMILY].preview.s);
}

/* . */
void
Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(QString style)
{
    strcpy(setting[PROMPT_FONT_STYLE].preview.s, qPrintable(style));
    setPromptFontStyle(setting[PROMPT_FONT_STYLE].preview.s);
}

/* . */
void
Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    setting[PROMPT_FONT_SIZE].preview.i = value;
    setPromptFontSize(setting[PROMPT_FONT_SIZE].preview.i);
}

/* . */
void
Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        char message[MAX_STRING_LENGTH];
        QString format = checkBox->text();
        sprintf(message, "CustomFilter: %s %d", qPrintable(format), checked);
        debug_message(message);
        if (checked) {
            strcat(setting[OPENSAVE_CUSTOM_FILTER].dialog.s,
                qPrintable(" *." + format.toLower()));
        }
        else {
            QString s;
            s = QString(setting[OPENSAVE_CUSTOM_FILTER].dialog.s).remove("*." + format, Qt::CaseInsensitive);
            strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, qPrintable(s));
        }
        /* setting[OPENSAVE_USE_CUSTOM_FILTER].dialog.s = checked; */ /* TODO */
    }
}

void
Settings_Dialog::buttonCustomFilterSelectAllClicked()
{
    emit buttonCustomFilterSelectAll(true);
    strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, "supported");
}

void
Settings_Dialog::buttonCustomFilterClearAllClicked()
{
    emit buttonCustomFilterClearAll(false);
    strcpy(setting[OPENSAVE_CUSTOM_FILTER].dialog.s, "");
}

/* . */
void
Settings_Dialog::spinBoxRecentMaxFilesValueChanged(int value)
{
    setting[OPENSAVE_RECENT_MAX_FILES].dialog.i = value;
}

/* . */
void
Settings_Dialog::spinBoxTrimDstNumJumpsValueChanged(int value)
{
    setting[OPENSAVE_TRIM_DST_NUM_JUMPS].dialog.i = value;
}

/* . */
void
Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b = checked;
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        updateAllViewGridColors(setting[DISPLAY_CROSSHAIR_PERCENT].accept.i);
    }
    else {
        updateAllViewGridColors(setting[GRID_COLOR].accept.i);
    }

    QObject* senderObj = sender();
    if (senderObj)
    {
        QObject* parent = senderObj->parent();
        if (parent)
        {
            QLabel* labelGridColor = parent->findChild<QLabel*>("labelGridColor");
            if (labelGridColor) labelGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
            QPushButton* buttonGridColor = parent->findChild<QPushButton*>("buttonGridColor");
            if (buttonGridColor) buttonGridColor->setEnabled(!setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b);
        }
    }
}

/* . */
void
Settings_Dialog::chooseGridColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, GRID_COLOR);
    }
}

/* . */
void
Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    setting[GRID_COLOR].preview.i = color.rgb();
    updateAllViewGridColors(setting[GRID_COLOR].preview.i);
}

/* . */
void
Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    setting[GRID_LOAD_FROM_FILE].dialog.b = checked;

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }

    bool dont_load = !setting[GRID_LOAD_FROM_FILE].dialog.b;
    set_enabled_group(senderObj, grid_load_from_file_group, dont_load);

    bool use_this_origin = !setting[GRID_LOAD_FROM_FILE].dialog.b && !setting[GRID_CENTER_ON_ORIGIN].dialog.b;
    set_enabled_group(senderObj, defined_origin_group, use_this_origin);
}

/* . */
void
Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(QString type)
{
    strcpy(setting[GRID_TYPE].dialog.s, qPrintable(type));

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    bool visibility = (type == "Circular");
    set_visibility_group(senderObj, rectangular_grid_group, !visibility);
    set_visibility_group(senderObj, circular_grid_group, visibility);
}

/* . */
void
Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    setting[GRID_CENTER_ON_ORIGIN].dialog.b = checked;

    QObject* senderObj = sender();
    if (senderObj) {
        set_enabled_group(senderObj, center_on_origin_group, !setting[GRID_CENTER_ON_ORIGIN].dialog.b);
    }
}

/* . */
void
Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        setting[RULER_METRIC].dialog.i = comboBox->itemData(index).toBool();
    }
    else {
        setting[RULER_METRIC].dialog.i = true;
    }
}

/* . */
void
Settings_Dialog::chooseRulerColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        color_dialog(button, RULER_COLOR);
    }
}

/* . */
void
Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    setting[RULER_COLOR].preview.i = color.rgb();
    updateAllViewRulerColors(setting[RULER_COLOR].preview.i);
}

/* . */
void
Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    setting[RULER_PIXEL_SIZE].dialog.r = value;
}

/* . */
void
Settings_Dialog::buttonQSnapSelectAllClicked()
{
    emit buttonQSnapSelectAll(true);
}

/* . */
void
Settings_Dialog::buttonQSnapClearAllClicked()
{
    emit buttonQSnapClearAll(false);
}

/* TODO: Figure out how to abstract the slot in a way that it can be used for comboBoxes in general
 *      Currently comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
 *                comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
 *                comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
 *      are all similar except the dialog_ variable being worked on and the QVariant.
 */

void
Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    /* TODO: Alert user if color matched the display bg color. */
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,255,0); /* Yellow */
    if (comboBox) {
        bool ok = 0;
        setting[QSNAP_LOCATOR_COLOR].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[QSNAP_LOCATOR_COLOR].dialog.i = defaultColor;
        }
    }
    else {
        setting[QSNAP_LOCATOR_COLOR].dialog.i = defaultColor;
    }
}

/* . */
void
Settings_Dialog::sliderQSnapLocatorSizeValueChanged(int value)
{
    setting[QSNAP_LOCATOR_SIZE].dialog.i = value;
}

/* . */
void
Settings_Dialog::sliderQSnapApertureSizeValueChanged(int value)
{
    setting[QSNAP_APERTURE_SIZE].dialog.i = value;
}

/* . */
void
Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    setting[LWT_SHOW_LWT].preview.b = checked;
    if (setting[LWT_SHOW_LWT].preview.b) {
        enableLwt();
    }
    else {
        disableLwt();
    }

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
            if (checkBoxRealRender)
                checkBoxRealRender->setEnabled(setting[LWT_SHOW_LWT].preview.b);
        }
    }
}

/* . */
void
Settings_Dialog::checkBoxLwtRealRenderStateChanged(int checked)
{
    setting[LWT_REAL_RENDER].preview.b = checked;
    if (setting[LWT_REAL_RENDER].preview.b) {
        enableReal();
    }
    else {
        disableReal();
    }
}

/* . */
void
Settings_Dialog::sliderSelectionGripSizeValueChanged(int value)
{
    setting[SELECTION_GRIP_SIZE].dialog.i = value;
}

/* . */
void
Settings_Dialog::sliderSelectionPickBoxSizeValueChanged(int value)
{
    setting[SELECTION_PICKBOX_SIZE].dialog.i = value;
}

/* . */
void
Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    /* TODO: Alert user if color matched the display bg color */
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(0,0,255); /* Blue */
    if (comboBox) {
        bool ok = 0;
        setting[SELECTION_COOLGRIP_COLOR].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[SELECTION_COOLGRIP_COLOR].dialog.i = defaultColor;
        }
    }
    else {
        setting[SELECTION_COOLGRIP_COLOR].dialog.i = defaultColor;
    }
}

/* . */
void
Settings_Dialog::comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
{
    /* TODO: Alert user if color matched the display bg color. */
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,0,0); /* Red */
    if (comboBox) {
        bool ok = 0;
        setting[SELECTION_HOTGRIP_COLOR].dialog.i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            setting[SELECTION_HOTGRIP_COLOR].dialog.i = defaultColor;
        }
    }
    else {
        setting[SELECTION_HOTGRIP_COLOR].dialog.i = defaultColor;
    }
}

void
update_view(void)
{
    mdiArea->useBackgroundLogo(setting[GENERAL_MDI_BG_USE_LOGO].dialog.b);
    mdiArea->useBackgroundTexture(setting[GENERAL_MDI_BG_USE_TEXTURE].dialog.b);
    mdiArea->useBackgroundColor(setting[GENERAL_MDI_BG_USE_COLOR].dialog.b);
    mdiArea->setBackgroundLogo(setting[GENERAL_MDI_BG_LOGO].dialog.s);
    mdiArea->setBackgroundTexture(setting[GENERAL_MDI_BG_TEXTURE].dialog.s);
    mdiArea->setBackgroundColor(setting[GENERAL_MDI_BG_COLOR].dialog.i);

    iconResize(setting[GENERAL_ICON_SIZE].dialog.i);

    updateAllViewScrollBars(setting[DISPLAY_SHOW_SCROLLBARS].dialog.b);
    updateAllViewCrossHairColors(setting[DISPLAY_CROSSHAIR_COLOR].dialog.i);
    updateAllViewBackgroundColors(setting[DISPLAY_BG_COLOR].dialog.i);
    updateAllViewSelectBoxColors(
        setting[DISPLAY_SELECTBOX_LEFT_COLOR].dialog.i,
        setting[DISPLAY_SELECTBOX_LEFT_FILL].dialog.i,
        setting[DISPLAY_SELECTBOX_RIGHT_COLOR].dialog.i,
        setting[DISPLAY_SELECTBOX_RIGHT_FILL].dialog.i,
        setting[DISPLAY_SELECTBOX_ALPHA].dialog.i);
    updateAllViewGridColors(setting[GRID_COLOR].dialog.i);
    updateAllViewRulerColors(setting[RULER_COLOR].dialog.i);

    setPromptTextColor(QColor(setting[PROMPT_TEXT_COLOR].dialog.i));
    setPromptBackgroundColor(QColor(setting[PROMPT_BG_COLOR].dialog.i));
    setPromptFontFamily(setting[PROMPT_FONT_FAMILY].dialog.s);
    setPromptFontStyle(setting[PROMPT_FONT_STYLE].dialog.s);
    setPromptFontSize(setting[PROMPT_FONT_SIZE].dialog.i);

    if (setting[LWT_SHOW_LWT].dialog.b) {
        enableLwt();
    }
    else {
        disableLwt();
    }
    if (setting[LWT_REAL_RENDER].dialog.b) {
        enableReal();
    }
    else {
        disableReal();
    }

    updatePickAddMode(setting[SELECTION_MODE_PICKADD].dialog.b);
}

/* . */
void
Settings_Dialog::acceptChanges()
{
    copy_setting(GENERAL_MDI_BG_USE_LOGO, SETTING_DIALOG, SETTING_PREVIEW);
    copy_setting(GENERAL_MDI_BG_USE_TEXTURE, SETTING_DIALOG, SETTING_PREVIEW);
    copy_setting(GENERAL_MDI_BG_USE_COLOR, SETTING_DIALOG, SETTING_PREVIEW);
    copy_setting(GENERAL_MDI_BG_LOGO, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(GENERAL_MDI_BG_TEXTURE, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(GENERAL_MDI_BG_COLOR, SETTING_DIALOG, SETTING_ACCEPT);

    copy_setting(DISPLAY_SHOW_SCROLLBARS, SETTING_DIALOG, SETTING_PREVIEW);
    copy_setting(DISPLAY_CROSSHAIR_COLOR, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(DISPLAY_BG_COLOR, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(DISPLAY_SELECTBOX_LEFT_COLOR, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(DISPLAY_SELECTBOX_LEFT_FILL, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(DISPLAY_SELECTBOX_RIGHT_COLOR, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(DISPLAY_SELECTBOX_RIGHT_FILL, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(DISPLAY_SELECTBOX_ALPHA, SETTING_DIALOG, SETTING_PREVIEW);

    copy_setting(PROMPT_TEXT_COLOR, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(PROMPT_BG_COLOR, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(PROMPT_FONT_FAMILY, SETTING_DIALOG, SETTING_PREVIEW);
    copy_setting(PROMPT_FONT_STYLE, SETTING_DIALOG, SETTING_PREVIEW);
    copy_setting(PROMPT_FONT_SIZE, SETTING_DIALOG, SETTING_PREVIEW);
    if (setting[GRID_COLOR_MATCH_CROSSHAIR].dialog.b) {
        setting[GRID_COLOR].dialog.i = setting[DISPLAY_CROSSHAIR_COLOR].accept.i;
    }
    else {
        copy_setting(GRID_COLOR, SETTING_DIALOG, SETTING_ACCEPT);
    }
    copy_setting(RULER_COLOR, SETTING_DIALOG, SETTING_ACCEPT);
    copy_setting(LWT_SHOW_LWT, SETTING_DIALOG, SETTING_PREVIEW);
    copy_setting(LWT_REAL_RENDER, SETTING_DIALOG, SETTING_PREVIEW);

    for (int i=0; i < N_SETTINGS; i++) {
        copy_setting(i, SETTING_SETTING, SETTING_DIALOG);
    }

    /* Make sure the user sees the changes applied immediately. */
    update_view();

    writeSettings();
    accept();
}

/* TODO: inform the user if they have changed settings */
void
Settings_Dialog::rejectChanges()
{
    /* Update the view since the user must accept the preview */
    update_view();

    reject();
}

