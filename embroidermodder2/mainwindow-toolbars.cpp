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

/**
 * \file mainwindow-toolbars.cpp
 */

#include "embroidermodder.h"

StringList
file_toolbar = {
    "new",
    "open",
    "save",
    "saveas",
    "print",
    "designdetails",
    "---",
    "undo",
    "redo",
    "---",
    "help"
};

StringList
edit_toolbar = {
    "cut",
    "copy",
    "paste"
};

StringList
view_toolbar = {
    "day",
    "night"
};

StringList
zoom_toolbar = {
    "zoomwindow",
    "zoomdynamic",
    "zoomscale",
    "---",
    "zoomcenter",
    "zoomin",
    "zoomout",
    "---",
    "zoomselected",
    "zoomall",
    "zoomextents"
};

StringList
pan_toolbar = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown"
};

StringList
icon_toolbar = {
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128"
};

StringList
help_toolbar = {
    "help",
    "---",
    "changelog",
    "---",
    "about",
    "---",
    "whatsthis"
};

/**
 * @brief get_action_index
 * @param cmd
 * @return
 */
int
get_action_index(std::string cmd)
{
    for (int i=0; i<(int)action_table.size(); i++) {
        if (cmd == action_table[i].icon) {
            return i;
        }
    }
    return 0;
}

/**
 * @brief MainWindow::create_toolbar
 * @param toolbar
 * @param label
 * @param entries
 */
void
MainWindow::create_toolbar(QToolBar* toolbar, std::string label, StringList entries)
{
    toolbar->setObjectName(QString::fromStdString(label));
    for (int i=0; i<(int)entries.size(); i++) {
        if (entries[i] == "---") {
            toolbar->addSeparator();
        }
        else {
            int index = get_action_index(entries[i]);
            toolbar->addAction(actionHash[index]);
        }
    }
    connect(toolbar, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

/**
 * @brief MainWindow::create_icon
 * @param stub
 * @return
 */
QIcon
MainWindow::create_icon(QString stub)
{
    QString appDir = qApp->applicationDirPath();
    QString icontheme = settings.general_icon_theme;
    return QIcon(appDir + "/icons/" + icontheme + "/" + stub + ".png");
}

/**
 * @brief MainWindow::createLayerToolbar
 */
void
MainWindow::createLayerToolbar()
{
    debug_message("MainWindow createLayerToolbar()");

    toolbarLayer->setObjectName("toolbarLayer");
    toolbarLayer->addAction(actionHash[get_action_index("makelayercurrent")]);
    toolbarLayer->addAction(actionHash[get_action_index("layers")]);

    layerSelector->setFocusProxy(prompt);
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
    toolbarLayer->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarLayer->addAction(actionHash[get_action_index("layerprevious")]);

    connect(toolbarLayer, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

/**
 * @brief MainWindow::createPropertiesToolbar
 */
void
MainWindow::createPropertiesToolbar()
{
    debug_message("MainWindow createPropertiesToolbar()");

    toolbarProperties->setObjectName("toolbarProperties");

    colorSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    colorSelector->addItem(create_icon("colorbylayer"), "ByLayer");
    colorSelector->addItem(create_icon("colorbyblock"), "ByBlock");
    colorSelector->addItem(create_icon("colorred"), tr("Red"),     qRgb(255,  0,  0));
    colorSelector->addItem(create_icon("coloryellow"), tr("Yellow"),  qRgb(255,255,  0));
    colorSelector->addItem(create_icon("colorgreen"), tr("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(create_icon("colorcyan"), tr("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(create_icon("colorblue"), tr("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(create_icon("colormagenta"), tr("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(create_icon("colorwhite"), tr("White"),   qRgb(255,255,255));
    colorSelector->addItem(create_icon("colorother"), tr("Other..."));
    toolbarProperties->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    linetypeSelector->addItem(create_icon("linetypebylayer"), "ByLayer");
    linetypeSelector->addItem(create_icon("linetypebyblock"), "ByBlock");
    linetypeSelector->addItem(create_icon("linetypecontinuous"), "Continuous");
    linetypeSelector->addItem(create_icon("linetypehidden"), "Hidden");
    linetypeSelector->addItem(create_icon("linetypecenter"), "Center");
    linetypeSelector->addItem(create_icon("linetypeother"), "Other...");
    toolbarProperties->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    lineweightSelector->addItem(create_icon( "lineweightbylayer"), "ByLayer", -2.00);
    lineweightSelector->addItem(create_icon( "lineweightbyblock"), "ByBlock", -1.00);
    lineweightSelector->addItem(create_icon( "lineweightdefault"), "Default",  0.00);
    //TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(create_icon( "lineweight01"), "0.00 mm", 0.00);
    lineweightSelector->addItem(create_icon( "lineweight02"), "0.05 mm", 0.05);
    lineweightSelector->addItem(create_icon( "lineweight03"), "0.15 mm", 0.15);
    lineweightSelector->addItem(create_icon( "lineweight04"), "0.20 mm", 0.20);
    lineweightSelector->addItem(create_icon( "lineweight05"), "0.25 mm", 0.25);
    lineweightSelector->addItem(create_icon( "lineweight06"), "0.30 mm", 0.30);
    lineweightSelector->addItem(create_icon( "lineweight07"), "0.35 mm", 0.35);
    lineweightSelector->addItem(create_icon( "lineweight08"), "0.40 mm", 0.40);
    lineweightSelector->addItem(create_icon( "lineweight09"), "0.45 mm", 0.45);
    lineweightSelector->addItem(create_icon( "lineweight10"), "0.50 mm", 0.50);
    lineweightSelector->addItem(create_icon( "lineweight11"), "0.55 mm", 0.55);
    lineweightSelector->addItem(create_icon( "lineweight12"), "0.60 mm", 0.60);
    lineweightSelector->addItem(create_icon( "lineweight13"), "0.65 mm", 0.65);
    lineweightSelector->addItem(create_icon( "lineweight14"), "0.70 mm", 0.70);
    lineweightSelector->addItem(create_icon( "lineweight15"), "0.75 mm", 0.75);
    lineweightSelector->addItem(create_icon( "lineweight16"), "0.80 mm", 0.80);
    lineweightSelector->addItem(create_icon( "lineweight17"), "0.85 mm", 0.85);
    lineweightSelector->addItem(create_icon( "lineweight18"), "0.90 mm", 0.90);
    lineweightSelector->addItem(create_icon( "lineweight19"), "0.95 mm", 0.95);
    lineweightSelector->addItem(create_icon("lineweight20"), "1.00 mm", 1.00);
    lineweightSelector->addItem(create_icon("lineweight21"), "1.05 mm", 1.05);
    lineweightSelector->addItem(create_icon("lineweight22"), "1.10 mm", 1.10);
    lineweightSelector->addItem(create_icon("lineweight23"), "1.15 mm", 1.15);
    lineweightSelector->addItem(create_icon("lineweight24"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarProperties->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarProperties, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

/**
 * @brief MainWindow::createTextToolbar
 */
void
MainWindow::createTextToolbar()
{
    debug_message("MainWindow createTextToolbar()");

    toolbarText->setObjectName("toolbarText");

    toolbarText->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(settings.text_font));
    connect(textFontSelector, SIGNAL(currentFontChanged(QFont)), this, SLOT(textFontSelectorCurrentFontChanged(QFont)));

    int textbold_index = get_action_index("textbold");
    toolbarText->addAction(actionHash[textbold_index]);
    actionHash[textbold_index]->setChecked(settings.text_style_bold);

    int textitalic_index = get_action_index("textitalic");
    toolbarText->addAction(actionHash[textitalic_index]);
    actionHash[get_action_index("textitalic")]->setChecked(settings.text_style_italic);

    int textunderline_index = get_action_index("textunderline");
    toolbarText->addAction(actionHash[textunderline_index]);
    actionHash[textunderline_index]->setChecked(settings.text_style_underline);

    int textstrikeout_index = get_action_index("textstrikeout");
    toolbarText->addAction(actionHash[textstrikeout_index]);
    actionHash[textstrikeout_index]->setChecked(settings.text_style_strikeout);

    int textoverline_index = get_action_index("textoverline");
    toolbarText->addAction(actionHash[textoverline_index]);
    actionHash[textoverline_index]->setChecked(settings.text_style_overline);

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
    setTextSize(settings.text_size);
    toolbarText->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarText, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

/**
 * @brief MainWindow::createPromptToolbar
 */
void
MainWindow::createPromptToolbar()
{
    debug_message("MainWindow createPromptToolbar()");

    toolbarPrompt->setObjectName("toolbarPrompt");
    toolbarPrompt->addWidget(prompt);
    toolbarPrompt->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarPrompt, SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));
}

/**
 * @brief MainWindow::createAllToolbars
 */
void
MainWindow::createAllToolbars()
{
    debug_message("MainWindow createAllToolbars()");

    create_toolbar(toolbarFile, "toolbarFile", file_toolbar);
    create_toolbar(toolbarEdit, "toolbarEdit", edit_toolbar);
    create_toolbar(toolbarView, "toolbarView", view_toolbar);
    create_toolbar(toolbarZoom, "toolbarZoom", zoom_toolbar);
    create_toolbar(toolbarPan, "toolbarPan", pan_toolbar);
    create_toolbar(toolbarIcon, "toolbarIcon", icon_toolbar);
    create_toolbar(toolbarHelp, "toolbarHelp", help_toolbar);

    createLayerToolbar();
    createPropertiesToolbar();
    createTextToolbar();
    createPromptToolbar();

    // Horizontal
    toolbarView->setOrientation(Qt::Horizontal);
    toolbarZoom->setOrientation(Qt::Horizontal);
    toolbarLayer->setOrientation(Qt::Horizontal);
    toolbarProperties->setOrientation(Qt::Horizontal);
    toolbarText->setOrientation(Qt::Horizontal);
    toolbarPrompt->setOrientation(Qt::Horizontal);

    // Top
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarFile);
    addToolBar(Qt::TopToolBarArea, toolbarEdit);
    addToolBar(Qt::TopToolBarArea, toolbarHelp);
    addToolBar(Qt::TopToolBarArea, toolbarIcon);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarZoom);
    addToolBar(Qt::TopToolBarArea, toolbarPan);
    addToolBar(Qt::TopToolBarArea, toolbarView);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarLayer);
    addToolBar(Qt::TopToolBarArea, toolbarProperties);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolbarText);

    // Bottom
    addToolBar(Qt::BottomToolBarArea, toolbarPrompt);

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}
