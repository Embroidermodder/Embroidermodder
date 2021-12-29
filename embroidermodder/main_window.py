#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    The MainWindow class definition file.

    To make the undo history easier to manage we use a dict for
    keeping all the action information together.
"""

from sys import exit
import tkinter as tk

from embroidermodder.color import Color
from embroidermodder.geometry import Vector, Line, Polyline
from embroidermodder.details_dialog import DetailsDialog
from embroidermodder.settings_dialog import SettingsDialog
import embroidermodder.utility as utility
from embroidermodder.utility import settings, translate, debug_message, load_image, write_settings

class MainWindow():
    r"""
    This is the heart of the program, we're working on replacing
    the Qt reliance, so these functions and data represent the eventual core
    of the program.

    The widget system is created here, but it is built on top of the
    SVG system created in libembroidery. So a widget is an svg drawing,
    with a position to draw it in relative to its parent. The widgets
    form a tree rooted at the global variable called root.

    toolbar = ToolBar[10]
    menu = Menu[10]
    status_bar = toolButton = [
        tk.Button() for i in range(8)
    ]
    toolButton = [
        tk.Button() for i in range(PROPERTY_EDITORS)
    ]
    lineEdit = [
        tk.LineEdit() for i in range(LINEEDIT_PROPERTY_EDITORS)
    ]
    comboBox =  [
        tk.ComboBox() for i in range(COMBOBOX_PROPERTY_EDITORS)
    ]

    opensave_recent_list_of_files = []
    opensave_custom_filter = ""

    toolButtonTextSingleHeight = 
    toolButtonTextSingleRotation = tk.Button()

    text_single_editors = {
        "contents": {
            "entry": tk.LineEdit(),
            "toolbutton": tk.Button()
        },
        "font": [tk.FontComboBox(), tk.Button()],
        "justify": [tk.ComboBox(), tk.Button()],
        "height": [tk.LineEdit(), tk.Button()],
        "rotation": [tk.LineEdit(), tk.Button()]
    }

    ToolButton toolButtonGeneralLayer
    ToolButton toolButtonGeneralColor
    ToolButton toolButtonGeneralLineType
    ToolButton toolButtonGeneralLineWeight

    ComboBox comboBoxGeneralLayer
    ComboBox comboBoxGeneralColor
    ComboBox comboBoxGeneralLineType
    ComboBox comboBoxGeneralLineWeight

    ToolButton toolButtonImageX
    ToolButton toolButtonImageY
    ToolButton toolButtonImageWidth
    ToolButton toolButtonImageHeight

    LineEdit lineEditImageX
    LineEdit lineEditImageY
    LineEdit lineEditImageWidth
    LineEdit lineEditImageHeight

    GroupBox groupBoxMiscImage

    ToolButton toolButtonImageName
    ToolButton toolButtonImagePath

    LineEdit lineEditImageName
    LineEdit lineEditImagePath

    ToolButton toolButtonPolygonCenterX
    ToolButton toolButtonPolygonCenterY
    ToolButton toolButtonPolygonRadiusVertex
    ToolButton toolButtonPolygonRadiusSide
    ToolButton toolButtonPolygonDiameterVertex
    ToolButton toolButtonPolygonDiameterSide
    ToolButton toolButtonPolygonInteriorAngle

    LineEdit lineEditPolygonCenterX
    LineEdit lineEditPolygonCenterY
    LineEdit lineEditPolygonRadiusVertex
    LineEdit lineEditPolygonRadiusSide
    LineEdit lineEditPolygonDiameterVertex
    LineEdit lineEditPolygonDiameterSide
    LineEdit lineEditPolygonInteriorAngle

    EmbVector pasteDelta
    Vector scenePressPoint
    Point pressPoint
    Vector sceneMovePoint
    Point movePoint
    Vector sceneReleasePoint
    Point releasePoint
    Vector sceneGripPoint

    Color rulerColor

    Point  viewMousePoint
    EmbVector sceneMousePoint
    unsigned int snapLocatorColor
    unsigned int gripColorCool
    unsigned int gripColorHot
    unsigned int crosshairColor
    int precisionAngle
    int precisionLength

    Label statusBarMouseCoord

    ToolButton toolButtonInfiniteLineX1
    ToolButton toolButtonInfiniteLineY1
    ToolButton toolButtonInfiniteLineX2
    ToolButton toolButtonInfiniteLineY2
    ToolButton toolButtonInfiniteLineVectorX
    ToolButton toolButtonInfiniteLineVectorY

    LineEdit lineEditInfiniteLineX1
    LineEdit lineEditInfiniteLineY1
    LineEdit lineEditInfiniteLineX2
    LineEdit lineEditInfiniteLineY2
    LineEdit lineEditInfiniteLineVectorX
    LineEdit lineEditInfiniteLineVectorY

    #Used when checking if fields vary
    fieldOldText = ""
    fieldNewText = ""
    fieldVariesText = ""
    fieldYesText = ""
    fieldNoText = ""
    fieldOnText = ""
    fieldOffText = ""

    ToolButton toolButtonArcClockwise
    ComboBox comboBoxArcClockwise

    GroupBox groupBoxGeometry[32]
    GroupBox groupBoxGeneral
    GroupBox groupBoxMiscArc
    GroupBox groupBoxMiscPath
    GroupBox groupBoxMiscPolyline
    GroupBox groupBoxTextTextSingle
    GroupBox groupBoxMiscTextSingle

    ToolButton toolButtonBlockX
    ToolButton toolButtonBlockY

    LineEdit lineEditBlockX
    LineEdit lineEditBlockY

    ToolButton toolButtonPathVertexNum
    ToolButton toolButtonPathVertexX
    ToolButton toolButtonPathVertexY
    ToolButton toolButtonPathArea
    ToolButton toolButtonPathLength

    ComboBox comboBoxPathVertexNum
    LineEdit lineEditPathVertexX
    LineEdit lineEditPathVertexY
    LineEdit lineEditPathArea
    LineEdit lineEditPathLength

    ToolButton toolButtonPathClosed

    ComboBox comboBoxPathClosed

    ToolButton toolButtonPolylineVertexNum
    ToolButton toolButtonPolylineVertexX
    ToolButton toolButtonPolylineVertexY
    ToolButton toolButtonPolylineArea
    ToolButton toolButtonPolylineLength

    ComboBox comboBoxPolylineVertexNum
    LineEdit lineEditPolylineVertexX
    LineEdit lineEditPolylineVertexY
    LineEdit lineEditPolylineArea
    LineEdit lineEditPolylineLength

    ToolButton toolButtonPolylineClosed

    ComboBox comboBoxPolylineClosed

    ToolButton toolButtonRayX1
    ToolButton toolButtonRayY1
    ToolButton toolButtonRayX2
    ToolButton toolButtonRayY2
    ToolButton toolButtonRayVectorX
    ToolButton toolButtonRayVectorY

    LineEdit lineEditRayX1
    LineEdit lineEditRayY1
    LineEdit lineEditRayX2
    LineEdit lineEditRayY2
    LineEdit lineEditRayVectorX
    LineEdit lineEditRayVectorY

    ToolButton toolButtonTextMultiX
    ToolButton toolButtonTextMultiY

    lineEditTextMultiX = LineEdit()
    lineEditTextMultiY = LineEdit()

    ToolButton toolButtonTextSingleX
    ToolButton toolButtonTextSingleY

    LineEdit lineEditTextSingleX
    LineEdit lineEditTextSingleY

    ToolButton toolButtonTextSingleBackward
    ToolButton toolButtonTextSingleUpsideDown

    ComboBox comboBoxTextSingleBackward
    ComboBox comboBoxTextSingleUpsideDown

    TODO: Set What's self Context Help to statusTip for now so there
    is some infos there.

    Make custom What's self Context Help popup with more descriptive help
    than just the status bar/tip one liner(short but not real long) with a
    hyperlink in the custom popup at the bottom to open full help file
    description. Ex: like wxPython AGW's SuperToolTip.
    """
    def __init__(self):
        " . "
        # To stop the garbage collector stealing our icons.
        settings["tkimg"] = {}

        self.root = tk.Tk()
        self.root.title(settings["title"])
        self.root.minsize(settings["width"], settings["height"])
        self.build_menubar()
        self.build_button_grid()
        self.undo_history = []
        self.undo_history_position = 0
        self.opensave_recent_list_of_files = []
        self.opensave_custom_filter = ""
        current_path = ""

        self.num_docs = 0
        self.doc_index = 0

        lang = settings["general_language"]
        debug_message("language: %s" % lang)

        debug_message("Overriding to test boot")
        self.root.mainloop()
        return

        #for i in range(nFolders):
        #    current_path = self.application_folder + folders[i]
        #    if not exists(current_path):
        #        critical(self, translate("Path Error"), translate("Cannot locate: ") + current_path)

        if lang == "system":
            lang = QLocale_system().languageToString(QLocale_system().language()).toLower()

        #Load translations provided by Qt - self covers dialog buttons and other common things.
        translatorQt = ""
        translatorQt.load("qt_" + QLocale_system().name(), QLibraryInfo_location(QLibraryInfo_TranslationsPath))
        # TODO: ensure self always loads, ship a copy of self with the app
        qApp.installTranslator(translatorQt)

        #Selectors
        self.layerSelector = ComboBox()
        self.colorSelector = ComboBox()
        self.linetypeSelector = ComboBox()
        self.lineweightSelector = ComboBox()
        self.textFontSelector = FontComboBox()
        self.textSizeSelector = ComboBox()

        settings["shiftKeyPressedState"] = 0

        setWindowIcon(app.png)
        # Require Minimum WVGA
        setMinimumSize(800, 480)

        loadFormats()

        # create the mdiArea
        vbox = Frame(self)
        layout = VBoxLayout(vbox)
        layout.setContentsMargins(Margins())
        vbox.setFrameStyle(Frame_StyledPanel | Frame_Sunken)
        mdiArea = MdiArea(self, vbox)
        mdiArea.useBackgroundLogo(settings["general_mdi_bg_use_logo"])
        mdiArea.useBackgroundTexture(settings["general_mdi_bg_use_texture"])
        mdiArea.useBackgroundColor(settings["general_mdi_bg_use_color"])
        mdiArea.setBackgroundLogo(settings["general_mdi_bg_logo"])
        mdiArea.setBackgroundTexture(settings["general_mdi_bg_texture"])
        mdiArea.setBackgroundColor(Color(settings["general_mdi_bg_color"]))
        mdiArea.setViewMode("TabbedView")
        mdiArea.setHorizontalScrollBarPolicy("ScrollBarAsNeeded")
        mdiArea.setVerticalScrollBarPolicy("ScrollBarAsNeeded")
        mdiArea.setActivationOrder("ActivationHistoryOrder")
        layout.addWidget(mdiArea)
        setCentralWidget(vbox)

        #setDockOptions(QAnimatedDocks | QAllowTabbedDocks | QVerticalTabs);
        # TODO: Load these from settings
        # tabifyDockWidget(dockPropEdit, dockUndoEdit);
        # TODO: load self from settings

        self.statusbar = StatusBar(self.root)
        self.setStatusBar(statusbar)

        for i in action_list.keys(self):
            Icon = action_list[i].icon
            ACTION = Action(Icon, action_list[i].menu_name, self)

            if len(action_list[i].shortcut)>0:
                ACTION.setShortcut(QKeySequence(action_list[i].shortcut))

            ACTION.setStatusTip(action_list[i].description)
            ACTION.setObjectName(action_list[i].abbreviation)
            ACTION.setWhatsself(action_list[i].description)
            #connect(ACTION, SIGNAL(triggered()), self, SLOT(actions()))
            actionHash.insert(i, ACTION)

        actionHash.value(ACTION_windowclose).setEnabled(numOfDocs > 0)
        actionHash.value(ACTION_designdetails).setEnabled(numOfDocs > 0)

        # menu["FILE"].addMenu(menu[RECENT_MENU])
        # Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
        # menu[RECENT_MENU].setTearOffEnabled(0)

        debug_message("MainWindow createWindowMenu()")
        menuBar().addMenu(menu["WINDOW"])
        #connect(menu["WINDOW"], SIGNAL(aboutToShow()), self, SLOT(windowMenuAboutToShow()))
        #Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
        menu["WINDOW"].setTearOffEnabled(0)

        for i in range(N_TOOLBARS):
            message = "MainWindow creating %s\n" % toolbar_label[i]
            debug_message(message)

            toolbar[i].setObjectName(toolbar_label[i])

            for j in toolbars[i]:
                if toolbars[i][j] >= 0:
                    toolbar[i].addAction(actionHash.value(toolbars[i][j]))
                else:
                    toolbar[i].addSeparator()

            #connect(toolbar[i], SIGNAL(topLevelChanged(int)), self, SLOT(floatingChangedToolBar(int)))

        debug_message("MainWindow createLayerToolbar()")

        toolbar["LAYER"].setObjectName("toolbarLayer")
        toolbar["LAYER"].addAction(actionHash.value(ACTION_makelayercurrent))
        toolbar["LAYER"].addAction(actionHash.value(ACTION_layers))

        #TODO: Create layer pixmaps by concatenating several icons
        layerSelector.addItem("linetypebylayer.png", "0")
        layerSelector.addItem("linetypebylayer.png", "1")
        layerSelector.addItem("linetypebylayer.png", "2")
        layerSelector.addItem("linetypebylayer.png", "3")
        layerSelector.addItem("linetypebylayer.png", "4")
        layerSelector.addItem("linetypebylayer.png", "5")
        layerSelector.addItem("linetypebylayer.png", "6")
        layerSelector.addItem("linetypebylayer.png", "7")
        layerSelector.addItem("linetypebylayer.png", "8")
        layerSelector.addItem("linetypebylayer.png", "9")
        toolbar["LAYER"].addWidget(layerSelector)
        #connect(layerSelector, SIGNAL(currentIndexChanged(int)), self, SLOT(layerSelectorIndexChanged(int)))

        toolbar["LAYER"].addAction(actionHash.value(ACTION_layerprevious))

        #connect(toolbar["LAYER"], SIGNAL(topLevelChanged(int)), self, SLOT(floatingChangedToolBar(int)))

        debug_message("MainWindow createPropertiesToolbar()")

        toolbar["PROPERTIES"].setObjectName("toolbarProperties")

        colorSelector.setFocusProxy(menu["FILE"])
        #NOTE: Qt4.7 wont load icons without an extension...
        colorSelector.addItem("colorbylayer.png", "ByLayer")
        colorSelector.addItem("colorbyblock.png", "ByBlock")
        colorSelector.addItem("colorred.png", translate("Red"), (255, 0, 0))
        colorSelector.addItem("coloryellow.png", translate("Yellow"), (255,255, 0))
        colorSelector.addItem("colorgreen.png", translate("Green"), (0, 255, 0))
        colorSelector.addItem("colorcyan.png", translate("Cyan"), (0,255,255))
        colorSelector.addItem("colorblue.png", translate("Blue"), (0, 0,255))
        colorSelector.addItem("colormagenta.png", translate("Magenta"), (255, 0,255))
        colorSelector.addItem("colorwhite.png", translate("White"), (255,255,255))
        colorSelector.addItem("colorother.png", translate("Other..."))
        toolbar["PROPERTIES"].addWidget(colorSelector)
        #connect(colorSelector, SIGNAL(currentIndexChanged(int)), self, SLOT(colorSelectorIndexChanged(int)))

        toolbar["PROPERTIES"].addSeparator()
        linetypeSelector.setFocusProxy(menu["FILE"])
        linetypeSelector.addItem("linetypebylayer.png", "ByLayer")
        linetypeSelector.addItem("linetypebyblock.png", "ByBlock")
        linetypeSelector.addItem("linetypecontinuous.png", "Continuous")
        linetypeSelector.addItem("linetypehidden.png", "Hidden")
        linetypeSelector.addItem("linetypecenter.png", "Center")
        linetypeSelector.addItem("linetypeother.png", "Other...")
        toolbar["PROPERTIES"].addWidget(linetypeSelector)
        #connect(linetypeSelector, SIGNAL(currentIndexChanged(int)), self, SLOT(linetypeSelectorIndexChanged(int)))

        toolbar["PROPERTIES"].addSeparator()
        lineweightSelector.setFocusProxy(menu["FILE"])
        #NOTE: Qt4.7 wont load icons without an extension...
        # TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
        for line in settings["thread_weights"]:
            lineweightSelector.addItem(line[0], line[1], line[2])
        lineweightSelector.setMinimumContentsLength(8)
        # Prevent dropdown text readability being squish...d.
        toolbar["PROPERTIES"].addWidget(lineweightSelector)
        #connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), self, SLOT(lineweightSelectorIndexChanged(int)))

        #connect(toolbar["PROPERTIES"], SIGNAL(topLevelChanged()), self, SLOT(floatingChangedToolBar()))

        debug_message("MainWindow createTextToolbar()")
        toolbar["TEXT"].setObjectName("toolbarText")
        toolbar["TEXT"].addWidget(textFontSelector)
        textFontSelector.setCurrentFont(Font(settings["text_font"]))
        #connect(textFontSelector, SIGNAL(currentFontChanged()), self, SLOT(textFontSelectorCurrentFontChanged()))

        # TODO: SEGFAULTING FOR SOME REASON
        toolbar["TEXT"].addAction(actionHash.value(ACTION_textbold))
        actionHash.value(ACTION_textbold).setChecked(settings["text_style_bold"])
        toolbar["TEXT"].addAction(actionHash.value(ACTION_textitalic))
        actionHash.value(ACTION_textitalic).setChecked(settings["text_style_italic"])
        toolbar["TEXT"].addAction(actionHash.value(ACTION_textunderline))
        actionHash.value(ACTION_textunderline).setChecked(settings["text_style_underline"])
        toolbar["TEXT"].addAction(actionHash.value(ACTION_textstrikeout))
        actionHash.value(ACTION_textstrikeout).setChecked(settings["text_style_strikeout"])
        toolbar["TEXT"].addAction(actionHash.value(ACTION_textoverline))
        actionHash.value(ACTION_textoverline).setChecked(settings["text_style_overline"])

        textSizeSelector.setFocusProxy(menu["FILE"])
        sizes = [6, 8, 9, 10, 11, 12, 14, 18, 24, 30, 36, 48, 60, 72]
        for size in sizes:
            textSizeSelector.addItem(str(size)+" pt", size)
        setTextSize(settings.text_size)
        toolbar["TEXT"].addWidget(textSizeSelector)
        #connect(textSizeSelector, SIGNAL(currentIndexChanged(int)), self, SLOT(textSizeSelectorIndexChanged(int)))

        #connect(toolbar["TEXT"], SIGNAL(topLevelChanged(int)), self, SLOT(floatingChangedToolBar(int)))

        # Horizontal
        toolbar["VIEW"].setOrientation(Qt_Horizontal)
        toolbar["ZOOM"].setOrientation(Qt_Horizontal)
        toolbar["LAYER"].setOrientation(Qt_Horizontal)
        toolbar["PROPERTIES"].setOrientation(Qt_Horizontal)
        toolbar["TEXT"].setOrientation(Qt_Horizontal)
        # Top
        addToolBarBreak(Qt_TopToolBarArea)
        addToolBar(Qt_TopToolBarArea, toolbar["FILE"])
        addToolBar(Qt_TopToolBarArea, toolbar["EDIT"])
        addToolBar(Qt_TopToolBarArea, toolbar["HELP"])
        addToolBar(Qt_TopToolBarArea, toolbar["ICON"])
        addToolBarBreak(Qt_TopToolBarArea)
        addToolBar(Qt_TopToolBarArea, toolbar["ZOOM"])
        addToolBar(Qt_TopToolBarArea, toolbar["PAN"])
        addToolBar(Qt_TopToolBarArea, toolbar["VIEW"])
        addToolBarBreak(Qt_TopToolBarArea)
        addToolBar(Qt_TopToolBarArea, toolbar["LAYER"])
        addToolBar(Qt_TopToolBarArea, toolbar["PROPERTIES"])
        addToolBarBreak(Qt_TopToolBarArea)
        addToolBar(Qt_TopToolBarArea, toolbar["TEXT"])

        #zoomToolBar.setToolButtonStyle(Qt_ToolButtonTextOnly);

        iconResize(settings.general_icon_size)
        updateMenuToolbarStatusbar()

        # Show date in statusbar after it has been updated
        # TODO: Switch to ISO dates.

        date = QDate_currentDate()
        datestr = date.toString("MMMM d, yyyy")
        statusbar.showMessage(datestr)

        showNormal()

        if settings["general_tip_of_the_day"]:
            tipOfTheDay()

    def readSettings(self):
        debug_message("Reading settings...")

        # self file needs to be read from the users home directory to ensure it is writable.
        pos = Vector(settings.window_x, settings.window_y)
        size = (settings.window_width, settings.window_height)

        #
        layoutState = settings_file.value("LayoutState").toByteArray()
        if not restoreState(layoutState):
            debug_message("LayoutState NOT restored! Setting Default Layout...")
            # someToolBar.setVisible(1)

        settings = load_data("config.json")

        # Sanitise data here
        settings.window_x = clamp(0, settings.window_x, 1000)
        settings.window_y = clamp(0, settings.window_y, 1000)

        move(pos)
        resize(size)

    def write_settings(self):
        " . "
        debug_message("Writing settings...")

        settings["window_x"] = _mainWin.pos().x()
        settings["window_y"] = _mainWin.pos().y()
        settings["window_width"] = _mainWin.size().width()
        settings["window_height"] = _mainWin.size().height()

        write_settings()

    def settings_dialog(self, showTab):
        " . "
        dialog_ = SettingsDialog(showTab)
        dialog_.exec()

    def draw_icon(self):
        r"""
        Would work on lists like self:

        "about": [
            "arc 0 0 128 128 1 -1 black 3",
            "arc 0 0 128 128 -2 2 black 3",
            "arc 20 20 108 108 40 -40 black 3"
        ]
        out = Image.new("RGB", (128, 128), (255, 255, 255))
        draw = ImageDraw.Draw(out)
        for line in code:
            cmd = line.split(" ")
            if cmd[0] == "arc":
                box = (int(cmd[1]), int(cmd[2]), int(cmd[3]), int(cmd[4]))
                start = int(cmd[5])
                end = int(cmd[6])
                draw.arc(box, start, end, fill=cmd[7], width=int(cmd[8]))
        return out
        """
        return "self function is overridden."

    def actuator(self, action):
        r"""
        In order to have a complex version of saving work, with backups,
        undo history and forks we need a good recording of what has happened.

        An action has been taken, we are at the current head of the stack.

        This wrapper connects the signal to the C-style actuator.

        Arguments are decided by ...?
        """
        if len(self.undo_history) < self.undo_history_position:
            self.undo_history[self.undo_history_position] = action
        else:
            self.undo_history += [action]

        self.undo_history_position += 1

        if action == "do_nothing":
            debug_message("Do Nothing")
        elif action == "new_file":
            self.new_file()
        elif action == "open_file":
            self.open_file()
        elif action == "save_file":
            self.save_file()
        elif action == "save_as_file":
            self.save_as_file()
        elif action == "stub_testing":
            self.stub_testing()
        elif action == "check_for_updates":
            self.check_for_updates()
        elif action == "select_all":
            self.select_all()
        elif action == "whats_self_context_help":
            self.whats_self_context_help()
        elif action == "about":
            utility.about()
        elif action == "design_details":
            self.design_details()
        """
        "print",
        "exit",
        "cut",
        "copy",
        "paste",
        "undo",
        "redo",
        "window_close",
        "window_close_all",
        "windowCascade",
        "windowtile",
        "windownext",
        "windowprevious",
        "help",
        "changelog",
        "tipoftheday",
        "about",
        "whatsthis",
        "icon16",
        "icon24",
        "icon32",
        "icon48",
        "icon64",
        "icon128",
        "settingsdialog",
        "makelayercurrent",
        "layers",
        "layerselector",
        "layerprevious",
        "colorselector",
        "linetypeselector",
        "lineweightselector",
        "hidealllayers",
        "showalllayers",
        "freezealllayers",
        "thawalllayers",
        "lockalllayers",
        "unlockalllayers",
        "textbold",
        "textitalic",
        "textunderline",
        "textstrikeout",
        "textoverline",
        "zoomrealtime",
        "zoomprevious",
        "zoomwindow",
        "zoomdynamic",
        "zoomscale",
        "zoomcenter",
        "zoomin",
        "zoomout",
        "zoomselected",
        "zoomall",
        "zoomextents",
        "panrealtime",
        "panpoint",
        "panleft",
        "panright",
        "panup",
        "pandown",
        "day",
        "night",
        "trebleclef",
        "path",
        "circle",
        "line",
        "distance",
        "dolphin",
        "ellipse",
        "delete",
        "heart",
        "locatepoint"
        """

    def stub_testing(self):
        warning(self, translate("Testing Feature"), translate("<b>self feature is in testing.</b>"))

    def check_for_updates(self):
        debug_message("checkForUpdates()")
        #TODO: Check website for versions, commands, etc...

    def select_all(self):
        debug_message("selectAll()")
        gview = activeView()
        if gview:
            gview.selectAll()

    def design_details(self):
        scene = activeScene()
        if scene:
            dialog = Details_Dialog(scene, self)
            dialog.exec()

    def whats_self_context_help(self):
        debug_message("whatsselfContextHelp()")
        QWhatsself_enterWhatsselfMode()

    def print(self):
        debug_message("print()")
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            mdiWin.print();

    def tipOfTheDay(self):
        debug_message("tipOfTheDay()")

        appDir = qApp.applicationDirPath()
        wizardTipOfTheDay = QDialog()
        button1 = QToolButton(wizardTipOfTheDay)
        button2 = QToolButton(wizardTipOfTheDay)
        button3 = QToolButton(wizardTipOfTheDay)

        imgBanner = ImageWidget(appDir + "/images/did-you-know.png", wizardTipOfTheDay)

        checkBoxTipOfTheDay = QCheckBox(translate("&Show tips on startup"), wizardTipOfTheDay)
        checkBoxTipOfTheDay.setChecked(settings.general_tip_of_the_day)
        #connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), self, SLOT(checkBoxTipOfTheDayStateChanged(int)))

        if tips[settings["general_current_tip"]] == "":
            settings["general_current_tip"] = 0
        labelTipOfTheDay = QLabel(tips[settings["general_current_tip"]], wizardTipOfTheDay)
        labelTipOfTheDay.setWordWrap(1)

        button1.setText("&Previous")
        button2.setText("&Next")
        button3.setText("&Close")
        #connect(button1, SIGNAL(triggered()), wizardTipOfTheDay, SLOT(wizardTipOfTheDay.close()))
        #connect(button2, SIGNAL(triggered()), wizardTipOfTheDay, SLOT(wizardTipOfTheDay.close()))
        #connect(button3, SIGNAL(triggered()), wizardTipOfTheDay, SLOT(wizardTipOfTheDay.close()))

        layout = QVBoxLayout(wizardTipOfTheDay)
        layout.addWidget(imgBanner)
        layout.addStrut(1)
        layout.addWidget(labelTipOfTheDay)
        layout.addStretch(1)
        layout.addWidget(checkBoxTipOfTheDay)
        layout.addStrut(1)
        layout.addWidget(button1)
        layout.addStrut(1)
        layout.addWidget(button2)
        layout.addStrut(1)
        layout.addWidget(button3)

        wizardTipOfTheDay.setLayout(layout)
        wizardTipOfTheDay.setWindowTitle("Tip of the Day")
        wizardTipOfTheDay.setMinimumSize(550, 400)
        wizardTipOfTheDay.exec()

    def buttonTipOfTheDayClicked(self, button):
        " . "
        debug_message("buttonTipOfTheDayClicked(%d)", button)
        if button == QWizard_CustomButton1:
            if settings["general_current_tip"] > 0:
                settings["general_current_tip"] -= 1
            else:
                settings.general_current_tip = listTipOfTheDay.size()-1
            labelTipOfTheDay.setText(listTipOfTheDay.value(settings.general_current_tip))

        elif button == QWizard_CustomButton2:
            settings.general_current_tip += 1
            if settings.general_current_tip >= listTipOfTheDay.size(self):
                settings.general_current_tip = 0
            labelTipOfTheDay.setText(listTipOfTheDay.value(settings.general_current_tip))

        elif button == QWizard_CustomButton3:
            wizardTipOfTheDay.close()

    def help(self):
        debug_message("help()")

        # display in a custom widget instead
        # Open the HTML Help in the default browser
        helpURL = QUrl ("file:///" + qApp.applicationDirPath() + "/help/doc-index.html")
        QDesktopServices_openUrl(helpURL)

        #TODO: self is how to start an external program. Use self elsewhere...
        #program = "firefox";
        #QStringList arguments;
        #arguments << "help/commands.html";
        #QProcess *myProcess = QProcess(self);
        #myProcess.start(program, arguments);

    def undo(self):
        debug_message("undo()")
        if undo_history_position > 0:
            last = undo_history[undo_history_position]
            undo_history_position -= 1
            print("undo_history_position = %d\n", undo_history_position)
            print("undo_history_length = %d\n", undo_history_length)

            # Create the reverse action from the last action and apply with
            # the main actuator.
            if last[0] == "donothing":
                debug_message("The last action has no undo candidate.")

            self.actuator(last)

    def redo(self):
        undo_call = ""
        debug_message("redo()")
        if undo_history_position < undo_history_length:
            undo_history_position += 1
            print("undo_history_position = %d\n", undo_history_position)
            print("undo_history_length = %d\n", undo_history_length)
            undo_call = undo_history[undo_history_position]
            # set reverse flag
            undo_call += " -r"
            actuator(undo_call)

    def is_shift_pressed(self):
        return settings["shiftKeyPressedState"]

    def set_shift_pressed(self):
        settings["shiftKeyPressedState"] = True

    def set_shift_released(self):
        settings["shiftKeyPressedState"] = False

    def icon_resize(self, iconSize):
        " . "
        self.setIconSize(QSize(iconSize, iconSize))
        layerSelector.setIconSize(QSize(iconSize*4, iconSize))
        colorSelector.setIconSize(QSize(iconSize, iconSize))
        linetypeSelector.setIconSize(QSize(iconSize*4, iconSize))
        lineweightSelector.setIconSize(QSize(iconSize*4, iconSize))
        #set the minimum combobox width so the text is always readable
        layerSelector.setMinimumWidth(iconSize*4)
        colorSelector.setMinimumWidth(iconSize*2)
        linetypeSelector.  setMinimumWidth(iconSize*4)
        lineweightSelector.setMinimumWidth(iconSize*4)

        # TODO: low-priority:
        # open app with iconSize set to 128. resize the icons to a smaller size.

        settings.general_icon_size = iconSize

    def activeMdiWindow(self):
        debug_message("activeMdiWindow()")
        mdiWin = mdiArea.activeSubWindow()
        return mdiWin

    def activeView(self):
        debug_message("activeView()")
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            v = mdiWin.getView()
            return v
        return 0

    def activeScene(self):
        debug_message("activeScene()")
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            return mdiWin.getScene()
        return 0

    def updateAllViewScrollBars(self, val):
        windowList = mdiArea.subWindowList()
        for mdiWin in windowList:
            mdiWin.showViewScrollBars(val)

    def updateAllViewCrossHairColors(self, color):
        windowList = mdiArea.subWindowList()
        for mdiWin in windowList:
            mdiWin.setViewCrossHairColor(color)

    def updateAllViewBackgroundColors(self, color):
        windowList = mdiArea.subWindowList()
        for mdiWin in windowList:
            mdiWin.setViewBackgroundColor(color)

    def updateAllViewSelectBoxColors(self, colorL, fillL, colorR, fillR, alpha):
        windowList = mdiArea.subWindowList()
        for mdiWin in windowList:
            mdiWin.setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha)

    def updateAllViewGridColors(self, color):
        windowList = mdiArea.subWindowList()
        for mdiWin in windowList:
            mdiWin.setViewGridColor(color)

    def update_all_view_ruler_colors(self, color):
        windowList = mdiArea.subWindowList()
        for mdiWin in windowList:
            mdiWin.setViewRulerColor(color)

    def update_pick_add_mode(self, val):
        settings.selection_mode_pickadd = val
        dockPropEdit.updatePickAddModeButton(val)

    def pick_add_mode_toggled(self):
        val = not settings.selection_mode_pickadd
        updatePickAddMode(val)

    def layer_selector_index_changed(self, index):
        debug_message("layerSelectorIndexChanged(%d)", index)

    def color_selector_index_changed(self, index):
        " Color selector index changed. "
        debug_message("colorSelectorIndexChanged(%d)", index)

        comboBox = sender()
        newColor = Color()
        if comboBox:
            ok = False
            #TODO: Handle ByLayer and ByBlock and Other...
            newColor, ok = comboBox.itemData(index).toUInt()
            if not ok:
                warning(self, translate("Color Selector Conversion Error"), translate("<b>An error has occurred while changing colors.</b>"))

        else:
            warning(self, translate("Color Selector Pointer Error"), translate("<b>An error has occurred while changing colors.</b>"))

        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            mdiWin.currentColorChanged(newColor)

    def linetypeSelectorIndexChanged(self, index):
        debug_message("linetypeSelectorIndexChanged(%d)", index)

    def lineweightSelectorIndexChanged(self, index):
        debug_message("lineweightSelectorIndexChanged(%d)", index)

    def textFontSelectorCurrentFontChanged(self, font):
        " text font selector current font changed. "
        debug_message("textFontSelectorCurrentFontChanged()")
        textFontSelector.setCurrentFont(Font(font.family()))
        settings["text_font"] = font.family().toLocal8Bit().constData()

    def textSizeSelectorIndexChanged(self, index):
        debug_message("textSizeSelectorIndexChanged(%d)", index)
        settings.text_style.size = abs(textSizeSelector.itemData(index).toReal())
        #TODO: check that the toReal() conversion is ok

    def textFont(self):
        return settings["text_font"]

    def setTextSize(self, num):
        settings["text_style"].size = abs(num)
        index = textSizeSelector.findText("Custom", Qt_MatchContains)
        if index != -1:
            textSizeSelector.removeItem(index)
        textSizeSelector.addItem("Custom " + "".setNum(num, 'f', 2) + " pt", num)
        index = textSizeSelector.findText("Custom", Qt_MatchContains)
        if index != -1:
            textSizeSelector.setCurrentIndex(index)

    def getCurrentLayer(self):
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            return mdiWin.getCurrentLayer()
        return "0"

    def getCurrentColor(self):
        " TODO: return color ByLayer "
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            return mdiWin.getCurrentColor()
        return 0

    def getCurrentLineType(self):
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            return mdiWin.getCurrentLineType()
        return "ByLayer"

    def getCurrentLineWeight(self):
        " . "
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin: return mdiWin.getCurrentLineWeight()
        return "ByLayer"

    def deletePressed(self):
        " . "
        debug_message("deletePressed()")
        setOverrideCursor = QApplication(Qt_WaitCursor)
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            mdiWin.deletePressed()
        QApplication_restoreOverrideCursor()

    def escapePressed(self):
        " . "
        debug_message("escapePressed()")
        QApplication_setOverrideCursor(Qt_WaitCursor)
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            mdiWin.escapePressed()
        QApplication_restoreOverrideCursor()

        gview = activeView()
        if gview:
            gview.clearRubberRoom()
            gview.previewOff()
            gview.disableMoveRapidFire()

    def toggleGrid(self):
        " . "
        debug_message("toggleGrid()")
        status_bar["GRID"].toggle()

    def toggleRuler(self):
        " . "
        debug_message("toggleRuler()")
        status_bar["RULER"].toggle()

    def toggleLwt(self):
        " . "
        debug_message("toggleLwt()")
        status_bar["LWT"].toggle()

    def enable_move_rapid_fire(self):
        " . "
        gview = activeView()
        if gview:
            gview.enable_move_rapid_fire()

    def disableMoveRapidFire(self):
        " . "
        gview = activeView()
        if gview:
            gview.disableMoveRapidFire()

    def nativeAddTextSingle(self, str, x, y, rot, fill, rubberMode):
        " . "
        gview = activeView()
        gscene = gview.scene()
        if gview and gscene:
            obj = TextSingle(str, x, -y, getCurrentColor())
            obj.objTextFont = settings.text_font
            obj.obj_text = settings.text_style
            obj.setObjectText(obj.objText)
            obj.setRotation(-rot)
            #TODO: single line text fill
            obj.setObjectRubberMode(rubberMode)
            if self.rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

    def nativeAddLine(self, x1, y1, x2, y2, rot, rubberMode):
        " . "
        gview = activeView()
        gscene = gview.scene()
        if gview and gscene:
            obj = Line(x1, -y1, x2, -y2, getCurrentColor())
            obj.setRotation(-rot)
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

    def native_add_rectangle(self, x, y, w, h, rot, fill, rubberMode):
        " . "
        gview = activeView()
        gscene = gview.scene()
        if gview and gscene:
            obj = Rect(x, -y, w, -h, getCurrentColor())
            obj.setRotation(-rot)
            obj.setObjectRubberMode(rubberMode)
            #TODO: rect fill
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

    def add_arc(self, startX, startY, midX, midY, endX, endY, rubberMode):
        gview = activeView()
        scene = activeScene()
        if gview and scene:
            arcObj = Arc(startX, -startY, midX, -midY, endX, -endY, getCurrentColor())
            arcObj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(arcObj)
            scene.addItem(arcObj)
            scene.update()

    def add_circle(self, centerX, centerY, radius, fill, rubberMode):
        gview = activeView()
        gscene = gview.scene()
        if gview and gscene:
            obj = Circle(centerX, -centerY, radius, getCurrentColor())
            obj.setObjectRubberMode(rubberMode)
            #TODO: circle fill
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

    def add_ellipse(self, centerX, centerY, width, height, rot, fill, rubberMode):
        gview = activeView()
        gscene = gview.scene()
        if gview and gscene:
            obj = Ellipse(centerX, -centerY, width, height, getCurrentColor())
            obj.setRotation(-rot)
            obj.setObjectRubberMode(rubberMode)
            #TODO: ellipse fill
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

    def add_point(self, x, y):
        " . "
        gview = activeView()
        if gview:
            obj = Point(x, -y, getCurrentColor())

    def add_polygon(self, startX, startY, p, rubberMode):
        """ 
        NOTE:
            self native is different than the rest in that the Y+ is down
            (scripters need not worry about self)
        """
        gview = activeView()
        gscene = gview.scene()
        if gview and gscene:
            obj = Polygon(startX, startY, p, getCurrentColor())
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

    def add_polyline(self, startX, startY, p, rubberMode):
        """
        NOTE:
            self native is different than the rest in that the Y+ is 
            down (scripters need not worry about self)
        """
        gview = activeView()
        gscene = gview.scene()
        if gview and gscene:
            obj = Polyline(startX, startY, p, getCurrentColor())
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

    def add_dim_leader(x1, y1, x2, y2, rot, rubberMode):
        " . "
        gview = activeView()
        gscene = gview.scene()
        if gview and gscene:
            obj = DimLeader(x1, -y1, x2, -y2, getCurrentColor())
            obj.setRotation(-rot)
            obj.setObjectRubberMode(rubberMode)
            if rubberMode:
                gview.addToRubberRoom(obj)
                gscene.addItem(obj)
                gscene.update()

    def calculate_angle(x1, y1, x2, y2):
        return Line(x1, -y1, x2, -y2).angle()

    def calculate_distance(x1, y1, x2, y2):
        return Line(x1, y1, x2, y2).length()

    def fill_menu(self, menu_id):
        debug_message("MainWindow creating %s", menu_label[menu_id])
        menuBar().addMenu(menu[menu_id])
        for menu in menus[menu_id]:
            if menus[menu_id][i] >= 0:
                menu[menu_id].addAction(actionHash.value(menus[menu_id][i]))

            else:
                menu[menu_id].addSeparator()

    def nativePerpendicularDistance(self):
        """
        self is currently causing a bug and is going to be replaced
        with a libembroidery function.
        """
        line = Line (x1, y1, x2, y2)
        norm = line.normalVector()
        dx = px-x1
        dy = py-y1
        norm.translate(dx, dy)
        iPoint = norm.intersects(line)
        return Line(px, py, iPoint.x(), iPoint.y()).length()

    def recent_menu_about_to_show(self):
        debug_message("recentMenuAboutToShow()")
        menu[RECENT_MENU].clear()

        recentFileInfo = ""
        recentValue = ""
        for i in range(len(opensave_recent_list_of_files)):
            #If less than the max amount of entries add to menu
            if i < settings.opensave_recent_max_files:
                recentFileInfo = QFileInfo(opensave_recent_list_of_files.at(i))
                if recentFileInfo.exists() and validFileFormat(recentFileInfo.fileName()):
                    recentValue.setNum(i+1)
                    rAction = 0
                    if recentValue.toInt() >= 1 and recentValue.toInt() <= 9:
                        rAction = QAction("&" + recentValue + " " + recentFileInfo.fileName(), self)
                    elif recentValue.toInt() == 10:
                        rAction = QAction("1&0 "                  + recentFileInfo.fileName(), self)
                    else:
                        rAction = QAction(recentValue + " " + recentFileInfo.fileName(), self)
                    rAction.setCheckable(0)
                    rAction.setData(opensave_recent_list_of_files.at(i))
                    menu[RECENT_MENU].addAction(rAction)
                    #connect(rAction, SIGNAL(triggered()), self, SLOT(openrecentfile()))

        #Ensure the list only has max amount of entries
        while opensave_recent_list_of_files.size() > settings.opensave_recent_max_files:
            opensave_recent_list_of_files.removeLast()

    def window_menu_about_to_show(self):
        debug_message("windowMenuAboutToShow()")
        menu["WINDOW"].clear()
        menu["WINDOW"].addAction(actionHash.value(ACTION_windowclose))
        menu["WINDOW"].addAction(actionHash.value(ACTION_windowcloseall))
        menu["WINDOW"].addSeparator()
        menu["WINDOW"].addAction(actionHash.value(ACTION_windowcascade))
        menu["WINDOW"].addAction(actionHash.value(ACTION_windowtile))
        menu["WINDOW"].addSeparator()
        menu["WINDOW"].addAction(actionHash.value(ACTION_windownext))
        menu["WINDOW"].addAction(actionHash.value(ACTION_windowprevious))

        menu["WINDOW"].addSeparator()
        windows = mdiArea.subWindowList()
        for i in range(len(windows)):
            aAction = QAction(windows[i].windowTitle(), self)
            aAction.setCheckable(1)
            aAction.setData(i)
            menu["WINDOW"].addAction(aAction)
            #connect(aAction, SIGNAL(toggled(int)), self, SLOT(windowMenuActivated(int)))
            aAction.setChecked(mdiArea.activeSubWindow() == windows[i])

    def windowMenuActivated(checked):
        debug_message("windowMenuActivated()")
        aSender = sender()
        if not aSender:
            return
        w = mdiArea.subWindowList().at[aSender.data().toInt()]
        if w and checked:
            w.setFocus()

    def new_file(self):
        " Creates a new file using the standard dialog. "
        debug_message("New File")
        docIndex += 1
        numOfDocs += 1
        mdiWin = MdiWindow(docIndex, mainWin, mdiArea, Qt_SubWindow)
        #connect(mdiWin, SIGNAL(sendCloseMdiWin()), self, SLOT(onCloseMdiWin()))
        #connect(mdiArea, SIGNAL(subWindowActivated()), self, SLOT(onWindowActivated()))

        updateMenuToolbarStatusbar()
        windowMenuAboutToShow()

        v = mdiWin.getView()
        if v:
            v.recalculateLimits()
            v.zoomExtents()

    def open_file(self, recent, recentFile):
        " Opens a file using the standard dialog. "
        debug_message("Open File")
        Application_setOverrideCursor(Qt_ArrowCursor)

        files = []
        preview = settings.opensave_open_thumbnail
        openFilesPath = settings.opensave_recent_directory

        #Check to see if self from the recent files list
        if recent:
            files.append(recentFile)
            openFilesSelected(files)
        elif not preview:
            #TODO: set getOpenFileNames' selectedFilter parameter from settings.opensave_open_format
            files = QFileDialog_getOpenFileNames(self, translate("Open"), openFilesPath, formatFilterOpen)
            openFilesSelected(files)
        elif preview:
            openDialog = PreviewDialog(self, translate("Open w/Preview"), openFilesPath, formatFilterOpen)
            #TODO: set openDialog.selectNameFilter(const QString& filter) from settings.opensave_open_format
            #connect(openDialog, SIGNAL(filesSelected()), self, SLOT(openFilesSelected()))
            openDialog.exec()

        Application_restoreOverrideCursor()

    def open_files_selected(self, filesToOpen):
        " . "
        doOnce = 1

        if filesToOpen.count(self):
            for i in range(len(filesToOpen)):
                if not validFileFormat(filesToOpen[i]):
                    continue

                existing = findMdiWindow(filesToOpen[i])
                if existing:
                    mdiArea.setActiveSubWindow(existing)
                    continue

                #The docIndex doesn't need increased as it is only used for unnamed files
                numOfDocs += 1
                mdiWin = MdiWindow(docIndex, mainWin, mdiArea, Qt_SubWindow)
                #connect(mdiWin, SIGNAL(sendCloseMdiWin()), self, SLOT(onCloseMdiWin()))
                #connect(mdiArea, SIGNAL(subWindowActivated()), self, SLOT(onWindowActivated()))

                # Make sure the toolbars/etc... are shown before doing their zoomExtents
                if doOnce:
                    updateMenuToolbarStatusbar()
                    doOnce = 0

                if mdiWin.loadFile(filesToOpen[i]):
                    statusbar.showMessage(translate("File(s) loaded"), 2000)
                    mdiWin.show()
                    mdiWin.showMaximized()
                    #Prevent duplicate entries in the recent files list
                    if not opensave_recent_list_of_files.contains(filesToOpen[i], Qt_CaseInsensitive):
                        opensave_recent_list_of_files.prepend(filesToOpen[i])

                    #Move the recent file to the top of the list
                    else:
                        opensave_recent_list_of_files.removeAll(filesToOpen[i])
                        opensave_recent_list_of_files.prepend(filesToOpen[i])

                    settings["opensave_recent_directory"] = QFileInfo(filesToOpen[i].absolutePath())

                    v = mdiWin.getView()
                    if v:
                        v.recalculateLimits()
                        v.zoomExtents()
                else:
                    mdiWin.close()

        windowMenuAboutToShow()

    def openrecentfile(self):
        debug_message("openrecentfile()")

        #Check to see if self from the recent files list
        recentSender = sender()
        if recentSender:
            openFile(1, str(recentSender.data()))

    def savefile(self):
        debug_message("savefile()")

    def saveasfile(self):
        debug_message("saveasfile()")
        # need to find the activeSubWindow before it loses focus to the FileDialog
        mdiWin = mdiArea.activeSubWindow()
        if not mdiWin:
            return

        openFilesPath = settings.opensave_recent_directory
        file = QFileDialog_getSaveFileName(self, translate("Save As"), openFilesPath, formatFilterSave)

        mdiWin.saveFile(file)

    def findMdiWindow(self, fileName):
        debug_message("findMdiWindow(%s)", qPrintable(fileName))
        canonicalFilePath = QFileInfo(fileName).canonicalFilePath()

        for subWindow in mdiArea.subWindowList(self):
            if subWindow.getCurrentFile() == canonicalFilePath:
                return subWindow
        return 0

    def closeEvent(self, event):
        mdiArea.closeAllSubWindows()
        writeSettings()
        event.accept()

    def onCloseWindow(self):
        debug_message("onCloseWindow()")
        mdiWin = mdiArea.activeSubWindow()
        if mdiWin:
            onCloseMdiWin(mdiWin)

    def onCloseMdiWin(self, theMdiWin):
        debug_message("onCloseMdiWin()")
        numOfDocs -= 1

        keepMaximized
        if theMdiWin:
            keepMaximized = theMdiWin.isMaximized()

        mdiArea.removeSubWindow(theMdiWin)
        theMdiWin.deleteLater()

        updateMenuToolbarStatusbar()
        windowMenuAboutToShow()

        if keepMaximized:
            mdiWin = mdiArea.activeSubWindow()
            if mdiWin:
                mdiWin.showMaximized()

    def onWindowActivated(self, w):
        debug_message("onWindowActivated()")
        mdiWin = w.MdiWindow()
        if mdiWin:
            mdiWin.onWindowActivated()

    def resizeEvent(self, e):
        debug_message("resizeEvent()")
        QresizeEvent(e)
        statusBar().setSizeGripEnabled(not isMaximized())

    def getFileSeparator(self):
        debug_message("getFileSeparator()")
        return myFileSeparator

    def updateMenuToolbarStatusbar(self):
        debug_message("updateMenuToolbarStatusbar()")

        actionHash.value(ACTION_print).setEnabled(numOfDocs > 0)
        actionHash.value(ACTION_windowclose).setEnabled(numOfDocs > 0)
        actionHash.value(ACTION_designdetails).setEnabled(numOfDocs > 0)

        if numOfDocs:
            # Toolbars
            for key in toolbar.keys():
                toolbar[key].show()

            # DockWidgets
            dockPropEdit.show()
            dockUndoEdit.show()

            # Menus
            menuBar().clear()
            menuBar().addMenu(menu["FILE"])
            menuBar().addMenu(menu["EDIT"])
            menuBar().addMenu(menu["VIEW"])

            for menu_ in menuHash:
                menuBar().addMenu(menu_)

            menuBar().addMenu(menu["SETTINGS"])
            menuBar().addMenu(menu["WINDOW"])
            menuBar().addMenu(menu["HELP"])

            menu["WINDOW"].setEnabled(1)

            # Statusbar
            statusbar.clearMessage()
            statusBarMouseCoord.show()
            status_bar["SNAP"].show()
            status_bar["GRID"].show()
            status_bar["RULER"].show()
            status_bar["ORTHO"].show()
            status_bar["POLAR"].show()
            status_bar["QSNAP"].show()
            status_bar["QTRACK"].show()
            status_bar["LWT"].show()

        else:
            # Toolbars
            toolbars_to_hide = [
                "VIEW", "ZOOM", "PAN", "ICON", "HELP", "LAYER",
                "TEXT", "PROPERTIES"
            ]
            for tb in toolbars_to_hide:
                toolbar[tb].hide()

            # DockWidgets
            dockPropEdit.hide()
            dockUndoEdit.hide()

            # Menus
            menuBar().clear()
            menuBar().addMenu(menu["FILE"])
            menuBar().addMenu(menu["EDIT"])
            menuBar().addMenu(menu["MENU"])
            menuBar().addMenu(menu["WINDOW"])
            menuBar().addMenu(menu["HELP"])

            menu["WINDOW"].setEnabled(0)

            # Statusbar
            statusbar.clearMessage()
            statusBarMouseCoord.hide()
            for k in status_bar.keys(self):
                status_bar[k].hide()

    def valid_file_format(self, fname):
        " . "
        if len(fname) == 0:
            return 0
        if emb_identify_format(fname) >= 0:
            return 1
        return 0

    def loadFormats(self):
        " . "
        supportedReaders = "All Supported Files ("
        individualReaders = "All Files (*);;"
        supportedWriters = "All Supported Files ("
        individualWriters = "All Files (*);;"
        supportedStr = ""
        individualStr = ""

        #TODO: Stable Only (Settings Option)
        # stable = 'S'
        # unstable = 'S'

        #Stable + Unstable
        stable = 'S'
        unstable = 'U'

        curFormat = 0
        for i in range(numberOfFormats):
            extension = formatTable[i].extension
            description = formatTable[i].description
            readerState = formatTable[i].reader_state
            writerState = formatTable[i].writer_state

            upperExt = QString(extension).toUpper()
            supportedStr = "*" + upperExt + " "
            individualStr = upperExt.replace(".", "") + " - " + description + " (*" + extension + ");;"
            if(readerState == stable or readerState == unstable):
                #Exclude color file formats from open dialogs
                if(upperExt != "COL" and upperExt != "EDR" and upperExt != "INF" and upperExt != "RGB"):
                    supportedReaders.append(supportedStr)
                    individualReaders.append(individualStr)

            if writerState == stable or writerState == unstable:
                supportedWriters.append(supportedStr)
                individualWriters.append(individualStr)

        supportedReaders.append(");;")
        supportedWriters.append(");;")

        formatFilterOpen = supportedReaders + individualReaders
        formatFilterSave = supportedWriters + individualWriters

        # TODO: Fixup custom filter
        custom = settings["custom_filter"]
        if custom.contains("supported", Qt_CaseInsensitive):
            # self will hide it
            custom = ""
        elif not custom.contains("*", Qt_CaseInsensitive):
            # self will hide it
            custom = ""
        else:
            custom = "Custom Filter(" + custom + ");;"

        return translate(qPrintable(custom + supported + all))

    def closeToolBar(self, action):
        " . "
        if action.objectName() == "toolbarclose":
            tb = sender()
            if tb:
                debug_message("%s closed.", qPrintable(tb.objectName()))
                tb.hide()

    def floatingChangedToolBar(isFloating):
        " . "
        tb = sender()
        if tb:
            if isFloating:
                # TODO: Determine best suited close button on various platforms.
                QStyle_SP_DockWidgetCloseButton
                QStyle_SP_TitleBarCloseButton
                QStyle_SP_DialogCloseButton

                ACTION = QAction(tb.style().standardIcon(QStyle_SP_DialogCloseButton), "Close", self)
                ACTION.setStatusTip("Close the " + tb.windowTitle() + " Toolbar")
                ACTION.setObjectName("toolbarclose")
                tb.addAction(ACTION)
                #connect(tb, SIGNAL(actionTriggered()), self, SLOT(closeToolBar()))

            else:
                for action in tb.actions(self):
                    if action.objectName() == "toolbarclose":
                        tb.removeAction(action)
                        dis#connect(tb, SIGNAL(actionTriggered()), self, SLOT(closeToolBar()))
                        del action

    def set_prompt_prefix(self, msg):
        " From an old version, when the prompt was embedded. "
        if DEBUG_MODE:
            with open("prompt.txt", "a", encoding="utf-8") as logfile:
                logfile.write(msg)

    def set_pen(self):
        """
        Missing function from Qt.

        Might need to be in View scope not Window.
        """
        debug_message("set_pen")

    def clear_selection(self):
        " self may need to be a method of Main Window. "
        debug_message("clear_selection")

    def do_nothing(self):
        r""" self function intentionally does nothing. """
        debug_message("do_nothing()")

    def save_file(self):
        " Saves a file using the standard dialog. "
        debug_message("Save File")

    def save_as_file(self):
        " Saves an existing file as a new format using the standard dialog. "
        debug_message("Save file as...")

    def exit_program(self):
        r"""
        Instead of closing using exit() self allows us to chain any checks,
        like the confirmation of the close, to the action.
        """
        debug_message("Closing Embroidermodder 2.0.")
        exit()

    def cut_object(self):
        r"""
        Cut acts on objects selected before the action is called.

        If no objects are selected an error is returned.
        """
        debug_message("cut")

    def copy_object(self):
        r"""
        Copy acts on objects selected before the action is called.

        If no objects are selected an error is returned.
        """
        debug_message("copy")

    def paste_object(self):
        r"""
        Paste whatever objects are on the clipboard.
        """
        debug_message("paste")

    def icon16(self):
        r"""
        Set icon size to 16x16px.
        """
        debug_message("icon16()")

    def icon24(self):
        r"""
        Set icon size to 24x24px.
        """
        debug_message("icon24()")

    def icon32(self):
        r"""
        Set icon size to 32x32px.
        """
        debug_message("icon32()")

    def icon48(self):
        r"""
        Set icon size to 48x48px.
        """
        debug_message("icon48()")

    def icon64(self):
        r"""
        Set icon size to 64x64px.
        """
        debug_message("icon64()")

    def icon128(self):
        r"""
        Set icon size to 128x128px.
        """
        debug_message("icon128()")

    def main_print(self):
        r"""
        Call print dialog to print out a render of the embroidery.
        """
        debug_message("print()")

    def main_redo(self):
        r"""
        Reapply the action that is next on the undo_history array,
        if we are at the top of the array then do nothing.
        """
        debug_message("copy()")
        #gview = _mainWin->activeView()
        #if (gview) {
        #    gview->copy()

    def main_undo(self):
        r"""
        Apply the reverse of the action on the current position
        of the undo_history array.
        """
        debug_message("main_paste()")
        #gview = _mainWin->activeView()
        #if gview:
        #    gview->paste()

    def tip_of_the_day(self):
        r"""
        Run the "tip of the day" dialog box.
        """
        debug_message("main_paste()")

    def changelog(self):
        r"""
        Display the changelog stored in the config.json data file.

        Not currently maintained.
        """
        debug_message("changelog()")

        # display in a custom widget instead
        #
        # QUrl changelogURL("help/changelog.html")
        # QDesktopServices_openUrl(changelogURL)

    def show_all_layers(self):
        r"""
        Apply True to all layer's visibility flags.
        """
        debug_message("showAllLayers()")

    def freeze_all_layers(self):
        r"""
        Make all layers not editable.
        """
        debug_message("freezeAllLayers()")

    def thaw_all_layers(self):
        r"""
        Make layers editable again.
        """
        debug_message("thawAllLayers()")

    def lock_all_layers(self):
        r"""
        Make all layers not editable.
        (What's the difference between freeze and lock?)
        """
        debug_message("lockAllLayers()")

    def windowNext(self):
        r"""
        """
        debug_message("windowNext()")

    def windowPrevious(self):
        r"""
        """
        debug_message("windowPrevious()")

    def textItalic(self):
        r"""
        """
        debug_message("textItalic()")
        settings["text_style_italic"] = not settings["text_style_italic"]

    def textBold(self):
        r"""
        """
        debug_message("textBold()")
        settings["text_style_bold"] = not settings["text_style_bold"]

    def textStrikeout(self):
        r"""
        """
        debug_message("textStrikeout()")
        settings.text_style.strikeout = not settings.text_style.strikeout

    def textUnderline(self):
        r"""
        """
        settings.text_style.underline = not settings.text_style.underline

    def textOverline(self):
        r"""
        """
        settings.text_style.overline = not settings.text_style.overline

    def layerManager(self):
        r"""
        """
        debug_message("layerManager()")
        debug_message("Implement layerManager.")
        #LayerManager layman( _mainWin,  _mainWin)
        #layman.exec()

    def layerPrevious(self):
        r"""
        """
        debug_message("layerPrevious()")
        debug_message("Implement layerPrevious.")

    def zoomRealtime(self):
        r"""
        """
        debug_message("zoomRealtime()")
        debug_message("Implement zoomRealtime.")

    def zoomPrevious(self):
        r"""
        """
        debug_message("zoomPrevious()")
        debug_message("Implement zoomPrevious.")

    def zoomWindow(self):
        r"""
        """
        debug_message("zoomWindow()")
        #gview = _mainWin->activeView()
        #if gview:
        #    gview->zoomWindow()

    def zoomDynamic(self):
        r"""
        """
        debug_message("zoomDynamic()")
        debug_message("Implement zoomDynamic.")

    def zoomScale(self):
        r"""
        """
        debug_message("zoomScale()")
        debug_message("Implement zoomScale.")

    def zoomCenter(self):
        r"""
        """
        debug_message("zoomCenter()")
        debug_message("Implement zoomCenter.")

    def zoomIn(self):
        r"""
        """
        debug_message("zoomIn()")

    def zoomOut(self):
        ""
        debug_message("zoomOut()")

    def zoom_selected(self):
        ""
        debug_message("zoomSelected()")

    def zoom_all(self):
        ""
        debug_message("zoomAll()")
        debug_message("Implement zoomAll.")

    def zoom_extents(self):
        ""
        debug_message("zoomExtents()")

    def pan_realtime(self):
        ""
        debug_message("panrealtime()")

    def pan_point(self):
        ""
        debug_message("panpoint()")

    def pan_left(self):
        ""
        debug_message("panLeft()")

    def pan_right(self):
        ""
        debug_message("panRight()")

    def pan_up(self):
        ""
        debug_message("panUp()")

    def pan_down(self):
        ""
        debug_message("panDown()")

    def dayVision(self):
        r""" TODO: Make day vision color settings.
        gview = _mainWin->activeView()
        if gview:
            gview->setBackgroundColor("#FFFFFF")
            gview->setCrossHairColor("#000000")
            gview->setGridColor("#000000")
            """
        debug_message("day_vision()")

    def nightVision(self):
        r""" TODO: Make night vision color settings.
        gview = _mainWin->activeView()
        if gview:
            gview->setBackgroundColor("#000000")
            gview->setCrossHairColor("#FFFFFF")
            gview->setGridColor("#FFFFFF")
            """
        debug_message("night_vision()")

    def whatsselfContextHelp(self):
        r"""
        """
        debug_message("whatsselfContextHelp()")

    def makeLayerCurrent(self):
        r"""
        """
        debug_message("makeLayerCurrent()")

    def layerSelector(self):
        r"""
        """
        debug_message("main_exit()")

    def main_help(self):
        r"""
        """
        debug_message("main_help")

    def settings_dialog(self):
        r"""
        """
        debug_message("settings_dialog()")

    def design_details(self):
        r"""
        """
        debug_message("main_exit()")

    def main_cut(self):
        r"""
        """
        debug_message("cut()")
        
        #gview = _mainWin->activeView()
        #if gview:
        #    gview->cut()

    def main_copy(self):
        r"""
        """
        debug_message("copy()")
        #gview = _mainWin->activeView()
        #if gview:
        #    gview->copy()

    def main_paste(self):
        r"""
        """
        debug_message("main_paste()")
        #gview = _mainWin->activeView()
        #if gview:
        #    gview->paste()

    def unlock_all_layers(self):
        r"""
        """
        debug_message("unlockAllLayers()")

    def hide_all_layers(self):
        r"""
        """
        debug_message("hideAllLayers()")

    def line_weight_selector(self):
        r"""
        """
        debug_message("main_exit()")

    def line_type_selector(self):
        " Open line type selector dialog. "
        debug_message("main_exit()")

    def color_selector(self):
        " Open color selector dialog. "
        debug_message("main_exit()")

    def window_close(self):
        " Close editor window. "
        debug_message("window_close()")

    def window_tile(self):
        " Tile editor windows. "
        debug_message("window_tile()")

    def window_close_all(self):
        " Close all editor windows. "
        debug_message("window_close_all()")

    def window_cascade(self):
        " Cascade editor windows. "
        debug_message("window_cascade()")

    def make_layer_active(self):
        " Make layer active. "
        debug_message("make_layer_active()")
        debug_message("Implement makeLayerActive.")

    def settings_snap(self):
        " settings snap. "
        debug_message("snap settings tab")

    def settings_grid(self):
        " settings grid. "
        debug_message("grid settings tab")

    def settings_ruler(self):
        " settings ruler. "
        debug_message("settings ruler")

    def settings_ortho(self):
        " settings ortho. "
        debug_message("settings ortho")

    def settings_polar(self):
        " . "
        debug_message("stub")

    def settingsQSnap(self):
        " . "
        debug_message("stub")

    def settingsQTrack(self):
        " . "
        debug_message("stub")

    def settings_lwt(self):
        " . "
        debug_message("stub")

    def toggle_grid(self):
        " . "
        debug_message("StatusBarButton toggleGrid()")
        settings["show_grid"] = not settings["show_grid"]

    def toggle_ruler(self):
        " . "
        debug_message("StatusBarButton toggleRuler()")
        settings["show_ruler"] = not settings["show_ruler"]

    def toggle_ortho(self):
        " . "
        debug_message("StatusBarButton toggleOrtho()")
        settings["show_ortho"] = not settings["show_ortho"]

    def toggle_polar(self):
        " . "
        debug_message("StatusBarButton togglePolar()")
        settings["show_polar"] = not settings["show_polar"]

    def toggle_snap(self):
        r"""
        """
        debug_message("StatusBarButton toggleQSnap()")
        settings["snap"] = not settings["snap"]

    def toggle_track(self):
        r"""
        """
        debug_message("StatusBarButton toggleQTrack()")
        settings["track"] = not settings["track"]

    def toggle_lwt(self):
        r"""
        """
        debug_message("StatusBarButton toggleLwt()")
        settings["show_lwt"] = not settings["show_lwt"]

    def enable_lwt(self):
        r"""
        Switch to rendering all line weights more accurately, so the effect
        of different thread weights can be understood.
        """
        debug_message("StatusBarButton enableLwt()")
        settings["show_lwt"] = True

    def disable_lwt(self):
        r"""
        Switch to rendering all line weights the same, so the effect
        of different thread weights can be ignored.
        """
        debug_message("StatusBarButton disableLwt()")
        settings["show_lwt"] = False

    def enable_real(self):
        r"""
        Turn real rendering on and see the pattern as an approximation of
        what the stitched embroidery will look like.
        """
        debug_message("StatusBarButton enableReal()")
        settings["real_render"] = True

    def disable_real(self):
        r"""
        Turn real rendering off and see the pattern as collection of
        geometric primatives.
        """
        debug_message("StatusBarButton disableReal()")
        settings["real_render"] = False

    def build_menubar(self):
        r"""
        Build the classic UI dropdown menus using the layout defined
        on file in our 'layout.json'.
        """
        debug_message("build_menubar")
        menu_layout = settings["menubar"]
        menubar = tk.Menu(self.root)
        for menu in menu_layout["order"]:
            debug_message(menu)
            menu_ = tk.Menu(menubar, tearoff=0)
            for item in menu_layout[menu]["order"]:
                debug_message(item)
                cmd=menu_layout[menu][item]
                menu_.add_command(label=translate(item), command=lambda: self.actuator(cmd))
            menubar.add_cascade(label=translate(menu), menu=menu_)
        self.root.config(menu=menubar)

    def build_button_grid(self):
        r"""
        Create the toolbars in the order given by the "order" list.
        """
        button_layout = settings["toolbar"]
        debug_message("build_buttongrid")
        for toolbar in button_layout["order"]:
            debug_message(toolbar)
            for button in button_layout[toolbar]["order"]:
                debug_message(button)
                B = button_layout[toolbar][button]
                settings["tkimg"][button] = load_image(B["icon"])
                button_ = tk.Button(
                    self.root,
                    command=lambda: self.actuator(B["command"]),
                    image=settings["tkimg"][button]
                )
                button_.grid(row=B["row"], column=B["column"])

    def old_main(self):
        app = Application(argc, argv)
        app.setApplicationName(_appName_)
        app.setApplicationVersion(_appVer_)

        filesToOpen = []

        for i in range(len(sys.argv)):
            if sys.argv[i] == "-d" or sys.argv[i] == "--debug":
                DEBUG_MODE = True
            elif sys.argv[i] == "-h" or sys.argv[i] == "--help":
                usage()
            elif sys.argv[i] == "-v" or sys.argv[i] == "--version":
                version()
            elif exists(sys.argv[i]) and MainWindow_validFileFormat(sys.argv[i]):
                filesToOpen += [sys.argv[i]]

        if exitApp:
            return 1

        _mainWin = MainWindow()
        app.setMainWin(_mainWin)

        _mainWin.setWindowTitle(app.applicationName() + " " + app.applicationVersion())
        _mainWin.show()

        #NOTE: If openFilesSelected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
        if len(filesToOpen) > 0:
            _mainWin.openFilesSelected(filesToOpen)

        return app.exec()

    def usage(self):
        print(r"""
    ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ 
    | __|     | _ \| _ \ /  \| |   \| __| _ \ |     |/  \|   \|   \| __| _ \  |__ \
    | __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/
    |___|_|_|_|___/|_|\_\\__/|_|___/|___|_|\_\|_|_|_|\__/|___/|___/|___|_|\_\ |___|
    _____________________________________________________________________________ 
    |                                                                             |
    |                         http://embroidermodder.org                          |
    |_____________________________________________________________________________|

    Usage: embroidermodder [options] files ...

    Options:
    -d, --debug      Print lots of debugging information.
    -h, --help       Print self message and exit.
    -v, --version    Print the version number of embroidermodder and exit.
    """)
        exitApp = 1

    def Application_event(event):
        if event.type() == QEvent_FileOpen:
            if _mainWin:
                _mainWin.openFilesSelected(QStringList(event.file()))
                return 1
            # Fall through
        else:
            return QApplication_event(event)
