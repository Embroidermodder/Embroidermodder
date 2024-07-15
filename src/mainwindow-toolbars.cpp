/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 * Visit https://www.libembroidery.org/refman for advice on altering this file,
 * or read the markdown version in embroidermodder2/docs/refman.
 *
 * Toolbars
 */

#include "embroidermodder.h"

void
add_to_toolbar(const char *toolbar_name, string_table toolbar_data)
{
    QString s(toolbar_name);
    toolbarHash[s]->setObjectName("toolbar" + s);

    int n = string_array_length(toolbar_data);
    for (int i=0; i<n; i++) {
        if (toolbar_data[i][0] == '-') {
            toolbarHash[s]->addSeparator();
        }
        else {
            QAction *action = get_action_by_icon(toolbar_data[i]);
            toolbarHash[s]->addAction(action);
        }
    }

    QAction::connect(toolbarHash[s], SIGNAL(topLevelChanged(bool)), _main,
        SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createLayerToolbar()
{
    qDebug("MainWindow createLayerToolbar()");

    toolbarHash["Layer"]->setObjectName("toolbarLayer");
    toolbarHash["Layer"]->addAction(actionHash.value(ACTION_MAKE_LAYER_CURRENT));
    toolbarHash["Layer"]->addAction(actionHash.value(ACTION_LAYERS));

    QString appDir = qApp->applicationDirPath();
    QString icontheme = general_icon_theme.setting;

    layerSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    //TODO: Create layer pixmaps by concatenating several icons
    layerSelector->addItem(create_icon("linetypebylayer"), "0");
    layerSelector->addItem(create_icon("linetypebylayer"), "1");
    layerSelector->addItem(create_icon("linetypebylayer"), "2");
    layerSelector->addItem(create_icon("linetypebylayer"), "3");
    layerSelector->addItem(create_icon("linetypebylayer"), "4");
    layerSelector->addItem(create_icon("linetypebylayer"), "5");
    layerSelector->addItem(create_icon("linetypebylayer"), "6");
    layerSelector->addItem(create_icon("linetypebylayer"), "7");
    layerSelector->addItem(create_icon("linetypebylayer"), "8");
    layerSelector->addItem(create_icon("linetypebylayer"), "9");
    toolbarHash["Layer"]->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarHash["Layer"]->addAction(actionHash.value(ACTION_LAYER_PREVIOUS));

    connect(toolbarHash["Layer"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPropertiesToolbar()
{
    qDebug("MainWindow createPropertiesToolbar()");

    toolbarHash["Properties"]->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    colorSelector->addItem(create_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(create_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(create_icon("colorred"), tr("Red"), qRgb(255, 0, 0));
    colorSelector->addItem(create_icon("coloryellow"), tr("Yellow"), qRgb(255, 255, 0));
    colorSelector->addItem(create_icon("colorgreen"), tr("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(create_icon("colorcyan"), tr("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(create_icon("colorblue"), tr("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(create_icon("colormagenta"), tr("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(create_icon("colorwhite"), tr("White"),   qRgb(255,255,255));
    colorSelector->addItem(create_icon("colorother"), tr("Other..."));
    toolbarHash["Properties"]->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarHash["Properties"]->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    linetypeSelector->addItem(create_icon("linetypebylayer"   ), "ByLayer");
    linetypeSelector->addItem(create_icon("linetypebyblock"   ), "ByBlock");
    linetypeSelector->addItem(create_icon("linetypecontinuous"), "Continuous");
    linetypeSelector->addItem(create_icon("linetypehidden"    ), "Hidden");
    linetypeSelector->addItem(create_icon("linetypecenter"    ), "Center");
    linetypeSelector->addItem(create_icon("linetypeother"     ), "Other...");
    toolbarHash["Properties"]->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarHash["Properties"]->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    lineweightSelector->addItem(create_icon("lineweightbylayer"), "ByLayer", -2.00);
    lineweightSelector->addItem(create_icon("lineweightbyblock"), "ByBlock", -1.00);
    lineweightSelector->addItem(create_icon("lineweightdefault"), "Default",  0.00);
    //TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(create_icon("lineweight01"), "0.00 mm", 0.00);
    lineweightSelector->addItem(create_icon("lineweight02"), "0.05 mm", 0.05);
    lineweightSelector->addItem(create_icon("lineweight03"), "0.15 mm", 0.15);
    lineweightSelector->addItem(create_icon("lineweight04"), "0.20 mm", 0.20);
    lineweightSelector->addItem(create_icon("lineweight05"), "0.25 mm", 0.25);
    lineweightSelector->addItem(create_icon("lineweight06"), "0.30 mm", 0.30);
    lineweightSelector->addItem(create_icon("lineweight07"), "0.35 mm", 0.35);
    lineweightSelector->addItem(create_icon("lineweight08"), "0.40 mm", 0.40);
    lineweightSelector->addItem(create_icon("lineweight09"), "0.45 mm", 0.45);
    lineweightSelector->addItem(create_icon("lineweight10"), "0.50 mm", 0.50);
    lineweightSelector->addItem(create_icon("lineweight11"), "0.55 mm", 0.55);
    lineweightSelector->addItem(create_icon("lineweight12"), "0.60 mm", 0.60);
    lineweightSelector->addItem(create_icon("lineweight13"), "0.65 mm", 0.65);
    lineweightSelector->addItem(create_icon("lineweight14"), "0.70 mm", 0.70);
    lineweightSelector->addItem(create_icon("lineweight15"), "0.75 mm", 0.75);
    lineweightSelector->addItem(create_icon("lineweight16"), "0.80 mm", 0.80);
    lineweightSelector->addItem(create_icon("lineweight17"), "0.85 mm", 0.85);
    lineweightSelector->addItem(create_icon("lineweight18"), "0.90 mm", 0.90);
    lineweightSelector->addItem(create_icon("lineweight19"), "0.95 mm", 0.95);
    lineweightSelector->addItem(create_icon("lineweight20"), "1.00 mm", 1.00);
    lineweightSelector->addItem(create_icon("lineweight21"), "1.05 mm", 1.05);
    lineweightSelector->addItem(create_icon("lineweight22"), "1.10 mm", 1.10);
    lineweightSelector->addItem(create_icon("lineweight23"), "1.15 mm", 1.15);
    lineweightSelector->addItem(create_icon("lineweight24"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarHash["Properties"]->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarHash["Properties"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createTextToolbar()
{
    qDebug("MainWindow createTextToolbar()");

    toolbarHash["Text"]->setObjectName("toolbarText");

    toolbarHash["Text"]->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(text_font.setting));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_BOLD));
    actionHash.value(ACTION_TEXT_BOLD)->setChecked(text_style_bold.setting);
    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_ITALIC));
    actionHash.value(ACTION_TEXT_ITALIC)->setChecked(text_style_italic.setting);
    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_UNDERLINE));
    actionHash.value(ACTION_TEXT_UNDERLINE)->setChecked(text_style_underline.setting);
    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_STRIKEOUT));
    actionHash.value(ACTION_TEXT_STRIKEOUT)->setChecked(text_style_strikeout.setting);
    toolbarHash["Text"]->addAction(actionHash.value(ACTION_TEXT_OVERLINE));
    actionHash.value(ACTION_TEXT_OVERLINE)->setChecked(text_style_overline.setting);

    textSizeSelector->setFocusProxy(prompt);
    textSizeSelector->addItem("6 pt",   6);
    textSizeSelector->addItem("8 pt",   8);
    textSizeSelector->addItem("9 pt",   9);
    textSizeSelector->addItem("10 pt", 10);
    textSizeSelector->addItem("11 pt", 11);
    textSizeSelector->addItem("12 pt", 12);
    textSizeSelector->addItem("14 pt", 14);
    textSizeSelector->addItem("18 pt", 18);
    textSizeSelector->addItem("24 pt", 24);
    textSizeSelector->addItem("30 pt", 30);
    textSizeSelector->addItem("36 pt", 36);
    textSizeSelector->addItem("48 pt", 48);
    textSizeSelector->addItem("60 pt", 60);
    textSizeSelector->addItem("72 pt", 72);
    setTextSize(text_size.setting);
    toolbarHash["Text"]->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarHash["Text"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPromptToolbar()
{
    qDebug("MainWindow createPromptToolbar()");

    toolbarHash["Prompt"]->setObjectName("toolbarPrompt");
    toolbarHash["Prompt"]->addWidget(prompt);
    toolbarHash["Prompt"]->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarHash["Prompt"], SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));
}

void MainWindow::createAllToolbars()
{
    qDebug("MainWindow createAllToolbars()");

    add_to_toolbar("File", file_toolbar);
    add_to_toolbar("Edit", edit_toolbar);
    add_to_toolbar("Zoom", zoom_toolbar);
    add_to_toolbar("Pan", pan_toolbar);
    add_to_toolbar("View", view_toolbar);
    add_to_toolbar("Icon", icon_toolbar);
    add_to_toolbar("Help", help_toolbar);

    createLayerToolbar();
    createPropertiesToolbar();
    createTextToolbar();
    createPromptToolbar();

    add_to_toolbar("Draw", draw_toolbar);
    add_to_toolbar("Modify", modify_toolbar);

    /* Horizontal */
    toolbarHash["View"]->setOrientation(Qt::Horizontal);
    toolbarHash["Zoom"]->setOrientation(Qt::Horizontal);
    toolbarHash["Layer"]->setOrientation(Qt::Horizontal);
    toolbarHash["Properties"]->setOrientation(Qt::Horizontal);
    toolbarHash["Text"]->setOrientation(Qt::Horizontal);
    toolbarHash["Prompt"]->setOrientation(Qt::Horizontal);

    /* Top */
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarHash["File"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Edit"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Help"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Icon"]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Zoom"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Pan"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["View"]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Layer"]);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Properties"]);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarHash["Text"]);
    /* Bottom */
    addToolBar(Qt::BottomToolBarArea, toolbarHash["Prompt"]);

    /* Left */
    addToolBar(Qt::LeftToolBarArea, toolbarHash["Draw"]);
    addToolBar(Qt::LeftToolBarArea, toolbarHash["Modify"]);
    addToolBarBreak(Qt::LeftToolBarArea);

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}
