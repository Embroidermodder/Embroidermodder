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

#define MAKE_SLOT(LINE) \
    [=](void) { LINE; }

#define MAX_FILE_TYPES  100

QWidget *create_scrollarea(QWidget *parent, QWidget *widget);
void set_vbox_layout(QWidget *parent, QWidget *entries[],
    int n_entries, int add_stretch);

int grid_ruler_props[] = {
	ST_GRID_LOAD_FROM_FILE,
	ST_GRID_TYPE,
	ST_GRID_CENTER_ORIGIN,
	ST_GRID_CENTER_X,
	ST_GRID_CENTER_Y,
	ST_GRID_SIZE_X,
	ST_GRID_SIZE_Y,
	ST_GRID_SPACING_X,
	ST_GRID_SPACING_Y,
	-1
};

WidgetData checkbox_grid_misc[] = {
    {
        .type = WIDGET_CHECKBOX,
        .label = "Initially show grid when loading a file",
        .icon = "blank",
        .key = ST_GRID_ON_LOAD,
        .position = {0, 0}
    },
    {
        .type = WIDGET_CHECKBOX,
        .label = "Show the origin when the grid is enabled",
        .icon = "blank",
        .key = ST_SHOW_ORIGIN,
        .position = {1, 0}
    }
};

WidgetData cb_center_on_origin = {
	.type = WIDGET_CHECKBOX,
	.label="Center the grid on the origin",
	.icon = "blank",
	.key = ST_GRID_CENTER_ORIGIN
};

WidgetData cb_grid_load_from_file = {
	.type = WIDGET_CHECKBOX,
	.label = "Set grid size from opened file",
	.icon = "blank",
	.key = ST_GRID_LOAD_FROM_FILE
};

WidgetData sb_ruler_pixel_size = {
	.type = WIDGET_DOUBLE_SPINBOX,
	.label = "spinBoxRulerPixelSize",
	.single_step = 1.0,
	.lower = 20.0,
	.upper = 100.0,
	.key = ST_RULER_SIZE
};

WidgetData cb_mdi_bg_use_color = {
	.type=WIDGET_CHECKBOX,
	.label="Use Color",
	.icon="blank",
	.key=ST_MDI_USE_COLOR
};

int qsnap_n_checkboxes = 13;

QCheckBox *checkBoxCustomFilter[MAX_FILE_TYPES];

void update_view_selectbox(Node *d);
void comboBoxColor(QComboBox* comboBox, QRgb defaultColor, int color_id, int index);

void
copy_node(Node *a, Node *b, int index)
{
    memcpy(a+index, b+index, sizeof a[0]);
}

void
copy_props(Node *a, Node *b, int *props)
{
    for (int i=0; props[i]>=0; i++) {
        memcpy(a+props[i], b+props[i], sizeof(Node));
    }
}

/* . */
void
make_editing_copy(int props[])
{
    copy_props(dialog, settings, props);
    copy_props(preview, settings, props);
    copy_props(accept_, settings, props);
}

/* . */
void
Settings_Dialog::chooseColor(int color_id)
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
	QColorDialog* colorDialog = new QColorDialog(QColor(accept_[color_id].i), this);
	connect(colorDialog, &QColorDialog::currentColorChanged, this,
        [=](const QColor& color) {
            colorChanged(color, preview, color_id);
        }
    );
	colorDialog->exec();

	if (colorDialog->result() == QDialog::Accepted) {
		accept_[color_id].i = (colorDialog->selectedColor().rgb());
		button->setIcon(swatch((accept_[color_id].i)));
        colorChanged(colorDialog->selectedColor(), accept_, color_id);
	}
	else {
        colorChanged(colorDialog->selectedColor(), dialog, color_id);
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

    /*
    layoutState = settingsfile.value("LayoutState").toByteArray();
    if (!restoreState(layoutState)) {
        debug_message("LayoutState NOT restored! Setting Default Layout...");
        //someToolBar->setVisible(true);
     }
    */

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    QString settings_dir = QDir::homePath() + "/.embroidermodder2";
//    settings[ST_SAVE_HISTORY].s = QDir::homePath() + "/.embroidermodder2/prompt.log";
#else
    QString settings_dir = qApp->applicationDirPath();
//    settings[ST_SAVE_HISTORY] = appDir + "prompt.log";
#endif

    std::string fname = settings_dir.toStdString() + "/em2_settings.ini";
    FILE *f = NULL; //fopen(fname.c_str(), "r");
    if (!f) {
        printf("WARNING: Failed to open settings file (%s), continuing with defaults.",
            fname.c_str());
        for (int i=0; i<SETTINGS_TOTAL; i++) {
            Setting s = settings_data[i];
            switch (s.type) {
            case 'i':
                settings[s.id].i = atoi(s.value);
                break;
            case 'r':
                settings[s.id].r = atof(s.value);
                break;
            case 's':
                strcpy(settings[s.id].s, s.value);
                break;
            default:
                break;
            }
        }
    }
    else {
        char config[MAX_SETTINGS][MAX_STRING_LENGTH];
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
        fclose(f);

        for (int i=0; i<SETTINGS_TOTAL; i++) {
            int eq_pos = str_contains(config[line], '=');
            if (eq_pos < 0) {
                continue;
            }
            Setting s = settings_data[i];
            char *value = config[i] + eq_pos + 1;
            switch (s.type) {
            case 'i':
                settings[s.id].i = atoi(value);
                break;
            case 'r':
                settings[s.id].r = atof(value);
                break;
            case 's':
                strcpy(settings[s.id].s, value);
                break;
            default:
                break;
            }
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

    settings[ST_WINDOW_POS_X].i = (int)_mainWin->pos().x();
    settings[ST_WINDOW_POS_Y].i = (int)_mainWin->pos().y();
    settings[ST_WINDOW_SIZE_X].i = (int)_mainWin->size().width();
    settings[ST_WINDOW_SIZE_Y].i = (int)_mainWin->size().height();

#if defined(Q_OS_UNIX) || defined(Q_OS_MAC)
    std::string settingsPath = QDir::homePath().toStdString() + "/.embroidermodder2/settings.ini";
#else
    std::string settingsPath = qApp->applicationDirPath().toStdString() + "/settings.ini";
#endif
    FILE *file = fopen(settingsPath.c_str(), "w");

    if (!file) {
        debug_message("ERROR: failed to open settings file for output.");
        return;
    }

    for (int i=0; i<SETTINGS_TOTAL; i++) {
        char line[2*MAX_STRING_LENGTH];
        Setting s = settings_data[i];
        switch (s.type) {
        case 'i':
            fprintf(file, "%s=%d\n", s.key, settings[s.id].i);
            break;
        case 'r':
            fprintf(file, "%s=%f\n", s.key, settings[s.id].r);
            break;
        case 's':
            fprintf(file, "%s=%s\n", s.key, settings[s.id].s);
            break;
        default:
            break;
        }
    }
    fclose(file);
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
    QString lang(settings[ST_LANGUAGE].s);
    lang = lang.toLower();
    strcpy(dialog[ST_LANGUAGE].s, lang.toStdString().c_str());
    comboBoxLanguage->addItem("Default");
    comboBoxLanguage->addItem("System");
    comboBoxLanguage->insertSeparator(2);
    QDir trDir(qApp->applicationDirPath());
    trDir.cd("translations");
    foreach(QString dirName, trDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        dirName[0] = dirName[0].toUpper();
        comboBoxLanguage->addItem(dirName);
    }
    QString current(dialog[ST_LANGUAGE].s);
    current[0] = current[0].toUpper();
    comboBoxLanguage->setCurrentIndex(comboBoxLanguage->findText(current));
    /*
    connect(
        comboBoxLanguage,
        SIGNAL(currentIndexChanged(QString)),
        this,
        [=](QString lang) {
            strcpy(dialog[ST_LANGUAGE].s,
                QString::fromStdString(lang).toLower().toStdString().c_str());
        }
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
    comboBoxIconTheme->setCurrentIndex(comboBoxIconTheme->findText(dialog[ST_ICON_THEME].s));
    connect(
        comboBoxIconTheme,
        &QComboBox::currentIndexChanged,
        this,
        [=](int i) {
            dialog[ST_ICON_THEME].i = i;
        }
    );

    QLabel* labelIconSize = new QLabel(translate_str("Icon Size"), groupBoxIcon);
    QComboBox* comboBoxIconSize = new QComboBox(groupBoxIcon);
    QString theme(dialog[ST_ICON_THEME].s);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon16"  + ".png"), "Very Small", 16);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon24"  + ".png"), "Small", 24);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon32"  + ".png"), "Medium", 32);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon48"  + ".png"), "Large", 48);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon64"  + ".png"), "Very Large", 64);
    comboBoxIconSize->addItem(QIcon("icons/" + theme + "/" + "icon128" + ".png"), "I'm Blind", 128);
    comboBoxIconSize->setCurrentIndex(comboBoxIconSize->findData(dialog[ST_ICON_SIZE].i));
    connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this,
        SLOT(comboBoxIconSizeCurrentIndexChanged(int)));

    QVBoxLayout* vboxLayoutIcon = new QVBoxLayout(groupBoxIcon);
    vboxLayoutIcon->addWidget(labelIconTheme);
    vboxLayoutIcon->addWidget(comboBoxIconTheme);
    vboxLayoutIcon->addWidget(labelIconSize);
    vboxLayoutIcon->addWidget(comboBoxIconSize);
    groupBoxIcon->setLayout(vboxLayoutIcon);

    //Mdi Background
    QGroupBox* groupBoxMdiBG = new QGroupBox(translate_str("Background"), widget);

    QCheckBox* checkBoxMdiBGUseLogo = new QCheckBox(translate_str("Use Logo"), groupBoxMdiBG);
    checkBoxMdiBGUseLogo->setChecked(preview[ST_MDI_USE_LOGO].i);
    connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this,
        SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)));

    QPushButton* buttonMdiBGLogo = new QPushButton(translate_str("Choose"), groupBoxMdiBG);
    buttonMdiBGLogo->setEnabled(dialog[ST_MDI_USE_LOGO].i);
    /*
    FIXME
    connect(buttonMdiBGLogo, &QPushButton::clicked, this,
        SLOT(chooseGeneralMdiBackgroundLogo()));
    */
    connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(bool)), buttonMdiBGLogo,
        SLOT(setEnabled(bool)));

    QCheckBox* checkBoxMdiBGUseTexture = new QCheckBox(translate_str("Use Texture"), groupBoxMdiBG);
    checkBoxMdiBGUseTexture->setChecked(preview[ST_MDI_USE_TEXTURE].i);
    connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this,
        SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)));

    QPushButton* buttonMdiBGTexture = new QPushButton(translate_str("Choose"), groupBoxMdiBG);
    buttonMdiBGTexture->setEnabled(dialog[ST_MDI_USE_TEXTURE].i);
    /* FIXME
    connect(buttonMdiBGTexture, &QPushButton::clicked, this, SLOT(chooseGeneralMdiBackgroundTexture()));
    connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(bool)), buttonMdiBGTexture,
        SLOT(setEnabled(bool)));
    */

    QWidget* checkBoxMdiBGUseColor = make_widget(groupBoxMdiBG, dialog, cb_mdi_bg_use_color);

    QPushButton* buttonMdiBGColor = new QPushButton(translate_str("Choose"), groupBoxMdiBG);
    buttonMdiBGColor->setEnabled(dialog[ST_MDI_USE_COLOR].i);
    buttonMdiBGColor->setIcon(swatch(preview[ST_MDI_COLOR].i));
    connect(buttonMdiBGColor, &QPushButton::clicked, this,
        MAKE_SLOT(chooseColor(ST_MDI_COLOR)));
    connect(checkBoxMdiBGUseColor, SIGNAL(toggled(bool)), buttonMdiBGColor,
        SLOT(setEnabled(bool)));

    QGridLayout* gridLayoutMdiBG = new QGridLayout(widget);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseLogo, 0, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGLogo, 0, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseTexture, 1, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGTexture, 1, 1, Qt::AlignRight);
    gridLayoutMdiBG->addWidget(checkBoxMdiBGUseColor, 2, 0, Qt::AlignLeft);
    gridLayoutMdiBG->addWidget(buttonMdiBGColor, 2, 1, Qt::AlignRight);
    groupBoxMdiBG->setLayout(gridLayoutMdiBG);

    /* Tips */
    QGroupBox* groupBoxTips = new QGroupBox(translate_str("Tips"), widget);

    QWidget* checkBoxTipOfTheDay = make_widget(groupBoxTips, dialog,
        {
            .type = WIDGET_CHECKBOX,
            .label = "Show Tip of the Day on startup",
            .icon = "blank",
            .key = ST_TIP_OF_THE_DAY
        }
    );

    QVBoxLayout* vboxLayoutTips = new QVBoxLayout(groupBoxTips);
    vboxLayoutTips->addWidget(checkBoxTipOfTheDay);
    groupBoxTips->setLayout(vboxLayoutTips);

    //Help Browser
    QGroupBox* groupBoxHelpBrowser = new QGroupBox(translate_str("Help Browser"), widget);

    QRadioButton* radioButtonSystemHelpBrowser = new QRadioButton(translate_str("System"), groupBoxHelpBrowser);
    radioButtonSystemHelpBrowser->setChecked(settings[ST_SYSTEM_HELP_BROWSER].i);
    QRadioButton* radioButtonCustomHelpBrowser = new QRadioButton(translate_str("Custom"), groupBoxHelpBrowser);
    radioButtonCustomHelpBrowser->setChecked(!settings[ST_SYSTEM_HELP_BROWSER].i);
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

    return create_scrollarea(this, widget);
}

QWidget *
create_scrollarea(QWidget *parent, QWidget *widget)
{
    QScrollArea* scrollArea = new QScrollArea(parent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);
    return scrollArea;
}

void
set_vbox_layout(QWidget *parent, QWidget *entries[], int n_entries, int add_stretch)
{
    QVBoxLayout* vboxLayout = new QVBoxLayout(parent);
    for (int i=0; i<n_entries; i++) {
        vboxLayout->addWidget(entries[i]);
    }
    if (add_stretch) {
        vboxLayout->addStretch(1);
    }
    parent->setLayout(vboxLayout);
}

/* . */
QWidget*
Settings_Dialog::createTabFilesPaths()
{
    QWidget* widget = new QWidget(this);

    return create_scrollarea(this, widget);
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

    QWidget *render_checkboxes[6];

    render_checkboxes[0] = make_widget(groupBoxRender, dialog,
        {
            .type=WIDGET_CHECKBOX,
            .label="Use OpenGL",
            .icon="blank",
            .key=ST_USE_OPENGL
        }
    );
    render_checkboxes[1] = make_widget(groupBoxRender, dialog,
        {
            .type=WIDGET_CHECKBOX,
            .label="Antialias",
            .icon="blank",
            .key=ST_ANTI_ALIAS
        }
    );
    render_checkboxes[2] = make_widget(groupBoxRender, dialog,
        {
            .type=WIDGET_CHECKBOX,
            .label="Antialias Text",
            .icon="blank",
            .key=ST_TEXT_ANTI_ALIAS
        }
    );
    render_checkboxes[3] = make_widget(groupBoxRender, dialog,
        {
            .type=WIDGET_CHECKBOX,
            .label="Smooth Pixmap",
            .icon="blank",
            .key=ST_SMOOTH_PIXMAP
        }
    );
    render_checkboxes[4] = make_widget(groupBoxRender, dialog,
        {
            .type=WIDGET_CHECKBOX,
            .label="High Quality Antialiasing (OpenGL)",
            .icon="blank",
            .key=ST_HQ_ANTI_ALIAS
        }
    );
    render_checkboxes[5] = make_widget(groupBoxRender, dialog,
        {
            .type=WIDGET_CHECKBOX,
            .label="Non Cosmetic",
            .icon="blank",
            .key=ST_NON_COSMETIC
        }
    );

    set_vbox_layout(groupBoxRender, render_checkboxes, 6, 0);

    //ScrollBars
    QGroupBox* groupBoxScrollBars = new QGroupBox(translate_str("ScrollBars"), widget);

    QWidget* checkBoxShowScrollBars = make_widget(groupBoxScrollBars, dialog,
        {
            .type=WIDGET_CHECKBOX,
            .label="Show ScrollBars",
            .icon="blank",
            .key=ST_SHOW_SCROLLBARS
        }
    );

    QLabel* labelScrollBarWidget = new QLabel(translate_str("Perform action when clicking corner widget"), groupBoxScrollBars);
    QComboBox* comboBoxScrollBarWidget = new QComboBox(groupBoxScrollBars);
    for (int i=0; i<N_ACTIONS; i++) {
        QAction* action = actionHash[i];
        if (action) {
            comboBoxScrollBarWidget->addItem(action->icon(), action->text().replace("&", ""));
        }
    }
    comboBoxScrollBarWidget->setCurrentIndex(dialog[ST_SCROLLBAR_WIDGET_NUM].i);
    connect(comboBoxScrollBarWidget, &QComboBox::currentIndexChanged, this,
        [=](int index)
        {
            dialog[ST_SCROLLBAR_WIDGET_NUM].i = index;
        }
    );

    QWidget *vbox_scrollbars[] = {
        checkBoxShowScrollBars,
        labelScrollBarWidget,
        comboBoxScrollBarWidget
    };

    set_vbox_layout(groupBoxScrollBars, vbox_scrollbars, 3, 0);

    //Colors
    QGroupBox* groupBoxColor = new QGroupBox(translate_str("Colors"), widget);

    QLabel* labelCrossHairColor = new QLabel(translate_str("Crosshair Color"), groupBoxColor);
    QPushButton* buttonCrossHairColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    buttonCrossHairColor->setIcon(swatch(preview[ST_CROSSHAIR_COLOR].i));
    connect(buttonCrossHairColor, &QPushButton::clicked, this,
        MAKE_SLOT(chooseColor(ST_CROSSHAIR_COLOR)));

    QLabel* labelBGColor = new QLabel(translate_str("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    buttonBGColor->setIcon(swatch(preview[ST_BG_COLOR].i));
    connect(buttonBGColor, &QPushButton::clicked, this,
        MAKE_SLOT(chooseColor(ST_BG_COLOR)));

    QLabel* labelSelectBoxLeftColor = new QLabel(translate_str("Selection Box Color (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    buttonSelectBoxLeftColor->setIcon(swatch(preview[ST_SELECTBOX_LEFT_COLOR].i));
    connect(buttonSelectBoxLeftColor, &QPushButton::clicked, this, 
        MAKE_SLOT(chooseColor(ST_SELECTBOX_LEFT_COLOR)));

    QLabel* labelSelectBoxLeftFill = new QLabel(translate_str("Selection Box Fill (Crossing)"), groupBoxColor);
    QPushButton* buttonSelectBoxLeftFill = new QPushButton(translate_str("Choose"), groupBoxColor);
    buttonSelectBoxLeftFill->setIcon(swatch(preview[ST_SELECTBOX_LEFT_FILL].i));
    connect(buttonSelectBoxLeftFill, &QPushButton::clicked, this, 
        MAKE_SLOT(chooseColor(ST_SELECTBOX_LEFT_FILL)));

    QLabel* labelSelectBoxRightColor = new QLabel(translate_str("Selection Box Color (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    buttonSelectBoxRightColor->setIcon(swatch(preview[ST_SELECTBOX_RIGHT_COLOR].i));
    connect(buttonSelectBoxRightColor, &QPushButton::clicked, this, 
        MAKE_SLOT(chooseColor(ST_SELECTBOX_RIGHT_COLOR)));

    QLabel* labelSelectBoxRightFill = new QLabel(translate_str("Selection Box Fill (Window)"), groupBoxColor);
    QPushButton* buttonSelectBoxRightFill = new QPushButton(translate_str("Choose"), groupBoxColor);
    buttonSelectBoxRightFill->setIcon(swatch(preview[ST_SELECTBOX_RIGHT_FILL].i));
    connect(buttonSelectBoxRightFill, &QPushButton::clicked, this, 
        MAKE_SLOT(chooseColor(ST_SELECTBOX_RIGHT_FILL)));

    QLabel* labelSelectBoxAlpha = new QLabel(
        translate_str("Selection Box Fill Alpha"), groupBoxColor);
    QSpinBox* spinBoxSelectBoxAlpha = new QSpinBox(groupBoxColor);
    spinBoxSelectBoxAlpha->setRange(0, 255);
    spinBoxSelectBoxAlpha->setValue(preview[ST_SELECTBOX_ALPHA].i);
    connect(spinBoxSelectBoxAlpha, &QSpinBox::valueChanged, this,
        [=](int value) {
            preview[ST_SELECTBOX_ALPHA].i = value;
            update_view_selectbox(accept_);
        }
    );

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
    spinBoxZoomScaleIn->setValue(dialog[ST_ZOOMSCALE_IN].r);
    spinBoxZoomScaleIn->setSingleStep(0.01);
    spinBoxZoomScaleIn->setRange(1.01, 10.00);
    connect(
        spinBoxZoomScaleIn,
        &QDoubleSpinBox::valueChanged,
        this,
        [=](double value) { dialog[ST_ZOOMSCALE_IN].r = (EmbReal)value; }
    );

    QLabel* labelZoomScaleOut = new QLabel(translate_str("Zoom Out Scale"), groupBoxZoom);
    QDoubleSpinBox* spinBoxZoomScaleOut = new QDoubleSpinBox(groupBoxZoom);
    spinBoxZoomScaleOut->setValue(dialog[ST_ZOOMSCALE_OUT].r);
    spinBoxZoomScaleOut->setSingleStep(0.01);
    spinBoxZoomScaleOut->setRange(0.01, 0.99);
    connect(
        spinBoxZoomScaleOut,
        &QDoubleSpinBox::valueChanged,
        this,
        [=](double value) { dialog[ST_ZOOMSCALE_OUT].r = (EmbReal)value; }
    );

    QGridLayout* gridLayoutZoom = new QGridLayout(groupBoxZoom);
    gridLayoutZoom->addWidget(labelZoomScaleIn, 0, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleIn, 0, 1, Qt::AlignRight);
    gridLayoutZoom->addWidget(labelZoomScaleOut, 1, 0, Qt::AlignLeft);
    gridLayoutZoom->addWidget(spinBoxZoomScaleOut, 1, 1, Qt::AlignRight);
    groupBoxZoom->setLayout(gridLayoutZoom);

    QWidget *display_groupboxes[] = {
        groupBoxRender,
        groupBoxScrollBars,
        groupBoxColor,
        groupBoxZoom
    };

    set_vbox_layout(widget, display_groupboxes, 4, 1);

    return create_scrollarea(this, widget);
}

WidgetData cb_prompt_save_history = {
	.type = WIDGET_CHECKBOX,
	.label="Save History",
	.icon="blank",
	.key=ST_SAVE_HISTORY
};

WidgetData cb_save_as_html = {
	.type = WIDGET_CHECKBOX,
	.label="Save As HTML",
	.icon="blank",
	.key=ST_HTML_OUTPUT
};

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
    buttonTextColor->setIcon(swatch(preview[ST_PROMPT_TEXT_COLOR].i));
    connect(buttonTextColor, &QPushButton::clicked, this,
        MAKE_SLOT(chooseColor(ST_PROMPT_TEXT_COLOR)));

    QLabel* labelBGColor = new QLabel(translate_str("Background Color"), groupBoxColor);
    QPushButton* buttonBGColor = new QPushButton(translate_str("Choose"), groupBoxColor);
    buttonBGColor->setIcon(swatch(preview[ST_PROMPT_BG_COLOR].i));
    connect(buttonBGColor, &QPushButton::clicked, this,
        MAKE_SLOT(chooseColor(ST_PROMPT_BG_COLOR)));

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
    comboBoxFontFamily->setCurrentFont(QFont(preview[ST_PROMPT_FONT_FAMILY].s));
    connect(comboBoxFontFamily, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxPromptFontFamilyCurrentIndexChanged(QString)));
    QLabel* labelFontStyle = new QLabel(translate_str("Font Style"), groupBoxFont);
    QComboBox* comboBoxFontStyle = new QComboBox(groupBoxFont);
    comboBoxFontStyle->addItem("Normal");
    comboBoxFontStyle->addItem("Italic");
    comboBoxFontStyle->setEditText(preview[ST_PROMPT_FONT_STYLE].s);
    connect(comboBoxFontStyle, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxPromptFontStyleCurrentIndexChanged(QString)));
    QLabel* labelFontSize = new QLabel(translate_str("Font Size"), groupBoxFont);
    QSpinBox* spinBoxFontSize = new QSpinBox(groupBoxFont);
    spinBoxFontSize->setRange(4, 64);
    spinBoxFontSize->setValue(preview[ST_PROMPT_FONT_SIZE].r);
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

    QWidget* layout_history[2];
    layout_history[0] = make_widget(groupBoxHistory, dialog, cb_prompt_save_history);
    layout_history[1] = make_widget(groupBoxHistory, dialog, cb_save_as_html);
    set_vbox_layout(groupBoxHistory, layout_history, 2, 0);

    //Widget Layout
    QVBoxLayout *vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxColor);
    vboxLayoutMain->addWidget(groupBoxFont);
    vboxLayoutMain->addWidget(groupBoxHistory);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    return create_scrollarea(this, widget);
}

/* . */
QCheckBox*
Settings_Dialog::create_checkbox(QGroupBox *groupbox, std::string label)
{
    QCheckBox* checkbox = new QCheckBox(label.c_str(), groupbox);
    std::string filter = "*." + label;
    checkbox->setChecked(QString(dialog[ST_OPENSAVE_FILTER].s).contains(filter.c_str(), Qt::CaseInsensitive));
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

    copy_node(dialog, settings, ST_OPENSAVE_FILTER);

    int n_extensions = string_array_length(extensions);

    for (int i=0; i<n_extensions; i++) {
        checkBoxCustomFilter[i] = create_checkbox(groupBoxCustomFilter, extensions[i]);
    }

    QPushButton* buttonCustomFilterSelectAll = new QPushButton(translate_str("Select All"), groupBoxCustomFilter);
    connect(buttonCustomFilterSelectAll, &QPushButton::clicked, this,
        [=](void) {
            for (int i=0; i<n_extensions; i++) {
                checkBoxCustomFilter[i]->setChecked(true);
            }
            strcpy(dialog[ST_OPENSAVE_FILTER].s, "supported");
        }
    );

    QPushButton* buttonCustomFilterClearAll = new QPushButton("Clear All", groupBoxCustomFilter);
    connect(buttonCustomFilterClearAll, &QPushButton::clicked, this,
        [=](void) {
            for (int i=0; i<n_extensions; i++) {
                checkBoxCustomFilter[i]->setChecked(false);
            }
            strcpy(dialog[ST_OPENSAVE_FILTER].s, "");
        }
    );

    QGridLayout* gridLayoutCustomFilter = new QGridLayout(groupBoxCustomFilter);
    for (int j=0; j<10; j++)
    for (int i=0; i<10; i++) {
        if (10*j+i >= n_extensions) {
            break;
        }
        gridLayoutCustomFilter->addWidget(checkBoxCustomFilter[10*j+i], i, j, Qt::AlignLeft);
    }
    gridLayoutCustomFilter->addWidget(buttonCustomFilterSelectAll, 0, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->addWidget(buttonCustomFilterClearAll, 1, 6, Qt::AlignLeft);
    gridLayoutCustomFilter->setColumnStretch(6,1);
    groupBoxCustomFilter->setLayout(gridLayoutCustomFilter);

    QString filter(dialog[ST_OPENSAVE_FILTER].s);
    if (filter.contains("supported", Qt::CaseInsensitive)) {
		for (int i=0; i<n_extensions; i++) {
			checkBoxCustomFilter[i]->setChecked(true);
		}
    }

    //Opening
    QGroupBox* groupBoxOpening = new QGroupBox(translate_str("File Open"), widget);

    QComboBox* comboBoxOpenFormat = new QComboBox(groupBoxOpening);

    QCheckBox* checkBoxOpenThumbnail = new QCheckBox(translate_str("Preview Thumbnails"), groupBoxOpening);
    checkBoxOpenThumbnail->setChecked(false);

    //TODO: Add a button to clear the recent history.

    copy_node(dialog, settings, ST_RECENT_MAX);

    QLabel* labelRecentMaxFiles = new QLabel(translate_str("Number of recently accessed files to show"), groupBoxOpening);
    QSpinBox* spinBoxRecentMaxFiles = new QSpinBox(groupBoxOpening);
    spinBoxRecentMaxFiles->setRange(0, 10);
    spinBoxRecentMaxFiles->setValue(dialog[ST_RECENT_MAX].i);
    connect(
        spinBoxRecentMaxFiles,
        &QSpinBox::valueChanged,
        this,
        [=](int value) { dialog[ST_RECENT_MAX].i = value; }
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
    dialog[ST_TRIM_NUM_JUMPS] = settings[ST_TRIM_NUM_JUMPS];
    spinBoxTrimDstNumJumps->setValue(dialog[ST_TRIM_NUM_JUMPS].i);
    connect(
        spinBoxTrimDstNumJumps,
        &QSpinBox::valueChanged,
        this,
        [=](int value){ dialog[ST_TRIM_NUM_JUMPS].i = value; }
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

    return create_scrollarea(this, widget);
}

QWidget* Settings_Dialog::createTabPrinting()
{
    QWidget* widget = new QWidget(this);

    //Default Printer
    QGroupBox* groupBoxDefaultPrinter = new QGroupBox(translate_str("Default Printer"), widget);

    QRadioButton* radioButtonUseSame = new QRadioButton(translate_str("Use as default device"), groupBoxDefaultPrinter);
    radioButtonUseSame->setChecked(!settings[ST_USE_LAST_PRINTER].i);
    QRadioButton* radioButtonUseLast = new QRadioButton(translate_str("Use last used device"), groupBoxDefaultPrinter);
    radioButtonUseLast->setChecked(settings[ST_USE_LAST_PRINTER].i);

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

    copy_node(dialog, settings, ST_PRINT_DISABLE_BG);

    QWidget* checkBoxDisableBG = make_widget(groupBoxSaveInk, dialog,
        {
            .type = WIDGET_CHECKBOX,
            .label = "Disable Background",
            .icon = "blank",
            .key = ST_PRINT_DISABLE_BG
        }
    );

    QVBoxLayout* vboxLayoutSaveInk = new QVBoxLayout(groupBoxSaveInk);
    vboxLayoutSaveInk->addWidget(checkBoxDisableBG);
    groupBoxSaveInk->setLayout(vboxLayoutSaveInk);

    //Widget Layout
    QVBoxLayout* vboxLayoutMain = new QVBoxLayout(widget);
    vboxLayoutMain->addWidget(groupBoxDefaultPrinter);
    vboxLayoutMain->addWidget(groupBoxSaveInk);
    vboxLayoutMain->addStretch(1);
    widget->setLayout(vboxLayoutMain);

    return create_scrollarea(this, widget);
}

QWidget* Settings_Dialog::createTabSnap()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    return create_scrollarea(this, widget);
}

QWidget* Settings_Dialog::createTabGridRuler()
{
    QWidget* widget = new QWidget(this);

    //Grid Misc
    QGroupBox* groupBoxGridMisc = new QGroupBox(translate_str("Grid Misc"), widget);

    copy_node(dialog, settings, ST_GRID_ON_LOAD);
    copy_node(dialog, settings, ST_SHOW_ORIGIN);
    copy_node(dialog, settings, ST_MATCH_GRID_CROSSHAIR);

    if (dialog[ST_MATCH_GRID_CROSSHAIR].r) {
        dialog[ST_GRID_COLOR].i = settings[ST_CROSSHAIR_COLOR].i;
    }
    else {
        dialog[ST_GRID_COLOR].i = settings[ST_GRID_COLOR].i;
    }
    preview[ST_GRID_COLOR] = dialog[ST_GRID_COLOR];
    accept_[ST_GRID_COLOR] = dialog[ST_GRID_COLOR];

    copy_props(dialog, settings, grid_ruler_props);

    copy_node(dialog, settings, ST_RULER_COLOR);
    copy_node(preview, settings, ST_RULER_COLOR);
    copy_node(accept_, settings, ST_RULER_COLOR);

    copy_node(dialog, settings, ST_RULER_SIZE);

    QGridLayout* gridLayoutGridMisc = new QGridLayout(widget);
    gridLayoutGridMisc->addWidget(make_widget(groupBoxGridMisc, dialog,
        checkbox_grid_misc[0]), 0, 0, Qt::AlignLeft);
    gridLayoutGridMisc->addWidget(make_widget(groupBoxGridMisc, dialog,
        checkbox_grid_misc[1]), 1, 0, Qt::AlignLeft);
    groupBoxGridMisc->setLayout(gridLayoutGridMisc);

    //Grid Color
    QGroupBox* groupBoxGridColor = new QGroupBox(translate_str("Grid Color"), widget);

    QCheckBox* checkBoxGridColorMatchCrossHair = new QCheckBox(translate_str("Match grid color to crosshair color"), groupBoxGridColor);
    checkBoxGridColorMatchCrossHair->setChecked(dialog[ST_MATCH_GRID_CROSSHAIR].i);
    connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)));

    QLabel* labelGridColor = new QLabel(translate_str("Grid Color"), groupBoxGridColor);
    labelGridColor->setObjectName("labelGridColor");
    QPushButton* buttonGridColor = new QPushButton(translate_str("Choose"), groupBoxGridColor);
    buttonGridColor->setObjectName("buttonGridColor");
    buttonGridColor->setIcon(swatch(preview[ST_GRID_COLOR].i));
    connect(buttonGridColor, &QPushButton::clicked, this,
        MAKE_SLOT(chooseColor(ST_GRID_COLOR)));

    labelGridColor->setEnabled(!dialog[ST_MATCH_GRID_CROSSHAIR].i);
    buttonGridColor->setEnabled(!dialog[ST_MATCH_GRID_CROSSHAIR].i);

    QGridLayout* gridLayoutGridColor = new QGridLayout(widget);
    gridLayoutGridColor->addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(labelGridColor, 1, 0, Qt::AlignLeft);
    gridLayoutGridColor->addWidget(buttonGridColor, 1, 1, Qt::AlignRight);
    groupBoxGridColor->setLayout(gridLayoutGridColor);

    //Grid Geometry
    QGroupBox* groupBoxGridGeom = new QGroupBox(translate_str("Grid Geometry"), widget);

    QWidget* checkBoxGridLoadFromFile = make_widget(groupBoxGridGeom, dialog,
        cb_grid_load_from_file);

    QLabel* labelGridType = new QLabel(translate_str("Grid Type"), groupBoxGridGeom);
    labelGridType->setObjectName("labelGridType");
    QComboBox* comboBoxGridType = new QComboBox(groupBoxGridGeom);
    comboBoxGridType->setObjectName("comboBoxGridType");
    comboBoxGridType->addItem("Rectangular");
    comboBoxGridType->addItem("Circular");
    comboBoxGridType->addItem("Isometric");
    QString grid_type(dialog[ST_GRID_TYPE].s);
    comboBoxGridType->setCurrentIndex(comboBoxGridType->findText(grid_type));
    connect(comboBoxGridType, SIGNAL(currentIndexChanged(QString)), this,
        SLOT(comboBoxGridTypeCurrentIndexChanged(QString)));

    QWidget* checkBoxGridCenterOnOrigin = make_widget(groupBoxGridGeom, dialog,
        cb_center_on_origin);

    int grid_load = dialog[ST_GRID_LOAD_FROM_FILE].i;
    labelGridType->setEnabled(!grid_load);
    comboBoxGridType->setEnabled(!grid_load);
    checkBoxGridCenterOnOrigin->setEnabled(!grid_load);

    bool enabled = !dialog[ST_GRID_LOAD_FROM_FILE].i;
    bool visible = !string_equal(dialog[ST_GRID_TYPE].s, "Circular");

    QGridLayout* layoutGridGeom = new QGridLayout(groupBoxGridGeom);
    layoutGridGeom->addWidget(checkBoxGridLoadFromFile, 0, 0, Qt::AlignLeft);
    layoutGridGeom->addWidget(labelGridType, 1, 0, Qt::AlignLeft);
    layoutGridGeom->addWidget(comboBoxGridType, 1, 1, Qt::AlignRight);
    layoutGridGeom->addWidget(checkBoxGridCenterOnOrigin, 2, 0, Qt::AlignLeft);
    for (int i=0; i<9; i++) {
        QWidget *label = make_widget(groupBoxGridGeom, NULL, grid_geometry_widgets[2*i+0]);
        label->setEnabled(enabled);
        label->setVisible(visible);
        layoutGridGeom->addWidget(label, 3+i, 0, Qt::AlignLeft);

        QWidget *spinbox = make_widget(groupBoxGridGeom, NULL, grid_geometry_widgets[2*i+1]);
        spinbox->setEnabled(enabled);
        spinbox->setVisible(visible);
        layoutGridGeom->addWidget(spinbox, 3+i, 1, Qt::AlignRight);
    }
    groupBoxGridGeom->setLayout(layoutGridGeom);

    //Ruler Misc
    QGroupBox* groupBoxRulerMisc = new QGroupBox(translate_str("Ruler Misc"), widget);

    copy_node(dialog, settings, ST_RULER_ON_LOAD);
    copy_node(dialog, settings, ST_RULER_METRIC);

    QCheckBox* checkBoxRulerShowOnLoad = new QCheckBox(translate_str("Initially show ruler when loading a file"), groupBoxRulerMisc);
    checkBoxRulerShowOnLoad->setChecked(dialog[ST_RULER_ON_LOAD].i);
    connect(checkBoxRulerShowOnLoad, &QCheckBox::stateChanged, this,
        [=](int value)
        {
            dialog[ST_RULER_ON_LOAD].i = value;
        }
    );

    QLabel* labelRulerMetric = new QLabel(translate_str("Ruler Units"), groupBoxRulerMisc);
    QComboBox* comboBoxRulerMetric = new QComboBox(groupBoxRulerMisc);
    comboBoxRulerMetric->addItem("Imperial", false);
    comboBoxRulerMetric->addItem("Metric", true);
    comboBoxRulerMetric->setCurrentIndex(comboBoxRulerMetric->findData(dialog[ST_RULER_METRIC].i));
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
    buttonRulerColor->setIcon(swatch(preview[ST_RULER_COLOR].i));
    connect(buttonRulerColor, &QPushButton::clicked, this,
        MAKE_SLOT(chooseColor(ST_RULER_COLOR)));

    QGridLayout* gridLayoutRulerColor = new QGridLayout(widget);
    gridLayoutRulerColor->addWidget(labelRulerColor, 1, 0, Qt::AlignLeft);
    gridLayoutRulerColor->addWidget(buttonRulerColor, 1, 1, Qt::AlignRight);
    groupBoxRulerColor->setLayout(gridLayoutRulerColor);

    //Ruler Geometry
    QGroupBox* groupBoxRulerGeom = new QGroupBox(translate_str("Ruler Geometry"), widget);

    QLabel* labelRulerPixelSize = new QLabel(translate_str("Ruler Pixel Size"), groupBoxRulerGeom);
    labelRulerPixelSize->setObjectName("labelRulerPixelSize");
    QWidget* spinBoxRulerPixelSize = make_widget(groupBoxRulerGeom, dialog,
        sb_ruler_pixel_size);

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

    return create_scrollarea(this, widget);
}

QWidget*
Settings_Dialog::createTabOrthoPolar()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    return create_scrollarea(this, widget);
}


QWidget*
Settings_Dialog::createTabQuickSnap()
{
    QWidget* widget = new QWidget(this);

    //QSnap Locators
    QGroupBox* groupBoxQSnapLoc = new QGroupBox(translate_str("Locators Used"), widget);

    make_editing_copy(quick_snap_props);

    QWidget* checkboxes[20];
    for (int i=0; i<qsnap_n_checkboxes; i++) {
        checkboxes[i] = make_widget(groupBoxQSnapLoc, dialog, qsnap_mode_checkboxes[i]);
    }

    QPushButton* buttonQSnapSelectAll = new QPushButton(translate_str("Select All"), groupBoxQSnapLoc);
    connect(buttonQSnapSelectAll, &QPushButton::clicked, this,
        [=]() {
            for (int i=ST_QSNAP_LOCATOR_COLOR; i<=ST_QSNAP_PARALLEL; i++) {
                dialog[i].i = true;
            }
        }
    );

    QPushButton* buttonQSnapClearAll = new QPushButton(translate_str("Clear All"), groupBoxQSnapLoc);
    connect(buttonQSnapClearAll, &QPushButton::clicked, this,
        [=]() {
            for (int i=ST_QSNAP_LOCATOR_COLOR; i<=ST_QSNAP_PARALLEL; i++) {
                dialog[i].i = false;
            }
        }
    );

    QGridLayout* gridLayoutQSnap = new QGridLayout(groupBoxQSnapLoc);
    for (int i=0; i<qsnap_n_checkboxes; i++) {
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
    comboBoxQSnapLocColor->setCurrentIndex(comboBoxQSnapLocColor->findData(dialog[ST_QSNAP_LOCATOR_COLOR].i));
    connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)));

    QLabel* labelQSnapLocSize = new QLabel(translate_str("Locator Size"), groupBoxQSnapVisual);
    QSlider* sliderQSnapLocSize = new QSlider(Qt::Horizontal, groupBoxQSnapVisual);
    sliderQSnapLocSize->setRange(1,20);
    sliderQSnapLocSize->setValue(dialog[ST_QSNAP_LOCATOR_SIZE].r);
    connect(sliderQSnapLocSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog[ST_QSNAP_LOCATOR_SIZE].i = x; }));

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
    sliderQSnapApertureSize->setValue(dialog[ST_QSNAP_APERTURE_SIZE].r);
    connect(sliderQSnapApertureSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog[ST_QSNAP_APERTURE_SIZE].r = x; } ));

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

    return create_scrollarea(this, widget);
}

QWidget*
Settings_Dialog::createTabQuickTrack()
{
    QWidget* widget = new QWidget(this);

    //TODO: finish this

    return create_scrollarea(this, widget);
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
        dialog[ST_LWT_SHOW].i = s->property("ENABLE_LWT").toBool();
    }
    else {
        copy_node(dialog, settings, ST_LWT_SHOW);
    }
    copy_node(preview, dialog, ST_LWT_SHOW);
    checkBoxShowLwt->setChecked(preview[ST_LWT_SHOW].i);
    connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)));

    QCheckBox* checkBoxRealRender = new QCheckBox(translate_str("RealRender"), groupBoxLwtMisc);
    checkBoxRealRender->setObjectName("checkBoxRealRender");
    if (s) {
        dialog[ST_LWT_REAL].i = s->property("ENABLE_REAL").toBool();
    }
    else  {
        copy_node(dialog, settings, ST_LWT_REAL);
    }
    copy_node(preview, dialog, ST_LWT_REAL);
    checkBoxRealRender->setChecked(preview[ST_LWT_REAL].i);
    connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)));
    checkBoxRealRender->setEnabled(dialog[ST_LWT_SHOW].i);

    QLabel* labelDefaultLwt = new QLabel(translate_str("Default weight"), groupBoxLwtMisc);
    labelDefaultLwt->setEnabled(false); //TODO: remove later
    QComboBox* comboBoxDefaultLwt = new QComboBox(groupBoxLwtMisc);
    copy_node(dialog, settings, ST_LWT_DEFAULT);
    //TODO: populate the comboBox and set the initial value
    comboBoxDefaultLwt->addItem(QString().setNum(dialog[ST_LWT_DEFAULT].r, 'F', 2).append(" mm"), dialog[ST_LWT_DEFAULT].r);
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

    return create_scrollarea(this, widget);
}

QWidget*
Settings_Dialog::createTabSelection()
{
    QWidget* widget = new QWidget(this);

    //Selection Modes
    QGroupBox* groupBoxSelectionModes = new QGroupBox(translate_str("Modes"), widget);

    copy_node(dialog, settings, ST_SELECTION_PICK_FIRST);
    copy_node(dialog, settings, ST_SELECTION_PICK_ADD);
    copy_node(dialog, settings, ST_SELECTION_PICK_DRAG);
    copy_node(dialog, settings, ST_SELECTION_COOLGRIP_COLOR);
    copy_node(dialog, settings, ST_SELECTION_HOTGRIP_COLOR);
    copy_node(dialog, settings, ST_SELECTION_GRIP_SIZE);
    copy_node(dialog, settings, ST_SELECTION_PICKBOX_SIZE);

    QCheckBox* checkBoxSelectionModePickFirst = new QCheckBox(translate_str("Allow Preselection (PickFirst)"), groupBoxSelectionModes);
    checkBoxSelectionModePickFirst->setChecked(dialog[ST_SELECTION_PICK_FIRST].i);
    checkBoxSelectionModePickFirst->setChecked(true);
    checkBoxSelectionModePickFirst->setEnabled(false); //TODO: Remove this line when Post-selection is available
    connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this,
        SLOT([=](int x) {dialog[ST_SELECTION_PICK_FIRST].i = x; } ));

    QWidget* checkBoxSelectionModePickAdd = make_widget(groupBoxSelectionModes, dialog,
    {
        .type=WIDGET_CHECKBOX,
        .label="Add to Selection (PickAdd)",
        .icon="blank",
        .key=ST_SELECTION_PICK_ADD
    }
    );

    QCheckBox* checkBoxSelectionModePickDrag = new QCheckBox(translate_str("Drag to Select (PickDrag)"), groupBoxSelectionModes);
    checkBoxSelectionModePickDrag->setChecked(dialog[ST_SELECTION_PICK_ADD].i);
    checkBoxSelectionModePickDrag->setChecked(false);
    checkBoxSelectionModePickDrag->setEnabled(false); //TODO: Remove this line when this functionality is available
    connect(checkBoxSelectionModePickDrag, SIGNAL(stateChanged(int)), this,
        SLOT([=](int x) {dialog[ST_SELECTION_PICK_ADD].i = x; } ));

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
    comboBoxCoolGripColor->setCurrentIndex(comboBoxCoolGripColor->findData(dialog[ST_SELECTION_COOLGRIP_COLOR].i));
    connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this,
        SLOT([=](int x) { dialog[ST_SELECTION_COOLGRIP_COLOR].i = x; } ));

    QLabel* labelHotGripColor = new QLabel(translate_str("Hot Grip (Selected)"), groupBoxSelectionColors);
    QComboBox* comboBoxHotGripColor = new QComboBox(groupBoxSelectionColors);
    addColorsToComboBox(comboBoxHotGripColor);
    comboBoxHotGripColor->setCurrentIndex(comboBoxHotGripColor->findData(dialog[ST_SELECTION_HOTGRIP_COLOR].i));
    connect(comboBoxHotGripColor, SIGNAL(currentIndexChanged(int)), this,
        SLOT([=](int x) { dialog[ST_HOTGRIP_COLOR].i = x; } ));

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
    sliderSelectionGripSize->setValue(dialog[ST_SELECTION_GRIP_SIZE].i);
    connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog[ST_SELECTION_GRIP_SIZE].i = x; } ));

    QLabel* labelSelectionPickBoxSize = new QLabel(translate_str("Pickbox Size"), groupBoxSelectionSizes);
    QSlider* sliderSelectionPickBoxSize = new QSlider(Qt::Horizontal, groupBoxSelectionSizes);
    sliderSelectionPickBoxSize->setRange(1,20);
    sliderSelectionPickBoxSize->setValue(dialog[ST_SELECTION_PICKBOX_SIZE].i);
    connect(sliderSelectionPickBoxSize, SIGNAL(valueChanged(int)), this,
        SLOT([=](int x) { dialog[ST_SELECTION_PICKBOX_SIZE].i = x; } ));

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

    return create_scrollarea(this, widget);
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

/*
void
Settings_Dialog::comboBoxIconSizeCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        dialog[ST_ICON_SIZE].i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog[ST_ICON_SIZE].i = 16;
        }
    }
    else {
        dialog[ST_ICON_SIZE].i = 16;
    }
}

void
Settings_Dialog::checkBoxGeneralMdiBGUseLogoStateChanged(int checked)
{
    preview[ST_MDI_USE_LOGO].i = checked;
    mdiArea->useLogo = checked;
    mdiArea->forceRepaint();
}

void
Settings_Dialog::chooseGeneralMdiBackgroundLogo()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
	QString selectedImage;
	selectedImage = QFileDialog::getOpenFileName(this, translate_str("Open File"),
					QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
					translate_str("Images (*.bmp *.png *.jpg)"));

	if (!selectedImage.isNull()) {
		strcpy(accept_[ST_MDI_LOGO].s, selectedImage.toStdString().c_str());
	}

	//Update immediately so it can be previewed
	mdiArea->bgLogo.load(accept_[ST_MDI_LOGO].s);
	mdiArea->forceRepaint();
}

void
Settings_Dialog::checkBoxGeneralMdiBGUseTextureStateChanged(int checked)
{
    preview[ST_MDI_USE_TEXTURE].i = checked;
    mdiArea->useTexture = checked;
    mdiArea->forceRepaint();
}

void Settings_Dialog::chooseGeneralMdiBackgroundTexture()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        return;
    }
	QString selectedImage;
	selectedImage = QFileDialog::getOpenFileName(this, translate_str("Open File"),
					QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
					translate_str("Images (*.bmp *.png *.jpg)"));

	if (!selectedImage.isNull()) {
		strcpy(accept_[ST_MDI_TEXTURE].s, selectedImage.toStdString().c_str());
	}

	//Update immediately so it can be previewed
	mdiArea->bgTexture.load(accept_[ST_MDI_TEXTURE].s);
	mdiArea->forceRepaint();
}

void Settings_Dialog::checkBoxGeneralMdiBGUseColorStateChanged(int checked)
{
    preview[ST_MDI_USE_COLOR].i = checked;
    mdiArea->useColor = checked;
    mdiArea->forceRepaint();
}

void Settings_Dialog::chooseGeneralMdiBackgroundColor()
{
    chooseColor(ST_MDI_COLOR);
}

void Settings_Dialog::currentGeneralMdiBackgroundColorChanged(const QColor& color)
{
    preview[ST_MDI_COLOR].i = color.rgb();
    mdiArea->bgColor = color;
    mdiArea->forceRepaint();
}

void Settings_Dialog::checkBoxShowScrollBarsStateChanged(int checked)
{
    preview[ST_SHOW_SCROLLBARS].i = checked;
    _mainWin->updateAllViewScrollBars(checked);
}
*/

void
update_view_selectbox(Node *d)
{
    _mainWin->updateAllViewSelectBoxColors(
        d[ST_SELECTBOX_LEFT_COLOR].i,
        d[ST_SELECTBOX_LEFT_FILL].i,
        d[ST_SELECTBOX_RIGHT_COLOR].i,
        d[ST_SELECTBOX_RIGHT_FILL].i,
        preview[ST_SELECTBOX_ALPHA].i);
}

void
Settings_Dialog::colorChanged(const QColor& color, Node *d, int color_id)
{
	d[color_id].i = color.rgb();
    switch (color_id) {
    case ST_PROMPT_TEXT_COLOR: {
        prompt->setPromptTextColor(color);
        break;
    }

    case ST_PROMPT_BG_COLOR: {
        prompt->setPromptBackgroundColor(color);
        break;
    }

    case ST_SELECTBOX_LEFT_COLOR:
    case ST_SELECTBOX_LEFT_FILL:
    case ST_SELECTBOX_RIGHT_COLOR:
    case ST_SELECTBOX_RIGHT_FILL: {
        update_view_selectbox(d);
        break;
    }

    case ST_MDI_COLOR: {
        mdiArea->bgColor = QColor(d[ST_MDI_COLOR].i);
        break;
    }

    case ST_CROSSHAIR_COLOR: {
        _mainWin->updateAllViewCrossHairColors(d[color_id].i);
        break;
    }

    case ST_BG_COLOR: {
		_mainWin->updateAllViewBackgroundColors(d[color_id].i);
        break;
    }

    default:
        break;
    }
}


/*
void Settings_Dialog::comboBoxPromptFontFamilyCurrentIndexChanged(QString family)
{
    strcpy(preview[ST_PROMPT_FONT_FAMILY].s, family.toStdString().c_str());
    prompt->setPromptFontFamily(preview[ST_PROMPT_FONT_FAMILY].s);
}

void Settings_Dialog::comboBoxPromptFontStyleCurrentIndexChanged(QString style)
{
    strcpy(preview[ST_PROMPT_FONT_STYLE].s, style.toStdString().c_str());
    prompt->setPromptFontStyle(preview[ST_PROMPT_FONT_STYLE].s);
}

void Settings_Dialog::spinBoxPromptFontSizeValueChanged(int value)
{
    preview[ST_PROMPT_FONT_SIZE].i = value;
    prompt->setPromptFontSize(preview[ST_PROMPT_FONT_SIZE].i);
}

void Settings_Dialog::checkBoxCustomFilterStateChanged(int checked)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox) {
        QString format = checkBox->text();
        qDebug("CustomFilter: %s %d", qPrintable(format), checked);
        if (checked) {
            dialog.opensave_custom_filter.append(" *." + format.toLower());
        }
        else {
            dialog.opensave_custom_filter.remove("*." + format, Qt::CaseInsensitive);
        }
        //dialog.opensave_custom_filter = checked; //TODO
    }
}

void Settings_Dialog::checkBoxGridColorMatchCrossHairStateChanged(int checked)
{
    dialog[ST_MATCH_GRID_CROSSHAIR].i = checked;
    if (checked) {
        _mainWin->updateAllViewGridColors(accept_[ST_MATCH_GRID_CROSSHAIR].i);
    }
    else {
        _mainWin->updateAllViewGridColors(accept_[ST_GRID_COLOR].i);
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
    chooseColor(ST_GRID_COLOR);
}

void Settings_Dialog::currentGridColorChanged(const QColor& color)
{
    preview[ST_GRID_COLOR].i = color.rgb();
    _mainWin->updateAllViewGridColors(color.rgb());
}

void Settings_Dialog::checkBoxGridLoadFromFileStateChanged(int checked)
{
    dialog[ST_GRID_LOAD_FROM_FILE].i = checked;

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

    bool not_center = !checked && !dialog[ST_GRID_CENTER_ORIGIN].i;

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


 * @brief Settings_Dialog::comboBoxGridTypeCurrentIndexChanged
void Settings_Dialog::comboBoxGridTypeCurrentIndexChanged(QString type)
{
    strcpy(dialog[ST_GRID_TYPE].s, type.toStdString().c_str());

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
    dialog[ST_GRID_CENTER_ORIGIN].i = checked;

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
    dialog[ST_RULER_ON_LOAD].i = checked;
}

void Settings_Dialog::comboBoxRulerMetricCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    if (comboBox) {
        bool ok = 0;
        dialog[ST_RULER_METRIC].i = (comboBox->itemData(index).toBool());
    }
    else {
        dialog[ST_RULER_METRIC].i = 1;
    }
}

void Settings_Dialog::chooseRulerColor()
{
    chooseColor(ST_RULER_COLOR);
}

void Settings_Dialog::currentRulerColorChanged(const QColor& color)
{
    preview[ST_RULER_COLOR].i = color.rgb();
    _mainWin->updateAllViewRulerColors(preview[ST_RULER_COLOR].i);
}

void Settings_Dialog::spinBoxRulerPixelSizeValueChanged(double value)
{
    dialog[ST_RULER_SIZE].r = (EmbReal)value;
}

void Settings_Dialog::buttonQSnapClearAllClicked()
{
    emit buttonQSnapClearAll(false);
}

void Settings_Dialog::comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
{
    //TODO: Alert user if color matched the display bg color
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    comboBoxColor(comboBox, qRgb(255, 255, 0), ST_QSNAP_LOCATOR_COLOR, index);
}

void Settings_Dialog::checkBoxLwtShowLwtStateChanged(int checked)
{
    preview[ST_LWT_SHOW].i = checked;
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
    preview[ST_LWT_REAL].i = checked;
    statusbar->toggle("LWT", checked);
}
*/

void
comboBoxColor(QComboBox* comboBox, QRgb defaultColor, int color_id, int index)
{
    if (comboBox) {
        bool ok = 0;
        dialog[color_id].i = comboBox->itemData(index).toUInt(&ok);
        if (!ok) {
            dialog[color_id].i = defaultColor;
        }
    }
    else {
        dialog[color_id].i = defaultColor;
    }
}

/* TODO: Alert user if color matched the display bg color.
void Settings_Dialog::comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    comboBoxColor(comboBox, qRgb(0,0,255), ST_SELECTION_COOLGRIP_COLOR, index);
}

TODO: Alert user if color matched the display bg color.
void
Settings_Dialog::comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
    comboBoxColor(comboBox, qRgb(255,0,0), ST_SELECTION_HOTGRIP_COLOR, index);
}
*/

/**
 * .
 */
void Settings_Dialog::acceptChanges()
{
    copy_node(dialog, preview, ST_MDI_USE_LOGO);
    copy_node(dialog, preview, ST_MDI_USE_TEXTURE);
    copy_node(dialog, preview, ST_MDI_USE_COLOR);
    copy_node(dialog, accept_, ST_MDI_LOGO);
    copy_node(dialog, accept_, ST_MDI_TEXTURE);
    copy_node(dialog, accept_, ST_MDI_COLOR);
    copy_node(dialog, preview, ST_SHOW_SCROLLBARS);
    copy_node(dialog, accept_, ST_CROSSHAIR_COLOR);
    copy_node(dialog, accept_, ST_BG_COLOR);
    copy_node(dialog, accept_, ST_SELECTBOX_LEFT_COLOR);
    copy_node(dialog, accept_, ST_SELECTBOX_LEFT_FILL);
    copy_node(dialog, accept_, ST_SELECTBOX_RIGHT_COLOR);
    copy_node(dialog, accept_, ST_SELECTBOX_RIGHT_FILL);
    copy_node(dialog, preview, ST_SELECTBOX_ALPHA);
    copy_node(dialog, accept_, ST_PROMPT_TEXT_COLOR);
    copy_node(dialog, accept_, ST_PROMPT_BG_COLOR);
    copy_node(dialog, preview, ST_PROMPT_FONT_FAMILY);
    copy_node(dialog, preview, ST_PROMPT_FONT_STYLE);
    copy_node(dialog, preview, ST_PROMPT_FONT_STYLE);
    if (dialog[ST_MATCH_GRID_CROSSHAIR].i) {
        dialog[ST_GRID_COLOR].i = accept_[ST_CROSSHAIR_COLOR].i;
    }
    else {
        copy_node(dialog, accept_, ST_GRID_COLOR);
    }
    copy_node(dialog, accept_, ST_RULER_COLOR);
    copy_node(dialog, accept_, ST_LWT_SHOW);
    copy_node(dialog, accept_, ST_LWT_REAL);

    memcpy(settings, dialog, SETTINGS_TOTAL * sizeof settings[0]);

    // Make sure the user sees the changes applied immediately
    mdiArea->useLogo = dialog[ST_MDI_USE_LOGO].i;
    mdiArea->useTexture = dialog[ST_MDI_USE_TEXTURE].i;
    mdiArea->useColor = dialog[ST_MDI_USE_COLOR].i;
    mdiArea->bgLogo.load(dialog[ST_MDI_LOGO].s);
    mdiArea->bgTexture.load(dialog[ST_MDI_TEXTURE].s);
    mdiArea->bgColor = dialog[ST_MDI_COLOR].i;
    _mainWin->iconResize(dialog[ST_ICON_SIZE].i);
    _mainWin->updateAllViewScrollBars(dialog[ST_SHOW_SCROLLBARS].i);
    _mainWin->updateAllViewCrossHairColors(dialog[ST_CROSSHAIR_COLOR].i);
    _mainWin->updateAllViewBackgroundColors(dialog[ST_BG_COLOR].i);
    _mainWin->updateAllViewSelectBoxColors(
        dialog[ST_SELECTBOX_LEFT_COLOR].i,
        dialog[ST_SELECTBOX_LEFT_FILL].i,
        dialog[ST_SELECTBOX_RIGHT_COLOR].i,
        dialog[ST_SELECTBOX_RIGHT_FILL].i,
        dialog[ST_SELECTBOX_ALPHA].i);
    prompt->setPromptTextColor(QColor(dialog[ST_PROMPT_TEXT_COLOR].i));
    prompt->setPromptBackgroundColor(QColor(dialog[ST_PROMPT_BG_COLOR].i));
    prompt->setPromptFontFamily(dialog[ST_PROMPT_FONT_FAMILY].s);
    prompt->setPromptFontStyle(dialog[ST_PROMPT_FONT_STYLE].s);
    prompt->setPromptFontSize(dialog[ST_PROMPT_FONT_SIZE].i);
    _mainWin->updateAllViewGridColors(dialog[ST_GRID_COLOR].i);
    _mainWin->updateAllViewRulerColors(dialog[ST_RULER_COLOR].i);
    statusbar->toggle("LWT", dialog[ST_LWT_SHOW].i);
    statusbar->toggle("REAL", dialog[ST_LWT_REAL].i);
    _mainWin->updatePickAddMode(dialog[ST_SELECTION_PICK_ADD].i);

    mdiArea->forceRepaint();

    write_settings();
    accept();
}

/* Reject changes. */
void
Settings_Dialog::rejectChanges()
{
    //TODO: inform the user if they have changed settings

    //Update the view since the user must accept the preview
    mdiArea->useLogo = dialog[ST_MDI_USE_LOGO].i;
    mdiArea->useTexture = dialog[ST_MDI_USE_TEXTURE].i;
    mdiArea->useColor = dialog[ST_MDI_USE_COLOR].i;
    mdiArea->bgLogo.load(dialog[ST_MDI_LOGO].s);
    mdiArea->bgTexture.load(dialog[ST_MDI_TEXTURE].s);
    mdiArea->bgColor = dialog[ST_MDI_COLOR].i;
    _mainWin->updateAllViewScrollBars(dialog[ST_SHOW_SCROLLBARS].i);
    _mainWin->updateAllViewCrossHairColors(dialog[ST_CROSSHAIR_COLOR].i);
    _mainWin->updateAllViewBackgroundColors(dialog[ST_BG_COLOR].i);
    _mainWin->updateAllViewSelectBoxColors(
        dialog[ST_SELECTBOX_LEFT_COLOR].i,
        dialog[ST_SELECTBOX_LEFT_FILL].i,
        dialog[ST_SELECTBOX_RIGHT_COLOR].i,
        dialog[ST_SELECTBOX_RIGHT_FILL].i,
        dialog[ST_SELECTBOX_ALPHA].i);
    prompt->setPromptTextColor(QColor(dialog[ST_PROMPT_TEXT_COLOR].i));
    prompt->setPromptBackgroundColor(QColor(dialog[ST_PROMPT_BG_COLOR].i));
    prompt->setPromptFontFamily(dialog[ST_PROMPT_FONT_FAMILY].s);
    prompt->setPromptFontStyle(dialog[ST_PROMPT_FONT_STYLE].s);
    prompt->setPromptFontSize(dialog[ST_PROMPT_FONT_SIZE].r);
    _mainWin->updateAllViewGridColors(dialog[ST_GRID_COLOR].i);
    _mainWin->updateAllViewRulerColors(dialog[ST_RULER_COLOR].i);
    statusbar->toggle("LWT", settings[ST_LWT_SHOW].i);
    statusbar->toggle("REAL", settings[ST_LWT_REAL].i);

    mdiArea->forceRepaint();

    reject();
}

