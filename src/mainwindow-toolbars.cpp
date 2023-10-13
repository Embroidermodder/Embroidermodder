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

/* Create toolbar. */
void
MainWindow::create_toolbar(std::string toolbar, std::string label, StringList entries)
{
    toolbarHash[toolbar]->setObjectName(QString::fromStdString(label));
    for (int i=0; i<(int)entries.size(); i++) {
        if (entries[i] == "---") {
            toolbarHash[toolbar]->addSeparator();
        }
        else {
            toolbarHash[toolbar]->addAction(actionHash[entries[i]]);
        }
    }
    connect(toolbarHash[toolbar],
        SIGNAL(topLevelChanged(bool)),
        this,
        SLOT(floatingChangedToolBar(bool)));
}

/* Create icon. */
QIcon
MainWindow::create_icon(QString stub)
{
    QString appDir = qApp->applicationDirPath();
    QString icontheme = get_qstr(settings, "general_icon_theme");
    return QIcon(appDir + "/icons/" + icontheme + "/" + stub + ".png");
}

/* Create all toolbars. */
void
MainWindow::createAllToolbars()
{
    debug_message("MainWindow createAllToolbars()");


    create_toolbar("file", "toolbarFile", config["file_toolbar"].sl);
    create_toolbar("edit", "toolbarEdit", config["edit_toolbar"].sl);
    create_toolbar("view", "toolbarView", config["view_toolbar"].sl);
    create_toolbar("zoom", "toolbarZoom", config["zoom_toolbar"].sl);
    create_toolbar("pan", "toolbarPan", config["pan_toolbar"].sl);
    create_toolbar("icon", "toolbarIcon", config["icon_toolbar"].sl);
    create_toolbar("help", "toolbarHelp", config["help_toolbar"].sl);
    create_toolbar("draw", "toolbarHelp", config["draw_toolbar"].sl);

    debug_message("MainWindow createLayerToolbar()");

    toolbarHash["layer"]->setObjectName("toolbarLayer");
    toolbarHash["layer"]->addAction(actionHash["makelayercurrent"]);
    toolbarHash["layer"]->addAction(actionHash["layers"]);

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
    toolbarHash["layer"]->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarHash["layer"]->addAction(actionHash["layerprevious"]);

    connect(toolbarHash["layer"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("MainWindow createPropertiesToolbar()");

    toolbarHash["properties"]->setObjectName("toolbarProperties");

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
    toolbarHash["properties"]->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarHash["properties"]->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    linetypeSelector->addItem(create_icon("linetypebylayer"), "ByLayer");
    linetypeSelector->addItem(create_icon("linetypebyblock"), "ByBlock");
    linetypeSelector->addItem(create_icon("linetypecontinuous"), "Continuous");
    linetypeSelector->addItem(create_icon("linetypehidden"), "Hidden");
    linetypeSelector->addItem(create_icon("linetypecenter"), "Center");
    linetypeSelector->addItem(create_icon("linetypeother"), "Other...");
    toolbarHash["properties"]->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarHash["properties"]->addSeparator();
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
    toolbarHash["properties"]->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarHash["properties"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("MainWindow createTextToolbar()");

    toolbarHash["text"]->setObjectName("toolbarText");

    toolbarHash["text"]->addWidget(textFontSelector);
    QString font_ = QString::fromStdString(get_str(settings, "text_font"));
    textFontSelector->setCurrentFont(QFont(font_));
    connect(textFontSelector, SIGNAL(currentFontChanged(QFont)), this, SLOT(textFontSelectorCurrentFontChanged(QFont)));

    toolbarHash["text"]->addAction(actionHash["textbold"]);
    actionHash["textbold"]->setChecked(get_bool(settings, "text_style_bold"));

    toolbarHash["text"]->addAction(actionHash["textitalic"]);
    actionHash["textitalic"]->setChecked(get_bool(settings, "text_style_italic"));

    toolbarHash["text"]->addAction(actionHash["textunderline"]);
    actionHash["textunderline"]->setChecked(get_bool(settings, "text_style_underline"));

    toolbarHash["text"]->addAction(actionHash["textstrikeout"]);
    actionHash["textstrikeout"]->setChecked(get_bool(settings, "text_style_strikeout"));

    toolbarHash["text"]->addAction(actionHash["textoverline"]);
    actionHash["textoverline"]->setChecked(get_bool(settings, "text_style_overline"));

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
    setTextSize(get_real(settings, "text_size"));
    toolbarHash["text"]->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarHash["text"], SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));

    debug_message("MainWindow createPromptToolbar()");

    toolbarHash["prompt"]->setObjectName("toolbarPrompt");
    toolbarHash["prompt"]->addWidget(prompt);
    toolbarHash["prompt"]->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarHash["prompt"], SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));

    // Horizontal
    toolbarHash["view"]->setOrientation(Qt::Horizontal);
    toolbarHash["zoom"]->setOrientation(Qt::Horizontal);
    toolbarHash["layer"]->setOrientation(Qt::Horizontal);
    toolbarHash["properties"]->setOrientation(Qt::Horizontal);
    toolbarHash["text"]->setOrientation(Qt::Horizontal);
    toolbarHash["prompt"]->setOrientation(Qt::Horizontal);
    toolbarHash["draw"]->setOrientation(Qt::Vertical);

    StringList top_toolbar = config["top_toolbar_layout"].sl;
    StringList bottom_toolbar = config["bottom_toolbar_layout"].sl;

    for (int i=0; i<(int)top_toolbar.size(); i++) {
        String entry = top_toolbar[i];
        if (entry == "---") {
            addToolBarBreak(Qt::TopToolBarArea);
        }
        else {
            addToolBar(Qt::TopToolBarArea, toolbarHash[entry]);
        }
    }

    addToolBar(Qt::TopToolBarArea, toolbarHash["draw"]);

    for (int i=0; i<(int)bottom_toolbar.size(); i++) {
        String entry = bottom_toolbar[i];
        if (entry == "---") {
            addToolBarBreak(Qt::BottomToolBarArea);
        }
        else {
            addToolBar(Qt::BottomToolBarArea, toolbarHash[entry]);
        }
    }

    //zoomToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
}
