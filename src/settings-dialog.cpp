/**
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

#include <fstream>

typedef std::string String;

extern std::unordered_map<std::string, QAction*> actionHash;

/**
 * The actuator changes the program state via these global variables.
 *
 * These copies of the settings struct are for restoring the state if
 * the user doesn't want to accept their changes in the settings dialog.
 */
std::unordered_map<std::string, Node> preview, accept_;

/**
 * .
 */
std::vector<std::string> extensions = {
    "100", "10o", "ART", "BMC", "BRO",
    "CND", "COL", "CSD", "CSV", "DAT",
    "DEM", "DSB", "DST", "DSZ", "DXF",
    "EDR", "EMD", "EXP", "EXY", "EYS",
    "FXY", "GNC", "GT", "HUS", "INB",
    "JEF", "KSM", "PCD", "PCM", "PCQ",
    "PCS", "PEC", "PEL", "PEM", "PES",
    "PHB", "PHC", "RGB", "SEW", "SHV",
    "SST", "STX", "SVG", "T09", "TAP",
    "THR", "TXT", "U00", "U01", "VIP",
    "VP3", "XXX", "ZSK"
};

const char *settings_labels[] = {
    "general_icon_theme",
    "general_icon_size",
    "general_mdi_bg_use_logo",
    "general_mdi_bg_logo",
    "general_mdi_bg_use_texture",
    "general_mdi_bg_texture",
    "general_mdi_bg_use_color",
    "general_mdi_bg_color",
    "general_tip_of_the_day",
    "END"
};

std::vector<std::string> general_props = {
    "general_icon_theme",
    "general_icon_size",
    "general_mdi_bg_use_logo",
    "general_mdi_bg_logo",
    "general_mdi_bg_use_texture",
    "general_mdi_bg_texture",
    "general_mdi_bg_use_color",
    "general_mdi_bg_color",
    "general_tip_of_the_day"
};

std::vector<std::string> display_props = {
    "display_use_opengl",
    "display_renderhint_aa",
    "display_renderhint_text_aa",
    "display_renderhint_smooth_pix",
    "display_renderhint_high_aa",
    "display_renderhint_noncosmetic",
    "display_show_scrollbars",
    "display_scrollbar_widget_num",
    "display_crosshair_color",
    "display_bg_color",
    "display_selectbox_left_color",
    "display_selectbox_left_fill",
    "display_selectbox_right_color",
    "display_selectbox_right_fill",
    "display_selectbox_alpha",
    "display_zoomscale_in",
    "display_zoomscale_out"
};

std::vector<std::string> prompt_props = {
    "prompt_text_color",
    "prompt_background_color",
    "prompt_font_family",
    "prompt_font_size",
    "prompt_save_history",
    "prompt_save_history_as_html"
};

std::vector<std::string> quick_snap_props = {
    "quicksnap_endpoint",
    "quicksnap_midpoint",
    "quicksnap_center",
    "quicksnap_node",
    "quicksnap_quadrant",
    "quicksnap_intersection",
    "quicksnap_extension",
    "quicksnap_insertion",
    "quicksnap_perpendicular",
    "quicksnap_tangent",
    "quicksnap_nearest",
    "quicksnap_apparent",
    "quicksnap_parallel",
    "quicksnap_locator_color",
    "quicksnap_locator_size",
    "quicksnap_aperture_size"
};

std::vector<std::string> opensave_props = {
    "opensave_custom_filter"
};

/* . */
void
make_editing_copy(std::vector<std::string> props)
{
    for (int i=0; i<(int)props.size(); i++) {
        String s = props[i];
        dialog[s] = settings[s];
        preview[s] = dialog[s];
        accept_[s] = dialog[s];
    }
}

/* Read settings from file.
 *
 * This file needs to be read from the users home directory to ensure it is writable.
 *
 * \brief Read the settings from file which are editable by the user.
 * These files need to be placed in the install folder.
 */
int
read_settings(void)
{
    debug_message("Reading Settings...");

    char config[MAX_SETTINGS][MAX_STRING_LENGTH];
    char config_pairs[MAX_SETTINGS][MAX_STRING_LENGTH];

    /*
    layoutState = settingsfile.value("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        debug_message("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
     }
    */

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QString settings_dir = QDir::homePath() + "/.embroidermodder2";
//    settings["prompt_save_history_filename"] = node_qstr(QDir::homePath() + "/.embroidermodder2/prompt.log");
#else
    QString settings_dir = qApp->applicationDirPath();
//    settings["prompt_save_history_filename"] = node_qstr(appDir + "prompt.log");
#endif

    std::string fname = settings_dir.toStdString() + "/em2_settings.ini";
    FILE *f = fopen(fname.c_str(), "r");
    if (!f) {
        printf("WARNING: Failed to open settings file (%s), continuing with defaults.",
            fname.c_str());
        for (int i=0; i<MAX_SETTINGS; i++) {
            strcpy(config[i], default_settings[i]);
        }
    }
    else {
        char c;
        int line = 0;
        int line_pos = 0;
        while (fread(&c, 1, 1, f)) {
            if (c == '\r') {
                continue;
            }
            config[line][line_pos] = c;
            line_pos++;
            if (c != '\n') {
                continue;
            }
            config[line][line_pos] = 0;
            line++;
            line_pos = 0;
            if (line == MAX_SETTINGS) {
                puts("WARNING too many lines in the settings file.");
                break;
            }
        }
        config[line][line_pos] = 0;
        line++;
        for (; line<MAX_SETTINGS; line++) {
            config[line][0] = 0;
        }
        fclose(f);
    }

    for (int line=0; line<MAX_SETTINGS; line++) {
        int eq_pos = str_contains(config[line], '=');
        if (eq_pos < 0) {
            continue;
        }
        config[line][eq_pos] = 0;
        std::string key(config[line]);
        std::string value(config[line]+eq_pos+1);
        char c = value.c_str()[0];
        if (c >= '0' && c <= '9') {
            if (str_contains(config[line]+eq_pos+1, '.') >= 0) {
                settings[key] = node_real(std::stof(value));
            }
            else {
                settings[key] = node_int(std::stoi(value));
            }
        }
        else {
            settings[key] = node_str(value);
        }
    }

    debug_message("Configuration loaded.");

    return 1;
}

/* Write settings to file.
 *
 * This file needs to be read from the users home directory to ensure it is writable
 */
void
write_settings(void)
{
    debug_message("Writing Settings...");
    String settingsPath;

    settings["window_position_x"] = node_int((int)_mainWin->pos().x());
    settings["window_position_y"] = node_int((int)_mainWin->pos().y());
    settings["window_size_x"] = node_int((int)_mainWin->size().width());
    settings["window_size_y"] = node_int((int)_mainWin->size().height());

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    settingsPath = QDir::homePath().toStdString() + "/.embroidermodder2/settings.ini";
#else
    settingsPath = qApp->applicationDirPath().toStdString() + "/settings.ini";
#endif
    std::ofstream file;
    file.open(settingsPath);

    for (auto iter = settings.begin(); iter != settings.end(); iter++) {
        file << iter->first << "=";
        switch (iter->second.type) {
        case NODE_INT:
            file << iter->second.i;
            break;
        case NODE_REAL:
            file << iter->second.r;
            break;
        case NODE_STRING:
            file << "\"" << iter->second.s << "\"";
            break;
        default:
            break;
        }
        file << std::endl;
    }
    file.close();
}

/* Create settings dialog object. */
Settings_Dialog::Settings_Dialog(QString  showTab, QWidget* parent) : QDialog(parent)
{
    setMinimumSize(750,550);

    tabWidget = new QTabWidget(this);

    //TODO: Add icons to tabs
    tabWidget->addTab(createTabGeneral(), translate_str("General"));
    tabWidget->addTab(createTabFilesPaths(), translate_str("Files/Paths"));
    tabWidget->addTab(createTabDisplay(), translate_str("Display"));
    tabWidget->addTab(createTabPrompt(), translate_str("Prompt"));
    tabWidget->addTab(createTabOpenSave(), translate_str("Open/Save"));
    tabWidget->addTab(createTabPrinting(), translate_str("Printing"));
    tabWidget->addTab(createTabSnap(), translate_str("Snap"));
    tabWidget->addTab(createTabGridRuler(), translate_str("Grid/Ruler"));
    tabWidget->addTab(createTabOrthoPolar(), translate_str("Ortho/Polar"));
    tabWidget->addTab(createTabQuickSnap(), translate_str("QuickSnap"));
    tabWidget->addTab(createTabQuickTrack(), translate_str("QuickTrack"));
    tabWidget->addTab(createTabLineWeight(), translate_str("LineWeight"));
    tabWidget->addTab(createTabSelection(), translate_str("Selection"));

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

    setWindowTitle(translate_str("Settings"));

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

    //Language
    QGroupBox* groupBoxLanguage = new QGroupBox(translate_str("Language"), widget);

    make_editing_copy(general_props);

    QLabel* labelLanguage = new QLabel(translate_str("Language (Requires Restart)"), groupBoxLanguage);
    QComboBox* comboBoxLanguage = new QComboBox(groupBoxLanguage);
    dialog["general_language"] = node_qstr(get_qstr(settings, "general_language").toLower());
    comboBoxLanguage->addItem("Default");
    comboBoxLanguage->addItem("System");
    comboBoxLanguage->insertSeparator(2);
    QDir trDir(qApp->applicationDirPath());
    trDir.cd("translations");
    foreach(QString dirName, trDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        dirName[0] = dirName[0].toUpper();
        comboBoxLanguage->addItem(dirName);
    }
    QString current = get_qstr(dialog, "general_language");
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    /*
    TODO: FIX
    connect(
        comboBoxLanguage,
        SIGNAL(currentIndexChanged(QString)),
        this,
        [=](const char *lang) { dialog["general_language"] = node_qstr(QString::fromStdString(lang).toLower()); }
    );
    */

    QVBoxLayout* vboxLayoutLanguage = new QVBoxLayout(groupBoxLanguage);
    vboxLayoutLanguage->addWidget(labelLanguage);
    vboxLayoutLanguage->addWidget(comboBoxLanguage);
    groupBoxLanguage->setLayout(vboxLayoutLanguage);

    //Icons
    QGroupBox* groupBoxIcon = new QGroupBox(translate_str("Icons"), widget);

    QLabel* labelIconTheme = new QLabel(translate_str("Icon Theme"), groupBoxIcon);
    QComboBox* comboBoxIconTheme = new QComboBox(groupBoxIcon);
    QDir dir(qApp->applicationDirPath());
    dir.cd("icons");
    foreach(QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        comboBoxIconTheme->addItem(QIcon("icons/" + dirName + "/" + "theme" + ".png"), dirName);
    }
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(get_qstr(dialog, "general_icon_theme")));
    /*
    TODO: FIX
    connect(
        comboBoxIconTheme,
        &QComboBox::currentIndexChanged,
        this,
        [=](int i) { dialog["general_icon_theme"] = node_qstr(theme); }
    );
    */

    QLabel* labelIconSize = new QLabel(translate_str("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    QString theme = get_qstr(dialog, "general_icon_theme");
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon16"  + ".png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon24"  + ".png"), "Small", 24);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon32"  + ".png"), "Medium", 32);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon48"  + ".png"), "Large", 48);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon64"  + ".png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon128" + ".png"), "I'm Blind", 128);
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(get_int(dialog, "general_icon_size")));
    connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIconSizeCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutIcon = new QVBoxLayout(groupBoxIcon);
    vboxLayoutIcon->addWidget(labelIconTheme);
    vboxLayoutIcon->addWidget(comboBoxIconTheme);
    vboxLayoutIcon->addWidget(labelIconSize);
    vboxLayoutIcon->addWidget(comboBoxIconSize);
    groupBoxIcon->setLayout(vboxLayoutIcon);

    //Mdi Background
    QGroupBox* groupBoxMdiBG = new QGroupBox(translate_str("Background"), widget);

    QCheckBox* checkBoxMdiBGUseLogo = new QCheckBox(translate_str("Use Logo"), groupBoxMdiBG);
    checkBoxMdiBGUseLogo->setChecked(get_bool(preview, "general_mdi_bg_use_logo"));
    connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)));

    QPushButton* buttonMdiBGLogo = new QPushButton(translate_str("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(get_bool(dialog, "general_mdi_bg_use_logo"));
    connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseTexture = new QCheckBox(translate_str("Use Texture"), groupBoxMdiBG);
    checkBoxMdiBGUseTexture->setChecked(get_bool(preview, "general_mdi_bg_use_texture"));
    connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)));

    QPushButton* buttonMdiBGTexture = new QPushButton(translate_str("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(get_bool(dialog, "general_mdi_bg_use_texture"));
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture, SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseColor = make_checkbox(groupBoxMdiBG, "dialog",
        "Use Color", "blank", "general_mdi_bg_use_color");

    QPushButton* buttonMdiBGColor = new QPushButton(translate_str("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setEnabled(get_bool(dialog, "general_mdi_bg_use_color"));
    QPixmap mdiBGPix(16,16);
    mdiBGPix.fill(QColor(get_uint(preview, "general_mdi_bg_color")));
    buttonMdiBGColor->setIcon(QIcon(mdiBGPix));
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

    //Tips
    QGroupBox* groupBoxTips = new QGroupBox(translate_str("Tips"), widget);

    QCheckBox* checkBoxTipOfTheDay = make_checkbox(groupBoxTips, "dialog",
        "Show Tip of the Day on startup", "blank", "general_tip_of_the_day");

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    //Help Browser
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(translate_str("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(translate_str("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(get_bool(settings, "general_system_help_browser"));
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(translate_str("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!get_bool(settings, "general_system_help_browser"));
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
QWidget*
Settings_Dialog::createTabDisplay()
{
    QWidget* widget = new QWidget(this);

    //Rendering
    //TODO: Review OpenGL and Rendering settings for future inclusion
    QGroupBox* groupBoxRender = new QGroupBox(translate_str("Rendering"), widget);

    make_editing_copy(display_props);

    QCheckBox* checkBoxUseOpenGL = make_checkbox(groupBoxRender, "dialog",
        "Use OpenGL", "blank", "display_use_opengl");
    QCheckBox* checkBoxRenderHintAA = make_checkbox(groupBoxRender, "dialog",
        "Antialias", "blank", "display_renderhint_aa");
    QCheckBox* checkBoxRenderHintTextAA = make_checkbox(groupBoxRender, "dialog",
        "Antialias Text", "blank", "display_renderhint_text_aa");
    QCheckBox* checkBoxRenderHintSmoothPix = make_checkbox(groupBoxRender, "dialog",
        "Smooth Pixmap", "blank", "display_renderhint_smooth_pix");
    QCheckBox* checkBoxRenderHintHighAA = make_checkbox(groupBoxRender, "dialog",
        "High Quality Antialiasing (OpenGL)", "blank", "display_renderhint_high_aa");
    QCheckBox* checkBoxRenderHintNonCosmetic = make_checkbox(groupBoxRender, "dialog",
        "Non Cosmetic", "blank", "display_renderhint_noncosmetic");

    QVBoxLayout* vboxLayoutRender = new QVBoxLayout(groupBoxRender);
    vboxLayoutRender->addWidget(checkBoxUseOpenGL);
    vboxLayoutRender->addWidget(checkBoxRenderHintAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintTextAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintSmoothPix);
    vboxLayoutRender->addWidget(checkBoxRenderHintHighAA);
    vboxLayoutRender->addWidget(checkBoxRenderHintNonCosmetic);
    groupBoxRender->setLayout(vboxLayoutRender);

    //ScrollBars
    QGroupBox* groupBoxScrollBars = new QGroupBox(translate_str("ScrollBars"), widget);

    QCheckBox* checkBoxShowScrollBars = make_checkbox(groupBoxScrollBars, "dialog",
        "Show ScrollBars", "blank", "display_show_scrollbars");

    QLabel* labelScrollBarWidget = new QLabel(translate_str("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    for (auto iter = actionHash.begin(); iter!=actionHash.end(); iter++) {
        QAction* action = iter->second;
        if (action) {
            comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        }
    }
    comboBoxScrollBarWidget->setCurrentIndex(get_int(dialog, "display_scrollbar_widget_num"));
    connect(comboBoxScrollBarWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxScrollBarWidgetCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutScrollBars = new QVBoxLayout(groupBoxScrollBars);
    vboxLayoutScrollBars->addWidget(checkBoxShowScrollBars);
    vboxLayoutScrollBars->addWidget(labelScrollBarWidget);
    vboxLayoutScrollBars->addWidget(comboBoxScrollBarWidget);
    groupBoxScrollBars->setLayout(vboxLayoutScrollBars);

    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(translate_str("Colors"), widget);

    QLabel* labelCrossHairColor = new QLabel(translate_str("Crosshair Color"), groupBoxColor);
    QPushButton* buttonCrossHairColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    QPixmap crosshairPix(16,16);
    crosshairPix.fill(QColor(get_uint(preview, "display_crosshair_color")));
    buttonCrossHairColor->setIcon(QIcon(crosshairPix));
    connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()));

    QLabel* labelBGColor = new QLabel(translate_str("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    QPixmap bgPix(16,16);
    bgPix.fill(QColor(get_uint(preview, "display_bg_color")));
    buttonBGColor->setIcon(QIcon(bgPix));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()));

    QLabel* labelSelectBoxLeftColor = new QLabel(translate_str("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    QPixmap sBoxLCPix(16,16);
    sBoxLCPix.fill(QColor(get_uint(preview, "display_selectbox_left_color")));
    buttonSelectBoxLeftColor->setIcon(QIcon(sBoxLCPix));
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()));

    QLabel* labelSelectBoxLeftFill = new QLabel(translate_str("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = new QPushButton(translate_str("Choose"), groupBoxColor);
    QPixmap sBoxLFPix(16,16);
    sBoxLFPix.fill(QColor(get_uint(preview, "display_selectbox_left_fill")));
    buttonSelectBoxLeftFill->setIcon(QIcon(sBoxLFPix));
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()));

    QLabel* labelSelectBoxRightColor = new QLabel(translate_str("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    QPixmap sBoxRCPix(16,16);
    sBoxRCPix.fill(QColor(get_uint(preview, "display_selectbox_right_color")));
    buttonSelectBoxRightColor->setIcon(QIcon(sBoxRCPix));
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()));

    QLabel* labelSelectBoxRightFill = new QLabel(translate_str("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = new QPushButton(translate_str("Choose"), groupBoxColor);
    QPixmap sBoxRFPix(16,16);
    sBoxRFPix.fill(QColor(get_uint(preview, "display_selectbox_right_fill")));
    buttonSelectBoxRightFill->setIcon(QIcon(sBoxRFPix));
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()));

    QLabel* labelSelectBoxAlpha = new QLabel(translate_str("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    spinBoxSelectBoxAlpha->setValue(get_int(preview, "display_selectbox_alpha"));
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
    QGroupBox* groupBoxZoom = new QGroupBox(translate_str("Zoom"), widget);

    QLabel* labelZoomScaleIn = new QLabel(translate_str("Zoom In Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleIn = new QDoubleSpinBox(groupBoxZoom);
    spinBoxZoomScaleIn->setValue(get_real(dialog, "display_zoomscale_in"));
    spinBoxZoomScaleIn->setSingleStep(0.01);
    spinBoxZoomScaleIn->setRange(1.01, 10.00);
    connect(
        spinBoxZoomScaleIn,
        &QDoubleSpinBox::valueChanged,
        this,
        [=](double value) { dialog["display_zoomscale_in"] = node_real((EmbReal)value); }
    );

    QLabel* labelZoomScaleOut = new QLabel(translate_str("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = new QDoubleSpinBox(groupBoxZoom);
    spinBoxZoomScaleOut->setValue(get_real(dialog, "display_zoomscale_out"));
    spinBoxZoomScaleOut->setSingleStep(0.01);
    spinBoxZoomScaleOut->setRange(0.01, 0.99);
    connect(
        spinBoxZoomScaleOut,
        &QDoubleSpinBox::valueChanged,
        this,
        [=](double value) { dialog["display_zoomscale_out"] = node_real((EmbReal)value); }
    );

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

/* TODO: finish prompt options. */
QWidget*
Settings_Dialog::createTabPrompt()
{
    QWidget* widget = new QWidget(this);

    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(translate_str("Colors"), widget);

    make_editing_copy(prompt_props);

    QLabel* labelTextColor = new QLabel(translate_str("Text Color"), groupBoxColor);
    QPushButton* buttonTextColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    QPixmap pix(16,16);
    pix.fill(QColor(get_uint(preview, "prompt_text_color")));
    buttonTextColor->setIcon(QIcon(pix));
    connect(buttonTextColor, SIGNAL(clicked()), this, SLOT(choosePromptTextColor()));

    QLabel* labelBGColor = new QLabel(translate_str("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    QPixmap pixx(16,16);
    pixx.fill(QColor(get_uint(preview, "prompt_background_color")));
    buttonBGColor->setIcon(QIcon(pixx));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(choosePromptBackgroundColor()));

    QGridLayout* gridLayoutColor = new QGridLayout(widget);
    gridLayoutColor->addWidget(labelTextColor, 0, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonTextColor, 0, 1, Qt::AlignRight);
    gridLayoutColor->addWidget(labelBGColor, 1, 0, Qt::AlignLeft);
    gridLayoutColor->addWidget(buttonBGColor, 1, 1, Qt::AlignRight);
    groupBoxColor->setLayout(gridLayoutColor);

    //TODO: Tweak the Prompt Font ComboBoxes so they work properly
    //Font
    QGroupBox* groupBoxFont = new QGroupBox(translate_str("Font"), widget);

    QLabel* labelFontFamily = new QLabel(translate_str("Font Family"), groupBoxFont);
    QFontComboBox* comboBoxFontFamily = new QFontComboBox(groupBoxFont);
    comboBoxFontFamily->setCurrentFont(QFont(get_qstr(preview, "prompt_font_family")));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(QString)));
    QLabel* labelFontStyle = new QLabel(translate_str("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(get_qstr(preview, "prompt_font_style"));
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(QString)));
    QLabel* labelFontSize = new QLabel(translate_str("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    spinBoxFontSize->setValue(get_real(preview, "prompt_font_size"));
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
    QGroupBox* groupBoxHistory = new QGroupBox(translate_str("History"), widget);

    QCheckBox* checkBoxPromptSaveHistory = make_checkbox(groupBoxHistory, "dialog",
        "Save History", "blank", "prompt_save_history");
    QCheckBox* checkBoxPromptSaveHistoryAsHtml = make_checkbox(groupBoxHistory, "dialog",
        "Save As HTML", "blank", "prompt_save_history_as_html");

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

/* . */
QCheckBox*
Settings_Dialog::create_checkbox(QGroupBox *groupbox, std::string label)
{
    QCheckBox* checkbox = new QCheckBox(label.c_str(), groupbox);
    std::string filter = "*." + label;
    checkbox->setChecked(get_qstr(dialog, "opensave_custom_filter").contains(filter.c_str(), Qt::CaseInsensitive));
    connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)));
    return checkbox;
}

/* TODO: finish open/save options. */
QWidget*
Settings_Dialog::createTabOpenSave()
{
    QWidget* widget = new QWidget(this);

    //Custom Filter
    QGroupBox* groupBoxCustomFilter = new QGroupBox(translate_str("Custom Filter"), widget);
    groupBoxCustomFilter->setEnabled(false); //TODO: Fixup custom filter

    dialog["opensave_custom_filter"] = settings["opensave_custom_filter"];

    std::unordered_map<std::string, QCheckBox*> checkBoxCustomFilter;
    int n_extensions = (int)extensions.size();

    for (int i=0; i<n_extensions; i++) {
        checkBoxCustomFilter[extensions[i]] = create_checkbox(groupBoxCustomFilter, extensions[i]);
    }

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(translate_str("Select All"), groupBoxCustomFilter);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()));
    for (int i=0; i<n_extensions; i++) {
        connect(
            this,
            SIGNAL(buttonCustomFilterSelectAll(bool)),
            checkBoxCustomFilter[extensions[i]],
            SLOT(setChecked(bool))
        );
    }

    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", groupBoxCustomFilter);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()));
    for (int i=0; i<n_extensions; i++) {
        connect(
            this,
            SIGNAL(buttonCustomFilterClearAll(bool)),
            checkBoxCustomFilter[extensions[i]],
            SLOT(setChecked(bool))
        );
    }

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    for (int j=0; j<10; j++)
    for (int i=0; i<10; i++) {
        if (10*j+i >= n_extensions) {
            break;
        }
        std::string ext = extensions[10*j+i];
        gridLayoutCustomFilter->addWidget(checkBoxCustomFilter[ext], i, j, Qt::AlignLeft);
    }
    gridLayoutCustomFilter->addWidget(buttonCustomFilterSelectAll, 0, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterClearAll, 1, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->setLayout(gridLayoutCustomFilter);

    if (get_qstr(dialog, "opensave_custom_filter").contains("supported", Qt::CaseInsensitive)) buttonCustomFilterSelectAllClicked();

    //Opening
    QGroupBox* groupBoxOpening = new QGroupBox(translate_str("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(translate_str("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    //TODO: Add a button to clear the recent history.

    dialog["opensave_recent_max_files"] = settings["opensave_recent_max_files"];

    QLabel* labelRecentMaxFiles = new QLabel(translate_str("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    spinBoxRecentMaxFiles->setValue(get_int(dialog, "opensave_recent_max_files"));
    connect(
        spinBoxRecentMaxFiles,
        &QSpinBox::valueChanged,
        this,
        [=](int value) { dialog["opensave_recent_max_files"] = node_int(value); }
    );

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
    QGroupBox* groupBoxSaving = new QGroupBox(translate_str("File Save"), widget);

    QComboBox* comboBoxSaveFormat = new QComboBox(groupBoxSaving);

    QCheckBox* checkBoxSaveThumbnail = new QCheckBox(translate_str("Save Thumbnails"), groupBoxSaving);
    checkBoxSaveThumbnail->setChecked(false);

    QCheckBox* checkBoxAutoSave = new QCheckBox(translate_str("AutoSave"), groupBoxSaving);
    checkBoxAutoSave->setChecked(false);

    QVBoxLayout* vboxLayoutSaving = new QVBoxLayout(groupBoxSaving);
    vboxLayoutSaving->addWidget(comboBoxSaveFormat);
    vboxLayoutSaving->addWidget(checkBoxSaveThumbnail);
    vboxLayoutSaving->addWidget(checkBoxAutoSave);
    groupBoxSaving->setLayout(vboxLayoutSaving);

    //Trimming
    QGroupBox* groupBoxTrim = new QGroupBox(translate_str("Trimming"), widget);

    QLabel* labelTrimDstNumJumps = new QLabel(translate_str("DST Only: Minimum number of jumps to trim"), groupBoxTrim);
    QSpinBox* spinBoxTrimDstNumJumps = new QSpinBox(groupBoxTrim);
    spinBoxTrimDstNumJumps->setRange(1, 20);
    dialog["opensave_trim_dst_num_jumps"] = settings["opensave_trim_dst_num_jumps"];
    spinBoxTrimDstNumJumps->setValue(get_int(dialog, "opensave_trim_dst_num_jumps"));
    connect(
        spinBoxTrimDstNumJumps,
        &QSpinBox::valueChanged,
        this,
        [=](int value){ dialog["opensave_trim_dst_num_jumps"] = node_int(value); }
    );

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
    QGroupBox* groupBoxDefaultPrinter = new QGroupBox(translate_str("Default Printer"), widget);

    QRadioButton* radioButtonUseSame = new QRadioButton(translate_str("Use as default device"), groupBoxDefaultPrinter);
    radioButtonUseSame->setChecked(!get_bool(settings, "printing_use_last_device"));
    QRadioButton* radioButtonUseLast = new QRadioButton(translate_str("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(get_bool(settings, "printing_use_last_device"));

    QComboBox* comboBoxDefaultDevice = new QComboBox(groupBoxDefaultPrinter);
    QList<QPrinterInfo> listAvailPrinters = QPrinterInfo::availablePrinters();
    foreach (QPrinterInfo info, listAvailPrinters) {
        comboBoxDefaultDevice->addItem(_mainWin->create_icon("print"), info.printerName());
    }

    QVBoxLayout* vboxLayoutDefaultPrinter = new QVBoxLayout(groupBoxDefaultPrinter);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseSame);
    vboxLayoutDefaultPrinter->addWidget(comboBoxDefaultDevice);
    vboxLayoutDefaultPrinter->addWidget(radioButtonUseLast);
    groupBoxDefaultPrinter->setLayout(vboxLayoutDefaultPrinter);

    //Save Ink
    QGroupBox* groupBoxSaveInk = new QGroupBox(translate_str("Save Ink"), widget);

    dialog["printing_disable_bg"] = settings["printing_disable_bg"];

    QCheckBox* checkBoxDisableBG = make_checkbox(groupBoxSaveInk, "dialog",
        "Disable Background", "blank", "printing_disable_bg");

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


void
Settings_Dialog::create_float_spinbox(
    QGroupBox *gb,
    QGridLayout* gridLayout,
    const char *label_in,
    EmbReal single_step,
    EmbReal lower,
    EmbReal upper,
    String key,
    int row)
{
    QString label_object_name(label_in);
    QString spinbox_object_name(label_in);
    label_object_name = "label" + label_object_name.simplified().remove(' ');
    spinbox_object_name = "spinBox" + spinbox_object_name.simplified().remove(' ');
    QLabel* label = new QLabel(tr(label_in), gb);
    label->setObjectName(label_object_name);
    QDoubleSpinBox* spinBox = make_spinbox(gb, "dialog", spinbox_object_name, single_step,
        lower, upper, key);

    label->setEnabled(!get_bool(dialog, "grid_load_from_file"));
    spinBox->setEnabled(!get_bool(dialog, "grid_load_from_file"));

    bool visibility = (dialog["grid_type"].s == "Circular");
    label->setVisible(!visibility);
    spinBox->setVisible(!visibility);

    gridLayout->addWidget(label, row, 0, Qt::AlignLeft);
    gridLayout->addWidget(spinBox, row, 1, Qt::AlignRight);
}

QWidget* Settings_Dialog::createTabGridRuler()
{
    QWidget* widget = new QWidget(this);

    //Grid Misc
    QGroupBox* groupBoxGridMisc = new QGroupBox(translate_str("Grid Misc"), widget);

    dialog["grid_show_on_load"] = settings["grid_show_on_load"];
    dialog["grid_show_origin"] = settings["grid_show_origin"];
    dialog["grid_color_match_crosshair"] = settings["grid_color_match_crosshair"];

    if (get_bool(dialog, "grid_color_match_crosshair")) {
        dialog["grid_color"] = settings["display_crosshair_color"];
    }
    else {
        dialog["grid_color"] = settings["grid_color"];
    }
    preview["grid_color"] = dialog["grid_color"];
    accept_["grid_color"] = dialog["grid_color"];

    dialog["grid_load_from_file"] = settings["grid_load_from_file"];
    dialog["grid_type"] = settings["grid_type"];
    dialog["grid_center_on_origin"] = settings["grid_center_on_origin"];
    dialog["grid_center"] = settings["grid_center"];
    dialog["grid_size"] = settings["grid_size"];
    dialog["grid_spacing"] = settings["grid_spacing"];

    dialog["ruler_color"] = settings["ruler_color"];
    preview["ruler_color"] = dialog["ruler_color"];
    accept_["ruler_color"]  = dialog["ruler_color"];

    dialog["ruler_pixel_size"] = settings["ruler_pixel_size"];

    QCheckBox* checkBoxGridShowOnLoad = make_checkbox(groupBoxGridMisc, "dialog",
        "Initially show grid when loading a file", "blank", "grid_show_on_load");
    QCheckBox* checkBoxGridShowOrigin = make_checkbox(groupBoxGridMisc, "dialog",
        "Show the origin when the grid is enabled", "blank", "grid_show_origin");

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(checkBoxGridShowOrigin, 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = new QGroupBox(translate_str("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(translate_str("Match grid color to crosshair color"), groupBoxGridColor);
    checkBoxGridColorMatchCrossHair->setChecked(get_bool(dialog, "grid_color_match_crosshair"));
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(translate_str("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(translate_str("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    QPixmap gridPix(16,16);
    gridPix.fill(QColor(get_uint(preview, "grid_color")));
    buttonGridColor->setIcon(QIcon(gridPix));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor->setEnabled(!get_bool(dialog, "grid_color_match_crosshair"));
    buttonGridColor->setEnabled(!get_bool(dialog, "grid_color_match_crosshair"));

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = new QGroupBox(translate_str("Grid Geometry"), widget);

    QCheckBox* checkBoxGridLoadFromFile = make_checkbox(groupBoxGridGeom, "dialog",
        "Set grid size from opened file", "blank", "grid_load_from_file");

    QLabel* labelGridType = new QLabel(translate_str("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(QString::fromStdString(dialog["grid_type"].s)));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxGridTypeCurrentIndexChanged(QString)));

    QCheckBox* checkBoxGridCenterOnOrigin = make_checkbox(groupBoxGridGeom, "dialog",
        "Center the grid on the origin", "blank", "grid_center_on_origin");
    checkBoxGridCenterOnOrigin->setObjectName("checkBoxGridCenterOnOrigin");

    labelGridType->setEnabled(!get_bool(dialog, "grid_load_from_file"));
    comboBoxGridType->setEnabled(!get_bool(dialog, "grid_load_from_file"));
    checkBoxGridCenterOnOrigin->setEnabled(!get_bool(dialog, "grid_load_from_file"));

    QGridLayout* gridLayoutGridGeom = new QGridLayout(groupBoxGridGeom);
    gridLayoutGridGeom->addWidget(checkBoxGridLoadFromFile, 0, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(labelGridType, 1, 0, Qt::AlignLeft);
    gridLayoutGridGeom->addWidget(comboBoxGridType, 1, 1, Qt::AlignRight);
    gridLayoutGridGeom->addWidget(checkBoxGridCenterOnOrigin, 2, 0, Qt::AlignLeft);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Center X", 1.0, -1000.0, 1000.0, "grid_center_x", 3);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Center Y", 1.0, -1000.0, 1000.0, "grid_center_y", 4);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Size X", 1.0, 1.0, 1000.0, "grid_size_x", 5);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Size Y", 1.0, 1.0, 1000.0, "grid_size_y", 6);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Spacing X", 1.0, 0.001, 1000.0, "grid_spacing_x", 7);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Spacing Y", 1.0, 0.001, 1000.0, "grid_spacing_y", 8);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Size Radius", 1.0, 1.0, 1000.0, "grid_size_radius", 9);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Spacing Radius", 1.0, 1.0, 1000.0, "grid_spacing_radius", 10);
    create_float_spinbox(groupBoxGridGeom, gridLayoutGridGeom,
        "Grid Spacing Angle", 1.0, 0.001, 1000.0, "grid_spacing_angle", 11);
    groupBoxGridGeom->setLayout(gridLayoutGridGeom);

    //Ruler Misc
    QGroupBox* groupBoxRulerMisc = new QGroupBox(translate_str("Ruler Misc"), widget);

    dialog["ruler_show_on_load"] = settings["ruler_show_on_load"];
    dialog["ruler_metric"] = settings["ruler_metric"];

    QCheckBox* checkBoxRulerShowOnLoad = new QCheckBox(translate_str("Initially show ruler when loading a file"), groupBoxRulerMisc);
    checkBoxRulerShowOnLoad->setChecked(get_bool(dialog, "ruler_show_on_load"));
    connect(checkBoxRulerShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRulerShowOnLoadStateChanged(int)));

    QLabel* labelRulerMetric = new QLabel(translate_str("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric", true);
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(get_bool(dialog, "ruler_metric")));
    connect(comboBoxRulerMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRulerMetricCurrentIndexChanged(int)));

    QGridLayout* gridLayoutRulerMisc = new QGridLayout(widget);
    gridLayoutRulerMisc->addWidget(checkBoxRulerShowOnLoad, 0, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(labelRulerMetric, 1, 0, Qt::AlignLeft);
    gridLayoutRulerMisc->addWidget(comboBoxRulerMetric, 1, 1, Qt::AlignRight);
    groupBoxRulerMisc->setLayout(gridLayoutRulerMisc);

    //Ruler Color
    QGroupBox* groupBoxRulerColor = new QGroupBox(translate_str("Ruler Color"), widget);

    QLabel* labelRulerColor = new QLabel(translate_str("Ruler Color"), groupBoxRulerColor);
    labelRulerColor->setObjectName("labelRulerColor");
    QPushButton* buttonRulerColor = new QPushButton(translate_str("Choose"), groupBoxRulerColor);
    buttonRulerColor->setObjectName("buttonRulerColor");
    QPixmap rulerPix(16,16);
    rulerPix.fill(QColor(get_uint(preview, "ruler_color")));
    buttonRulerColor->setIcon(QIcon(rulerPix));
    connect(buttonRulerColor, SIGNAL(clicked()), this, SLOT(chooseRulerColor()));

    QGridLayout* gridLayoutRulerColor = new QGridLayout(widget);
    gridLayoutRulerColor->addWidget(labelRulerColor, 1, 0, Qt::AlignLeft);
    gridLayoutRulerColor->addWidget(buttonRulerColor, 1, 1, Qt::AlignRight);
    groupBoxRulerColor->setLayout(gridLayoutRulerColor);

    //Ruler Geometry
    QGroupBox* groupBoxRulerGeom = new QGroupBox(translate_str("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = new QLabel(translate_str("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QDoubleSpinBox* spinBoxRulerPixelSize = make_spinbox(groupBoxRulerGeom,
        "dialog", "spinBoxRulerPixelSize", 1.0, 20.0, 100.0, "ruler_pixel_size");

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

QWidget*
Settings_Dialog::createTabOrthoPolar()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}


QWidget*
Settings_Dialog::createTabQuickSnap()
{
    QWidget* widget = new QWidget(this);

    //QSnap Locators
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(translate_str("Locators Used"), widget);

    make_editing_copy(quick_snap_props);

    std::vector<QCheckBox*> checkboxes = {
        make_checkbox(groupBoxQSnapLoc, "dialog", "Endpoint", "locator-snaptoendpoint", "quicksnap_endpoint"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Midpoint", "locator-snaptomidpoint", "quicksnap_midpoint"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Center", "locator-snaptocenter", "quicksnap_center"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Node", "locator-snaptonode", "quicksnap_node"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Quadrant", "locator-snaptoquadrant", "quicksnap_quadrant"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Intersection", "locator-snaptointersection", "quicksnap_intersection"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Extension", "locator-snaptoextension", "quicksnap_extension"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Insertion", "locator-snaptoinsert", "quicksnap_insertion"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Perpendicular", "locator-snaptoperpendicular", "quicksnap_perpendicular"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Tangent", "locator-snaptotangent", "quicksnap_tangent"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Nearest", "locator-snaptonearest", "quicksnap_nearest"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Apparent Intersection", "locator-snaptoapparentintersection", "quicksnap_apparent"),
        make_checkbox(groupBoxQSnapLoc, "dialog", "Parallel", "locator-snaptoparallel", "quicksnap_parallel")
    };
    int n_checkboxes = (int)checkboxes.size();

    QPushButton* buttonQSnapSelectAll = new QPushButton(translate_str("Select All"), groupBoxQSnapLoc);
    connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(buttonQSnapSelectAllClicked()));
    for (int i=0; i<n_checkboxes; i++) {
        connect(this, SIGNAL(buttonQSnapSelectAll(bool)), checkboxes[i], SLOT(setChecked(bool)));
    }

    QPushButton* buttonQSnapClearAll = new QPushButton(translate_str("Clear All"), groupBoxQSnapLoc);
    connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()));
    for (int i=0; i<n_checkboxes; i++) {
        connect(this, SIGNAL(buttonQSnapClearAll(bool)), checkboxes[i], SLOT(setChecked(bool)));
    }

    QGridLayout* gridLayoutQSnap = new QGridLayout(groupBoxQSnapLoc);
    for (int i=0; i<n_checkboxes; i++) {
        gridLayoutQSnap->addWidget(checkboxes[i], i%7, i/7, Qt::AlignLeft);
    }
    gridLayoutQSnap->addWidget(buttonQSnapSelectAll, 0, 2, Qt::AlignLeft);
    gridLayoutQSnap->addWidget(buttonQSnapClearAll, 1, 2, Qt::AlignLeft);
    gridLayoutQSnap->setColumnStretch(2,1);
    groupBoxQSnapLoc->setLayout(gridLayoutQSnap);

    //QSnap Visual Config
    QGroupBox* groupBoxQSnapVisual = new QGroupBox(translate_str("Visual Configuration"), widget);

    QLabel* labelQSnapLocColor = new QLabel(translate_str("Locator Color"), groupBoxQSnapVisual);
    QComboBox* comboBoxQSnapLocColor = new QComboBox(groupBoxQSnapVisual);
    addColorsToComboBox(comboBoxQSnapLocColor);
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(get_uint(dialog, "quicksnap_locator_color")));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(translate_str("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    sliderQSnapLocSize->setValue(get_real(dialog, "quicksnap_locator_size"));
    auto f = [=](int x) { dialog["quicksnap_locator_size"] = node_real(x); };
    connect(sliderQSnapLocSize, SIGNAL(valueChanged(int)), this, SLOT(f));

    QVBoxLayout* vboxLayoutQSnapVisual = new QVBoxLayout(groupBoxQSnapVisual);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(comboBoxQSnapLocColor);
    vboxLayoutQSnapVisual->addWidget(labelQSnapLocSize);
    vboxLayoutQSnapVisual->addWidget(sliderQSnapLocSize);
    groupBoxQSnapVisual->setLayout(vboxLayoutQSnapVisual);

    //QSnap Sensitivity Config
    QGroupBox* groupBoxQSnapSensitivity = new QGroupBox(translate_str("Sensitivity"), widget);

    QLabel* labelQSnapApertureSize = new QLabel(translate_str("Aperture Size"), groupBoxQSnapSensitivity);
    QSlider* sliderQSnapApertureSize = new QSlider(Qt::Horizontal, groupBoxQSnapSensitivity);
    sliderQSnapApertureSize->setRange(1,20);
    sliderQSnapApertureSize->setValue(get_real(dialog, "quicksnap_aperture_size"));
    connect(sliderQSnapApertureSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog["quicksnap_aperture_size"] = node_real(x); } ));

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

QWidget*
Settings_Dialog::createTabQuickTrack()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

QWidget*
Settings_Dialog::createTabLineWeight()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    //Misc
    QGroupBox* groupBoxLwtMisc = new QGroupBox(translate_str("LineWeight Misc"), widget);

    QGraphicsScene* s = activeScene();

    QCheckBox* checkBoxShowLwt = new QCheckBox(translate_str("Show LineWeight"), groupBoxLwtMisc);
    if (s) {
        dialog["lwt_show_lwt"] = node_int(s->property("ENABLE_LWT").toBool());
    }
    else {
        dialog["lwt_show_lwt"] = settings["lwt_show_lwt"];
    }
    preview["lwt_show_lwt"] = dialog["lwt_show_lwt"];
    checkBoxShowLwt->setChecked(get_bool(preview, "lwt_show_lwt"));
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(translate_str("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if (s) {
        dialog["lwt_real_render"] = node_int(s->property("ENABLE_REAL").toBool());
    }
    else  {
        dialog["lwt_real_render"] = settings["lwt_real_render"];
    }
    preview["lwt_real_render"] = dialog["lwt_real_render"];
    checkBoxRealRender->setChecked(get_bool(preview, "lwt_real_render"));
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(get_bool(dialog, "lwt_show_lwt"));

    QLabel* labelDefaultLwt = new QLabel(translate_str("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //TODO: remove later
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    dialog["lwt_default_lwt"] = settings["lwt_default_lwt"];
    //TODO: populate the comboBox and set the initial value
    comboBoxDefaultLwt->addItem(QString().setNum(get_real(dialog, "lwt_default_lwt"), 'F', 2).append(" mm"), get_real(dialog, "lwt_default_lwt"));
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

QWidget*
Settings_Dialog::createTabSelection()
{
    QWidget* widget = new QWidget(this);

    //Selection Modes
    QGroupBox* groupBoxSelectionModes = new QGroupBox(translate_str("Modes"), widget);

    dialog["selection_mode_pickfirst"] = settings["selection_mode_pickfirst"];
    dialog["selection_mode_pickadd"] = settings["selection_mode_pickadd"];
    dialog["selection_mode_pickdrag"] = settings["selection_mode_pickdrag"];
    dialog["selection_coolgrip_color"] = settings["selection_coolgrip_color"];
    dialog["selection_hotgrip_color"] = settings["selection_hotgrip_color"];
    dialog["selection_grip_size"] = settings["selection_grip_size"];
    dialog["selection_pickbox_size"] = settings["selection_pickbox_size"];

    QCheckBox* checkBoxSelectionModePickFirst = new QCheckBox(translate_str("Allow Preselection (PickFirst)"), groupBoxSelectionModes);
    checkBoxSelectionModePickFirst->setChecked(get_bool(dialog, "selection_mode_pickfirst"));
    checkBoxSelectionModePickFirst->setChecked(true);
    checkBoxSelectionModePickFirst->setEnabled(false); //TODO: Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this,
        SLOT([=](int x) {dialog["selection_mode_pickfirst"] = x; } ));

    QCheckBox* checkBoxSelectionModePickAdd = make_checkbox(groupBoxSelectionModes, "dialog",
        "Add to Selection (PickAdd)", "blank", "selection_mode_pickadd");

    QCheckBox* checkBoxSelectionModePickDrag = new QCheckBox(translate_str("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    checkBoxSelectionModePickDrag->setChecked(get_bool(dialog, "selection_mode_pickdrag"));
    checkBoxSelectionModePickDrag->setChecked(false);
    checkBoxSelectionModePickDrag->setEnabled(false); //TODO: Remove this line when this functionality is available
    connect(checkBoxSelectionModePickDrag, SIGNAL(stateChanged(int)), this,
        SLOT([=](int x) {dialog["selection_mode_pickdrag"] = x; } ));

    QVBoxLayout* vboxLayoutSelectionModes = new QVBoxLayout(groupBoxSelectionModes);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickFirst);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickAdd);
    vboxLayoutSelectionModes->addWidget(checkBoxSelectionModePickDrag);
    groupBoxSelectionModes->setLayout(vboxLayoutSelectionModes);

    //Selection Colors
    QGroupBox* groupBoxSelectionColors = new QGroupBox(translate_str("Colors"), widget);

    QLabel* labelCoolGripColor = new QLabel(translate_str("Cool Grip (Unselected)"), groupBoxSelectionColors);
    QComboBox* comboBoxCoolGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxCoolGripColor);
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(get_uint(dialog, "selection_coolgrip_color")));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this,
        SLOT([=](int x) { dialog["selection_coolgrip_color"] = node_int(x); } ));

    QLabel* labelHotGripColor = new QLabel(translate_str("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(get_uint(dialog, "selection_hotgrip_color")));
    connect(comboBoxHotGripColor, SIGNAL(currentIndexChanged(int)), this,
        SLOT([=](int x) { dialog["selection_hotgrip_color"] = node_int(x); } ));

    QVBoxLayout* vboxLayoutSelectionColors = new QVBoxLayout(groupBoxSelectionColors);
    vboxLayoutSelectionColors->addWidget(labelCoolGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxCoolGripColor);
    vboxLayoutSelectionColors->addWidget(labelHotGripColor);
    vboxLayoutSelectionColors->addWidget(comboBoxHotGripColor);
    groupBoxSelectionColors->setLayout(vboxLayoutSelectionColors);

    //Selection Sizes
    QGroupBox* groupBoxSelectionSizes = new QGroupBox(translate_str("Sizes"), widget);

    QLabel* labelSelectionGripSize = new QLabel(translate_str("Grip Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionGripSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionGripSize->setRange(1,20);
    sliderSelectionGripSize->setValue(get_int(dialog, "selection_grip_size"));
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog["selection_grip_size"] = node_int(x); } ));

    QLabel* labelSelectionPickBoxSize = new QLabel(translate_str("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    sliderSelectionPickBoxSize->setValue(get_int(dialog, "selection_pickbox_size"));
    connect(sliderSelectionPickBoxSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog["selection_pickbox_size"] = node_int(x); } ));

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

/* */
void
Settings_Dialog::addColorsToComboBox(QComboBox* comboBox)
{
    comboBox->addItem(_mainWin->create_icon("colorred"), translate_str("Red"), qRgb(255, 0, 0));
    comboBox->addItem(_mainWin->create_icon("coloryellow"), translate_str("Yellow"), qRgb(255,255, 0));
    comboBox->addItem(_mainWin->create_icon("colorgreen"), translate_str("Green"), qRgb(0, 255, 0));
    comboBox->addItem(_mainWin->create_icon("colorcyan"), translate_str("Cyan"), qRgb(0, 255, 255));
    comboBox->addItem(_mainWin->create_icon("colorblue"), translate_str("Blue"), qRgb(0, 0, 255));
    comboBox->addItem(_mainWin->create_icon("colormagenta"), translate_str("Magenta"), qRgb(255, 0,255));
    comboBox->addItem(_mainWin->create_icon("colorwhite"), translate_str("White"), qRgb(255,255,255));
    //TODO: Add Other... so the user can select custom colors
}

/**
 * @brief Settings_Dialog::comboBoxIconSizeCurrentIndexChanged
 * @param index
 */
void
Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        dialog["general_icon_size"] = node_uint(comboBox->itemData(index).toUInt(&ok));
        if (!ok) {
            dialog["general_icon_size"] = node_uint(16);
        }
    }
    else {
        dialog["general_icon_size"] = node_uint(16);
    }
}

void
Settings_Dialog::checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    preview["general_mdi_bg_use_logo"] = node_int(checked != 0);
    mdiArea->useBackgroundLogo(checked);
}

void
Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, translate_str("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        translate_str("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            accept_["general_mdi_bg_logo"] = node_str(selectedImage.toStdString());
        }

        //Update immediately so it can be previewed
        mdiArea->setBackgroundLogo(QString::fromStdString(get_str(accept_, "general_mdi_bg_logo")));
    }
}

/**
 * @brief Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged
 * @param checked
 */
void
Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    preview["general_mdi_bg_use_texture"] = node_int(checked != 0);
    mdiArea->useBackgroundTexture(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString selectedImage;
        selectedImage = QFileDialog::getOpenFileName(this, translate_str("Open File"),
                        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                        translate_str("Images (*.bmp *.png *.jpg)"));

        if (!selectedImage.isNull()) {
            accept_["general_mdi_bg_texture"] = node_qstr(selectedImage);
        }

        //Update immediately so it can be previewed
        mdiArea->setBackgroundTexture(get_qstr(accept_, "general_mdi_bg_texture"));
    }
}

void Settings_Dialog::checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    preview["general_mdi_bg_use_color"] = node_int(checked != 0);
    mdiArea->useBackgroundColor(checked);
}

void Settings_Dialog::chooseGeneralMdiBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "general_mdi_bg_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGeneralMdiBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["general_mdi_bg_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "general_mdi_bg_color")));
            button->setIcon(QIcon(pix));
            mdiArea->setBackgroundColor(QColor(get_uint(accept_, "general_mdi_bg_color")));
        }
        else {
            mdiArea->setBackgroundColor(QColor(get_uint(dialog, "general_mdi_bg_color")));
        }
    }
}

void Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    preview["general_mdi_bg_color"] = node_uint(color.rgb());
    mdiArea->setBackgroundColor(color);
}

void Settings_Dialog::checkBoxShowScrollBarsStateChanged(int checked)
{
    preview["display_show_scrollbars"] = node_int(checked != 0);
    _mainWin->updateAllViewScrollBars(checked);
}

void Settings_Dialog::comboBoxScrollBarWidgetCurrentIndexChanged(int index)
{
    dialog["display_scrollbar_widget_num"] = node_int(index);
}

void Settings_Dialog::chooseDisplayCrossHairColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "display_crosshair_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayCrossHairColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["display_crosshair_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "display_crosshair_color")));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewCrossHairColors(get_uint(accept_, "display_crosshair_color"));
        }
        else {
            _mainWin->updateAllViewCrossHairColors(get_uint(dialog, "display_crosshair_color"));
        }
    }
}

void Settings_Dialog::currentDisplayCrossHairColorChanged(const QColor& color)
{
    preview["display_crosshair_color"] = node_uint(color.rgb());
    _mainWin->updateAllViewCrossHairColors(color.rgb());
}

void Settings_Dialog::chooseDisplayBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "display_bg_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplayBackgroundColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["display_bg_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "display_bg_color")));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewBackgroundColors(get_uint(accept_, "display_bg_color"));
        }
        else {
            _mainWin->updateAllViewBackgroundColors(get_uint(dialog, "display_bg_color"));
        }
    }
}

void Settings_Dialog::currentDisplayBackgroundColorChanged(const QColor& color)
{
    preview["display_bg_color"] = node_uint(color.rgb());
    _mainWin->updateAllViewBackgroundColors(color.rgb());
}

void Settings_Dialog::chooseDisplaySelectBoxLeftColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "display_selectbox_left_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["display_selectbox_left_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "display_selectbox_left_color")));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(
                get_uint(accept_, "display_selectbox_left_color"),
                get_uint(accept_, "display_selectbox_left_fill"),
                get_uint(accept_, "display_selectbox_right_color"),
                get_uint(accept_, "display_selectbox_right_fill"),
                get_int(preview, "display_selectbox_alpha"));
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                get_uint(dialog, "display_selectbox_left_color"),
                get_uint(dialog, "display_selectbox_left_fill"),
                get_uint(dialog, "display_selectbox_right_color"),
                get_uint(dialog, "display_selectbox_right_fill"),
                get_int(preview, "display_selectbox_alpha"));
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftColorChanged(const QColor& color)
{
    preview["display_selectbox_left_color"] = node_uint(color.rgb());
    _mainWin->updateAllViewSelectBoxColors(
        get_uint(preview, "display_selectbox_left_color"),
        get_uint(preview, "display_selectbox_left_fill"),
        get_uint(preview, "display_selectbox_right_color"),
        get_uint(preview, "display_selectbox_right_fill"),
        get_int(preview, "display_selectbox_alpha"));
}

void Settings_Dialog::chooseDisplaySelectBoxLeftFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "display_selectbox_left_fill")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxLeftFillChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["display_selectbox_left_fill"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "display_selectbox_left_fill")));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(
                get_uint(accept_, "display_selectbox_left_color"),
                get_uint(accept_, "display_selectbox_left_fill"),
                get_uint(accept_, "display_selectbox_right_color"),
                get_uint(accept_, "display_selectbox_right_fill"),
                get_int(preview, "display_selectbox_alpha"));
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                get_uint(dialog, "display_selectbox_left_color"),
                get_uint(dialog, "display_selectbox_left_fill"),
                get_uint(dialog, "display_selectbox_right_color"),
                get_uint(dialog, "display_selectbox_right_fill"),
                get_int(preview, "display_selectbox_alpha"));
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxLeftFillChanged(const QColor& color)
{
    preview["display_selectbox_left_fill"] = node_uint(color.rgb());
    _mainWin->updateAllViewSelectBoxColors(
        get_uint(preview, "display_selectbox_left_color"),
        get_uint(preview, "display_selectbox_left_fill"),
        get_uint(preview, "display_selectbox_right_color"),
        get_uint(preview, "display_selectbox_right_fill"),
        get_int(preview, "display_selectbox_alpha"));
}

void Settings_Dialog::chooseDisplaySelectBoxRightColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "display_selectbox_right_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["display_selectbox_right_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "display_selectbox_right_color")));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(
                get_uint(accept_, "display_selectbox_left_color"),
                get_uint(accept_, "display_selectbox_left_fill"),
                get_uint(accept_, "display_selectbox_right_color"),
                get_uint(accept_, "display_selectbox_right_fill"),
                get_int(preview, "display_selectbox_alpha"));
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                get_uint(dialog, "display_selectbox_left_color"),
                get_uint(dialog, "display_selectbox_left_fill"),
                get_uint(dialog, "display_selectbox_right_color"),
                get_uint(dialog, "display_selectbox_right_fill"),
                get_int(preview, "display_selectbox_alpha"));
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxRightColorChanged(const QColor& color)
{
    preview["display_selectbox_right_color"] = node_uint(color.rgb());
    _mainWin->updateAllViewSelectBoxColors(
        get_uint(preview, "display_selectbox_left_color"),
        get_uint(preview, "display_selectbox_left_fill"),
        get_uint(preview, "display_selectbox_right_color"),
        get_uint(preview, "display_selectbox_right_fill"),
        get_int(preview, "display_selectbox_alpha"));
}

void Settings_Dialog::chooseDisplaySelectBoxRightFill()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "display_selectbox_right_fill")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentDisplaySelectBoxRightFillChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["display_selectbox_right_fill"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "display_selectbox_right_fill")));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewSelectBoxColors(
                get_uint(accept_, "display_selectbox_left_color"),
                get_uint(accept_, "display_selectbox_left_fill"),
                get_uint(accept_, "display_selectbox_right_color"),
                get_uint(accept_, "display_selectbox_right_fill"),
                get_int(preview, "display_selectbox_alpha"));
        }
        else {
            _mainWin->updateAllViewSelectBoxColors(
                get_uint(dialog, "display_selectbox_left_color"),
                get_uint(dialog, "display_selectbox_left_fill"),
                get_uint(dialog, "display_selectbox_right_color"),
                get_uint(dialog, "display_selectbox_right_fill"),
                get_int(preview, "display_selectbox_alpha"));
        }
    }
}

void Settings_Dialog::currentDisplaySelectBoxRightFillChanged(const QColor& color)
{
    preview["display_selectbox_right_fill"] = node_uint(color.rgb());
    _mainWin->updateAllViewSelectBoxColors(
        get_uint(preview, "display_selectbox_left_color"),
        get_uint(preview, "display_selectbox_left_fill"),
        get_uint(preview, "display_selectbox_right_color"),
        get_uint(preview, "display_selectbox_right_fill"),
        get_int(preview, "display_selectbox_alpha"));
}

void Settings_Dialog::spinBoxDisplaySelectBoxAlphaValueChanged(int value)
{
    preview["display_selectbox_alpha"] = node_int(value);
    _mainWin->updateAllViewSelectBoxColors(
        get_uint(accept_, "display_selectbox_left_color"),
        get_uint(accept_, "display_selectbox_left_fill"),
        get_uint(accept_, "display_selectbox_right_color"),
        get_uint(accept_, "display_selectbox_right_fill"),
        get_int(preview, "display_selectbox_alpha"));
}

void Settings_Dialog::choosePromptTextColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "prompt_text_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentPromptTextColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["prompt_text_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "prompt_text_color")));
            button->setIcon(QIcon(pix));
            prompt->setPromptTextColor(QColor(get_uint(accept_, "prompt_text_color")));
        }
        else {
            prompt->setPromptTextColor(QColor(get_uint(dialog, "prompt_text_color")));
        }
    }
}

void Settings_Dialog::currentPromptTextColorChanged(const QColor& color)
{
    preview["prompt_text_color"] = node_uint(color.rgb());
    prompt->setPromptTextColor(QColor(get_uint(preview, "prompt_text_color")));
}

void Settings_Dialog::choosePromptBackgroundColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "prompt_background_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(currentPromptBackgroundColorChanged(QColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["prompt_background_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "prompt_background_color")));
            button->setIcon(QIcon(pix));
            prompt->setPromptBackgroundColor(QColor(get_uint(accept_, "prompt_background_color")));
        }
        else {
            prompt->setPromptBackgroundColor(QColor(get_uint(dialog, "prompt_background_color")));
        }
    }
}

void Settings_Dialog::currentPromptBackgroundColorChanged(const QColor& color)
{
    preview["prompt_background_color"] = node_uint(color.rgb());
    prompt->setPromptBackgroundColor(color);
}

void Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(QString family)
{
    preview["prompt_font_family"] = node_qstr(family);
    prompt->setPromptFontFamily(get_qstr(preview, "prompt_font_family"));
}

void Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(QString style)
{
    preview["prompt_font_style"] = node_qstr(style);
    prompt->setPromptFontStyle(get_qstr(preview, "prompt_font_style"));
}

void Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    preview["prompt_font_size"] = node_int(value);
    prompt->setPromptFontSize(get_int(preview, "prompt_font_size"));
}

void Settings_Dialog::checkBoxPromptSaveHistoryAsHtmlStateChanged(int checked)
{
    dialog["prompt_save_history_as_html"] = node_int(checked != 0);
}

void Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        QString format = checkBox->text();
        qDebug("CustomFilter: %s %d", qPrintable(format), checked);
        /*
        if (checked) {
            dialog.opensave_custom_filter.append(" *." + format.toLower());
        }
        else {
            dialog.opensave_custom_filter.remove("*." + format, Qt::CaseInsensitive);
        }
        //dialog.opensave_custom_filter = checked; //TODO
        */
    }
}

void Settings_Dialog::buttonCustomFilterSelectAllClicked()
{
    emit buttonCustomFilterSelectAll(true);
    dialog["opensave_custom_filter"] = node_str("supported");
}

void Settings_Dialog::buttonCustomFilterClearAllClicked()
{
    emit buttonCustomFilterClearAll(false);
    dialog["opensave_custom_filter"] = node_str("");
}

void Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    dialog["grid_color_match_crosshair"] = node_int(checked != 0);
    if (checked) {
        _mainWin->updateAllViewGridColors(get_uint(accept_, "display_crosshair_color"));
    }
    else {
        _mainWin->updateAllViewGridColors(get_uint(accept_, "grid_color"));
    }

    QObject* senderObj = sender();
    if (senderObj) {
        QObject* parent = senderObj->parent();
        if (parent) {
            set_enabled(parent, "labelGridColor", !checked);
            set_enabled(parent, "buttonGridColor", !checked);
        }
    }
}

void Settings_Dialog::chooseGridColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "grid_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(const QColor&)), this, SLOT(currentGridColorChanged(const QColor&)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["grid_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "grid_color")));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewGridColors(get_uint(accept_, "grid_color"));
        }
        else {
            _mainWin->updateAllViewGridColors(get_uint(dialog, "grid_color"));
        }
    }
}

void Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    preview["grid_color"] = node_uint(color.rgb());
    _mainWin->updateAllViewGridColors(color.rgb());
}

void Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    dialog["grid_load_from_file"] = node_int(checked != 0);

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }

    QObject* parent = senderObj->parent();
    if (parent) {
        return;
    }

    set_enabled(parent, "labelGridType", !checked);
    set_enabled(parent, "comboBoxGridType", !checked);
    set_enabled(parent, "checkBoxGridCenterOnOrigin", !checked);

    bool not_center = !checked && !get_bool(dialog, "grid_center_on_origin");

    set_enabled(parent, "labelGridCenterX", not_center);
    set_enabled(parent, "spinBoxGridCenterX", not_center);

    set_enabled(parent, "labelGridCenterY", not_center);
    set_enabled(parent, "spinBoxGridCenterY", not_center);

    set_enabled(parent, "labelGridSizeX", !checked);
    set_enabled(parent, "spinBoxGridSizeX", !checked);

    set_enabled(parent, "labelGridSizeY", !checked);
    set_enabled(parent, "spinBoxGridSizeY", !checked);

    set_enabled(parent, "labelGridSpacingX", !checked);
    set_enabled(parent, "spinBoxGridSpacingX", !checked);

    set_enabled(parent, "labelGridSpacingY", !checked);
    set_enabled(parent, "spinBoxGridSpacingY", !checked);

    set_enabled(parent, "labelGridSizeRadius", !checked);
    set_enabled(parent, "spinBoxGridSizeRadius", !checked);

    set_enabled(parent, "labelGridSpacingRadius", !checked);
    set_enabled(parent, "spinBoxGridSpacingRadius", !checked);

    set_enabled(parent, "labelGridSpacingAngle", !checked);
    set_enabled(parent, "spinBoxGridSpacingAngle", !checked);
}

/**
 * @brief Settings_Dialog::comboBoxGridTypeCurrentIndexChanged
 * @param type
 */
void Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(QString type)
{
    dialog["grid_type"] = node_qstr(type);

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (!parent) {
        return;
    }
    bool visibility = (type == "Circular");

    set_visibility(parent, "labelGridSizeX", !visibility);
    set_visibility(parent, "spinBoxGridSizeX", !visibility);

    set_visibility(parent, "labelGridSizeY", !visibility);
    set_visibility(parent, "spinBoxGridSizeY", !visibility);

    set_visibility(parent, "labelGridSpacingX", !visibility);
    set_visibility(parent, "spinBoxGridSpacingX", !visibility);

    set_visibility(parent, "labelGridSpacingY", !visibility);
    set_visibility(parent, "spinBoxGridSpacingY", !visibility);

    set_visibility(parent, "labelGridSizeRadius", visibility);
    set_visibility(parent, "spinBoxGridSizeRadius", visibility);

    set_visibility(parent, "labelGridSpacingRadius", visibility);
    set_visibility(parent, "spinBoxGridSpacingRadius", visibility);

    set_visibility(parent, "labelGridSpacingAngle", visibility);
    set_visibility(parent, "spinBoxGridSpacingAngle", visibility);
}

void Settings_Dialog::checkBoxGridCenterOnOriginStateChanged(int checked)
{
    dialog["grid_center_on_origin"] = node_int(checked != 0);

    QObject* senderObj = sender();
    if (senderObj) {
        return;
    }

    QObject* parent = senderObj->parent();
    if (parent) {
        return;
    }

    set_enabled(parent, "labelGridCenterX", !checked);
    set_enabled(parent, "spinBoxGridCenterX", !checked);

    set_enabled(parent, "labelGridCenterY", !checked);
    set_enabled(parent, "spinBoxGridCenterY", !checked);
}

void Settings_Dialog::checkBoxRulerShowOnLoadStateChanged(int checked)
{
    dialog["ruler_show_on_load"] = node_int(checked != 0);
}

void Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        dialog["ruler_metric"] = node_int(comboBox->itemData(index).toBool());
    }
    else {
        dialog["ruler_metric"] = node_int(true);
    }
}

void Settings_Dialog::chooseRulerColor()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QColorDialog* colorDialog = new QColorDialog(QColor(get_uint(accept_, "ruler_color")), this);
        connect(colorDialog, SIGNAL(currentColorChanged(QColor)), this, SLOT(currentRulerColorChanged(QColor)));
        colorDialog->exec();

        if (colorDialog->result() == QDialog::Accepted) {
            accept_["ruler_color"] = node_uint(colorDialog->selectedColor().rgb());
            QPixmap pix(16,16);
            pix.fill(QColor(get_uint(accept_, "ruler_color")));
            button->setIcon(QIcon(pix));
            _mainWin->updateAllViewRulerColors(get_uint(accept_, "ruler_color"));
        }
        else {
            _mainWin->updateAllViewRulerColors(get_uint(dialog, "ruler_color"));
        }
    }
}

void Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    preview["ruler_color"] = node_uint(color.rgb());
    _mainWin->updateAllViewRulerColors(get_uint(preview, "ruler_color"));
}

void Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    dialog["ruler_pixel_size"] = node_real((EmbReal)value);
}

void Settings_Dialog::buttonQSnapSelectAllClicked()
{
    emit buttonQSnapSelectAll(true);
}

void Settings_Dialog::buttonQSnapClearAllClicked()
{
    emit buttonQSnapClearAll(false);
}

/**
 *
 */
void Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(255,255,0); //Yellow
    if (comboBox) {
        bool ok = 0;
        dialog["quicksnap_locator_color"] = node_uint(comboBox->itemData(index).toUInt(&ok));
        if (!ok)
            dialog["quicksnap_locator_color"] = node_uint(defaultColor);
    }
    else {
        dialog["quicksnap_locator_color"] = node_uint(defaultColor);
    }
}

void Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    preview["lwt_show_lwt"] = node_int(checked != 0);
    statusbar->toggle("LWT", checked);

    QObject* senderObj = sender();
    if (!senderObj) {
        return;
    }
    QObject* parent = senderObj->parent();
    if (parent) {
        return;
    }
    QCheckBox* checkBoxRealRender = parent->findChild<QCheckBox*>("checkBoxRealRender");
    if (checkBoxRealRender) {
        checkBoxRealRender->setEnabled(checked);
    }
}

void Settings_Dialog::checkBoxLwtRealRenderStateChanged(int checked)
{
    preview["lwt_real_render"] = node_int(checked != 0);
    statusbar->toggle("LWT", checked);
}

void Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    QRgb defaultColor = qRgb(0,0,255); //Blue
    if (comboBox) {
        bool ok = 0;
        dialog["selection_coolgrip_color"] = node_uint(comboBox->itemData(index).toUInt(&ok));
        if (!ok) {
            dialog["selection_coolgrip_color"] = node_uint(defaultColor);
        }
    }
    else {
        dialog["selection_coolgrip_color"] = node_uint(defaultColor);
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
        dialog["selection_hotgrip_color"] = node_uint(comboBox->itemData(index).toUInt(&ok));
        if (!ok) {
            dialog["selection_hotgrip_color"] = node_uint(defaultColor);
        }
    }
    else {
        dialog["selection_hotgrip_color"] = node_uint(defaultColor);
    }
}

/**
 * .
 */
void Settings_Dialog::acceptChanges()
{
    dialog["general_mdi_bg_use_logo"] = preview["general_mdi_bg_use_logo"];
    dialog["general_mdi_bg_use_texture"] = preview["general_mdi_bg_use_texture"];
    dialog["general_mdi_bg_use_color"] = preview["general_mdi_bg_use_color"];
    dialog["general_mdi_bg_logo"] = accept_["general_mdi_bg_logo"];
    dialog["general_mdi_bg_texture"] = accept_["general_mdi_bg_texture"];
    dialog["general_mdi_bg_color"] = accept_["general_mdi_bg_color"];
    dialog["display_show_scrollbars"] = preview["display_show_scrollbars"];
    dialog["display_crosshair_color"] = accept_["display_crosshair_color"];
    dialog["display_bg_color"] = accept_["display_bg_color"];
    dialog["display_selectbox_left_color"] = accept_["display_selectbox_left_color"];
    dialog["display_selectbox_left_fill"] = accept_["display_selectbox_left_fill"];
    dialog["display_selectbox_right_color"] = accept_["display_selectbox_right_color"];
    dialog["display_selectbox_right_fill"] = accept_["display_selectbox_right_fill"];
    dialog["display_selectbox_alpha"] = preview["display_selectbox_alpha"];
    dialog["prompt_text_color"] = accept_["prompt_text_color"];
    dialog["prompt_background_color"] = accept_["prompt_background_color"];
    dialog["prompt_font_family"] = preview["prompt_font_family"];
    dialog["prompt_font_style"] = preview["prompt_font_style"];
    dialog["prompt_font_size"] = preview["prompt_font_size"];
    if (get_bool(dialog, "grid_color_match_crosshair")) {
        dialog["grid_color"] = accept_["display_crosshair_color"];
    }
    else {
        dialog["grid_color"] = accept_["grid_color"];
    }
    dialog["ruler_color"] = accept_["ruler_color"];
    dialog["lwt_show_lwt"] = preview["lwt_show_lwt"];
    dialog["lwt_real_render"] = preview["lwt_real_render"];

    for (auto iter = settings.begin(); iter != settings.end(); iter++) {
        settings[iter->first] = dialog[iter->first];
    }

    //Make sure the user sees the changes applied immediately
    mdiArea->useBackgroundLogo(get_int(dialog, "general_mdi_bg_use_logo"));
    mdiArea->useBackgroundTexture(get_int(dialog, "general_mdi_bg_use_texture"));
    mdiArea->useBackgroundColor(get_uint(dialog, "general_mdi_bg_use_color"));
    mdiArea->setBackgroundLogo(get_qstr(dialog, "general_mdi_bg_logo"));
    mdiArea->setBackgroundTexture(get_qstr(dialog, "general_mdi_bg_texture"));
    mdiArea->setBackgroundColor(get_uint(dialog, "general_mdi_bg_color"));
    _mainWin->iconResize(get_int(dialog, "general_icon_size"));
    _mainWin->updateAllViewScrollBars(get_bool(dialog, "display_show_scrollbars"));
    _mainWin->updateAllViewCrossHairColors(get_uint(dialog, "display_crosshair_color"));
    _mainWin->updateAllViewBackgroundColors(get_uint(dialog, "display_bg_color"));
    _mainWin->updateAllViewSelectBoxColors(
        get_uint(dialog, "display_selectbox_left_color"),
        get_uint(dialog, "display_selectbox_left_fill"),
        get_uint(dialog, "display_selectbox_right_color"),
        get_uint(dialog, "display_selectbox_right_fill"),
        get_int(dialog, "display_selectbox_alpha"));
    prompt->setPromptTextColor(QColor(get_uint(dialog, "prompt_text_color")));
    prompt->setPromptBackgroundColor(QColor(get_uint(dialog, "prompt_background_color")));
    prompt->setPromptFontFamily(get_qstr(dialog, "prompt_font_family"));
    prompt->setPromptFontStyle(get_qstr(dialog, "prompt_font_style"));
    prompt->setPromptFontSize(get_int(dialog, "prompt_font_size"));
    _mainWin->updateAllViewGridColors(get_uint(dialog, "grid_color"));
    _mainWin->updateAllViewRulerColors(get_uint(dialog, "ruler_color"));
    statusbar->toggle("LWT", get_bool(dialog, "lwt_show_lwt"));
    /*
    statusbar->toggle("REAL", get_bool(dialog, "lwt_real_render")); ?
    */
    _mainWin->updatePickAddMode(get_bool(dialog, "selection_mode_pickadd"));

    write_settings();
    accept();
}

/**
 * @brief Settings_Dialog::rejectChanges
 */
void
Settings_Dialog::rejectChanges()
{
    //TODO: inform the user if they have changed settings

    //Update the view since the user must accept the preview
    mdiArea->useBackgroundLogo(get_bool(dialog, "general_mdi_bg_use_logo"));
    mdiArea->useBackgroundTexture(get_bool(dialog, "general_mdi_bg_use_texture"));
    mdiArea->useBackgroundColor(get_bool(dialog, "general_mdi_bg_use_color"));
    mdiArea->setBackgroundLogo(get_qstr(dialog, "general_mdi_bg_logo"));
    mdiArea->setBackgroundTexture(get_qstr(dialog, "general_mdi_bg_texture"));
    mdiArea->setBackgroundColor(get_uint(dialog, "general_mdi_bg_color"));
    _mainWin->updateAllViewScrollBars(get_bool(dialog, "display_show_scrollbars"));
    _mainWin->updateAllViewCrossHairColors(get_uint(dialog, "display_crosshair_color"));
    _mainWin->updateAllViewBackgroundColors(get_uint(dialog, "display_bg_color"));
    _mainWin->updateAllViewSelectBoxColors(
        get_uint(dialog, "display_selectbox_left_color"),
        get_uint(dialog, "display_selectbox_left_fill"),
        get_uint(dialog, "display_selectbox_right_color"),
        get_uint(dialog, "display_selectbox_right_fill"),
        get_int(dialog, "display_selectbox_alpha"));
    prompt->setPromptTextColor(QColor(get_uint(dialog, "prompt_text_color")));
    prompt->setPromptBackgroundColor(QColor(get_uint(dialog, "prompt_background_color")));
    prompt->setPromptFontFamily(get_qstr(dialog, "prompt_font_family"));
    prompt->setPromptFontStyle(get_qstr(dialog, "prompt_font_style"));
    prompt->setPromptFontSize(get_real(dialog, "prompt_font_size"));
    _mainWin->updateAllViewGridColors(get_uint(dialog, "grid_color"));
    _mainWin->updateAllViewRulerColors(get_uint(dialog, "ruler_color"));
    statusbar->toggle("LWT", get_bool(settings, "lwt_show_lwt"));
    /*
    statusbar->toggle("REAL", settings.lwt_real_render); ?
    */

    reject();
}

