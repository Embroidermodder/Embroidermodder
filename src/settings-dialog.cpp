#include <QtGui>

#include "settings-dialog.h"
#include "object-data.h"
#include "statusbar.h"

#include <QStandardPaths>

/* Temporary until changes are accepted. */
Settings dialog;
Settings accept_;
/* Temporary for instant preview. */
Settings preview;

QTabWidget* tabWidget;
QDialogButtonBox* buttonBox;

Settings_Dialog::Settings_Dialog(MainWindow* mw, const QString& showTab, QWidget* parent) : QDialog(parent)
{
    mainWin = mw;
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

QWidget* Settings_Dialog::createTabGeneral()
{
    QWidget* widget = new QWidget(this);

    //Language
    QGroupBox* groupBoxLanguage = new QGroupBox(tr("Language"), widget);

    QLabel* labelLanguage = new QLabel(tr("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    dialog.general_language = settings.general_language.toLower();
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
    QString current = dialog.general_language;
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
    dialog.general_icon_theme = settings.general_icon_theme;
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/" + "theme" + ".png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(dialog.general_icon_theme));
    connect(comboBoxIconTheme, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxIconThemeCurrentIndexChanged(const QString&)));

    QLabel* labelIconSize = new QLabel(tr("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    comboBoxIconSize->addItem(QIcon("icons/" + dialog.general_icon_theme + "/" + "icon16"  + ".png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon("icons/" + dialog.general_icon_theme + "/" + "icon24"  + ".png"), "Small",      24);
    comboBoxIconSize->addItem(QIcon("icons/" + dialog.general_icon_theme + "/" + "icon32"  + ".png"), "Medium",     32);
    comboBoxIconSize->addItem(QIcon("icons/" + dialog.general_icon_theme + "/" + "icon48"  + ".png"), "Large",      48);
    comboBoxIconSize->addItem(QIcon("icons/" + dialog.general_icon_theme + "/" + "icon64"  + ".png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon("icons/" + dialog.general_icon_theme + "/" + "icon128" + ".png"), "I'm Blind", 128);
    dialog.general_icon_size = settings.general_icon_size;
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(dialog.general_icon_size));
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
    dialog.general_mdi_bg_use_logo = settings.general_mdi_bg_use_logo;
    preview.general_mdi_bg_use_logo = dialog.general_mdi_bg_use_logo;
    checkBoxMdiBGUseLogo->setChecked(preview.general_mdi_bg_use_logo);
    connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)));

    QPushButton* buttonMdiBGLogo = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(dialog.general_mdi_bg_use_logo);
    dialog.general_mdi_bg_logo = settings.general_mdi_bg_logo;
    accept_.general_mdi_bg_logo = dialog.general_mdi_bg_logo;
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseTexture = new QCheckBox(tr("Use Texture"), groupBoxMdiBG);
    dialog.general_mdi_bg_use_texture = settings.general_mdi_bg_use_texture;
    preview.general_mdi_bg_use_texture = dialog.general_mdi_bg_use_texture;
    checkBoxMdiBGUseTexture->setChecked(preview.general_mdi_bg_use_texture);
    connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)));

    QPushButton* buttonMdiBGTexture = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(dialog.general_mdi_bg_use_texture);
    dialog.general_mdi_bg_texture = settings.general_mdi_bg_texture;
    accept_.general_mdi_bg_texture = dialog.general_mdi_bg_texture;
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseColor = new QCheckBox(tr("Use Color"), groupBoxMdiBG);
    dialog.general_mdi_bg_use_color = settings.general_mdi_bg_use_color;
    preview.general_mdi_bg_use_color = dialog.general_mdi_bg_use_color;
    checkBoxMdiBGUseColor->setChecked(preview.general_mdi_bg_use_color);
    connect(checkBoxMdiBGUseColor, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseColorStateChanged(int)));

    QPushButton* buttonMdiBGColor = new QPushButton(tr("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setEnabled(dialog.general_mdi_bg_use_color);
    dialog.general_mdi_bg_color = settings.general_mdi_bg_color;
    preview.general_mdi_bg_color = dialog.general_mdi_bg_color;
    accept_.general_mdi_bg_color = dialog.general_mdi_bg_color;
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
    dialog.general_tip_of_the_day = settings.general_tip_of_the_day;
    checkBoxTipOfTheDay->setChecked(dialog.general_tip_of_the_day);
    connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)));

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    //Help Browser
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(tr("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(tr("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(settings.general_system_help_browser);
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(tr("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!settings.general_system_help_browser);
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
    /*
    QGroupBox* groupBoxRender = new QGroupBox(tr("Rendering"), widget);

    QCheckBox* checkBoxUseOpenGL = new QCheckBox(tr("Use OpenGL"), groupBoxRender);
    dialog.display_use_opengl = settings.display_use_opengl;
    checkBoxUseOpenGL->setChecked(dialog.display_use_opengl);
    connect(checkBoxUseOpenGL, SIGNAL(stateChanged(int)), this, SLOT(checkBoxUseOpenGLStateChanged(int)));

    QCheckBox* checkBoxRenderHintAA = new QCheckBox(tr("Antialias"), groupBoxRender);
    dialog.display_renderhint_aa = settings.display_render_hint_aa;
    checkBoxRenderHintAA->setChecked(dialog.display_renderhint_aa);
    connect(checkBoxRenderHintAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintTextAA = new QCheckBox(tr("Antialias Text"), groupBoxRender);
    dialog.display_renderhint_text_aa = settings.display_renderhint_text_aa;
    checkBoxRenderHintTextAA->setChecked(dialog.display_renderhint_text_aa);
    connect(checkBoxRenderHintTextAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintTextAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintSmoothPix = new QCheckBox(tr("Smooth Pixmap"), groupBoxRender);
    dialog.display_renderhint_smooth_pix = settingsDisplayRenderHintSmoothPix();
    checkBoxRenderHintSmoothPix->setChecked(dialog.display_renderhint_smooth_pix);
    connect(checkBoxRenderHintSmoothPix, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintSmoothPixStateChanged(int)));

    QCheckBox* checkBoxRenderHintHighAA = new QCheckBox(tr("High Quality Antialiasing (OpenGL)"), groupBoxRender);
    dialog.display_renderhint_high_aa = settingsDisplayRenderHintHighAA();
    checkBoxRenderHintHighAA->setChecked(dialog.display_renderhint_high_aa);
    connect(checkBoxRenderHintHighAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintHighAAStateChanged(int)));

    QCheckBox* checkBoxRenderHintNonCosmetic = new QCheckBox(tr("Non Cosmetic"), groupBoxRender);
    dialog.display_renderhint_noncosmetic = settingsDisplayRenderHintNonCosmetic();
    checkBoxRenderHintNonCosmetic->setChecked(dialog.display_renderhint_noncosmetic);
    connect(checkBoxRenderHintNonCosmetic, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintNonCosmeticStateChanged(int)));

    QVBoxLayout* vboxLayoutRender = new QVBoxLayout(groupBoxRender);
    vboxLayoutRender->addWidget(checkBoxUseOpenGL);
    vboxLayoutRender->addWidget(checkBoxRenderHintAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintTextAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintSmoothPix);
    vboxLayoutRender->addWidget(checkBoxRenderHintHighAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintNonCosmetic);
    groupBoxRender->setLayout(vboxLayoutRender);
    */

    //ScrollBars
    QGroupBox* groupBoxScrollBars = new QGroupBox(tr("ScrollBars"), widget);

    QCheckBox* checkBoxShowScrollBars = new QCheckBox(tr("Show ScrollBars"), groupBoxScrollBars);
    dialog.display_show_scrollbars = settings.display_show_scrollbars;
    preview.display_show_scrollbars = dialog.display_show_scrollbars;
    checkBoxShowScrollBars->setChecked(preview.display_show_scrollbars);
    connect(checkBoxShowScrollBars, SIGNAL(stateChanged(int)), this, SLOT(checkBoxShowScrollBarsStateChanged(int)));

    QLabel* labelScrollBarWidget = new QLabel(tr("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    dialog.display_scrollbar_widget_num = settings.display_scrollbar_widget_num;
    int numActions = mainWin->actionHash.size();
    for(int i = 0; i < numActions; i++)
    {
    /* FIXME:
        QAction* action = mainWin->actionHash.value(i);
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
    dialog.display_crosshair_color = settings.display_crosshair_color;
    preview.display_crosshair_color = dialog.display_crosshair_color;
    accept_.display_crosshair_color = dialog.display_crosshair_color;
    QPixmap crosshairPix(16,16);
    crosshairPix.fill(QColor(preview.display_crosshair_color));
    buttonCrossHairColor->setIcon(QIcon(crosshairPix));
    connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_bg_color = settings.display_bg_color;
    preview.display_bg_color = dialog.display_bg_color;
    accept_.display_bg_color = dialog.display_bg_color;
    QPixmap bgPix(16,16);
    bgPix.fill(QColor(preview.display_bg_color));
    buttonBGColor->setIcon(QIcon(bgPix));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()));

    QLabel* labelSelectBoxLeftColor = new QLabel(tr("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_selectbox_left_color = settings.display_selectbox_left_color;
    preview.display_selectbox_left_color = dialog.display_selectbox_left_color;
    accept_.display_selectbox_left_color = dialog.display_selectbox_left_color;
    QPixmap sBoxLCPix(16,16);
    sBoxLCPix.fill(QColor(preview.display_selectbox_left_color));
    buttonSelectBoxLeftColor->setIcon(QIcon(sBoxLCPix));
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()));

    QLabel* labelSelectBoxLeftFill = new QLabel(tr("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_selectbox_left_fill = settings.display_selectbox_left_fill;
    preview.display_selectbox_left_fill = dialog.display_selectbox_left_fill;
    accept_.display_selectbox_left_fill = dialog.display_selectbox_left_fill;
    QPixmap sBoxLFPix(16,16);
    sBoxLFPix.fill(QColor(preview.display_selectbox_left_fill));
    buttonSelectBoxLeftFill->setIcon(QIcon(sBoxLFPix));
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()));

    QLabel* labelSelectBoxRightColor = new QLabel(tr("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_selectbox_right_color = settings.display_selectbox_right_color;
    preview.display_selectbox_right_color = dialog.display_selectbox_right_color;
    accept_.display_selectbox_right_color  = dialog.display_selectbox_right_color;
    QPixmap sBoxRCPix(16,16);
    sBoxRCPix.fill(QColor(preview.display_selectbox_right_color));
    buttonSelectBoxRightColor->setIcon(QIcon(sBoxRCPix));
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()));

    QLabel* labelSelectBoxRightFill = new QLabel(tr("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.display_selectbox_right_fill  = settings.display_selectbox_right_fill;
    preview.display_selectbox_right_fill = dialog.display_selectbox_right_fill;
    accept_.display_selectbox_right_fill  = dialog.display_selectbox_right_fill;
    QPixmap sBoxRFPix(16,16);
    sBoxRFPix.fill(QColor(preview.display_selectbox_right_fill));
    buttonSelectBoxRightFill->setIcon(QIcon(sBoxRFPix));
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()));

    QLabel* labelSelectBoxAlpha = new QLabel(tr("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    dialog.display_selectbox_alpha = settings.display_selectbox_alpha;
    preview.display_selectbox_alpha = dialog.display_selectbox_alpha;
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
    QDoubleSpinBox* spinBoxZoomScaleIn = new QDoubleSpinBox(groupBoxZoom);
    dialog.display_zoomscale_in = settings.display_zoomscale_in;
    spinBoxZoomScaleIn->setValue(dialog.display_zoomscale_in);
    spinBoxZoomScaleIn->setSingleStep(0.01);
    spinBoxZoomScaleIn->setRange(1.01, 10.00);
    connect(spinBoxZoomScaleIn, SIGNAL(valueChanged(double)), this, SLOT(spinBoxZoomScaleInValueChanged(double)));

    QLabel* labelZoomScaleOut = new QLabel(tr("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = new QDoubleSpinBox(groupBoxZoom);
    dialog.display_zoomscale_out = settings.display_zoomscale_out;
    spinBoxZoomScaleOut->setValue(dialog.display_zoomscale_out);
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
    dialog.prompt_text_color = settings.prompt_text_color;
    preview.prompt_text_color = dialog.prompt_text_color;
    accept_.prompt_text_color  = dialog.prompt_text_color;
    QPixmap pix(16,16);
    pix.fill(QColor(preview.prompt_text_color));
    buttonTextColor->setIcon(QIcon(pix));
    connect(buttonTextColor, SIGNAL(clicked()), this, SLOT(choosePromptTextColor()));

    QLabel* labelBGColor = new QLabel(tr("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(tr("Choose"), groupBoxColor);
    dialog.prompt_bg_color = settings.prompt_bg_color;
    preview.prompt_bg_color = dialog.prompt_bg_color;
    accept_.prompt_bg_color  = dialog.prompt_bg_color;
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
    dialog.prompt_font_family = settings.prompt_font_family;
    preview.prompt_font_family = dialog.prompt_font_family;
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
    dialog.prompt_font_size = settings.prompt_font_size;
    preview.prompt_font_size = dialog.prompt_font_size;
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

    QCheckBox* checkBoxPromptSaveHistory = new QCheckBox(tr("Save History"), groupBoxHistory);
    dialog.prompt_save_history = settings.prompt_save_history;
    checkBoxPromptSaveHistory->setChecked(dialog.prompt_save_history);
    connect(checkBoxPromptSaveHistory, SIGNAL(stateChanged(int)), this, SLOT(checkBoxPromptSaveHistoryStateChanged(int)));

    QCheckBox* checkBoxPromptSaveHistoryAsHtml = new QCheckBox(tr("Save As HTML"), groupBoxHistory);
    dialog.prompt_save_history_as_html = settings.prompt_save_history_as_html;
    checkBoxPromptSaveHistoryAsHtml->setChecked(dialog.prompt_save_history_as_html);
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

    dialog.opensave_custom_filter = settings.opensave_custom_filter;

    QCheckBox* checkBoxCustomFilter100 = new QCheckBox("100", groupBoxCustomFilter);
    checkBoxCustomFilter100->setChecked(dialog.opensave_custom_filter.contains("*.100", Qt::CaseInsensitive));
    connect(checkBoxCustomFilter100, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilter10o = new QCheckBox("10o", groupBoxCustomFilter);
    checkBoxCustomFilter10o->setChecked(dialog.opensave_custom_filter.contains("*.10o", Qt::CaseInsensitive));
    connect(checkBoxCustomFilter10o, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterART = new QCheckBox("ART", groupBoxCustomFilter);
    checkBoxCustomFilterART->setChecked(dialog.opensave_custom_filter.contains("*.ART", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterART, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterBMC = new QCheckBox("BMC", groupBoxCustomFilter);
    checkBoxCustomFilterBMC->setChecked(dialog.opensave_custom_filter.contains("*.BMC", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterBMC, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterBRO = new QCheckBox("BRO", groupBoxCustomFilter);
    checkBoxCustomFilterBRO->setChecked(dialog.opensave_custom_filter.contains("*.BRO", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterBRO, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterCND = new QCheckBox("CND", groupBoxCustomFilter);
    checkBoxCustomFilterCND->setChecked(dialog.opensave_custom_filter.contains("*.CND", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterCND, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterCOL = new QCheckBox("COL", groupBoxCustomFilter);
    checkBoxCustomFilterCOL->setChecked(dialog.opensave_custom_filter.contains("*.COL", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterCOL, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterCSD = new QCheckBox("CSD", groupBoxCustomFilter);
    checkBoxCustomFilterCSD->setChecked(dialog.opensave_custom_filter.contains("*.CSD", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterCSD, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterCSV = new QCheckBox("CSV", groupBoxCustomFilter);
    checkBoxCustomFilterCSV->setChecked(dialog.opensave_custom_filter.contains("*.CSV", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterCSV, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterDAT = new QCheckBox("DAT", groupBoxCustomFilter);
    checkBoxCustomFilterDAT->setChecked(dialog.opensave_custom_filter.contains("*.DAT", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterDAT, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterDEM = new QCheckBox("DEM", groupBoxCustomFilter);
    checkBoxCustomFilterDEM->setChecked(dialog.opensave_custom_filter.contains("*.DEM", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterDEM, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterDSB = new QCheckBox("DSB", groupBoxCustomFilter);
    checkBoxCustomFilterDSB->setChecked(dialog.opensave_custom_filter.contains("*.DSB", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterDSB, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterDST = new QCheckBox("DST", groupBoxCustomFilter);
    checkBoxCustomFilterDST->setChecked(dialog.opensave_custom_filter.contains("*.DST", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterDST, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterDSZ = new QCheckBox("DSZ", groupBoxCustomFilter);
    checkBoxCustomFilterDSZ->setChecked(dialog.opensave_custom_filter.contains("*.DSZ", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterDSZ, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterDXF = new QCheckBox("DXF", groupBoxCustomFilter);
    checkBoxCustomFilterDXF->setChecked(dialog.opensave_custom_filter.contains("*.DXF", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterDXF, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterEDR = new QCheckBox("EDR", groupBoxCustomFilter);
    checkBoxCustomFilterEDR->setChecked(dialog.opensave_custom_filter.contains("*.EDR", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterEDR, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterEMD = new QCheckBox("EMD", groupBoxCustomFilter);
    checkBoxCustomFilterEMD->setChecked(dialog.opensave_custom_filter.contains("*.EMD", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterEMD, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterEXP = new QCheckBox("EXP", groupBoxCustomFilter);
    checkBoxCustomFilterEXP->setChecked(dialog.opensave_custom_filter.contains("*.EXP", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterEXP, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterEXY = new QCheckBox("EXY", groupBoxCustomFilter);
    checkBoxCustomFilterEXY->setChecked(dialog.opensave_custom_filter.contains("*.EXY", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterEXY, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterEYS = new QCheckBox("EYS", groupBoxCustomFilter);
    checkBoxCustomFilterEYS->setChecked(dialog.opensave_custom_filter.contains("*.EYS", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterEYS, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterFXY = new QCheckBox("FXY", groupBoxCustomFilter);
    checkBoxCustomFilterFXY->setChecked(dialog.opensave_custom_filter.contains("*.FXY", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterFXY, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterGNC = new QCheckBox("GNC", groupBoxCustomFilter);
    checkBoxCustomFilterGNC->setChecked(dialog.opensave_custom_filter.contains("*.GNC", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterGNC, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterGT = new QCheckBox("GT", groupBoxCustomFilter);
    checkBoxCustomFilterGT->setChecked(dialog.opensave_custom_filter.contains("*.GT", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterGT, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterHUS = new QCheckBox("HUS", groupBoxCustomFilter);
    checkBoxCustomFilterHUS->setChecked(dialog.opensave_custom_filter.contains("*.HUS", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterHUS, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterINB = new QCheckBox("INB", groupBoxCustomFilter);
    checkBoxCustomFilterINB->setChecked(dialog.opensave_custom_filter.contains("*.INB", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterINB, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterJEF = new QCheckBox("JEF", groupBoxCustomFilter);
    checkBoxCustomFilterJEF->setChecked(dialog.opensave_custom_filter.contains("*.JEF", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterJEF, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterKSM = new QCheckBox("KSM", groupBoxCustomFilter);
    checkBoxCustomFilterKSM->setChecked(dialog.opensave_custom_filter.contains("*.KSM", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterKSM, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPCD = new QCheckBox("PCD", groupBoxCustomFilter);
    checkBoxCustomFilterPCD->setChecked(dialog.opensave_custom_filter.contains("*.PCD", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPCD, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPCM = new QCheckBox("PCM", groupBoxCustomFilter);
    checkBoxCustomFilterPCM->setChecked(dialog.opensave_custom_filter.contains("*.PCM", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPCM, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPCQ = new QCheckBox("PCQ", groupBoxCustomFilter);
    checkBoxCustomFilterPCQ->setChecked(dialog.opensave_custom_filter.contains("*.PCQ", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPCQ, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPCS = new QCheckBox("PCS", groupBoxCustomFilter);
    checkBoxCustomFilterPCS->setChecked(dialog.opensave_custom_filter.contains("*.PCS", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPCS, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPEC = new QCheckBox("PEC", groupBoxCustomFilter);
    checkBoxCustomFilterPEC->setChecked(dialog.opensave_custom_filter.contains("*.PEC", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPEC, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPEL = new QCheckBox("PEL", groupBoxCustomFilter);
    checkBoxCustomFilterPEL->setChecked(dialog.opensave_custom_filter.contains("*.PEL", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPEL, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPEM = new QCheckBox("PEM", groupBoxCustomFilter);
    checkBoxCustomFilterPEM->setChecked(dialog.opensave_custom_filter.contains("*.PEM", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPEM, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPES = new QCheckBox("PES", groupBoxCustomFilter);
    checkBoxCustomFilterPES->setChecked(dialog.opensave_custom_filter.contains("*.PES", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPES, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPHB = new QCheckBox("PHB", groupBoxCustomFilter);
    checkBoxCustomFilterPHB->setChecked(dialog.opensave_custom_filter.contains("*.PHB", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPHB, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterPHC = new QCheckBox("PHC", groupBoxCustomFilter);
    checkBoxCustomFilterPHC->setChecked(dialog.opensave_custom_filter.contains("*.PHC", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterPHC, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterRGB = new QCheckBox("RGB", groupBoxCustomFilter);
    checkBoxCustomFilterRGB->setChecked(dialog.opensave_custom_filter.contains("*.RGB", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterRGB, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterSEW = new QCheckBox("SEW", groupBoxCustomFilter);
    checkBoxCustomFilterSEW->setChecked(dialog.opensave_custom_filter.contains("*.SEW", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterSEW, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterSHV = new QCheckBox("SHV", groupBoxCustomFilter);
    checkBoxCustomFilterSHV->setChecked(dialog.opensave_custom_filter.contains("*.SHV", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterSHV, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterSST = new QCheckBox("SST", groupBoxCustomFilter);
    checkBoxCustomFilterSST->setChecked(dialog.opensave_custom_filter.contains("*.SST", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterSST, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterSTX = new QCheckBox("STX", groupBoxCustomFilter);
    checkBoxCustomFilterSTX->setChecked(dialog.opensave_custom_filter.contains("*.STX", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterSTX, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterSVG = new QCheckBox("SVG", groupBoxCustomFilter);
    checkBoxCustomFilterSVG->setChecked(dialog.opensave_custom_filter.contains("*.SVG", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterSVG, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterT09 = new QCheckBox("T09", groupBoxCustomFilter);
    checkBoxCustomFilterT09->setChecked(dialog.opensave_custom_filter.contains("*.T09", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterT09, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterTAP = new QCheckBox("TAP", groupBoxCustomFilter);
    checkBoxCustomFilterTAP->setChecked(dialog.opensave_custom_filter.contains("*.TAP", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterTAP, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterTHR = new QCheckBox("THR", groupBoxCustomFilter);
    checkBoxCustomFilterTHR->setChecked(dialog.opensave_custom_filter.contains("*.THR", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterTHR, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterTXT = new QCheckBox("TXT", groupBoxCustomFilter);
    checkBoxCustomFilterTXT->setChecked(dialog.opensave_custom_filter.contains("*.TXT", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterTXT, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterU00 = new QCheckBox("U00", groupBoxCustomFilter);
    checkBoxCustomFilterU00->setChecked(dialog.opensave_custom_filter.contains("*.U00", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterU00, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterU01 = new QCheckBox("U01", groupBoxCustomFilter);
    checkBoxCustomFilterU01->setChecked(dialog.opensave_custom_filter.contains("*.U01", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterU01, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterVIP = new QCheckBox("VIP", groupBoxCustomFilter);
    checkBoxCustomFilterVIP->setChecked(dialog.opensave_custom_filter.contains("*.VIP", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterVIP, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterVP3 = new QCheckBox("VP3", groupBoxCustomFilter);
    checkBoxCustomFilterVP3->setChecked(dialog.opensave_custom_filter.contains("*.VP3", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterVP3, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterXXX = new QCheckBox("XXX", groupBoxCustomFilter);
    checkBoxCustomFilterXXX->setChecked(dialog.opensave_custom_filter.contains("*.XXX", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterXXX, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QCheckBox* checkBoxCustomFilterZSK = new QCheckBox("ZSK", groupBoxCustomFilter);
    checkBoxCustomFilterZSK->setChecked(dialog.opensave_custom_filter.contains("*.ZSK", Qt::CaseInsensitive));
    connect(checkBoxCustomFilterZSK, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(tr("Select All"), groupBoxCustomFilter);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilter100, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilter10o, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterART, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterBMC, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterBRO, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterCND, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterCOL, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterCSD, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterCSV, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterDAT, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterDEM, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterDSB, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterDST, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterDSZ, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterDXF, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterEDR, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterEMD, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterEXP, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterEXY, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterEYS, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterFXY, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterGNC, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterGT,  SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterHUS, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterINB, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterJEF, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterKSM, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPCD, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPCM, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPCQ, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPCS, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPEC, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPEL, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPEM, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPES, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPHB, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterPHC, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterRGB, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterSEW, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterSHV, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterSST, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterSTX, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterSVG, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterT09, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterTAP, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterTHR, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterTXT, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterU00, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterU01, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterVIP, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterVP3, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterXXX, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterSelectAll(bool)), checkBoxCustomFilterZSK, SLOT(setChecked(bool)));

    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", groupBoxCustomFilter);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilter100, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilter10o, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterART, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterBMC, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterBRO, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterCND, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterCOL, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterCSD, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterCSV, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterDAT, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterDEM, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterDSB, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterDST, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterDSZ, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterDXF, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterEDR, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterEMD, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterEXP, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterEXY, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterEYS, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterFXY, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterGNC, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterGT,  SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterHUS, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterINB, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterJEF, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterKSM, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPCD, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPCM, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPCQ, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPCS, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPEC, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPEL, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPEM, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPES, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPHB, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterPHC, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterRGB, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterSEW, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterSHV, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterSST, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterSTX, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterSVG, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterT09, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterTAP, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterTHR, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterTXT, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterU00, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterU01, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterVIP, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterVP3, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterXXX, SLOT(setChecked(bool)));
    connect(this, SIGNAL(buttonCustomFilterClearAll(bool)), checkBoxCustomFilterZSK, SLOT(setChecked(bool)));

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilter100,     0, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilter10o,     1, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterART,     2, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterBMC,     3, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterBRO,     4, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterCND,     5, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterCOL,     6, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterCSD,     7, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterCSV,     8, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterDAT,     9, 0, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterDEM,     0, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterDSB,     1, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterDST,     2, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterDSZ,     3, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterDXF,     4, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterEDR,     5, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterEMD,     6, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterEXP,     7, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterEXY,     8, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterEYS,     9, 1, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterFXY,     0, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterGNC,     1, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterGT,      2, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterHUS,     3, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterINB,     4, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterJEF,     5, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterKSM,     6, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPCD,     7, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPCM,     8, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPCQ,     9, 2, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPCS,     0, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPEC,     1, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPEL,     2, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPEM,     3, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPES,     4, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPHB,     5, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterPHC,     6, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterRGB,     7, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterSEW,     8, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterSHV,     9, 3, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterSST,     0, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterSTX,     1, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterSVG,     2, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterT09,     3, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterTAP,     4, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterTHR,     5, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterTXT,     6, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterU00,     7, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterU01,     8, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterVIP,     9, 4, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterVP3,     0, 5, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterXXX,     1, 5, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(checkBoxCustomFilterZSK,     2, 5, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterSelectAll, 0, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterClearAll,  1, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->setLayout(gridLayoutCustomFilter);

    if(dialog.opensave_custom_filter.contains("supported", Qt::CaseInsensitive)) buttonCustomFilterSelectAllClicked();

    //Opening
    QGroupBox* groupBoxOpening = new QGroupBox(tr("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(tr("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    //TODO: Add a button to clear the recent history.

    QLabel* labelRecentMaxFiles = new QLabel(tr("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    dialog.opensave_recent_max_files = settings.opensave_recent_max_files;
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
    dialog.opensave_trim_dst_num_jumps = settings.opensave_trim_dst_num_jumps;
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
    radioButtonUseSame->setChecked(!settings.printing_use_last_device);
    QRadioButton* radioButtonUseLast = new QRadioButton(tr("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(settings.printing_use_last_device);

    QComboBox* comboBoxDefaultDevice = new QComboBox(groupBoxDefaultPrinter);
    QList<QPrinterInfo> listAvailPrinters = QPrinterInfo::availablePrinters();
    foreach(QPrinterInfo info, listAvailPrinters)
    {
        comboBoxDefaultDevice->addItem(QIcon("icons/" + settings.general_icon_theme + "/" + "print" + ".png"), info.printerName());
    }

    QVBoxLayout* vboxLayoutDefaultPrinter = new QVBoxLayout(groupBoxDefaultPrinter);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter->addWidget(comboBoxDefaultDevice);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseLast);
    groupBoxDefaultPrinter->setLayout(vboxLayoutDefaultPrinter);

    //Save Ink
    QGroupBox* groupBoxSaveInk = new QGroupBox(tr("Save Ink"), widget);

    QCheckBox* checkBoxDisableBG = new QCheckBox(tr("Disable Background"), groupBoxSaveInk);
    dialog.printing_disable_bg = settings.printing_disable_bg;
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
    dialog.grid_show_on_load = settings.grid_show_on_load;
    checkBoxGridShowOnLoad->setChecked(dialog.grid_show_on_load);
    connect(checkBoxGridShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOnLoadStateChanged(int)));

    QCheckBox* checkBoxGridShowOrigin = new QCheckBox(tr("Show the origin when the grid is enabled"), groupBoxGridMisc);
    dialog.grid_show_origin = settings.grid_show_origin;
    checkBoxGridShowOrigin->setChecked(dialog.grid_show_origin);
    connect(checkBoxGridShowOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOriginStateChanged(int)));

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = new QGroupBox(tr("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(tr("Match grid color to crosshair color"), groupBoxGridColor);
    dialog.grid_color_match_crosshair = settings.grid_color_match_crosshair;
    checkBoxGridColorMatchCrossHair->setChecked(dialog.grid_color_match_crosshair);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(tr("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(tr("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    if (dialog.grid_color_match_crosshair) {
        dialog.grid_color = settings.display_crosshair_color;
    }
    else {
        dialog.grid_color = settings.grid_color;
    }
    preview.grid_color = dialog.grid_color;
    accept_.grid_color  = dialog.grid_color;
    QPixmap gridPix(16,16);
    gridPix.fill(QColor(preview.grid_color));
    buttonGridColor->setIcon(QIcon(gridPix));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor->setEnabled(!dialog.grid_color_match_crosshair);
    buttonGridColor->setEnabled(!dialog.grid_color_match_crosshair);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor,                  1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor,                 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = new QGroupBox(tr("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = new QCheckBox(tr("Set grid size from opened file"), groupBoxGridGeom);
    dialog.grid_load_from_file = settings.grid_load_from_file;
    checkBoxGridLoadFromFile->setChecked(dialog.grid_load_from_file);
    connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)));

    QLabel* labelGridType = new QLabel(tr("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    dialog.grid_type = settings.grid_type;
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(dialog.grid_type));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(const QString&)));

    QCheckBox* checkBoxGridCenterOnOrigin = new QCheckBox(tr("Center the grid on the origin"), groupBoxGridGeom);
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");
    dialog.grid_center_on_origin = settings.grid_center_on_origin;
    checkBoxGridCenterOnOrigin->setChecked(dialog.grid_center_on_origin);
    connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)));

    QLabel* labelGridCenterX = new QLabel(tr("Grid Center X"), groupBoxGridGeom);
    labelGridCenterX->setObjectName("labelGridCenterX");
    QDoubleSpinBox* spinBoxGridCenterX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridCenterX->setObjectName("spinBoxGridCenterX");
    dialog.grid_center_x = settings.grid_center_x;
    spinBoxGridCenterX->setSingleStep(1.000);
    spinBoxGridCenterX->setRange(-1000.000, 1000.000);
    spinBoxGridCenterX->setValue(dialog.grid_center_x);
    connect(spinBoxGridCenterX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridCenterXValueChanged(double)));

    QLabel* labelGridCenterY = new QLabel(tr("Grid Center Y"), groupBoxGridGeom);
    labelGridCenterY->setObjectName("labelGridCenterY");
    QDoubleSpinBox* spinBoxGridCenterY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridCenterY->setObjectName("spinBoxGridCenterY");
    dialog.grid_center_y = settings.grid_center_y;
    spinBoxGridCenterY->setSingleStep(1.000);
    spinBoxGridCenterY->setRange(-1000.000, 1000.000);
    spinBoxGridCenterY->setValue(dialog.grid_center_y);
    connect(spinBoxGridCenterY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridCenterYValueChanged(double)));

    QLabel* labelGridSizeX = new QLabel(tr("Grid Size X"), groupBoxGridGeom);
    labelGridSizeX->setObjectName("labelGridSizeX");
    QDoubleSpinBox* spinBoxGridSizeX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeX->setObjectName("spinBoxGridSizeX");
    dialog.grid_size_x = settings.grid_size_x;
    spinBoxGridSizeX->setSingleStep(1.000);
    spinBoxGridSizeX->setRange(1.000, 1000.000);
    spinBoxGridSizeX->setValue(dialog.grid_size_x);
    connect(spinBoxGridSizeX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeXValueChanged(double)));

    QLabel* labelGridSizeY = new QLabel(tr("Grid Size Y"), groupBoxGridGeom);
    labelGridSizeY->setObjectName("labelGridSizeY");
    QDoubleSpinBox* spinBoxGridSizeY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeY->setObjectName("spinBoxGridSizeY");
    dialog.grid_size_y = settings.grid_size_y;
    spinBoxGridSizeY->setSingleStep(1.000);
    spinBoxGridSizeY->setRange(1.000, 1000.000);
    spinBoxGridSizeY->setValue(dialog.grid_size_y);
    connect(spinBoxGridSizeY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeYValueChanged(double)));

    QLabel* labelGridSpacingX = new QLabel(tr("Grid Spacing X"), groupBoxGridGeom);
    labelGridSpacingX->setObjectName("labelGridSpacingX");
    QDoubleSpinBox* spinBoxGridSpacingX = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingX->setObjectName("spinBoxGridSpacingX");
    dialog.grid_spacing_x = settings.grid_spacing_x;
    spinBoxGridSpacingX->setSingleStep(1.000);
    spinBoxGridSpacingX->setRange(0.001, 1000.000);
    spinBoxGridSpacingX->setValue(dialog.grid_spacing_x);
    connect(spinBoxGridSpacingX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingXValueChanged(double)));

    QLabel* labelGridSpacingY = new QLabel(tr("Grid Spacing Y"), groupBoxGridGeom);
    labelGridSpacingY->setObjectName("labelGridSpacingY");
    QDoubleSpinBox* spinBoxGridSpacingY = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingY->setObjectName("spinBoxGridSpacingY");
    dialog.grid_spacing_y = settings.grid_spacing_y;
    spinBoxGridSpacingY->setSingleStep(1.000);
    spinBoxGridSpacingY->setRange(0.001, 1000.000);
    spinBoxGridSpacingY->setValue(dialog.grid_spacing_y);
    connect(spinBoxGridSpacingY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingYValueChanged(double)));

    QLabel* labelGridSizeRadius = new QLabel(tr("Grid Size Radius"), groupBoxGridGeom);
    labelGridSizeRadius->setObjectName("labelGridSizeRadius");
    QDoubleSpinBox* spinBoxGridSizeRadius = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSizeRadius->setObjectName("spinBoxGridSizeRadius");
    dialog.grid_size_radius = settings.grid_size_radius;
    spinBoxGridSizeRadius->setSingleStep(1.000);
    spinBoxGridSizeRadius->setRange(1.000, 1000.000);
    spinBoxGridSizeRadius->setValue(dialog.grid_size_radius);
    connect(spinBoxGridSizeRadius, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeRadiusValueChanged(double)));

    QLabel* labelGridSpacingRadius = new QLabel(tr("Grid Spacing Radius"), groupBoxGridGeom);
    labelGridSpacingRadius->setObjectName("labelGridSpacingRadius");
    QDoubleSpinBox* spinBoxGridSpacingRadius = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingRadius->setObjectName("spinBoxGridSpacingRadius");
    dialog.grid_spacing_radius = settings.grid_spacing_radius;
    spinBoxGridSpacingRadius->setSingleStep(1.000);
    spinBoxGridSpacingRadius->setRange(0.001, 1000.000);
    spinBoxGridSpacingRadius->setValue(dialog.grid_spacing_radius);
    connect(spinBoxGridSpacingRadius, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingRadiusValueChanged(double)));

    QLabel* labelGridSpacingAngle = new QLabel(tr("Grid Spacing Angle"), groupBoxGridGeom);
    labelGridSpacingAngle->setObjectName("labelGridSpacingAngle");
    QDoubleSpinBox* spinBoxGridSpacingAngle = new QDoubleSpinBox(groupBoxGridGeom);
    spinBoxGridSpacingAngle->setObjectName("spinBoxGridSpacingAngle");
    dialog.grid_spacing_angle = settings.grid_spacing_angle;
    spinBoxGridSpacingAngle->setSingleStep(1.000);
    spinBoxGridSpacingAngle->setRange(0.001, 1000.000);
    spinBoxGridSpacingAngle->setValue(dialog.grid_spacing_angle);
    connect(spinBoxGridSpacingAngle, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingAngleValueChanged(double)));

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
    dialog.ruler_show_on_load = settings.ruler_show_on_load;
    checkBoxRulerShowOnLoad->setChecked(dialog.ruler_show_on_load);
    connect(checkBoxRulerShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRulerShowOnLoadStateChanged(int)));

    QLabel* labelRulerMetric = new QLabel(tr("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric",   true);
    dialog.ruler_metric = settings.ruler_metric;
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
    dialog.ruler_color = settings.ruler_color;
    preview.ruler_color = dialog.ruler_color;
    accept_.ruler_color  = dialog.ruler_color;
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
    dialog.ruler_pixel_size = settings.ruler_pixel_size;
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

    QString iconTheme = settings.general_icon_theme;

    //QSnap Locators
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(tr("Locators Used"), widget);

    QCheckBox* checkBoxQSnapEndPoint = new QCheckBox(tr("Endpoint"), groupBoxQSnapLoc);
    dialog.qsnap_endpoint = settings.qsnap_endpoint;
    checkBoxQSnapEndPoint->setChecked(dialog.qsnap_endpoint);
    checkBoxQSnapEndPoint->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoendpoint" + ".png"));
    connect(checkBoxQSnapEndPoint, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapEndPointStateChanged(int)));

    QCheckBox* checkBoxQSnapMidPoint = new QCheckBox(tr("Midpoint"), groupBoxQSnapLoc);
    dialog.qsnap_midpoint = settings.qsnap_midpoint;
    checkBoxQSnapMidPoint->setChecked(dialog.qsnap_midpoint);
    checkBoxQSnapMidPoint->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptomidpoint" + ".png"));
    connect(checkBoxQSnapMidPoint, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapMidPointStateChanged(int)));

    QCheckBox* checkBoxQSnapCenter = new QCheckBox(tr("Center"), groupBoxQSnapLoc);
    dialog.qsnap_center = settings.qsnap_center;
    checkBoxQSnapCenter->setChecked(dialog.qsnap_center);
    checkBoxQSnapCenter->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptocenter" + ".png"));
    connect(checkBoxQSnapCenter, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapCenterStateChanged(int)));

    QCheckBox* checkBoxQSnapNode = new QCheckBox(tr("Node"), groupBoxQSnapLoc);
    dialog.qsnap_node = settings.qsnap_node;
    checkBoxQSnapNode->setChecked(dialog.qsnap_node);
    checkBoxQSnapNode->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptonode" + ".png"));
    connect(checkBoxQSnapNode, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapNodeStateChanged(int)));

    QCheckBox* checkBoxQSnapQuadrant = new QCheckBox(tr("Quadrant"), groupBoxQSnapLoc);
    dialog.qsnap_quadrant = settings.qsnap_quadrant;
    checkBoxQSnapQuadrant->setChecked(dialog.qsnap_quadrant);
    checkBoxQSnapQuadrant->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoquadrant" + ".png"));
    connect(checkBoxQSnapQuadrant, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapQuadrantStateChanged(int)));

    QCheckBox* checkBoxQSnapIntersection = new QCheckBox(tr("Intersection"), groupBoxQSnapLoc);
    dialog.qsnap_intersection = settings.qsnap_intersection;
    checkBoxQSnapIntersection->setChecked(dialog.qsnap_intersection);
    checkBoxQSnapIntersection->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptointersection" + ".png"));
    connect(checkBoxQSnapIntersection, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapIntersectionStateChanged(int)));

    QCheckBox* checkBoxQSnapExtension = new QCheckBox(tr("Extension"), groupBoxQSnapLoc);
    dialog.qsnap_extension = settings.qsnap_extension;
    checkBoxQSnapExtension->setChecked(dialog.qsnap_extension);
    checkBoxQSnapExtension->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoextension" + ".png"));
    connect(checkBoxQSnapExtension, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapExtensionStateChanged(int)));

    QCheckBox* checkBoxQSnapInsertion = new QCheckBox(tr("Insertion"), groupBoxQSnapLoc);
    dialog.qsnap_insertion = settings.qsnap_insertion;
    checkBoxQSnapInsertion->setChecked(dialog.qsnap_insertion);
    checkBoxQSnapInsertion->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoinsert" + ".png"));
    connect(checkBoxQSnapInsertion, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapInsertionStateChanged(int)));

    QCheckBox* checkBoxQSnapPerpendicular = new QCheckBox(tr("Perpendicular"), groupBoxQSnapLoc);
    dialog.qsnap_perpendicular = settings.qsnap_perpendicular;
    checkBoxQSnapPerpendicular->setChecked(dialog.qsnap_perpendicular);
    checkBoxQSnapPerpendicular->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoperpendicular" + ".png"));
    connect(checkBoxQSnapPerpendicular, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapPerpendicularStateChanged(int)));

    QCheckBox* checkBoxQSnapTangent = new QCheckBox(tr("Tangent"), groupBoxQSnapLoc);
    dialog.qsnap_tangent = settings.qsnap_tangent;
    checkBoxQSnapTangent->setChecked(dialog.qsnap_tangent);
    checkBoxQSnapTangent->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptotangent" + ".png"));
    connect(checkBoxQSnapTangent, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapTangentStateChanged(int)));

    QCheckBox* checkBoxQSnapNearest = new QCheckBox(tr("Nearest"), groupBoxQSnapLoc);
    dialog.qsnap_nearest = settings.qsnap_nearest;
    checkBoxQSnapNearest->setChecked(dialog.qsnap_nearest);
    checkBoxQSnapNearest->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptonearest" + ".png"));
    connect(checkBoxQSnapNearest, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapNearestStateChanged(int)));

    QCheckBox* checkBoxQSnapApparent = new QCheckBox(tr("Apparent Intersection"), groupBoxQSnapLoc);
    dialog.qsnap_apparent = settings.qsnap_apparent;
    checkBoxQSnapApparent->setChecked(dialog.qsnap_apparent);
    checkBoxQSnapApparent->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoapparentintersection" + ".png"));
    connect(checkBoxQSnapApparent, SIGNAL(stateChanged(int)), this, SLOT(checkBoxQSnapApparentStateChanged(int)));

    QCheckBox* checkBoxQSnapParallel = new QCheckBox(tr("Parallel"), groupBoxQSnapLoc);
    dialog.qsnap_parallel = settings.qsnap_parallel;
    checkBoxQSnapParallel->setChecked(dialog.qsnap_parallel);
    checkBoxQSnapParallel->setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoparallel" + ".png"));
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
    dialog.qsnap_locator_color = settings.qsnap_locator_color;
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(dialog.qsnap_locator_color));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(tr("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    dialog.qsnap_locator_size = settings.qsnap_locator_size;
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
    dialog.qsnap_aperture_size = settings.qsnap_aperture_size;
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

    QGraphicsScene* s = mainWin->activeScene();

    QCheckBox* checkBoxShowLwt = new QCheckBox(tr("Show LineWeight"), groupBoxLwtMisc);
    if(s) { dialog.lwt_show_lwt = s->property(ENABLE_LWT).toBool(); }
    else  { dialog.lwt_show_lwt = settings.lwt_show_lwt; }
    preview.lwt_show_lwt = dialog.lwt_show_lwt;
    checkBoxShowLwt->setChecked(preview.lwt_show_lwt);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(tr("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if(s) { dialog.lwt_real_render = s->property(ENABLE_REAL).toBool(); }
    else  { dialog.lwt_real_render = settings.lwt_real_render; }
    preview.lwt_real_render = dialog.lwt_real_render;
    checkBoxRealRender->setChecked(preview.lwt_real_render);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(dialog.lwt_show_lwt);

    QLabel* labelDefaultLwt = new QLabel(tr("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //TODO: remove later
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    dialog.lwt_default_lwt = settings.lwt_default_lwt;
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
    dialog.selection_mode_pickfirst = settings.selection_mode_pickfirst;
    checkBoxSelectionModePickFirst->setChecked(dialog.selection_mode_pickfirst);
    checkBoxSelectionModePickFirst->setChecked(true); checkBoxSelectionModePickFirst->setEnabled(false); //TODO: Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickFirstStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickAdd = new QCheckBox(tr("Add to Selection (PickAdd)"), groupBoxSelectionModes);
    dialog.selection_mode_pickadd = settings.selection_mode_pickadd;
    checkBoxSelectionModePickAdd->setChecked(dialog.selection_mode_pickadd);
    connect(checkBoxSelectionModePickAdd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickAddStateChanged(int)));

    QCheckBox* checkBoxSelectionModePickDrag = new QCheckBox(tr("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    dialog.selection_mode_pickdrag = settings.selection_mode_pickdrag;
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
    dialog.selection_coolgrip_color = settings.selection_coolgrip_color;
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(dialog.selection_coolgrip_color));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionCoolGripColorCurrentIndexChanged(int)));

    QLabel* labelHotGripColor = new QLabel(tr("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    dialog.selection_hotgrip_color = settings.selection_hotgrip_color;
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
    dialog.selection_grip_size = settings.selection_grip_size;
    sliderSelectionGripSize->setValue(dialog.selection_grip_size);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionGripSizeValueChanged(int)));

    QLabel* labelSelectionPickBoxSize = new QLabel(tr("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    dialog.selection_pickbox_size = settings.selection_pickbox_size;
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

void Settings_Dialog::addColorsToComboBox(QComboBox* comboBox)
{
    QString iconTheme = settings.general_icon_theme;

    comboBox->addItem(QIcon("icons/" + iconTheme + "/" + "colorred" + ".png"),     tr("Red"),     qRgb(255,  0,  0));
    comboBox->addItem(QIcon("icons/" + iconTheme + "/" + "coloryellow" + ".png"),  tr("Yellow"),  qRgb(255,255,  0));
    comboBox->addItem(QIcon("icons/" + iconTheme + "/" + "colorgreen" + ".png"),   tr("Green"),   qRgb(  0,255,  0));
    comboBox->addItem(QIcon("icons/" + iconTheme + "/" + "colorcyan" + ".png"),    tr("Cyan"),    qRgb(  0,255,255));
    comboBox->addItem(QIcon("icons/" + iconTheme + "/" + "colorblue" + ".png"),    tr("Blue"),    qRgb(  0,  0,255));
    comboBox->addItem(QIcon("icons/" + iconTheme + "/" + "colormagenta" + ".png"), tr("Magenta"), qRgb(255,  0,255));
    comboBox->addItem(QIcon("icons/" + iconTheme + "/" + "colorwhite" + ".png"),   tr("White"),   qRgb(255,255,255));
    //TODO: Add Other... so the user can select custom colors
}

void Settings_Dialog::comboBoxLanguageCurrentIndexChanged(const QString& lang)
{
    dialog.general_language = lang.toLower();
}

void Settings_Dialog::comboBoxIconThemeCurrentIndexChanged(const QString& theme)
{
    dialog.general_icon_theme = theme;
}

void Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if(comboBox)
    {
        bool ok = 0;
        dialog.general_icon_size = comboBox->itemData(index).toUInt(&ok);
        if(!ok)
            dialog.general_icon_size = 16;
    }
    else
        dialog.general_icon_size = 16;
}

void Settings_Dialog::checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    preview.general_mdi_bg_use_logo = checked;
    mainWin->mdiArea->useBackgroundLogo(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        #if QT_VERSION >= 0x050000
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), //Qt5
                        #else
                        QDesktopServices::storageLocation(QDesktopServices::PicturesLocation), //Qt4
                        #endif
                        tr("Images (*.bmp *.png *.jpg)"));

        if(!selectedImage.isNull())
            accept_.general_mdi_bg_logo = selectedImage;

        //Update immediately so it can be previewed
        mainWin->mdiArea->setBackgroundLogo(accept_.general_mdi_bg_logo);
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    preview.general_mdi_bg_use_texture = checked;
    mainWin->mdiArea->useBackgroundTexture(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, tr("Open File"),
                        #if QT_VERSION >= 0x050000
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation), //Qt5
                        #else
                        QDesktopServices::storageLocation(QDesktopServices::PicturesLocation), //Qt4
                        #endif
                        tr("Images (*.bmp *.png *.jpg)"));

        if(!selectedImage.isNull())
            accept_.general_mdi_bg_texture = selectedImage;

        //Update immediately so it can be previewed
        mainWin->mdiArea->setBackgroundTexture(accept_.general_mdi_bg_texture);
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    preview.general_mdi_bg_use_color = checked;
    mainWin->mdiArea->useBackgroundColor(checked);
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
            mainWin->mdiArea->setBackgroundColor(QColor(accept_.general_mdi_bg_color));
        }
        else
        {
            mainWin->mdiArea->setBackgroundColor(QColor(dialog.general_mdi_bg_color));
        }
    }
}

void Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    preview.general_mdi_bg_color = color.rgb();
    mainWin->mdiArea->setBackgroundColor(QColor(preview.general_mdi_bg_color));
}

void Settings_Dialog::checkBoxTipOfTheDayStateChanged(int checked)
{
    dialog.general_tip_of_the_day = checked;
}

void Settings_Dialog::checkBoxUseOpenGLStateChanged(int checked)
{
    dialog.display_use_opengl = checked;
}

void Settings_Dialog::checkBoxRenderHintAAStateChanged(int checked)
{
    dialog.display_renderhint_aa = checked;
}

void Settings_Dialog::checkBoxRenderHintTextAAStateChanged(int checked)
{
    dialog.display_renderhint_text_aa = checked;
}

void Settings_Dialog::checkBoxRenderHintSmoothPixStateChanged(int checked)
{
    dialog.display_renderhint_smooth_pix = checked;
}

void Settings_Dialog::checkBoxRenderHintHighAAStateChanged(int checked)
{
    dialog.display_renderhint_high_aa = checked;
}

void Settings_Dialog::checkBoxRenderHintNonCosmeticStateChanged(int checked)
{
    dialog.display_renderhint_noncosmetic = checked;
}

void Settings_Dialog::checkBoxShowScrollBarsStateChanged(int checked)
{
    preview.display_show_scrollbars = checked;
    mainWin->updateAllViewScrollBars(preview.display_show_scrollbars);
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
            mainWin->updateAllViewCrossHairColors(accept_.display_crosshair_color);
        }
        else
        {
            mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
        }
    }
}

void Settings_Dialog::currentDisplayCrossHairColorChanged(const QColor& color)
{
    preview.display_crosshair_color = color.rgb();
    mainWin->updateAllViewCrossHairColors(preview.display_crosshair_color);
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
            mainWin->updateAllViewBackgroundColors(accept_.display_bg_color);
        }
        else
        {
            mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
        }
    }
}

void Settings_Dialog::currentDisplayBackgroundColorChanged(const QColor& color)
{
    preview.display_bg_color = color.rgb();
    mainWin->updateAllViewBackgroundColors(preview.display_bg_color);
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
            mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                                  accept_.display_selectbox_left_fill,
                                                  accept_.display_selectbox_right_color,
                                                  accept_.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
        else
        {
            mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
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
    mainWin->updateAllViewSelectBoxColors(preview.display_selectbox_left_color,
                                          preview.display_selectbox_left_fill,
                                          preview.display_selectbox_right_color,
                                          preview.display_selectbox_right_fill,
                                          preview.display_selectbox_alpha);
}

void Settings_Dialog::chooseDisplaySelectBoxLeftFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button)
    {
        QColorDialog* colorDialog = new QColorDialog(QColor(accept_.display_selectbox_left_fill), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftFillChanged(const QColor&)));
        colorDialog->exec();

        if(colorDialog->result() == QDialog::Accepted)
        {
            accept_.display_selectbox_left_fill = colorDialog->selectedColor().rgb();
            QPixmap pix(16,16);
            pix.fill(QColor(accept_.display_selectbox_left_fill));
            button->setIcon(QIcon(pix));
            mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                                  accept_.display_selectbox_left_fill,
                                                  accept_.display_selectbox_right_color,
                                                  accept_.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
        else
        {
            mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                                  dialog.display_selectbox_left_fill,
                                                  dialog.display_selectbox_right_color,
                                                  dialog.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftFillChanged(const QColor& color)
{
    preview.display_selectbox_left_fill = color.rgb();
    mainWin->updateAllViewSelectBoxColors(preview.display_selectbox_left_color,
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
            mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                                  accept_.display_selectbox_left_fill,
                                                  accept_.display_selectbox_right_color,
                                                  accept_.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
        else
        {
            mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
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
    mainWin->updateAllViewSelectBoxColors(preview.display_selectbox_left_color,
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
            mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
                                                  accept_.display_selectbox_left_fill,
                                                  accept_.display_selectbox_right_color,
                                                  accept_.display_selectbox_right_fill,
                                                  preview.display_selectbox_alpha);
        }
        else
        {
            mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
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
    mainWin->updateAllViewSelectBoxColors(preview.display_selectbox_left_color,
                                          preview.display_selectbox_left_fill,
                                          preview.display_selectbox_right_color,
                                          preview.display_selectbox_right_fill,
                                          preview.display_selectbox_alpha);
}

void Settings_Dialog::spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    preview.display_selectbox_alpha = value;
    mainWin->updateAllViewSelectBoxColors(accept_.display_selectbox_left_color,
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
            mainWin->prompt->setPromptTextColor(QColor(accept_.prompt_text_color));
        }
        else
        {
            mainWin->prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
        }
    }
}

void Settings_Dialog::currentPromptTextColorChanged(const QColor& color)
{
    preview.prompt_text_color = color.rgb();
    mainWin->prompt->setPromptTextColor(QColor(preview.prompt_text_color));
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
            mainWin->prompt->setPromptBackgroundColor(QColor(accept_.prompt_bg_color));
        }
        else
        {
            mainWin->prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
        }
    }
}

void Settings_Dialog::currentPromptBackgroundColorChanged(const QColor& color)
{
    preview.prompt_bg_color = color.rgb();
    mainWin->prompt->setPromptBackgroundColor(QColor(preview.prompt_bg_color));
}

void Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(const QString& family)
{
    preview.prompt_font_family = family;
    mainWin->prompt->setPromptFontFamily(preview.prompt_font_family);
}

void Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(const QString& style)
{
    preview.prompt_font_style = style;
    mainWin->prompt->setPromptFontStyle(preview.prompt_font_style);
}

void Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    preview.prompt_font_size = value;
    mainWin->prompt->setPromptFontSize(preview.prompt_font_size);
}

void Settings_Dialog::checkBoxPromptSaveHistoryStateChanged(int checked)
{
    dialog.prompt_save_history = checked;
}

void Settings_Dialog::checkBoxPromptSaveHistoryAsHtmlStateChanged(int checked)
{
    dialog.prompt_save_history_as_html = checked;
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
    if(dialog.grid_color_match_crosshair) { mainWin->updateAllViewGridColors(accept_.display_crosshair_color); }
    else                                  { mainWin->updateAllViewGridColors(accept_.grid_color);              }

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
            mainWin->updateAllViewGridColors(accept_.grid_color);
        }
        else
        {
            mainWin->updateAllViewGridColors(dialog.grid_color);
        }
    }
}

void Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    preview.grid_color = color.rgb();
    mainWin->updateAllViewGridColors(preview.grid_color);
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
    if(senderObj)
    {
        QObject* parent = senderObj->parent();
        if(parent)
        {
            bool visibility = false;
            if(type == "Circular") visibility = true;

            QLabel* labelGridSizeX = parent->findChild<QLabel*>("labelGridSizeX");
            if(labelGridSizeX) labelGridSizeX->setVisible(!visibility);
            QDoubleSpinBox* spinBoxGridSizeX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeX");
            if(spinBoxGridSizeX) spinBoxGridSizeX->setVisible(!visibility);
            QLabel* labelGridSizeY = parent->findChild<QLabel*>("labelGridSizeY");
            if(labelGridSizeY) labelGridSizeY->setVisible(!visibility);
            QDoubleSpinBox* spinBoxGridSizeY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeY");
            if(spinBoxGridSizeY) spinBoxGridSizeY->setVisible(!visibility);
            QLabel* labelGridSpacingX = parent->findChild<QLabel*>("labelGridSpacingX");
            if(labelGridSpacingX) labelGridSpacingX->setVisible(!visibility);
            QDoubleSpinBox* spinBoxGridSpacingX = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingX");
            if(spinBoxGridSpacingX) spinBoxGridSpacingX->setVisible(!visibility);
            QLabel* labelGridSpacingY = parent->findChild<QLabel*>("labelGridSpacingY");
            if(labelGridSpacingY) labelGridSpacingY->setVisible(!visibility);
            QDoubleSpinBox* spinBoxGridSpacingY = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingY");
            if(spinBoxGridSpacingY) spinBoxGridSpacingY->setVisible(!visibility);
            QLabel* labelGridSizeRadius = parent->findChild<QLabel*>("labelGridSizeRadius");
            if(labelGridSizeRadius) labelGridSizeRadius->setVisible(visibility);
            QDoubleSpinBox* spinBoxGridSizeRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSizeRadius");
            if(spinBoxGridSizeRadius) spinBoxGridSizeRadius->setVisible(visibility);
            QLabel* labelGridSpacingRadius = parent->findChild<QLabel*>("labelGridSpacingRadius");
            if(labelGridSpacingRadius) labelGridSpacingRadius->setVisible(visibility);
            QDoubleSpinBox* spinBoxGridSpacingRadius = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingRadius");
            if(spinBoxGridSpacingRadius) spinBoxGridSpacingRadius->setVisible(visibility);
            QLabel* labelGridSpacingAngle = parent->findChild<QLabel*>("labelGridSpacingAngle");
            if(labelGridSpacingAngle) labelGridSpacingAngle->setVisible(visibility);
            QDoubleSpinBox* spinBoxGridSpacingAngle = parent->findChild<QDoubleSpinBox*>("spinBoxGridSpacingAngle");
            if(spinBoxGridSpacingAngle) spinBoxGridSpacingAngle->setVisible(visibility);
        }
    }
}

void Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    dialog.grid_center_on_origin = checked;

    QObject* senderObj = sender();
    if(senderObj)
    {
        QObject* parent = senderObj->parent();
        if(parent)
        {
            QLabel* labelGridCenterX = parent->findChild<QLabel*>("labelGridCenterX");
            if(labelGridCenterX) labelGridCenterX->setEnabled(!dialog.grid_center_on_origin);
            QDoubleSpinBox* spinBoxGridCenterX = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterX");
            if(spinBoxGridCenterX) spinBoxGridCenterX->setEnabled(!dialog.grid_center_on_origin);
            QLabel* labelGridCenterY = parent->findChild<QLabel*>("labelGridCenterY");
            if(labelGridCenterY) labelGridCenterY->setEnabled(!dialog.grid_center_on_origin);
            QDoubleSpinBox* spinBoxGridCenterY = parent->findChild<QDoubleSpinBox*>("spinBoxGridCenterY");
            if(spinBoxGridCenterY) spinBoxGridCenterY->setEnabled(!dialog.grid_center_on_origin);
        }
    }
}

void Settings_Dialog::spinBoxGridCenterXValueChanged(double value)
{
    dialog.grid_center_x = value;
}

void Settings_Dialog::spinBoxGridCenterYValueChanged(double value)
{
    dialog.grid_center_y = value;
}

void Settings_Dialog::spinBoxGridSizeXValueChanged(double value)
{
    dialog.grid_size_x = value;
}

void Settings_Dialog::spinBoxGridSizeYValueChanged(double value)
{
    dialog.grid_size_y = value;
}

void Settings_Dialog::spinBoxGridSpacingXValueChanged(double value)
{
    dialog.grid_spacing_x = value;
}

void Settings_Dialog::spinBoxGridSpacingYValueChanged(double value)
{
    dialog.grid_spacing_y = value;
}

void Settings_Dialog::spinBoxGridSizeRadiusValueChanged(double value)
{
    dialog.grid_size_radius = value;
}

void Settings_Dialog::spinBoxGridSpacingRadiusValueChanged(double value)
{
    dialog.grid_spacing_radius = value;
}

void Settings_Dialog::spinBoxGridSpacingAngleValueChanged(double value)
{
    dialog.grid_spacing_angle = value;
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
            mainWin->updateAllViewRulerColors(accept_.ruler_color);
        }
        else
        {
            mainWin->updateAllViewRulerColors(dialog.ruler_color);
        }
    }
}

void Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    preview.ruler_color = color.rgb();
    mainWin->updateAllViewRulerColors(preview.ruler_color);
}

void Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    dialog.ruler_pixel_size = value;
}

void Settings_Dialog::checkBoxQSnapEndPointStateChanged(int checked)
{
    dialog.qsnap_endpoint = checked;
}

void Settings_Dialog::checkBoxQSnapMidPointStateChanged(int checked)
{
    dialog.qsnap_midpoint = checked;
}

void Settings_Dialog::checkBoxQSnapCenterStateChanged(int checked)
{
    dialog.qsnap_center = checked;
}

void Settings_Dialog::checkBoxQSnapNodeStateChanged(int checked)
{
    dialog.qsnap_node = checked;
}

void Settings_Dialog::checkBoxQSnapQuadrantStateChanged(int checked)
{
    dialog.qsnap_quadrant = checked;
}

void Settings_Dialog::checkBoxQSnapIntersectionStateChanged(int checked)
{
    dialog.qsnap_intersection = checked;
}

void Settings_Dialog::checkBoxQSnapExtensionStateChanged(int checked)
{
    dialog.qsnap_extension = checked;
}

void Settings_Dialog::checkBoxQSnapInsertionStateChanged(int checked)
{
    dialog.qsnap_insertion = checked;
}

void Settings_Dialog::checkBoxQSnapPerpendicularStateChanged(int checked)
{
    dialog.qsnap_perpendicular = checked;
}

void Settings_Dialog::checkBoxQSnapTangentStateChanged(int checked)
{
    dialog.qsnap_tangent = checked;
}

void Settings_Dialog::checkBoxQSnapNearestStateChanged(int checked)
{
    dialog.qsnap_nearest = checked;
}

void Settings_Dialog::checkBoxQSnapApparentStateChanged(int checked)
{
    dialog.qsnap_apparent = checked;
}

void Settings_Dialog::checkBoxQSnapParallelStateChanged(int checked)
{
    dialog.qsnap_parallel = checked;
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
    if(preview.lwt_show_lwt) { mainWin->statusbar->statusBarLwtButton->enableLwt(); }
    else                     { mainWin->statusbar->statusBarLwtButton->disableLwt(); }

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
    if(preview.lwt_real_render) { mainWin->statusbar->statusBarLwtButton->enableReal(); }
    else                        { mainWin->statusbar->statusBarLwtButton->disableReal(); }
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

void Settings_Dialog::acceptChanges()
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

    settings.general_language = dialog.general_language;
    settings.general_icon_theme = dialog.general_icon_theme;
    settings.general_icon_size = dialog.general_icon_size;
    settings.general_mdi_bg_use_logo = dialog.general_mdi_bg_use_logo;
    settings.general_mdi_bg_use_texture = dialog.general_mdi_bg_use_texture;
    settings.general_mdi_bg_use_color = dialog.general_mdi_bg_use_color;
    settings.general_mdi_bg_logo = dialog.general_mdi_bg_logo;
    settings.general_mdi_bg_texture = dialog.general_mdi_bg_texture;
    settings.general_mdi_bg_color = dialog.general_mdi_bg_color;
    settings.general_tip_of_the_day = dialog.general_tip_of_the_day;
    //TODO: settings.general_system_help_browser = dialog.general_system_help_browser;
    settings.display_use_opengl = dialog.display_use_opengl;
    settings.display_renderhint_aa = dialog.display_renderhint_aa;
    settings.display_renderhint_text_aa = dialog.display_renderhint_text_aa;
    settings.display_renderhint_smooth_pix = dialog.display_renderhint_smooth_pix;
    settings.display_renderhint_high_aa = dialog.display_renderhint_high_aa;
    settings.display_renderhint_noncosmetic = dialog.display_renderhint_noncosmetic;
    settings.display_show_scrollbars = dialog.display_show_scrollbars;
    settings.display_scrollbar_widget_num = dialog.display_scrollbar_widget_num;
    settings.display_crosshair_color = dialog.display_crosshair_color;
    settings.display_bg_color = dialog.display_bg_color;
    settings.display_selectbox_left_color = dialog.display_selectbox_left_color;
    settings.display_selectbox_left_fill = dialog.display_selectbox_left_fill;
    settings.display_selectbox_right_color = dialog.display_selectbox_right_color;
    settings.display_selectbox_right_fill = dialog.display_selectbox_right_fill;
    settings.display_selectbox_alpha = dialog.display_selectbox_alpha;
    settings.display_zoomscale_in = dialog.display_zoomscale_in;
    settings.display_zoomscale_out = dialog.display_zoomscale_out;
    //TODO: settingsDisplayCrossHairPercent = dialog.display_crosshair_percent;
    //TODO: settingsDisplayUnits = dialog.display_units;
    settings.prompt_text_color = dialog.prompt_text_color;
    settings.prompt_bg_color = dialog.prompt_bg_color;
    settings.prompt_font_family = dialog.prompt_font_family;
    settings.prompt_font_style = dialog.prompt_font_style;
    settings.prompt_font_size = dialog.prompt_font_size;
    settings.prompt_save_history = dialog.prompt_save_history;
    settings.prompt_save_history_as_html = dialog.prompt_save_history_as_html;
    //TODO: settings.prompt_save_history_filename = dialog.prompt_save_history_filename;
    settings.opensave_custom_filter = dialog.opensave_custom_filter;
    //TODO: settings.opensave_open_format = dialog.opensave_open_format;
    //TODO: settings.opensave_open_thumbnail = dialog.opensave_open_thumbnail;
    //TODO: settings.opensave_save_format = dialog.opensave_save_format;
    //TODO: settings.opensave_save_thumbnail = dialog.opensave_save_thumbnail;
    settings.opensave_recent_max_files = dialog.opensave_recent_max_files;
    settings.opensave_trim_dst_num_jumps = dialog.opensave_trim_dst_num_jumps;
    //TODO: settings.printing_default_device = dialog.printing_default_device;
    //TODO: settings.printing_use_last_device = dialog.printing_use_last_device;
    settings.printing_disable_bg = dialog.printing_disable_bg;
    settings.grid_show_on_load = dialog.grid_show_on_load;
    settings.grid_show_origin = dialog.grid_show_origin;
    settings.grid_color_match_crosshair = dialog.grid_color_match_crosshair;
    settings.grid_color = dialog.grid_color;
    //TODO: settings.grid_load_from_file = dialog.grid_load_from_file;
    settings.grid_type = dialog.grid_type;
    settings.grid_center_on_origin = dialog.grid_center_on_origin;
    settings.grid_center_x = dialog.grid_center_x;
    settings.grid_center_y = dialog.grid_center_y;
    settings.grid_size_x = dialog.grid_size_x;
    settings.grid_size_y = dialog.grid_size_y;
    settings.grid_spacing_x = dialog.grid_spacing_x;
    settings.grid_spacing_y = dialog.grid_spacing_y;
    settings.grid_size_radius = dialog.grid_size_radius;
    settings.grid_spacing_radius = dialog.grid_spacing_radius;
    settings.grid_spacing_angle = dialog.grid_spacing_angle;
    settings.ruler_show_on_load = dialog.ruler_show_on_load;
    settings.ruler_metric = dialog.ruler_metric;
    settings.ruler_color = dialog.ruler_color;
    settings.ruler_pixel_size = dialog.ruler_pixel_size;
    //TODO: settings.qsnap_enabled = dialog.qsnap_enabled;
    settings.qsnap_locator_color = dialog.qsnap_locator_color;
    settings.qsnap_locator_size = dialog.qsnap_locator_size;
    settings.qsnap_aperture_size = dialog.qsnap_aperture_size;
    settings.qsnap_endpoint = dialog.qsnap_endpoint;
    settings.qsnap_midpoint = dialog.qsnap_midpoint;
    settings.qsnap_center = dialog.qsnap_center;
    settings.qsnap_node = dialog.qsnap_node;
    settings.qsnap_quadrant = dialog.qsnap_quadrant;
    settings.qsnap_intersection = dialog.qsnap_intersection;
    settings.qsnap_extension = dialog.qsnap_extension;
    settings.qsnap_insertion = dialog.qsnap_insertion;
    settings.qsnap_perpendicular = dialog.qsnap_perpendicular;
    settings.qsnap_tangent = dialog.qsnap_tangent;
    settings.qsnap_nearest = dialog.qsnap_nearest;
    settings.qsnap_apparent = dialog.qsnap_apparent;
    settings.qsnap_parallel = dialog.qsnap_parallel;
    settings.lwt_show_lwt = dialog.lwt_show_lwt;
    settings.lwt_real_render = dialog.lwt_real_render;
    settings.selection_mode_pickfirst = dialog.selection_mode_pickfirst;
    settings.selection_mode_pickadd = dialog.selection_mode_pickadd;
    settings.selection_mode_pickdrag = dialog.selection_mode_pickdrag;
    settings.selection_coolgrip_color = dialog.selection_coolgrip_color;
    settings.selection_hotgrip_color = dialog.selection_hotgrip_color;
    settings.selection_grip_size = dialog.selection_grip_size;
    settings.selection_pickbox_size = dialog.selection_pickbox_size;

    //Make sure the user sees the changes applied immediately
    mainWin->mdiArea->useBackgroundLogo(dialog.general_mdi_bg_use_logo);
    mainWin->mdiArea->useBackgroundTexture(dialog.general_mdi_bg_use_texture);
    mainWin->mdiArea->useBackgroundColor(dialog.general_mdi_bg_use_color);
    mainWin->mdiArea->setBackgroundLogo(dialog.general_mdi_bg_logo);
    mainWin->mdiArea->setBackgroundTexture(dialog.general_mdi_bg_texture);
    mainWin->mdiArea->setBackgroundColor(dialog.general_mdi_bg_color);
    mainWin->iconResize(dialog.general_icon_size);
    mainWin->updateAllViewScrollBars(dialog.display_show_scrollbars);
    mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
    mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
    mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                          dialog.display_selectbox_left_fill,
                                          dialog.display_selectbox_right_color,
                                          dialog.display_selectbox_right_fill,
                                          dialog.display_selectbox_alpha);
    mainWin->prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
    mainWin->prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
    mainWin->prompt->setPromptFontFamily(dialog.prompt_font_family);
    mainWin->prompt->setPromptFontStyle(dialog.prompt_font_style);
    mainWin->prompt->setPromptFontSize(dialog.prompt_font_size);
    mainWin->updateAllViewGridColors(dialog.grid_color);
    mainWin->updateAllViewRulerColors(dialog.ruler_color);
    if(dialog.lwt_show_lwt) { mainWin->statusbar->statusBarLwtButton->enableLwt(); }
    else                    { mainWin->statusbar->statusBarLwtButton->disableLwt(); }
    if(dialog.lwt_real_render) { mainWin->statusbar->statusBarLwtButton->enableReal(); }
    else                       { mainWin->statusbar->statusBarLwtButton->disableReal(); }
    mainWin->updatePickAddMode(dialog.selection_mode_pickadd);

    mainWin->writeSettings();
    accept();
}

void Settings_Dialog::rejectChanges()
{
    //TODO: inform the user if they have changed settings

    //Update the view since the user must accept the preview
    mainWin->mdiArea->useBackgroundLogo(dialog.general_mdi_bg_use_logo);
    mainWin->mdiArea->useBackgroundTexture(dialog.general_mdi_bg_use_texture);
    mainWin->mdiArea->useBackgroundColor(dialog.general_mdi_bg_use_color);
    mainWin->mdiArea->setBackgroundLogo(dialog.general_mdi_bg_logo);
    mainWin->mdiArea->setBackgroundTexture(dialog.general_mdi_bg_texture);
    mainWin->mdiArea->setBackgroundColor(dialog.general_mdi_bg_color);
    mainWin->updateAllViewScrollBars(dialog.display_show_scrollbars);
    mainWin->updateAllViewCrossHairColors(dialog.display_crosshair_color);
    mainWin->updateAllViewBackgroundColors(dialog.display_bg_color);
    mainWin->updateAllViewSelectBoxColors(dialog.display_selectbox_left_color,
                                          dialog.display_selectbox_left_fill,
                                          dialog.display_selectbox_right_color,
                                          dialog.display_selectbox_right_fill,
                                          dialog.display_selectbox_alpha);
    mainWin->prompt->setPromptTextColor(QColor(dialog.prompt_text_color));
    mainWin->prompt->setPromptBackgroundColor(QColor(dialog.prompt_bg_color));
    mainWin->prompt->setPromptFontFamily(dialog.prompt_font_family);
    mainWin->prompt->setPromptFontStyle(dialog.prompt_font_style);
    mainWin->prompt->setPromptFontSize(dialog.prompt_font_size);
    mainWin->updateAllViewGridColors(dialog.grid_color);
    mainWin->updateAllViewRulerColors(dialog.ruler_color);
    if(dialog.lwt_show_lwt) { mainWin->statusbar->statusBarLwtButton->enableLwt(); }
    else                    { mainWin->statusbar->statusBarLwtButton->disableLwt(); }
    if(dialog.lwt_real_render) { mainWin->statusbar->statusBarLwtButton->enableReal(); }
    else                       { mainWin->statusbar->statusBarLwtButton->disableReal(); }

    reject();
}
