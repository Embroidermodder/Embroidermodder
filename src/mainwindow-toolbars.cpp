#include "embroidermodder.h"

void MainWindow::createFileToolbar()
{
    qDebug("MainWindow createFileToolbar()");

    toolbarFile->setObjectName("toolbarFile");
    toolbarFile->addAction(actionHash.value("new"));
    toolbarFile->addAction(actionHash.value("open"));
    toolbarFile->addAction(actionHash.value("save"));
    toolbarFile->addAction(actionHash.value("saveas"));
    toolbarFile->addAction(actionHash.value("print"));
    toolbarFile->addAction(actionHash.value("designdetails"));

    toolbarFile->addSeparator();
    toolbarFile->addAction(actionHash.value("undo"));
    toolbarFile->addAction(actionHash.value("redo"));
    toolbarFile->addSeparator();
    toolbarFile->addAction(actionHash.value("help"));

    connect(toolbarFile, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createEditToolbar()
{
    qDebug("MainWindow createEditToolbar()");

    toolbarEdit->setObjectName("toolbarEdit");
    toolbarEdit->addAction(actionHash.value("cut"));
    toolbarEdit->addAction(actionHash.value("copy"));
    toolbarEdit->addAction(actionHash.value("paste"));

    connect(toolbarEdit, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createViewToolbar()
{
    qDebug("MainWindow createViewToolbar()");

    toolbarView->setObjectName("toolbarView");
    toolbarView->addAction(actionHash.value("day"));
    toolbarView->addAction(actionHash.value("night"));

    connect(toolbarView, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createZoomToolbar()
{
    qDebug("MainWindow createZoomToolbar()");

    toolbarZoom->setObjectName("toolbarZoom");
    toolbarZoom->addAction(actionHash.value("zoomwindow"));
    toolbarZoom->addAction(actionHash.value("zoomdynamic"));
    toolbarZoom->addAction(actionHash.value("zoomscale"));
    toolbarZoom->addSeparator();
    toolbarZoom->addAction(actionHash.value("zoomcenter"));
    toolbarZoom->addAction(actionHash.value("zoomin"));
    toolbarZoom->addAction(actionHash.value("zoomout"));
    toolbarZoom->addSeparator();
    toolbarZoom->addAction(actionHash.value("zoomselected"));
    toolbarZoom->addAction(actionHash.value("zoomall"));
    toolbarZoom->addAction(actionHash.value("zoomextents"));

    connect(toolbarZoom, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPanToolbar()
{
    qDebug("MainWindow createPanToolbar()");

    toolbarPan->setObjectName("toolbarPan");
    toolbarPan->addAction(actionHash.value("panrealtime"));
    toolbarPan->addAction(actionHash.value("panpoint"));
    toolbarPan->addSeparator();
    toolbarPan->addAction(actionHash.value("panleft"));
    toolbarPan->addAction(actionHash.value("panright"));
    toolbarPan->addAction(actionHash.value("panup"));
    toolbarPan->addAction(actionHash.value("pandown"));

    connect(toolbarPan, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createIconToolbar()
{
    qDebug("MainWindow createIconToolbar()");

    toolbarIcon->setObjectName("toolbarIcon");
    toolbarIcon->addAction(actionHash.value("icon16"));
    toolbarIcon->addAction(actionHash.value("icon24"));
    toolbarIcon->addAction(actionHash.value("icon32"));
    toolbarIcon->addAction(actionHash.value("icon48"));
    toolbarIcon->addAction(actionHash.value("icon64"));
    toolbarIcon->addAction(actionHash.value("icon128"));

    connect(toolbarIcon, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createHelpToolbar()
{
    qDebug("MainWindow createHelpToolbar()");

    toolbarHelp->setObjectName("toolbarHelp");
    toolbarHelp->addAction(actionHash.value("help"));
    toolbarHelp->addSeparator();
    toolbarHelp->addAction(actionHash.value("changelog"));
    toolbarHelp->addSeparator();
    toolbarHelp->addAction(actionHash.value("about"));
    toolbarHelp->addSeparator();
    toolbarHelp->addAction(actionHash.value("whatsthis"));

    connect(toolbarHelp, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createLayerToolbar()
{
    qDebug("MainWindow createLayerToolbar()");

    toolbarLayer->setObjectName("toolbarLayer");
    toolbarLayer->addAction(actionHash.value("makelayercurrent"));
    toolbarLayer->addAction(actionHash.value("layers"));

    QString appDir = qApp->applicationDirPath();
    QString icontheme = settings.general_icon_theme;

    layerSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    //TODO: Create layer pixmaps by concatenating several icons
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "0");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "1");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "2");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "3");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "4");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "5");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "6");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "7");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "8");
    layerSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer" + ".png"), "9");
    toolbarLayer->addWidget(layerSelector);
    connect(layerSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(layerSelectorIndexChanged(int)));

    toolbarLayer->addAction(actionHash.value("layerprevious"));

    connect(toolbarLayer, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPropertiesToolbar()
{
    qDebug("MainWindow createPropertiesToolbar()");

    toolbarProperties->setObjectName("toolbarProperties");

    QString appDir = qApp->applicationDirPath();
    QString icontheme = settings.general_icon_theme;

    colorSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorbylayer" + ".png"), "ByLayer");
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorbyblock" + ".png"), "ByBlock");
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorred"     + ".png"), tr("Red"),     qRgb(255,  0,  0));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "coloryellow"  + ".png"), tr("Yellow"),  qRgb(255,255,  0));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorgreen"   + ".png"), tr("Green"),   qRgb(  0,255,  0));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorcyan"    + ".png"), tr("Cyan"),    qRgb(  0,255,255));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorblue"    + ".png"), tr("Blue"),    qRgb(  0,  0,255));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colormagenta" + ".png"), tr("Magenta"), qRgb(255,  0,255));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorwhite"   + ".png"), tr("White"),   qRgb(255,255,255));
    colorSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "colorother"   + ".png"), tr("Other..."));
    toolbarProperties->addWidget(colorSelector);
    connect(colorSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(colorSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    linetypeSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebylayer"    + ".png"), "ByLayer");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypebyblock"    + ".png"), "ByBlock");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypecontinuous" + ".png"), "Continuous");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypehidden"     + ".png"), "Hidden");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypecenter"     + ".png"), "Center");
    linetypeSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "linetypeother"      + ".png"), "Other...");
    toolbarProperties->addWidget(linetypeSelector);
    connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(linetypeSelectorIndexChanged(int)));

    toolbarProperties->addSeparator();
    lineweightSelector->setFocusProxy(prompt);
    //NOTE: Qt4.7 wont load icons without an extension...
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweightbylayer" + ".png"), "ByLayer", -2.00);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweightbyblock" + ".png"), "ByBlock", -1.00);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweightdefault" + ".png"), "Default",  0.00);
    //TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight01" + ".png"), "0.00 mm", 0.00);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight02" + ".png"), "0.05 mm", 0.05);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight03" + ".png"), "0.15 mm", 0.15);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight04" + ".png"), "0.20 mm", 0.20);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight05" + ".png"), "0.25 mm", 0.25);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight06" + ".png"), "0.30 mm", 0.30);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight07" + ".png"), "0.35 mm", 0.35);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight08" + ".png"), "0.40 mm", 0.40);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight09" + ".png"), "0.45 mm", 0.45);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight10" + ".png"), "0.50 mm", 0.50);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight11" + ".png"), "0.55 mm", 0.55);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight12" + ".png"), "0.60 mm", 0.60);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight13" + ".png"), "0.65 mm", 0.65);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight14" + ".png"), "0.70 mm", 0.70);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight15" + ".png"), "0.75 mm", 0.75);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight16" + ".png"), "0.80 mm", 0.80);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight17" + ".png"), "0.85 mm", 0.85);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight18" + ".png"), "0.90 mm", 0.90);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight19" + ".png"), "0.95 mm", 0.95);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight20" + ".png"), "1.00 mm", 1.00);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight21" + ".png"), "1.05 mm", 1.05);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight22" + ".png"), "1.10 mm", 1.10);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight23" + ".png"), "1.15 mm", 1.15);
    lineweightSelector->addItem(QIcon(appDir + "/icons/" + icontheme + "/" + "lineweight24" + ".png"), "1.20 mm", 1.20);
    lineweightSelector->setMinimumContentsLength(8); // Prevent dropdown text readability being squish...d.
    toolbarProperties->addWidget(lineweightSelector);
    connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(lineweightSelectorIndexChanged(int)));

    connect(toolbarProperties, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createTextToolbar()
{
    qDebug("MainWindow createTextToolbar()");

    toolbarText->setObjectName("toolbarText");

    toolbarText->addWidget(textFontSelector);
    textFontSelector->setCurrentFont(QFont(settings.text_font));
    connect(textFontSelector, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(textFontSelectorCurrentFontChanged(const QFont&)));

    toolbarText->addAction(actionHash.value("textbold"));
    actionHash.value("textbold")->setChecked(settings.text_style_bold);
    toolbarText->addAction(actionHash.value("textitalic"));
    actionHash.value("textitalic")->setChecked(settings.text_style_italic);
    toolbarText->addAction(actionHash.value("textunderline"));
    actionHash.value("textunderline")->setChecked(settings.text_style_underline);
    toolbarText->addAction(actionHash.value("textstrikeout"));
    actionHash.value("textstrikeout")->setChecked(settings.text_style_strikeout);
    toolbarText->addAction(actionHash.value("textoverline"));
    actionHash.value("textoverline")->setChecked(settings.text_style_overline);

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
    char command[200];
    sprintf(command, "set_text_size(%f)", settings.text_size);
    run(command);
    toolbarText->addWidget(textSizeSelector);
    connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(textSizeSelectorIndexChanged(int)));

    connect(toolbarText, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChangedToolBar(bool)));
}

void MainWindow::createPromptToolbar()
{
    qDebug("MainWindow createPromptToolbar()");

    toolbarPrompt->setObjectName("toolbarPrompt");
    toolbarPrompt->addWidget(prompt);
    toolbarPrompt->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    connect(toolbarPrompt, SIGNAL(topLevelChanged(bool)), prompt, SLOT(floatingChanged(bool)));
}

void MainWindow::createAllToolbars()
{
    qDebug("MainWindow createAllToolbars()");

    createFileToolbar();
    createEditToolbar();
    createViewToolbar();
    createZoomToolbar();
    createPanToolbar();
    createIconToolbar();
    createHelpToolbar();
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

