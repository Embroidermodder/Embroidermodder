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

ScriptValue st[N_SETTINGS];

void
color_swatch(QPushButton *button, uint32_t color)
{
    QPixmap pix(16, 16);
    pix.fill(QColor(color));
    button->setIcon(QIcon(pix));
}

void
set_label_visible(QObject* parent, const char *key, bool visibility)
{
    QLabel* label = parent->findChild<QLabel*>(key);
    if (label) {
        label->setVisible(visibility);
    }
    else {
        debug("ERROR: Failed to find label with the key \"%s\".", key);
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
        debug("ERROR: Failed to find spinbox with the key \"%s\".", key);
    }
}

Settings_Dialog::Settings_Dialog(const QString& showTab, QWidget* parent) : QDialog(parent)
{
    memcpy(st_dialog, st, sizeof(st));
    memcpy(st_accept, st, sizeof(st));
    memcpy(st_preview, st, sizeof(st));

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

/*! Create a checkbox that only needs the basic features. */
QCheckBox *
Settings_Dialog::basic_checkbox(QGroupBox *gb, int id)
{
    QCheckBox* cb = new QCheckBox(tr(settings_table[id].description), gb);
    cb->setChecked(st_dialog[id].b);
    cb->setEnabled(settings_table[id].enabled);
    if (settings_table[id].icon != "") {
        cb->setIcon(_mainWin->createIcon(settings_table[id].icon));
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
    sb->setObjectName(QString("spinBox") + settings_table[id].key);
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
    QString current = st_dialog[ST_LANGUAGE].s;
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    connect(comboBoxLanguage, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxLanguageCurrentIndexChanged(const QString&)));

    QVBoxLayout* vboxLayoutLanguage = new QVBoxLayout(groupBoxLanguage);
    vboxLayoutLanguage->addWidget(labelLanguage);
    vboxLayoutLanguage->addWidget(comboBoxLanguage);
    groupBoxLanguage->setLayout(vboxLayoutLanguage);

    //Icons
    QGroupBox* groupBoxIcon = new QGroupBox(tr("Icons"), widget);

    QString theme = st_dialog[ST_ICON_THEME].s;
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
    checkBoxMdiBGUseLogo->setChecked(st_preview[ST_MDI_BG_USE_LOGO].b);
    connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)));

    QPushButton* buttonMdiBGLogo = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(st_dialog[ST_MDI_BG_USE_LOGO].b);
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseTexture = new QCheckBox(tr("Use Texture"), groupBoxMdiBG);
    checkBoxMdiBGUseTexture->setChecked(st_preview[ST_MDI_BG_USE_TEXTURE].b);
    connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)));

    QPushButton* buttonMdiBGTexture = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(st_dialog[ST_MDI_BG_USE_TEXTURE].b);
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseColor = new QCheckBox(tr("Use Color"), groupBoxMdiBG);
    checkBoxMdiBGUseColor->setChecked(st_preview[ST_MDI_BG_USE_COLOR].b);
    connect(checkBoxMdiBGUseColor, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseColorStateChanged(int)));

    QPushButton* buttonMdiBGColor = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setEnabled(st_dialog[ST_MDI_BG_USE_COLOR].b);
    color_swatch(buttonMdiBGColor, st_preview[ST_MDI_BG_COLOR].u);
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
    checkBoxShowScrollBars->setChecked(st_preview[ST_SHOW_SCROLLBARS].b);
    connect(checkBoxShowScrollBars, SIGNAL(stateChanged(int)), this, SLOT(checkBoxShowScrollBarsStateChanged(int)));

    QLabel* labelScrollBarWidget = new QLabel(tr("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    int numActions = actionHash.size();
    for (int i = 0; i < numActions; i++) {
        /* FIXME:
        QAction* action = actionHash.value(i);
        if (action) comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        */
    }
    comboBoxScrollBarWidget->setCurrentIndex(st_dialog[ST_SCROLLBAR_WIDGET_NUM].i);
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
    crosshairPix.fill(QColor(st_preview[ST_CROSSHAIR_COLOR].u));
    buttonCrossHairColor->setIcon(QIcon(crosshairPix));
    connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap bgPix(16,16);
    bgPix.fill(QColor(st_preview[ST_BG_COLOR].u));
    buttonBGColor->setIcon(QIcon(bgPix));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()));

    QLabel* labelSelectBoxLeftColor = new QLabel(tr("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap sBoxLCPix(16,16);
    sBoxLCPix.fill(QColor(st_preview[ST_SELECTBOX_LEFT_COLOR].u));
    buttonSelectBoxLeftColor->setIcon(QIcon(sBoxLCPix));
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()));

    QLabel* labelSelectBoxLeftFill = new QLabel(tr("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap sBoxLFPix(16,16);
    sBoxLFPix.fill(QColor(st_preview[ST_SELECTBOX_LEFT_FILL].u));
    buttonSelectBoxLeftFill->setIcon(QIcon(sBoxLFPix));
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()));

    QLabel* labelSelectBoxRightColor = new QLabel(tr("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap sBoxRCPix(16,16);
    sBoxRCPix.fill(QColor(st_preview[ST_SELECTBOX_RIGHT_COLOR].u));
    buttonSelectBoxRightColor->setIcon(QIcon(sBoxRCPix));
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()));

    QLabel* labelSelectBoxRightFill = new QLabel(tr("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap sBoxRFPix(16,16);
    sBoxRFPix.fill(QColor(st_preview[ST_SELECTBOX_RIGHT_FILL].u));
    buttonSelectBoxRightFill->setIcon(QIcon(sBoxRFPix));
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()));

    QLabel* labelSelectBoxAlpha = new QLabel(tr("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    spinBoxSelectBoxAlpha->setValue(st_preview[ST_SELECTBOX_ALPHA].i);
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
    pix.fill(QColor(st_preview[ST_PROMPT_TEXT_COLOR].u));
    buttonTextColor->setIcon(QIcon(pix));
    connect(buttonTextColor, SIGNAL(clicked()), this, SLOT(choosePromptTextColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    QPixmap pixx(16,16);
    pixx.fill(QColor(st_preview[ST_PROMPT_BG_COLOR].u));
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
    comboBoxFontFamily->setCurrentFont(QFont(st_preview[ST_PROMPT_FONT_FAMILY].s));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(const QString&)));
    QLabel* labelFontStyle = new QLabel(tr("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(st_preview[ST_PROMPT_FONT_STYLE].s);
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(const QString&)));
    QLabel* labelFontSize = new QLabel(tr("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    spinBoxFontSize->setValue(st_preview[ST_PROMPT_FONT_SIZE].i);
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
    QString opensave_custom_filter = st_dialog[ST_OPENSAVE_CUSTOM_FILTER].s;
    for (size_t i=0; i<ext.size(); i++) {
        checkBoxCustomFilter[i] = new QCheckBox(ext[i].c_str(),
            groupBoxCustomFilter);
        checkBoxCustomFilter[i]->setChecked(
            opensave_custom_filter.contains(QString("*.") + ext[i].c_str(),
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

    if (opensave_custom_filter.contains("supported", Qt::CaseInsensitive)) {
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
    spinBoxRecentMaxFiles->setValue(st_dialog[ST_RECENT_MAX_FILES].i);
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
    spinBoxTrimDstNumJumps->setValue(st_dialog[ST_TRIM_DST_NUM_JUMPS].i);
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
    checkBoxDisableBG->setChecked(st_dialog[ST_PRINTING_DISABLE_BG].b);
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
    checkBoxGridShowOnLoad->setChecked(st_dialog[ST_GRID_SHOW_ON_LOAD].b);
    connect(checkBoxGridShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOnLoadStateChanged(int)));

    QCheckBox* checkBoxGridShowOrigin = new QCheckBox(tr("Show the origin when the grid is enabled"), groupBoxGridMisc);
    checkBoxGridShowOrigin->setChecked(st_dialog[ST_GRID_SHOW_ORIGIN].b);
    connect(checkBoxGridShowOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOriginStateChanged(int)));

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = new QGroupBox(tr("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(tr("Match grid color to crosshair color"), groupBoxGridColor);
    checkBoxGridColorMatchCrossHair->setChecked(st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b);
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
    gridPix.fill(QColor(st_preview[ST_GRID_COLOR].u));
    buttonGridColor->setIcon(QIcon(gridPix));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor->setEnabled(!st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b);
    buttonGridColor->setEnabled(!st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = new QGroupBox(tr("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(tr("Set grid size from opened file"), groupBoxGridGeom);
    checkBoxGridLoadFromFile->setChecked(st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(tr("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(st_dialog[ST_GRID_TYPE].s));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(const QString&)));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(tr("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    checkBoxGridCenterOnOrigin->setChecked(st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
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

    labelGridType->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    comboBoxGridType->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    checkBoxGridCenterOnOrigin->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridCenterX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridCenterX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridCenterY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridCenterY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridSizeX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridSizeX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridSizeY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridSizeY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridSpacingX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridSpacingX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridSpacingY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridSpacingY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridSizeRadius->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridSizeRadius->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridSpacingRadius->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridSpacingRadius->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    labelGridSpacingAngle->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
    spinBoxGridSpacingAngle->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);

    bool visibility = false;
    if (QString(st_dialog[ST_GRID_TYPE].s) == "Circular") {
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
    checkBoxRulerShowOnLoad->setChecked(st_dialog[ST_RULER_SHOW_ON_LOAD].b);
    connect(checkBoxRulerShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRulerShowOnLoadStateChanged(int)));

    QLabel* labelRulerMetric = new QLabel(tr("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric", true);
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(st_dialog[ST_RULER_METRIC].b));
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
    rulerPix.fill(QColor(st_preview[ST_RULER_COLOR].u));
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
    spinBoxRulerPixelSize->setValue(st_dialog[ST_RULER_PIXEL_SIZE].i);
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
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(st_dialog[ST_QSNAP_LOCATOR_COLOR].u));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(tr("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    sliderQSnapLocSize->setValue(st_dialog[ST_QSNAP_LOCATOR_SIZE].i);
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
    sliderQSnapApertureSize->setValue(st_dialog[ST_QSNAP_APERTURE_SIZE].i);
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
        st_dialog[ST_LWT_SHOW_LWT].b = s->property(ENABLE_LWT).toBool();
        st_preview[ST_LWT_SHOW_LWT].b = st_dialog[ST_LWT_SHOW_LWT].b;

        st_dialog[ST_LWT_REAL_RENDER].b = s->property(ENABLE_REAL).toBool();
        st_preview[ST_LWT_REAL_RENDER].b = st_dialog[ST_LWT_REAL_RENDER].b;
    }

    QCheckBox* checkBoxShowLwt = new QCheckBox(tr("Show LineWeight"), groupBoxLwtMisc);
    checkBoxShowLwt->setChecked(st_preview[ST_LWT_SHOW_LWT].b);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(tr("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    checkBoxRealRender->setChecked(st_preview[ST_LWT_REAL_RENDER].b);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(st_dialog[ST_LWT_SHOW_LWT].b);

    QLabel* labelDefaultLwt = new QLabel(tr("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //TODO: remove later
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    //TODO: populate the comboBox and set the initial value
    comboBoxDefaultLwt->addItem(
        QString().setNum(st_dialog[ST_LWT_DEFAULT_LWT].r, 'F', 2).append(" mm"),
        st_dialog[ST_LWT_DEFAULT_LWT].r);
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
    checkBoxSelectionModePickFirst->setChecked(st_dialog[ST_SELECTION_MODE_PICKFIRST].b);
    checkBoxSelectionModePickFirst->setChecked(true); checkBoxSelectionModePickFirst->setEnabled(false); //TODO: Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickFirstStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickAdd = new QCheckBox(tr("Add to Selection (PickAdd)"), groupBoxSelectionModes);
    checkBoxSelectionModePickAdd->setChecked(st_dialog[ST_SELECTION_MODE_PICKADD].b);
    connect(checkBoxSelectionModePickAdd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickAddStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickDrag = new QCheckBox(tr("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    checkBoxSelectionModePickDrag->setChecked(st_dialog[ST_SELECTION_MODE_PICKDRAG].b);
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
    comboBoxCoolGripColor->setCurrentIndex(
        comboBoxCoolGripColor->findData(st_dialog[ST_SELECTION_COOLGRIP_COLOR].u));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionCoolGripColorCurrentIndexChanged(int)));

    QLabel* labelHotGripColor = new QLabel(tr("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    comboBoxHotGripColor->setCurrentIndex(
        comboBoxHotGripColor->findData(st_dialog[ST_SELECTION_HOTGRIP_COLOR].u));
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
    sliderSelectionGripSize->setValue(st_dialog[ST_SELECTION_GRIP_SIZE].i);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionGripSizeValueChanged(int)));

    QLabel* labelSelectionPickBoxSize = new QLabel(tr("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    sliderSelectionPickBoxSize->setValue(st_dialog[ST_SELECTION_PICKBOX_SIZE].i);
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
    strncpy(st_dialog[ST_LANGUAGE].s, qPrintable(lang.toLower()), 200);
}

/* Combobox callback for the general interface icon theme. */
void
Settings_Dialog::comboBoxIconThemeCurrentIndexChanged(const QString& theme)
{
    strncpy(st_dialog[ST_ICON_THEME].s, qPrintable(theme), 200);
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

void
Settings_Dialog::checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    st_preview[ST_MDI_BG_USE_LOGO].b = checked;
    mdiArea->useBackgroundLogo(checked);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        tr("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            strcpy(st_accept[ST_MDI_BG_LOGO].s, qPrintable(selectedImage));
        }

        /* Update immediately so it can be previewed. */
        mdiArea->setBackgroundLogo(st_accept[ST_MDI_BG_LOGO].s);
    }
}

void
Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    st_preview[ST_MDI_BG_USE_TEXTURE].b = checked;
    mdiArea->useBackgroundTexture(checked);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        tr("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            strcpy(st_accept[ST_MDI_BG_TEXTURE].s, qPrintable(selectedImage));
        }

        /* Update immediately so it can be previewed. */
        mdiArea->setBackgroundTexture(st_accept[ST_MDI_BG_TEXTURE].s);
    }
}

void
Settings_Dialog::checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    st_preview[ST_MDI_BG_USE_COLOR].b = checked;
    mdiArea->useBackgroundColor(checked);
}

QColor
Settings_Dialog::colorDialog(QPushButton* button, int id)
{
    QColorDialog* colorDialog = new QColorDialog(QColor(st_accept[id].u), this);
    connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this,
        SLOT(currentGeneralMdiBackgroundColorChanged(const QColor&)));
    colorDialog->exec();

    if (colorDialog->result() == QDialog::Accepted) {
        st_accept[id].u = colorDialog->selectedColor().rgb();
        color_swatch(button, st_accept[id].u);
        return QColor(st_accept[id].u);
    }
    return QColor(st_dialog[id].u);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColor color = colorDialog(button, ST_MDI_BG_COLOR);
        mdiArea->setBackgroundColor(color);
    }
}

void
Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    st_preview[ST_MDI_BG_COLOR].u = color.rgb();
    mdiArea->setBackgroundColor(QColor(st_preview[ST_MDI_BG_COLOR].u));
}

void
Settings_Dialog::checkBoxTipOfTheDayStateChanged(int checked)
{
    st_dialog[ST_TIP_OF_THE_DAY].b = checked;
}

void
Settings_Dialog::checkBoxShowScrollBarsStateChanged(int checked)
{
    st_preview[ST_SHOW_SCROLLBARS].b = checked;
    _mainWin->updateAllViewScrollBars(st_preview[ST_SHOW_SCROLLBARS].b);
}

void
Settings_Dialog::comboBoxScrollBarWidgetCurrentIndexChanged(int index)
{
    st_dialog[ST_SCROLLBAR_WIDGET_NUM].i = index;
}

void
Settings_Dialog::spinBoxZoomScaleInValueChanged(double value)
{
    st_dialog[ST_ZOOMSCALE_IN].r = value;
}

void
Settings_Dialog::spinBoxZoomScaleOutValueChanged(double value)
{
    st_dialog[ST_ZOOMSCALE_OUT].r = value;
}

void
Settings_Dialog::checkBoxDisableBGStateChanged(int checked)
{
    st_dialog[ST_PRINTING_DISABLE_BG].b = checked;
}

void
Settings_Dialog::chooseDisplayCrossHairColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColor color = colorDialog(button, ST_CROSSHAIR_COLOR);
        _mainWin->updateAllViewCrossHairColors(color.rgb());
    }
}

void
Settings_Dialog::currentDisplayCrossHairColorChanged(const QColor& color)
{
    st_preview[ST_CROSSHAIR_COLOR].u = color.rgb();
    _mainWin->updateAllViewCrossHairColors(st_preview[ST_CROSSHAIR_COLOR].u);
}

void
Settings_Dialog::chooseDisplayBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColor color = colorDialog(button, ST_BG_COLOR);
        _mainWin->updateAllViewBackgroundColors(color.rgb());
    }
}

void
Settings_Dialog::currentDisplayBackgroundColorChanged(const QColor& color)
{
    st_preview[ST_BG_COLOR].u = color.rgb();
    _mainWin->updateAllViewBackgroundColors(st_preview[ST_BG_COLOR].u);
}

void
Settings_Dialog::chooseDisplaySelectBoxLeftColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(st_accept[ST_SELECTBOX_LEFT_COLOR].u), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            st_accept[ST_SELECTBOX_LEFT_COLOR].u = colorDialog->selectedColor().rgb();
            color_swatch(button, st_accept[ST_SELECTBOX_LEFT_COLOR].u);
            _mainWin->updateAllViewSelectBoxColors(
                st_accept[ST_SELECTBOX_LEFT_COLOR].u,
                st_accept[ST_SELECTBOX_LEFT_FILL].u,
                st_accept[ST_SELECTBOX_RIGHT_COLOR].u,
                st_accept[ST_SELECTBOX_RIGHT_FILL].u,
                st_preview[ST_SELECTBOX_ALPHA].i);
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                st_dialog[ST_SELECTBOX_LEFT_COLOR].u,
                st_dialog[ST_SELECTBOX_LEFT_FILL].u,
                st_dialog[ST_SELECTBOX_RIGHT_COLOR].u,
                st_dialog[ST_SELECTBOX_RIGHT_FILL].u,
                st_preview[ST_SELECTBOX_ALPHA].i);
        }
    }
}

void
Settings_Dialog::currentDisplaySelectBoxLeftColorChanged(const QColor& color)
{
    st_preview[ST_SELECTBOX_LEFT_COLOR].u = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(
        st_preview[ST_SELECTBOX_LEFT_COLOR].u,
        st_preview[ST_SELECTBOX_LEFT_FILL].u,
        st_preview[ST_SELECTBOX_RIGHT_COLOR].u,
        st_preview[ST_SELECTBOX_RIGHT_FILL].u,
        st_preview[ST_SELECTBOX_ALPHA].i);
}

void
Settings_Dialog::chooseDisplaySelectBoxLeftFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button == NULL) {
        return;
    }

    QColorDialog* colorDialog = new QColorDialog(QColor(st_accept[ST_SELECTBOX_LEFT_FILL].u), this);
    connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftFillChanged(const QColor&)));
    colorDialog->exec();

    if (colorDialog->result() == QDialog::Accepted) {
        st_accept[ST_SELECTBOX_LEFT_FILL].u = colorDialog->selectedColor().rgb();
        color_swatch(button, st_accept[ST_SELECTBOX_LEFT_FILL].u);
        _mainWin->updateAllViewSelectBoxColors(st_accept[ST_SELECTBOX_LEFT_COLOR].u,
                                              st_accept[ST_SELECTBOX_LEFT_FILL].u,
                                              st_accept[ST_SELECTBOX_RIGHT_COLOR].u,
                                              st_accept[ST_SELECTBOX_RIGHT_FILL].u,
                                              st_preview[ST_SELECTBOX_ALPHA].i);
    }
    else {
        _mainWin->updateAllViewSelectBoxColors(
            st_dialog[ST_SELECTBOX_LEFT_COLOR].u,
            st_dialog[ST_SELECTBOX_LEFT_FILL].u,
            st_dialog[ST_SELECTBOX_RIGHT_COLOR].u,
            st_dialog[ST_SELECTBOX_RIGHT_FILL].u,
            st_preview[ST_SELECTBOX_ALPHA].i);
    }
}

void
Settings_Dialog::currentDisplaySelectBoxLeftFillChanged(const QColor& color)
{
    st_preview[ST_SELECTBOX_LEFT_FILL].u = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(
        st_preview[ST_SELECTBOX_LEFT_COLOR].u,
        st_preview[ST_SELECTBOX_LEFT_FILL].u,
        st_preview[ST_SELECTBOX_RIGHT_COLOR].u,
        st_preview[ST_SELECTBOX_RIGHT_FILL].u,
        st_preview[ST_SELECTBOX_ALPHA].i);
}

void
Settings_Dialog::chooseDisplaySelectBoxRightColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(st_accept[ST_SELECTBOX_RIGHT_COLOR].u), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this,
            SLOT(currentDisplaySelectBoxRightColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            st_accept[ST_SELECTBOX_RIGHT_COLOR].u = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(st_accept[ST_SELECTBOX_RIGHT_COLOR].u));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(st_accept[ST_SELECTBOX_LEFT_COLOR].u,
                                                  st_accept[ST_SELECTBOX_LEFT_FILL].u,
                                                  st_accept[ST_SELECTBOX_RIGHT_COLOR].u,
                                                  st_accept[ST_SELECTBOX_RIGHT_FILL].u,
                                                  st_preview[ST_SELECTBOX_ALPHA].i);
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                st_dialog[ST_SELECTBOX_LEFT_COLOR].u,
                st_dialog[ST_SELECTBOX_LEFT_FILL].u,
                st_dialog[ST_SELECTBOX_RIGHT_COLOR].u,
                st_dialog[ST_SELECTBOX_RIGHT_FILL].u,
                st_preview[ST_SELECTBOX_ALPHA].i);
        }
    }
}

void
Settings_Dialog::currentDisplaySelectBoxRightColorChanged(const QColor& color)
{
    st_preview[ST_SELECTBOX_RIGHT_COLOR].u = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(st_preview[ST_SELECTBOX_LEFT_COLOR].u,
                                          st_preview[ST_SELECTBOX_LEFT_FILL].u,
                                          st_preview[ST_SELECTBOX_RIGHT_COLOR].u,
                                          st_preview[ST_SELECTBOX_RIGHT_FILL].u,
                                          st_preview[ST_SELECTBOX_ALPHA].i);
}

void
Settings_Dialog::chooseDisplaySelectBoxRightFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(st_dialog[ST_SELECTBOX_RIGHT_FILL].u), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this,
            SLOT(currentDisplaySelectBoxRightFillChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            st_accept[ST_SELECTBOX_RIGHT_FILL].u = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(st_accept[ST_SELECTBOX_RIGHT_FILL].u));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(st_accept[ST_SELECTBOX_LEFT_COLOR].u,
                                                  st_accept[ST_SELECTBOX_LEFT_FILL].u,
                                                  st_accept[ST_SELECTBOX_RIGHT_COLOR].u,
                                                  st_accept[ST_SELECTBOX_RIGHT_FILL].u,
                                                  st_preview[ST_SELECTBOX_ALPHA].i);
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                st_dialog[ST_SELECTBOX_LEFT_COLOR].u,
                st_dialog[ST_SELECTBOX_LEFT_FILL].u,
                st_dialog[ST_SELECTBOX_RIGHT_COLOR].u,
                st_dialog[ST_SELECTBOX_RIGHT_FILL].u,
                st_preview[ST_SELECTBOX_ALPHA].i);
        }
    }
}

void
Settings_Dialog::currentDisplaySelectBoxRightFillChanged(const QColor& color)
{
    st_preview[ST_SELECTBOX_RIGHT_FILL].u = color.rgb();
    _mainWin->updateAllViewSelectBoxColors(st_preview[ST_SELECTBOX_LEFT_COLOR].u,
                                          st_preview[ST_SELECTBOX_LEFT_FILL].u,
                                          st_preview[ST_SELECTBOX_RIGHT_COLOR].u,
                                          st_preview[ST_SELECTBOX_RIGHT_FILL].u,
                                          st_preview[ST_SELECTBOX_ALPHA].i);
}

void
Settings_Dialog::spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    st_preview[ST_SELECTBOX_ALPHA].i = value;
    _mainWin->updateAllViewSelectBoxColors(st_accept[ST_SELECTBOX_LEFT_COLOR].u,
                                          st_accept[ST_SELECTBOX_LEFT_FILL].u,
                                          st_accept[ST_SELECTBOX_RIGHT_COLOR].u,
                                          st_accept[ST_SELECTBOX_RIGHT_FILL].u,
                                          st_preview[ST_SELECTBOX_ALPHA].i);
}

void
Settings_Dialog::choosePromptTextColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColor color = colorDialog(button, ST_PROMPT_TEXT_COLOR);
        prompt->setPromptTextColor(color);
    }
}

void
Settings_Dialog::currentPromptTextColorChanged(const QColor& color)
{
    st_preview[ST_PROMPT_TEXT_COLOR].u = color.rgb();
    prompt->setPromptTextColor(QColor(st_preview[ST_PROMPT_TEXT_COLOR].u));
}

void
Settings_Dialog::choosePromptBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColor color = colorDialog(button, ST_PROMPT_BG_COLOR);
        prompt->setPromptTextColor(color);
    }
}

void
Settings_Dialog::currentPromptBackgroundColorChanged(const QColor& color)
{
    st_preview[ST_PROMPT_BG_COLOR].u = color.rgb();
    prompt->setPromptBackgroundColor(QColor(st_preview[ST_PROMPT_BG_COLOR].u));
}

void
Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(const QString& family)
{
    strcpy(st_preview[ST_PROMPT_FONT_FAMILY].s, qPrintable(family));
    prompt->setPromptFontFamily(st_preview[ST_PROMPT_FONT_FAMILY].s);
}

void
Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(const QString& style)
{
    strcpy(st_preview[ST_PROMPT_FONT_STYLE].s, qPrintable(style));
    prompt->setPromptFontStyle(st_preview[ST_PROMPT_FONT_STYLE].s);
}

void
Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    st_preview[ST_PROMPT_FONT_SIZE].i = value;
    prompt->setPromptFontSize(st_preview[ST_PROMPT_FONT_SIZE].i);
}

void
Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        QString format = checkBox->text();
        debug("CustomFilter: %s %d", qPrintable(format), checked);
        QString opensave_custom_filter = st_dialog[ST_OPENSAVE_CUSTOM_FILTER].s;
        if (checked) {
            strcpy(st_dialog[ST_OPENSAVE_CUSTOM_FILTER].s,
                qPrintable(opensave_custom_filter.append(" *." + format.toLower())));
        }
        else {
            strcpy(st_dialog[ST_OPENSAVE_CUSTOM_FILTER].s,
                qPrintable(opensave_custom_filter.remove("*." + format, Qt::CaseInsensitive)));
        }
        /* TODO */
        //st_dialog[ST_OPENSAVE_CUSTOM_FILTER].s = checked;
    }
}

void
Settings_Dialog::buttonCustomFilterSelectAllClicked()
{
    emit buttonCustomFilterSelectAll(true);
    strcpy(st_dialog[ST_OPENSAVE_CUSTOM_FILTER].s, "supported");
}

void
Settings_Dialog::buttonCustomFilterClearAllClicked()
{
    emit buttonCustomFilterClearAll(false);
    st_dialog[ST_OPENSAVE_CUSTOM_FILTER].s[0] = 0;
}

void
Settings_Dialog::spinBoxRecentMaxFilesValueChanged(int value)
{
    st_dialog[ST_RECENT_MAX_FILES].i = value;
}

void
Settings_Dialog::spinBoxTrimDstNumJumpsValueChanged(int value)
{
    st_dialog[ST_TRIM_DST_NUM_JUMPS].i = value;
}

void
Settings_Dialog::checkBoxGridShowOnLoadStateChanged(int checked)
{
    st_dialog[ST_GRID_SHOW_ON_LOAD].b = checked;
}

void
Settings_Dialog::checkBoxGridShowOriginStateChanged(int checked)
{
    st_dialog[ST_GRID_SHOW_ORIGIN].b = checked;
}

void
Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b = checked;
    if (st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b) {
        _mainWin->updateAllViewGridColors(st_accept[ST_CROSSHAIR_COLOR].u);
    }
    else {
        _mainWin->updateAllViewGridColors(st_accept[ST_GRID_COLOR].u);
    }

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QLabel* labelGridColor = parent->findChild<QLabel*>("labelGridColor");
            if (labelGridColor) labelGridColor->setEnabled(!st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b);
            QPushButton* buttonGridColor = parent->findChild<QPushButton*>("buttonGridColor");
            if (buttonGridColor) buttonGridColor->setEnabled(!st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b);
        }
    }
}

void
Settings_Dialog::chooseGridColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColor color = colorDialog(button, ST_GRID_COLOR);
        _mainWin->updateAllViewGridColors(color.rgb());
    }
}

void
Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    st_preview[ST_GRID_COLOR].u = color.rgb();
    _mainWin->updateAllViewGridColors(st_preview[ST_GRID_COLOR].u);
}

void
Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    st_dialog[ST_GRID_LOAD_FROM_FILE].b = checked;

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QLabel* labelGridType = parent->findChild<QLabel*>("labelGridType");
            if (labelGridType) labelGridType->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QComboBox* comboBoxGridType = parent->findChild<QComboBox*>("comboBoxGridType");
            if (comboBoxGridType) comboBoxGridType->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QCheckBox* checkBoxGridCenterOnOrigin = parent->findChild<QCheckBox*>("checkBoxGridCenterOnOrigin");
            if (checkBoxGridCenterOnOrigin) checkBoxGridCenterOnOrigin->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
            if (labelGridCenterX) labelGridCenterX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b && !st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
            QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
            if (spinBoxGridCenterX) spinBoxGridCenterX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b && !st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
            QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
            if (labelGridCenterY) labelGridCenterY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b && !st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
            QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
            if (spinBoxGridCenterY) spinBoxGridCenterY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b && !st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
            QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
            if (labelGridSizeX) labelGridSizeX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
            if (spinBoxGridSizeX) spinBoxGridSizeX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
            if (labelGridSizeY) labelGridSizeY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
            if (spinBoxGridSizeY) spinBoxGridSizeY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
            if (labelGridSpacingX) labelGridSpacingX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QDoubleSpinBox* spinBoxGridSpacingX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingX");
            if (spinBoxGridSpacingX) spinBoxGridSpacingX->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QLabel* labelGridSpacingY = parent->findChild<QLabel*>("labelGridSpacingY");
            if (labelGridSpacingY) labelGridSpacingY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QDoubleSpinBox* spinBoxGridSpacingY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingY");
            if (spinBoxGridSpacingY) spinBoxGridSpacingY->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QLabel* labelGridSizeRadius = parent->findChild<QLabel*>("labelGridSizeRadius");
            if (labelGridSizeRadius) labelGridSizeRadius->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QDoubleSpinBox* spinBoxGridSizeRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeRadius");
            if (spinBoxGridSizeRadius) spinBoxGridSizeRadius->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QLabel* labelGridSpacingRadius = parent->findChild<QLabel*>("labelGridSpacingRadius");
            if (labelGridSpacingRadius) labelGridSpacingRadius->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QDoubleSpinBox* spinBoxGridSpacingRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingRadius");
            if (spinBoxGridSpacingRadius) spinBoxGridSpacingRadius->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QLabel* labelGridSpacingAngle = parent->findChild<QLabel*>("labelGridSpacingAngle");
            if (labelGridSpacingAngle) labelGridSpacingAngle->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
            QDoubleSpinBox* spinBoxGridSpacingAngle = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingAngle");
            if (spinBoxGridSpacingAngle) spinBoxGridSpacingAngle->setEnabled(!st_dialog[ST_GRID_LOAD_FROM_FILE].b);
        }
    }
}

void
Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(const QString& type)
{
    strcpy(st_dialog[ST_GRID_TYPE].s, qPrintable(type));

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

void
Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    st_dialog[ST_GRID_CENTER_ON_ORIGIN].b = checked;

    QObject* senderObj = sender();
    if (senderObj == NULL) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (parent == NULL) {
        return;
    }

    QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
    if (labelGridCenterX) labelGridCenterX->setEnabled(!st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
    QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
    if (spinBoxGridCenterX) spinBoxGridCenterX->setEnabled(!st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
    QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
    if (labelGridCenterY) labelGridCenterY->setEnabled(!st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
    QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
    if (spinBoxGridCenterY) spinBoxGridCenterY->setEnabled(!st_dialog[ST_GRID_CENTER_ON_ORIGIN].b);
}

void
Settings_Dialog::checkBoxRulerShowOnLoadStateChanged(int checked)
{
    st_dialog[ST_RULER_SHOW_ON_LOAD].b = checked;
}

void
Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        st_dialog[ST_RULER_METRIC].b = comboBox->itemData(index).toBool();
    }
    else {
        st_dialog[ST_RULER_METRIC].b = true;
    }
}

void
Settings_Dialog::chooseRulerColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColor color = colorDialog(button, ST_RULER_COLOR);
        _mainWin->updateAllViewRulerColors(color.rgb());
    }
}

void
Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    st_preview[ST_RULER_COLOR].u = color.rgb();
    _mainWin->updateAllViewRulerColors(st_preview[ST_RULER_COLOR].u);
}

void
Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    st_dialog[ST_RULER_PIXEL_SIZE].i = value;
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

void
Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,255,0); //Yellow
    if (comboBox)
    {
        bool ok = 0;
        st_dialog[ST_QSNAP_LOCATOR_COLOR].u = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            st_dialog[ST_QSNAP_LOCATOR_COLOR].u = defaultColor;
        }
    }
    else {
        st_dialog[ST_QSNAP_LOCATOR_COLOR].u = defaultColor;
    }
}

void
Settings_Dialog::sliderQSnapLocatorSizeValueChanged(int value)
{
    st_dialog[ST_QSNAP_LOCATOR_SIZE].i = value;
}

void
Settings_Dialog::sliderQSnapApertureSizeValueChanged(int value)
{
    st_dialog[ST_QSNAP_APERTURE_SIZE].i = value;
}

void
Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    st_preview[ST_LWT_SHOW_LWT].b = checked;
    if (st_preview[ST_LWT_SHOW_LWT].b) {
        statusbar->statusBarLwtButton->enableLwt();
    }
    else {
        statusbar->statusBarLwtButton->disableLwt();
    }

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
            if (checkBoxRealRender) checkBoxRealRender->setEnabled(st_preview[ST_LWT_SHOW_LWT].b);
        }
    }
}

void
Settings_Dialog::checkBoxLwtRealRenderStateChanged(int checked)
{
    st_preview[ST_LWT_REAL_RENDER].b = checked;
    if (st_preview[ST_LWT_REAL_RENDER].b) {
        statusbar->statusBarLwtButton->enableReal();
    }
    else {
        statusbar->statusBarLwtButton->disableReal();
    }
}

void
Settings_Dialog::checkBoxSelectionModePickFirstStateChanged(int checked)
{
    st_dialog[ST_SELECTION_MODE_PICKFIRST].b = checked;
}

void
Settings_Dialog::checkBoxSelectionModePickAddStateChanged(int checked)
{
    st_dialog[ST_SELECTION_MODE_PICKADD].b = checked;
}

void
Settings_Dialog::checkBoxSelectionModePickDragStateChanged(int checked)
{
    st_dialog[ST_SELECTION_MODE_PICKDRAG].b = checked;
}

void
Settings_Dialog::sliderSelectionGripSizeValueChanged(int value)
{
    st_dialog[ST_SELECTION_GRIP_SIZE].i = value;
}

void
Settings_Dialog::sliderSelectionPickBoxSizeValueChanged(int value)
{
    st_dialog[ST_SELECTION_PICKBOX_SIZE].i = value;
}

void
Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(0,0,255); //Blue
    if (comboBox) {
        bool ok = 0;
        st_dialog[ST_SELECTION_COOLGRIP_COLOR].u = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            st_dialog[ST_SELECTION_COOLGRIP_COLOR].u = defaultColor;
        }
    }
    else {
        st_dialog[ST_SELECTION_COOLGRIP_COLOR].u = defaultColor;
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
        st_dialog[ST_SELECTION_HOTGRIP_COLOR].u = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            st_dialog[ST_SELECTION_HOTGRIP_COLOR].u = defaultColor;
        }
    }
    else {
        st_dialog[ST_SELECTION_HOTGRIP_COLOR].u = defaultColor;
    }
}

/* Make sure the user sees the changes applied immediately. */
void
Settings_Dialog::update_interface(void)
{
    mdiArea->useBackgroundLogo(st_dialog[ST_MDI_BG_USE_LOGO].b);
    mdiArea->useBackgroundTexture(st_dialog[ST_MDI_BG_USE_TEXTURE].b);
    mdiArea->useBackgroundColor(st_dialog[ST_MDI_BG_USE_COLOR].b);
    mdiArea->setBackgroundLogo(st_dialog[ST_MDI_BG_LOGO].s);
    mdiArea->setBackgroundTexture(st_dialog[ST_MDI_BG_TEXTURE].s);
    mdiArea->setBackgroundColor(st_dialog[ST_MDI_BG_COLOR].u);

    _mainWin->iconResize(st_dialog[ST_ICON_SIZE].i);

    _mainWin->updateAllViewScrollBars(st_dialog[ST_SHOW_SCROLLBARS].b);
    _mainWin->updateAllViewCrossHairColors(st_dialog[ST_CROSSHAIR_COLOR].u);
    _mainWin->updateAllViewBackgroundColors(st_dialog[ST_BG_COLOR].u);
    _mainWin->updateAllViewSelectBoxColors(
        st_dialog[ST_SELECTBOX_LEFT_COLOR].u,
        st_dialog[ST_SELECTBOX_LEFT_FILL].u,
        st_dialog[ST_SELECTBOX_RIGHT_COLOR].u,
        st_dialog[ST_SELECTBOX_RIGHT_FILL].u,
        st_dialog[ST_SELECTBOX_ALPHA].i);

    prompt->setPromptTextColor(QColor(st_dialog[ST_PROMPT_TEXT_COLOR].u));
    prompt->setPromptBackgroundColor(QColor(st_dialog[ST_PROMPT_BG_COLOR].s));
    prompt->setPromptFontFamily(st_dialog[ST_PROMPT_FONT_FAMILY].s);
    prompt->setPromptFontStyle(st_dialog[ST_PROMPT_FONT_STYLE].s);
    prompt->setPromptFontSize(st_dialog[ST_PROMPT_FONT_SIZE].i);

    _mainWin->updateAllViewGridColors(st_dialog[ST_GRID_COLOR].u);
    _mainWin->updateAllViewRulerColors(st_dialog[ST_RULER_COLOR].u);

    if (st_dialog[ST_LWT_SHOW_LWT].b) {
        statusbar->statusBarLwtButton->enableLwt();
    }
    else {
        statusbar->statusBarLwtButton->disableLwt();
    }
    if (st_dialog[ST_LWT_REAL_RENDER].b) {
        statusbar->statusBarLwtButton->enableReal();
    }
    else {
        statusbar->statusBarLwtButton->disableReal();
    }
    _mainWin->updatePickAddMode(st_dialog[ST_SELECTION_MODE_PICKADD].b);
}

/* Update the interface settings to the dialog state. */
void
Settings_Dialog::acceptChanges(void)
{
    st_dialog[ST_MDI_BG_USE_LOGO].b = st_preview[ST_MDI_BG_USE_LOGO].b;
    st_dialog[ST_MDI_BG_USE_TEXTURE].b = st_preview[ST_MDI_BG_USE_TEXTURE].b;
    st_dialog[ST_MDI_BG_USE_COLOR].b = st_preview[ST_MDI_BG_USE_COLOR].b;
    memcpy(st_dialog + ST_MDI_BG_LOGO, st_accept + ST_MDI_BG_LOGO, sizeof(ScriptValue));
    memcpy(st_dialog + ST_MDI_BG_TEXTURE, st_accept + ST_MDI_BG_TEXTURE, sizeof(ScriptValue));
    st_dialog[ST_MDI_BG_COLOR].u = st_accept[ST_MDI_BG_COLOR].u;
    st_dialog[ST_SHOW_SCROLLBARS].b = st_preview[ST_SHOW_SCROLLBARS].b;
    st_dialog[ST_CROSSHAIR_COLOR].u = st_accept[ST_CROSSHAIR_COLOR].u;
    st_dialog[ST_BG_COLOR].u = st_accept[ST_BG_COLOR].u;
    st_dialog[ST_SELECTBOX_LEFT_COLOR].u = st_accept[ST_SELECTBOX_LEFT_COLOR].u;
    st_dialog[ST_SELECTBOX_LEFT_FILL].u = st_accept[ST_SELECTBOX_LEFT_FILL].u;
    st_dialog[ST_SELECTBOX_RIGHT_COLOR].u = st_accept[ST_SELECTBOX_RIGHT_COLOR].u;
    st_dialog[ST_SELECTBOX_RIGHT_FILL].u = st_accept[ST_SELECTBOX_RIGHT_FILL].u;
    st_dialog[ST_SELECTBOX_ALPHA].i = st_preview[ST_SELECTBOX_ALPHA].i;
    st_dialog[ST_PROMPT_TEXT_COLOR].u = st_accept[ST_PROMPT_TEXT_COLOR].u;
    st_dialog[ST_PROMPT_BG_COLOR].u = st_accept[ST_PROMPT_BG_COLOR].u;
    memcpy(st_dialog + ST_PROMPT_FONT_FAMILY, st_preview + ST_PROMPT_FONT_FAMILY,
        sizeof(ScriptValue));
    memcpy(st_dialog + ST_PROMPT_FONT_STYLE, st_preview + ST_PROMPT_FONT_STYLE,
        sizeof(ScriptValue));
    st_dialog[ST_PROMPT_FONT_SIZE].i = st_preview[ST_PROMPT_FONT_SIZE].i;
    if (st_dialog[ST_GRID_COLOR_MATCH_CROSSHAIR].b) {
        st_dialog[ST_GRID_COLOR].u = st_accept[ST_CROSSHAIR_COLOR].u;
    }
    else {
        st_dialog[ST_GRID_COLOR].u = st_accept[ST_GRID_COLOR].u;
    }
    st_dialog[ST_RULER_COLOR].u = st_accept[ST_RULER_COLOR].u;
    st_dialog[ST_LWT_SHOW_LWT].b = st_preview[ST_LWT_SHOW_LWT].b;
    st_dialog[ST_LWT_REAL_RENDER].b = st_preview[ST_LWT_REAL_RENDER].b;

    memcpy(st, st_dialog, sizeof(st_dialog));

    update_interface();

    _mainWin->writeSettings();
    accept();
}

/* Reset all fo the interface settings to ensure everything is displayed as before. */
void
Settings_Dialog::rejectChanges(void)
{
    //TODO: inform the user if they have changed settings

    update_interface();

    reject();
}

