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

import sys
import tkinter as tk
import time

from embroidermodder.color import Color
from libembroidery import Vector, Line, Pattern
from embroidermodder.image_widget import ImageWidget
from embroidermodder.details_dialog import DetailsDialog
from embroidermodder.settings_dialog import SettingsDialog
from embroidermodder.utility import (settings, translate, debug_message,
                                     load_image, write_settings, about)


def tip_of_the_day():
    r"""
    Run the "tip of the day" dialog box.
    """
    debug_message("tip_of_the_day()")

    appDir = qApp.applicationDirPath()
    wizard_tip_of_the_day = tk.Dialog()
    button1 = tk.Button(wizard_tip_of_the_day)
    button2 = tk.Button(wizard_tip_of_the_day)
    button3 = tk.Button(wizard_tip_of_the_day)

    img_banner = ImageWidget(
        appDir + "/images/did-you-know.png", wizard_tip_of_the_day
    )

    checkBoxTipOfTheDay = tk.CheckBox(translate("&Show tips on startup"), wizard_tip_of_the_day)
    checkBoxTipOfTheDay.set_checked(settings["general_tip_of_the_day"])
    # connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), self, SLOT(checkBoxTipOfTheDayStateChanged(int)))

    tips = settings["tips"]
    if tips[settings["general_current_tip"]] == "":
        settings["general_current_tip"] = 0
    label_tip_of_the_day = tk.Label(tips[settings["general_current_tip"]], wizard_tip_of_the_day)
    label_tip_of_the_day.setWordWrap(1)

    button1.setText("&Previous")
    button2.setText("&Next")
    button3.setText("&Close")
    # connect(button1, SIGNAL(triggered()), wizard_tip_of_the_day,
    #     SLOT(wizard_tip_of_the_day.close()))
    # connect(button2, SIGNAL(triggered()), wizard_tip_of_the_day,
    #     SLOT(wizard_tip_of_the_day.close()))
    # connect(button3, SIGNAL(triggered()), wizard_tip_of_the_day,
    #     SLOT(wizard_tip_of_the_day.close()))

    layout = tk.VBoxLayout(wizard_tip_of_the_day)
    layout.add_widget(img_banner)
    layout.add_strut(1)
    layout.add_widget(label_tip_of_the_day)
    layout.add_stretch(1)
    layout.add_widget(checkBoxTipOfTheDay)
    layout.add_strut(1)
    layout.add_widget(button1)
    layout.add_strut(1)
    layout.add_widget(button2)
    layout.add_strut(1)
    layout.add_widget(button3)

    wizard_tip_of_the_day.set_layout(layout)
    wizard_tip_of_the_day.set_window_title("Tip of the Day")
    wizard_tip_of_the_day.set_minimum_size(550, 400)
    wizard_tip_of_the_day.exec()


def old_main(self, argv):
    r" How the program used to boot in C++/Qt. "
    app = Application(argc, argv)
    app.set_application_name(settings["name"])
    app.set_application_version(settings["version"])

    filesToOpen = []

    for i in range(len(argv)):
        if argv[i] == "-d" or argv[i] == "--debug":
            settings["DEBUG_MODE"] = 1
        elif argv[i] == "-h" or argv[i] == "--help":
            usage()
        elif argv[i] == "-v" or argv[i] == "--version":
            version()
        elif exists(argv[i]) and valid_file_format(argv[i]):
            filesToOpen += [argv[i]]

    main_win = MainWindow()
    app.set_main_win(main_win)

    main_win.set_window_title(settings["title"] + " " + settings["version"])
    main_win.show()

    #NOTE: If open_files_selected() is called from within the mainWin constructor, slot commands wont work and the window menu will be screwed
    if len(filesToOpen) > 0:
        main_win.open_files_selected(filesToOpen)

    return app.mainloop()


class MainWindow():
    r"""
    This is the heart of the program, we're working on replacing
    the Qt reliance, so these functions and data represent the eventual core
    of the program.

    The widget system is created here, but it is built on top of the
    SVG system created in libembroidery. So a widget is an svg drawing,
    with a position to draw it in relative to its parent. The widgets
    form a tree rooted at the global variable called root.

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
        self.build_menu_bar()
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

        # This is a View() instance
        self.canvas = tk.Canvas(self.root, bg="#FFFFFF",
                                width=500, height=400)
        self.canvas.grid(row=4, column=0, columnspan=20, rowspan=2, sticky="W")

        # Use PropertyEditor
        # need to make a tk.Entry test
        self.property_editor = tk.Label(self.root, text="Property Editor",
                                        bg="#FFFFFF")
        self.property_editor.grid(row=4, column=21, columnspan=5, sticky="NE")

        self.undo_history_editor = tk.Label(self.root, text="Undo History",
                                            bg="#FFFFFF")
        self.undo_history_editor.grid(row=5, column=21, columnspan=5, 
                                      sticky="NE")

        self.message_bar = tk.Label(self.root,
                                    text=time.strftime("%d %B %Y"),
                                    bg="#BBBBBB")
        #self.message_bar.set_text("test")
        self.message_bar.grid(row=6, column=0, columnspan=20, sticky="SW")
        #self.message_bar_tip = Tooltip(
        #    self.message_bar,
        #    "*Message Bar*\nShows current state of the program, useful for bug checking information.")

        return

        #for i in range(nFolders):
        #    current_path = self.application_folder + folders[i]
        #    if not exists(current_path):
        #        critical(self, translate("Path Error"), translate("Cannot locate: ") + current_path)

        if lang == "system":
            lang = QLocale_system().languageToString(QLocale_system().language()).lower()

        #Load translations provided by Qt - self covers dialog buttons and other common things.
        translatorQt = ""
        translatorQt.load("" + QLocale_system().name(), QLibraryInfo_location(QLibraryInfo_TranslationsPath))
        # TODO: ensure self always loads, ship a copy of self with the app
        qApp.installTranslator(translatorQt)

        #Selectors
        self.layer_selector = ComboBox()
        self.color_selector = ComboBox()
        self.linetype_selector = ComboBox()
        self.lineweightSelector = ComboBox()
        self.textFontSelector = FontComboBox()
        self.text_size_selector = ComboBox()

        settings["shiftKeyPressedState"] = 0

        set_windowIcon(app.png)
        # Require Minimum WVGA
        set_minimum_size(800, 480)

        loadFormats()

        # create the mdi_area
        vbox = Frame(self)
        layout = VBoxLayout(vbox)
        layout.setContentsMargins(Margins())
        vbox.setFrameStyle(Frame_StyledPanel | Frame_Sunken)
        mdi_area = mdi_area(self, vbox)
        mdi_area.useBackgroundLogo(settings["general_mdi_bg_use_logo"])
        mdi_area.useBackgroundTexture(settings["general_mdi_bg_use_texture"])
        mdi_area.useBackgroundColor(settings["general_mdi_bg_use_color"])
        mdi_area.setBackgroundLogo(settings["general_mdi_bg_logo"])
        mdi_area.setBackgroundTexture(settings["general_mdi_bg_texture"])
        mdi_area.setBackgroundColor(Color(settings["general_mdi_bg_color"]))
        mdi_area.setViewMode("TabbedView")
        mdi_area.setHorizontalScrollBarPolicy("ScrollBarAsNeeded")
        mdi_area.setVerticalScrollBarPolicy("ScrollBarAsNeeded")
        mdi_area.setActivationOrder("ActivationHistoryOrder")
        layout.add_widget(mdi_area)
        setCentralWidget(vbox)

        # setDockOptions(QAnimatedDocks | QAllowTabbedDocks | QVerticalTabs)
        # TODO: Load these from settings
        # tabifyDockWidget(dockPropEdit, dockUndoEdit)
        # TODO: load self from settings

        self.statusbar = StatusBar(self.root)
        self.setStatusBar(statusbar)

        for i in action_list.keys(self):
            Icon = action_list[i].icon
            ACTION = Action(Icon, action_list[i].menu_name, self)

            if len(action_list[i].shortcut)>0:
                ACTION.setShortcut(QKeySequence(action_list[i].shortcut))

            ACTION.set_status_tip(action_list[i].description)
            ACTION.set_object_name(action_list[i].abbreviation)
            ACTION.setWhatsself(action_list[i].description)
            # connect(ACTION, SIGNAL(triggered()), self, SLOT(actions()))
            self.action_hash.insert(i, ACTION)

        self.action_hash.value("window_close").set_enabled(num_of_docs > 0)
        self.action_hash.value("design_details").set_enabled(num_of_docs > 0)

        # menu["FILE"].add_menu(menu[RECENT_MENU])
        # Do not allow the Recent Menu to be torn off. It's a pain in the ass to maintain.
        # menu[RECENT_MENU].set_tear_off_enabled(0)

        debug_message("MainWindow createWindowMenu()")
        menu_bar().add_menu(menu["WINDOW"])
        # connect(menu["WINDOW"], SIGNAL(aboutToShow()), self, SLOT(window_menu_about_to_show()))
        #Do not allow the Window Menu to be torn off. It's a pain in the ass to maintain.
        menu["WINDOW"].set_tear_off_enabled(0)

        for i in range(N_TOOLBARS):
            message = "MainWindow creating %s\n" % toolbar_label[i]
            debug_message(message)

            toolbar[i].set_object_name(toolbar_label[i])

            for j in toolbars[i]:
                if toolbars[i][j] >= 0:
                    toolbar[i].add_action(action_hash.value(toolbars[i][j]))
                else:
                    toolbar[i].add_separator()

            # connect(toolbar[i], SIGNAL(topLevelChanged(int)), self, SLOT(floating_changed_toolbar(int)))

        debug_message("MainWindow createLayerToolbar()")

        toolbar["LAYER"].set_object_name("toolbarLayer")
        toolbar["LAYER"].add_action(action_hash.value("make_layer_current"))
        toolbar["LAYER"].add_action(action_hash.value("layers"))

        #TODO: Create layer pixmaps by concatenating several icons
        layer_selector.add_item("linetypebylayer.png", "0")
        layer_selector.add_item("linetypebylayer.png", "1")
        layer_selector.add_item("linetypebylayer.png", "2")
        layer_selector.add_item("linetypebylayer.png", "3")
        layer_selector.add_item("linetypebylayer.png", "4")
        layer_selector.add_item("linetypebylayer.png", "5")
        layer_selector.add_item("linetypebylayer.png", "6")
        layer_selector.add_item("linetypebylayer.png", "7")
        layer_selector.add_item("linetypebylayer.png", "8")
        layer_selector.add_item("linetypebylayer.png", "9")
        toolbar["LAYER"].add_widget(layer_selector)
        # connect(layer_selector, SIGNAL(currentIndexChanged(int)), self, SLOT(layer_selectorIndexChanged(int)))

        toolbar["LAYER"].add_action(action_hash.value("layer_previous"))

        # connect(toolbar["LAYER"], SIGNAL(topLevelChanged(int)), self, SLOT(floating_changed_toolbar(int)))

        debug_message("MainWindow createPropertiesToolbar()")

        toolbar["PROPERTIES"].set_object_name("toolbar_properties")

        color_selector.set_focusProxy(menu["FILE"])
        # NOTE: Qt4.7 wont load icons without an extension...
        color_selector.add_item("colorbylayer.png", "ByLayer")
        color_selector.add_item("colorbyblock.png", "ByBlock")
        color_selector.add_item("colorred.png", translate("Red"), (255, 0, 0))
        color_selector.add_item("coloryellow.png", translate("Yellow"), (255,255, 0))
        color_selector.add_item("colorgreen.png", translate("Green"), (0, 255, 0))
        color_selector.add_item("colorcyan.png", translate("Cyan"), (0,255,255))
        color_selector.add_item("colorblue.png", translate("Blue"), (0, 0,255))
        color_selector.add_item("colormagenta.png", translate("Magenta"), (255, 0,255))
        color_selector.add_item("colorwhite.png", translate("White"), (255,255,255))
        color_selector.add_item("colorother.png", translate("Other..."))
        toolbar["PROPERTIES"].add_widget(color_selector)
        # connect(color_selector, SIGNAL(currentIndexChanged(int)), self, SLOT(color_selectorIndexChanged(int)))

        toolbar["PROPERTIES"].add_separator()
        linetype_selector.set_focusProxy(menu["FILE"])
        linetype_selector.add_item("linetypebylayer.png", "ByLayer")
        linetype_selector.add_item("linetypebyblock.png", "ByBlock")
        linetype_selector.add_item("linetypecontinuous.png", "Continuous")
        linetype_selector.add_item("linetypehidden.png", "Hidden")
        linetype_selector.add_item("linetypecenter.png", "Center")
        linetype_selector.add_item("linetypeother.png", "Other...")
        toolbar["PROPERTIES"].add_widget(linetype_selector)
        # connect(linetype_selector, SIGNAL(currentIndexChanged(int)), self, SLOT(linetype_selectorIndexChanged(int)))

        toolbar["PROPERTIES"].add_separator()
        lineweightSelector.set_focusProxy(menu["FILE"])
        #NOTE: Qt4.7 wont load icons without an extension...
        # TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight
        for line in settings["thread_weights"]:
            lineweightSelector.add_item(line[0], line[1], line[2])
        lineweightSelector.setMinimumContentsLength(8)
        # Prevent dropdown text readability being squish...d.
        toolbar["PROPERTIES"].add_widget(lineweightSelector)
        # connect(lineweightSelector, SIGNAL(currentIndexChanged(int)), self, SLOT(lineweightSelectorIndexChanged(int)))

        # connect(toolbar["PROPERTIES"], SIGNAL(topLevelChanged()), self, SLOT(floating_changed_toolbar()))

        debug_message("MainWindow createTextToolbar()")
        toolbar["TEXT"].set_object_name("toolbarText")
        toolbar["TEXT"].add_widget(textFontSelector)
        textFontSelector.setCurrentFont(Font(settings["text_font"]))
        # connect(textFontSelector, SIGNAL(currentFontChanged()), self, SLOT(textFontSelectorCurrentFontChanged()))

        # TODO: SEGFAULTING FOR SOME REASON
        toolbar["TEXT"].add_action(action_hash.value("text_bold"))
        self.action_hash.value("text_bold").set_checked(settings["text_style_bold"])
        toolbar["TEXT"].add_action(action_hash.value("text_italic"))
        self.action_hash.value("text_italic").set_checked(settings["text_style_italic"])
        toolbar["TEXT"].add_action(action_hash.value("text_underline"))
        self.action_hash.value("text_underline").set_checked(settings["text_style_underline"])
        toolbar["TEXT"].add_action(action_hash.value("text_strikeout"))
        self.action_hash.value("text_strikeout").set_checked(settings["text_style_strikeout"])
        toolbar["TEXT"].add_action(action_hash.value("text_overline"))
        self.action_hash.value("text_overline").set_checked(settings["text_style_overline"])

        text_size_selector.set_focusProxy(menu["FILE"])
        sizes = [6, 8, 9, 10, 11, 12, 14, 18, 24, 30, 36, 48, 60, 72]
        for size in sizes:
            text_size_selector.add_item(str(size)+" pt", size)
        setTextSize(settings["text_size"])
        toolbar["TEXT"].add_widget(text_size_selector)
        # connect(text_size_selector, SIGNAL(currentIndexChanged(int)), self, SLOT(text_size_selectorIndexChanged(int)))

        # connect(toolbar["TEXT"], SIGNAL(topLevelChanged(int)), self, SLOT(floating_changed_toolbar(int)))

        # Horizontal
        toolbar["VIEW"].set_orientation("Horizontal")
        toolbar["ZOOM"].set_orientation("Horizontal")
        toolbar["LAYER"].set_orientation("Horizontal")
        toolbar["PROPERTIES"].set_orientation("Horizontal")
        toolbar["TEXT"].set_orientation("Horizontal")
        # Top
        add_toolbar_break("TopToolBarArea")
        add_toolbar("TopToolBarArea", toolbar["FILE"])
        add_toolbar("Top toolbar area", toolbar["EDIT"])
        add_toolbar("Top toolbar area", toolbar["HELP"])
        add_toolbar("Top toolbar area", toolbar["ICON"])
        add_toolbar_break("TopToolBarArea")
        add_toolbar("Top toolbar area", toolbar["ZOOM"])
        add_toolbar("Top toolbar area", toolbar["PAN"])
        add_toolbar("Top toolbar area", toolbar["VIEW"])
        add_toolbar_break("TopToolBarArea")
        add_toolbar("Top toolbar area", toolbar["LAYER"])
        add_toolbar("Top toolbar area", toolbar["PROPERTIES"])
        add_toolbar_break("TopToolBarArea")
        add_toolbar("Top toolbar area", toolbar["TEXT"])

        #zoomToolBar.setToolButtonStyle("ToolButtonTextOnly")

        self.icon_resize(settings["general_icon_size"])
        self.update_menu_toolbar_statusbar()

        # Show date in statusbar after it has been updated
        # TODO: Switch to ISO dates.

        date = time.currentDate()
        datestr = date.toString("MMMM d, yyyy")
        statusbar.showMessage(datestr)

        showNormal()

        if settings["general_tip_of_the_day"]:
            tipOfTheDay()

    def read_settings(self):
        debug_message("Reading settings...")

        # self file needs to be read from the users home directory to ensure it is writable.
        pos = Vector(settings["window_x"], settings["window_y"])
        size = (settings["window_width"], settings["window_height"])

        #
        layoutState = settings_file.value("LayoutState").toByteArray()
        if not restoreState(layoutState):
            debug_message("LayoutState NOT restored! Setting Default Layout...")
            # someToolBar.setVisible(1)

        settings = load_data("config.json")

        # Sanitise data here
        settings["window_x"] = clamp(0, settings["window_x"], 1000)
        settings["window_y"] = clamp(0, settings["window_y"], 1000)

        move(pos)
        resize(size)

    def write_settings(self):
        " . "
        debug_message("Writing settings...")

        settings["window_x"] = self.pos().x()
        settings["window_y"] = self.pos().y()
        settings["window_width"] = self.size().width()
        settings["window_height"] = self.size().height()

        write_settings()

    def settings_dialog(self, showTab):
        " . "
        dialog = SettingsDialog(showTab)
        dialog.mainloop()

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
            about()
        elif action == "design_details":
            self.design_details()
        elif action == "print":
            self.print()
        elif action == "exit":
            self.exit_program()
        elif action == "cut":
            self.main_cut()
        elif action == "copy":
            self.main_copy()
        elif action == "paste":
            self.main_paste()
        elif action == "undo":
            self.main_undo()
        elif action == "redo":
            self.main_redo()
        elif action == "window_close":
            self.window_close()
        elif action == "window_close_all":
            self.window_close_all()
        elif action == "window_cascade":
            self.window_cascade()
        elif action == "window_tile":
            self.window_tile()
        elif action == "window_next":
            self.window_next()
        elif action == "window_previous":
            self.window_previous()
        elif action == "help":
            self.help()
        elif action == "changelog":
            self.changelog()
        elif action == "tipoftheday":
            self.tipoftheday()
        elif action == "about":
            self.about()
        elif action == "whats_this":
            self.whats_this()
        elif action == "icon16":
            self.icon16()
        elif action == "icon24":
            self.icon24()
        elif action == "icon32":
            self.icon32()
        elif action == "icon48":
            self.icon48()
        elif action == "icon64":
            self.icon64()
        elif action == "icon128":
            self.icon128()
        elif action == "settings_dialog":
            self.settings_dialog()
        elif action == "makelayercurrent":
            self.make_layer_current()
        elif action == "layers":
            self.layers()
        elif action == "layer_selector":
            self.layer_selector()
        elif action == "layerprevious":
            self.layer_previous()
        elif action == "color_selector":
            self.color_selector()
        elif action == "linetype_selector":
            self.line_type_selector()
        elif action == "lineweightselector":
            self.line_weight_selector()
        elif action == "hidealllayers":
            self.hide_all_layers()
        elif action == "showalllayers":
            self.show_all_layers()
        elif action == "freezealllayers":
            self.freeze_all_layers()
        elif action == "thawalllayers":
            self.thaw_all_layers()
        elif action == "lockalllayers":
            self.lock_all_layers()
        elif action == "unlockalllayers":
            self.unlock_all_layers()
        elif action == "textbold":
            self.text_bold()
        elif action == "text_italic":
            self.text_italic()
        elif action == "text_underline":
            self.text_underline()
        elif action == "textstrikeout":
            self.text_strikeout()
        elif action == "textoverline":
            self.text_overline()
        elif action == "zoomrealtime":
            self.zoom_realtime()
        elif action == "zoomprevious":
            self.zoom_previous()
        elif action == "zoomwindow":
            self.zoom_window()
        elif action == "zoomdynamic":
            self.zoom_dynamic()
        elif action == "zoomscale":
            self.zoom_scale()
        elif action == "zoomcenter":
            self.zoom_center()
        elif action == "zoomin":
            self.zoom_in()
        elif action == "zoomout":
            self.zoom_out()
        elif action == "zoomselected":
            self.zoom_selected()
        elif action == "zoomall":
            self.zoom_all()
        elif action == "zoomextents":
            self.zoom_extents()
        elif action == "panrealtime":
            self.pan_real_time()
        elif action == "panpoint":
            self.pan_point()
        elif action == "panleft":
            self.pan_left()
        elif action == "panright":
            self.pan_right()
        elif action == "panup":
            self.pan_up()
        elif action == "pandown":
            self.pandown()
        elif action == "day":
            self.day()
        elif action == "night":
            self.night()
        elif action == "treble_clef":
            self.treble_clef()
        elif action == "path":
            self.path()
        elif action == "circle":
            self.circle()
        elif action == "line":
            self.line()
        elif action == "distance":
            self.distance()
        elif action == "dolphin":
            self.dolphin()
        elif action == "ellipse":
            self.ellipse()
        elif action == "delete":
            self.delete()
        elif action == "heart":
            self.heart()
        elif action == "locatepoint":
            self.locate_point()

    def stub_testing(self):
        warning(self, translate("Testing Feature"), translate("<b>self feature is in testing.</b>"))

    def check_for_updates(self):
        debug_message("check_for_updates()")
        #TODO: Check website for versions, commands, etc...

    def select_all(self):
        debug_message("selectAll()")
        gview = active_view()
        if gview:
            gview.selectAll()

    def design_details(self):
        scene = active_scene()
        if scene:
            dialog = Details_Dialog(scene, self)
            dialog.exec()

    def whats_this(self):
        debug_message("whatsselfContextHelp()")
        QWhatsself_enterWhatsselfMode()

    def print(self):
        debug_message("print()")
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            mdi_win.print()

    def button_tip_of_the_day_clicked(self, button):
        r" . "
        debug_message("buttonTipOfTheDayClicked(%d)", button)
        if button == QWizard_CustomButton1:
            if settings["general_current_tip"] > 0:
                settings["general_current_tip"] -= 1
            else:
                settings["general_current_tip"] = listTipOfTheDay.size()-1
            label_tip_of_the_day.setText(listTipOfTheDay.value(settings["general_current_tip"]))

        elif button == QWizard_CustomButton2:
            settings["general_current_tip"] += 1
            if settings["general_current_tip"] >= listTipOfTheDay.size(self):
                settings["general_current_tip"] = 0
            label_tip_of_the_day.setText(listTipOfTheDay.value(settings["general_current_tip"]))

        elif button == QWizard_CustomButton3:
            wizard_tip_of_the_day.close()

    def help(self):
        debug_message("help()")

        # display in a custom widget instead
        # Open the HTML Help in the default browser
        helpURL = QUrl ("file:///" + qApp.applicationDirPath() + "/help/doc-index.html")
        QDesktopServices_openUrl(helpURL)

        #TODO: self is how to start an external program. Use this elsewhere...
        # program = "firefox"
        # arguments = ["help/commands.html"]
        # myProcess = Process()
        # myProcess.start(program, arguments)

    def is_shift_pressed(self):
        return settings["shiftKeyPressedState"]

    def set_shift_pressed(self):
        " . "
        settings["shiftKeyPressedState"] = True

    def set_shift_released(self):
        " . "
        settings["shiftKeyPressedState"] = False

    def icon_resize(self, icon_size=16):
        " . "
        self.seticon_size(icon_size, icon_size)
        layer_selector.seticon_size(icon_size*4, icon_size)
        color_selector.seticon_size(icon_size, icon_size)
        linetype_selector.seticon_size(icon_size*4, icon_size)
        lineweightSelector.seticon_size(icon_size*4, icon_size)
        # set the minimum combobox width so the text is always readable
        layer_selector.set_minimum_width(icon_size*4)
        color_selector.set_minimum_width(icon_size*2)
        linetype_selector.set_minimum_width(icon_size*4)
        lineweightSelector.set_minimum_width(icon_size*4)

        # TODO: low-priority:
        # open app with icon_size set to 128. resize the icons to a smaller size.

        settings["general_icon_size"] = icon_size

    def active_mdi_window(self):
        " . "
        debug_message("activemdi_window()")
        mdi_win = mdi_area.active_sub_window()
        return mdi_win

    def active_view(self):
        " . "
        debug_message("active_view()")
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            v = mdi_win.getView()
            return v
        return 0

    def active_scene(self):
        " . "
        debug_message("active_scene()")
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            return mdi_win.getScene()
        return 0

    def update_all_view_scroll_bars(self, val):
        r" . "
        windowList = mdi_area.sub_window_list()
        for mdi_win in windowList:
            mdi_win.showViewScrollBars(val)

    def update_all_view_cross_hair_colors(self, color):
        r" . "
        windowList = mdi_area.sub_window_list()
        for mdi_win in windowList:
            mdi_win.setViewCrossHairColor(color)

    def updateAllViewBackgroundColors(self, color):
        r" . "
        windowList = mdi_area.sub_window_list()
        for mdi_win in windowList:
            mdi_win.setViewBackgroundColor(color)

    def updateAllViewSelectBoxColors(self, colorL, fillL, colorR, fillR, alpha):
        " . "
        windowList = mdi_area.sub_window_list()
        for mdi_win in windowList:
            mdi_win.setViewSelectBoxColors(colorL, fillL, colorR, fillR, alpha)

    def updateAllViewGridColors(self, color):
        " . "
        windowList = mdi_area.sub_window_list()
        for mdi_win in windowList:
            mdi_win.setViewGridColor(color)

    def update_all_view_ruler_colors(self, color):
        " . "
        windowList = mdi_area.sub_window_list()
        for mdi_win in windowList:
            mdi_win.setViewRulerColor(color)

    def update_pick_add_mode(self, val):
        " . "
        settings["selection_mode_pickadd"] = val
        dockPropEdit.update_pick_add_mode_button(val)

    def pick_add_mode_toggled(self):
        " . "
        val = not settings["selection_mode_pickadd"]
        updatePickAddMode(val)

    def layer_selector_index_changed(self, index):
        " . "
        debug_message("layer_selectorIndexChanged(%d)", index)

    def color_selector_index_changed(self, index):
        " Color selector index changed. "
        debug_message("color_selectorIndexChanged(%d)", index)

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

        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            mdi_win.currentColorChanged(newColor)

    def linetype_selector_index_changed(self, index):
        " . "
        debug_message("linetype_selectorIndexChanged(%d)", index)
        self.actuator("do_nothing")

    def lineweight_selector_index_changed(self, index):
        " . "
        debug_message("lineweightSelectorIndexChanged(%d)", index)
        self.actuator("do_nothing")

    def text_fontSelectorCurrentFontChanged(self, font):
        " text font selector current font changed. "
        debug_message("textFontSelectorCurrentFontChanged()")
        textFontSelector.setCurrentFont(Font(font.family()))
        settings["text_font"] = font.family().toLocal8Bit().constData()

    def text_size_selectorIndexChanged(self, index):
        " . "
        debug_message("text_size_selectorIndexChanged(%d)", index)
        settings["text_style.size"] = abs(text_size_selector.itemData(index).toReal())
        #TODO: check that the toReal() conversion is ok

    def text_font(self):
        " . "
        return settings["text_font"]

    def setTextSize(self, num):
        settings["text_style"].size = abs(num)
        index = text_size_selector.find_text("Custom", "MatchContains")
        if index != -1:
            text_size_selector.remove_item(index)
        text_size_selector.add_item("Custom " + "".set_num(num, 'f', 2) + " pt", num)
        index = text_size_selector.find_text("Custom", "MatchContains")
        if index != -1:
            text_size_selector.setCurrentIndex(index)

    def getCurrentLayer(self):
        " . "
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            return mdi_win.getCurrentLayer()
        return "0"

    def get_current_color(self):
        " TODO: return color ByLayer "
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            return mdi_win.get_current_color()
        return 0

    def getCurrentLineType(self):
        " . "
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            return mdi_win.getCurrentLineType()
        return "ByLayer"

    def get_current_line_weight(self):
        " . "
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            return mdi_win.get_current_line_weight()
        return "ByLayer"

    def delete_pressed(self):
        " . "
        debug_message("deletePressed()")
        set_override_cursor = tk.Application("WaitCursor")
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            mdi_win.deletePressed()
        self.restore_override_cursor()

    def escape_pressed(self):
        " . "
        debug_message("escapePressed()")
        self.set_override_cursor("WaitCursor")
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            mdi_win.escapePressed()
        self.restoreOverrideCursor()

        gview = active_view()
        if gview:
            gview.clearRubberRoom()
            gview.previewOff()
            gview.disableMoveRapidFire()

    def enable_move_rapid_fire(self):
        " . "
        gview = active_view()
        if gview:
            gview.enable_move_rapid_fire()

    def disable_move_rapid_fire(self):
        " . "
        gview = active_view()
        if gview:
            gview.disable_move_rapid_fire()

    def native_add_text_single(self, str, x, y, rot, fill, rubber_mode):
        " . "
        gview = active_view()
        gscene = gview.scene()
        if gview and gscene:
            obj = TextSingle(str, x, -y, get_current_color())
            obj.objTextFont = settings["text_font"]
            obj.obj_text = settings["text_style"]
            obj.set_object_text(obj.objText)
            obj.set_rotation(-rot)
            #TODO: single line text fill
            obj.set_object_rubber_mode(rubber_mode)
            if self.rubber_mode:
                gview.add_to_rubber_room(obj)
                gscene.add_item(obj)
                gscene.update()

    def native_add_line(self, x1, y1, x2, y2, rot, rubber_mode):
        " . "
        gview = active_view()
        gscene = gview.scene()
        if gview and gscene:
            obj = Line(x1, -y1, x2, -y2, get_current_color())
            obj.set_rotation(-rot)
            obj.set_object_rubber_mode(rubber_mode)
            if rubber_mode:
                gview.add_to_rubber_room(obj)
                gscene.add_item(obj)
                gscene.update()

    def native_add_rectangle(self, x, y, w, h, rot, fill, rubber_mode):
        " . "
        gview = active_view()
        gscene = gview.scene()
        if gview and gscene:
            obj = Rect(x, -y, w, -h, get_current_color())
            obj.set_rotation(-rot)
            obj.set_object_rubber_mode(rubber_mode)
            #TODO: rect fill
            if rubber_mode:
                gview.add_to_rubber_room(obj)
                gscene.add_item(obj)
                gscene.update()

    def add_arc(self, start, mid, end, rubber_mode):
        " . "
        gview = active_view()
        scene = active_scene()
        if gview and scene:
            arc_obj = Arc(start.x, -start.y, mid.x, -mid.y, end.x, -end.y, get_current_color())
            arc_obj.set_object_rubber_mode(rubber_mode)
            if rubber_mode:
                gview.add_to_rubber_room(arc_obj)
            scene.add_item(arc_obj)
            scene.update()

    def add_circle(self, center, radius, fill, rubber_mode):
        " . "
        gview = active_view()
        gscene = gview.scene()
        if gview and gscene:
            obj = Circle(center.x, -center.y, radius, get_current_color())
            obj.set_object_rubber_mode(rubber_mode)
            #TODO: circle fill
            if rubber_mode:
                gview.add_to_rubber_room(obj)
                gscene.add_item(obj)
                gscene.update()

    def add_ellipse(self, center, width, height, rot, fill, rubber_mode):
        " . "
        gview = active_view()
        gscene = gview.scene()
        if gview and gscene:
            obj = Ellipse(center.x, -center.y, width, height, get_current_color())
            obj.set_rotation(-rot)
            obj.set_object_rubber_mode(rubber_mode)
            #TODO: ellipse fill
            if rubber_mode:
                gview.add_to_rubber_room(obj)
                gscene.add_item(obj)
                gscene.update()

    def add_point(self, x, y):
        " . "
        gview = active_view()
        if gview:
            obj = Point(x, -y, get_current_color())

    def add_polygon(self, start, p, rubber_mode):
        """
        NOTE:
            self native is different than the rest in that the Y+ is down
            (scripters need not worry about self)
        """
        gview = active_view()
        gscene = gview.scene()
        if gview and gscene:
            obj = Polygon(start, p, get_current_color())
            obj.set_object_rubber_mode(rubber_mode)
            if rubber_mode:
                gview.add_to_rubber_room(obj)
                gscene.add_item(obj)
                gscene.update()

    def add_polyline(self, start, p, rubber_mode):
        """
        NOTE:
            self native is different than the rest in that the Y+ is
            down (scripters need not worry about self)
        """
        gview = active_view()
        gscene = gview.scene()
        if gview and gscene:
            obj = Polyline(start, p, get_current_color())
            obj.set_object_rubber_mode(rubber_mode)
            if rubber_mode:
                gview.add_to_rubber_room(obj)
                gscene.add_item(obj)
                gscene.update()

    def add_dim_leader(x1, y1, x2, y2, rot, rubber_mode):
        " . "
        gview = active_view()
        gscene = gview.scene()
        if gview and gscene:
            obj = DimLeader(x1, -y1, x2, -y2, get_current_color())
            obj.set_rotation(-rot)
            obj.set_object_rubber_mode(rubber_mode)
            if rubber_mode:
                gview.add_to_rubber_room(obj)
                gscene.add_item(obj)
                gscene.update()

    def calculate_angle(x1, y1, x2, y2):
        " . "
        return Line(x1, -y1, x2, -y2).angle()

    def calculate_distance(x1, y1, x2, y2):
        " . "
        return Line(x1, y1, x2, y2).length()

    def fill_menu(self, menu_id):
        " . "
        debug_message("MainWindow creating %s", menu_label[menu_id])
        menu_bar().add_menu(menu[menu_id])
        for menu in menus[menu_id]:
            if menus[menu_id][i] >= 0:
                menu[menu_id].add_action(action_hash.value(menus[menu_id][i]))

            else:
                menu[menu_id].add_separator()

    def native_perpendicular_distance(self):
        """
        This is currently causing a bug and is going to be replaced
        with a libembroidery function.
        """
        line = Line(x1, y1, x2, y2)
        norm = line.normal()
        delta.x = point.x-x1
        delta.y = point.y-y1
        norm.translate(delta)
        iPoint = norm.intersects(line)
        return Line(point, iPoint).length()

    def recent_menu_about_to_show(self):
        " . "
        debug_message("recentMenuAboutToShow()")
        menu[RECENT_MENU].clear()

        recent_file_info = ""
        recent_value = ""
        for i in range(len(opensave_recent_list_of_files)):
            #If less than the max amount of entries add to menu
            if i < settings["opensave_recent_max_files"]:
                recent_file_info = FileInfo(opensave_recent_list_of_files.at(i))
                if recent_file_info.exists() and valid_file_format(recent_file_info.fileName()):
                    recent_value.set_num(i+1)
                    rAction = 0
                    if recent_value.toInt() >= 1 and recent_value.toInt() <= 9:
                        rAction = Action("&" + recent_value + " " + recent_file_info.fileName(), self)
                    elif recent_value.toInt() == 10:
                        rAction = Action("1&0 "                  + recent_file_info.fileName(), self)
                    else:
                        rAction = Action(recent_value + " " + recent_file_info.fileName(), self)
                    rAction.set_checkable(0)
                    rAction.set_data(opensave_recent_list_of_files.at(i))
                    menu[RECENT_MENU].add_action(rAction)
                    # connect(rAction, SIGNAL(triggered()), self, SLOT(openrecentfile()))

        # Ensure the list only has max amount of entries
        while opensave_recent_list_of_files.size() > settings["opensave_recent_max_files"]:
            opensave_recent_list_of_files.removeLast()

    def window_menu_about_to_show(self):
        " . "
        debug_message("window_menu_about_to_show()")
        menu["WINDOW"].clear()
        menu["WINDOW"].add_action(action_hash.value("window_close"))
        menu["WINDOW"].add_action(action_hash.value("window_close_all"))
        menu["WINDOW"].add_separator()
        menu["WINDOW"].add_action(action_hash.value("window_cascade"))
        menu["WINDOW"].add_action(action_hash.value("window_tile"))
        menu["WINDOW"].add_separator()
        menu["WINDOW"].add_action(action_hash.value("window_next"))
        menu["WINDOW"].add_action(action_hash.value("window_previous"))

        menu["WINDOW"].add_separator()
        windows = mdi_area.sub_window_list()
        for i in range(len(windows)):
            an_action = Action(windows[i].window_title(), self)
            an_action.set_checkable(1)
            an_action.set_data(i)
            menu["WINDOW"].add_action(an_action)
            # connect(an_action, SIGNAL(toggled(int)), self, SLOT(windowMenuActivated(int)))
            an_action.set_checked(mdi_area.active_sub_window() == windows[i])

    def window_menu_activated(checked):
        " . "
        debug_message("windowMenuActivated()")
        a_sender = sender()
        if not a_sender:
            return
        w = mdi_area.sub_window_list().at[a_sender.data().toInt()]
        if w and checked:
            w.set_focus()

    def new_file(self):
        " Creates a new file using the standard dialog. "
        debug_message("New File")
        self.doc_index += 1
        self.num_of_docs += 1
        mdi_win = mdi_window(self.doc_index, self, mdi_area, "SubWindow")
        # connect(mdi_win, SIGNAL(sendClosemdi_win()), self, SLOT(on_close_mdi_win()))
        # connect(mdi_area, SIGNAL(subWindowActivated()), self, SLOT(on_window_activated()))

        self.update_menu_toolbar_statusbar()
        self.window_menu_about_to_show()

        v = mdi_win.get_view()
        if v:
            v.recalculateLimits()
            v.zoomExtents()

    def open_file(self, recent, recentFile):
        " Opens a file using the standard dialog. "
        debug_message("Open File")
        Application_setOverrideCursor("ArrowCursor")

        files = []
        preview = settings["opensave_open_thumbnail"]
        openFilesPath = settings["opensave_recent_directory"]

        #Check to see if self from the recent files list
        if recent:
            files.append(recentFile)
            open_files_selected(files)
        elif not preview:
            #TODO: set getOpenFileNames' selectedFilter parameter from settings["opensave_open_format"]
            files = FileDialog_getOpenFileNames(self, translate("Open"), openFilesPath, format_filter_open)
            open_files_selected(files)
        elif preview:
            openDialog = PreviewDialog(self, translate("Open w/Preview"), openFilesPath, format_filter_open)
            #TODO: set openDialog.selectNameFilter(const String& filter) from settings["opensave_open_format"]
            # connect(openDialog, SIGNAL(filesSelected()), self, SLOT(open_files_selected()))
            openDialog.exec()

        Application_restoreOverrideCursor()

    def open_files_selected(self, filesToOpen):
        " . "
        doOnce = 1

        if filesToOpen.count(self):
            for i in range(len(filesToOpen)):
                if not valid_file_format(filesToOpen[i]):
                    continue

                existing = findmdi_window(filesToOpen[i])
                if existing:
                    mdi_area.setactive_sub_window(existing)
                    continue

                #The docIndex doesn't need increased as it is only used for unnamed files
                self.num_of_docs += 1
                mdi_win = mdi_window(self.doc_index, self, mdi_area, "SubWindow")
                # connect(mdi_win, SIGNAL(sendClosemdi_win()), self, SLOT(onClosemdi_win()))
                # connect(mdi_area, SIGNAL(subWindowActivated()), self, SLOT(onWindowActivated()))

                # Make sure the toolbars/etc... are shown before doing their zoomExtents
                if doOnce:
                    self.update_menu_toolbar_statusbar()
                    doOnce = 0

                if mdi_win.loadFile(filesToOpen[i]):
                    statusbar.showMessage(translate("File(s) loaded"), 2000)
                    mdi_win.show()
                    mdi_win.showMaximized()
                    #Prevent duplicate entries in the recent files list
                    if not opensave_recent_list_of_files.contains(filesToOpen[i], CaseInsensitive):
                        opensave_recent_list_of_files.prepend(filesToOpen[i])

                    #Move the recent file to the top of the list
                    else:
                        opensave_recent_list_of_files.removeAll(filesToOpen[i])
                        opensave_recent_list_of_files.prepend(filesToOpen[i])

                    settings["opensave_recent_directory"] = FileInfo(filesToOpen[i].absolutePath())

                    v = mdi_win.getView()
                    if v:
                        v.recalculateLimits()
                        v.zoomExtents()
                else:
                    mdi_win.close()

        window_menu_about_to_show()

    def open_recent_file(self):
        " . "
        debug_message("openrecentfile()")

        #Check to see if self from the recent files list
        recent_sender = sender()
        if recent_sender:
            openFile(1, str(recent_sender.data()))

    def findmdi_window(self, fileName):
        " . "
        debug_message(f"findmdi_window({fileName})")
        canonicalFilePath = FileInfo(fileName).canonicalFilePath()

        for subWindow in mdi_area.sub_window_list(self):
            if subWindow.getCurrentFile() == canonicalFilePath:
                return subWindow
        return 0

    def closeEvent(self, event):
        " . "
        mdi_area.close_all_sub_windows()
        write_settings()
        event.accept()

    def on_close_window(self):
        " . "
        debug_message("onCloseWindow()")
        mdi_win = mdi_area.active_sub_window()
        if mdi_win:
            onClosemdi_win(mdi_win)

    def onClosemdi_win(self, themdi_win):
        " . "
        debug_message("onClosemdi_win()")
        num_of_docs -= 1

        keep_maximized = False
        if themdi_win:
            keep_maximized = themdi_win.isMaximized()

        mdi_area.removeSubWindow(themdi_win)
        themdi_win.deleteLater()

        self.update_menu_toolbar_statusbar()
        window_menu_about_to_show()

        if keep_maximized:
            mdi_win = mdi_area.active_sub_window()
            if mdi_win:
                mdi_win.show_maximized()

    def on_window_activated(self, w):
        " . "
        debug_message("onWindowActivated()")
        mdi_win = w.mdi_window()
        if mdi_win:
            mdi_win.on_window_activated()

    def resize_event(self, e):
        " . "
        debug_message("resizeEvent()")
        resizeEvent(e)
        statusBar().setSizeGripEnabled(not isMaximized())

    def get_file_separator(self):
        " . "
        debug_message("getFileSeparator()")
        return my_file_separator

    def update_menu_toolbar_statusbar(self):
        " . "
        debug_message("updateMenuToolbarStatusbar()")

        self.action_hash.value("print").set_enabled(num_of_docs > 0)
        self.action_hash.value("window_close").set_enabled(num_of_docs > 0)
        self.action_hash.value("design_details").set_enabled(num_of_docs > 0)

        if num_of_docs:
            # Toolbars
            for key in toolbar.keys():
                toolbar[key].show()

            # DockWidgets
            dock_prop_edit.show()
            dock_undo_edit.show()

            # Menus
            menu_bar().clear()
            menu_bar().add_menu(menu["FILE"])
            menu_bar().add_menu(menu["EDIT"])
            menu_bar().add_menu(menu["VIEW"])

            for menu_ in menuHash:
                menu_bar().add_menu(menu_)

            menu_bar().add_menu(menu["SETTINGS"])
            menu_bar().add_menu(menu["WINDOW"])
            menu_bar().add_menu(menu["HELP"])

            menu["WINDOW"].set_enabled(1)

            # Statusbar
            statusbar.clear_message()
            status_bar_mouse_coord.show()
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
            menu_bar().clear()
            menu_bar().add_menu(menu["FILE"])
            menu_bar().add_menu(menu["EDIT"])
            menu_bar().add_menu(menu["MENU"])
            menu_bar().add_menu(menu["WINDOW"])
            menu_bar().add_menu(menu["HELP"])

            menu["WINDOW"].set_enabled(0)

            # Statusbar
            statusbar.clear_message()
            status_bar_mouse_coord.hide()
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
        supported_readers = "All Supported Files ("
        individual_readers = "All Files (*);;"
        supported_writers = "All Supported Files ("
        individual_writers = "All Files (*);;"
        supported_str = ""
        individual_str = ""

        #TODO: Stable Only (Settings Option)
        # stable = 'S'
        # unstable = 'S'

        #Stable + Unstable
        stable = 'S'
        unstable = 'U'

        curFormat = 0
        for i in range(numberOfFormats):
            extension = format_table[i].extension
            description = format_table[i].description
            readerState = format_table[i].reader_state
            writerState = format_table[i].writer_state

            upper_ext = extension.upper()
            supported_str = "*" + upper_ext + " "
            individual_str = upper_ext.replace(".", "") + " - " + description + " (*" + extension + ");;"
            if(readerState == stable or readerState == unstable):
                #Exclude color file formats from open dialogs
                if(upper_ext != "COL" and upper_ext != "EDR" and upper_ext != "INF" and upper_ext != "RGB"):
                    supported_readers.append(supported_str)
                    individual_readers.append(individual_str)

            if writerState == stable or writerState == unstable:
                supported_writers.append(supported_str)
                individual_writers.append(individual_str)

        supported_readers.append(");;")
        supported_writers.append(");;")

        format_filter_open = supported_readers + individual_readers
        format_filter_save = supported_writers + individual_writers

        # TODO: Fixup custom filter
        custom = settings["custom_filter"]
        if custom.contains("supported", "CaseInsensitive"):
            # self will hide it
            custom = ""
        elif not custom.contains("*", "CaseInsensitive"):
            # self will hide it
            custom = ""
        else:
            custom = "Custom Filter(" + custom + ");;"

        return translate(custom + supported + all)

    def close_toolbar(self, action):
        " . "
        if action.object_name() == "toolbarclose":
            tb = sender()
            if tb:
                debug_message("%s closed.", str(tb.object_name()))
                tb.hide()

    def floating_changed_toolbar(isFloating):
        " . "
        tb = sender()
        if tb:
            if isFloating:
                # TODO: Determine best suited close button on various platforms.
                # Style_SP_DockWidgetCloseButton
                # Style_SP_TitleBarCloseButton
                # Style_SP_DialogCloseButton

                ACTION = Action(tb.style().standard_icon("Style_SP_DialogCloseButton"), "Close", self)
                ACTION.set_status_tip("Close the " + tb.window_title() + " Toolbar")
                ACTION.set_object_name("toolbarclose")
                tb.add_action(ACTION)
                # connect(tb, SIGNAL(actionTriggered()), self, SLOT(close_toolbar()))

            else:
                for action in tb.actions(self):
                    if action.object_name() == "toolbarclose":
                        tb.remove_action(action)
                        #disconnect(tb, SIGNAL(actionTriggered()), self, SLOT(close_toolbar()))
                        del action

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
        debug_message("Save file()")

    def save_as_file(self):
        " Saves an existing file as a new format using the standard dialog. "
        debug_message("Save file as...")
        # need to find the active_sub_window before it loses focus to the FileDialog
        mdi_win = self.active_sub_window
        if not mdi_win:
            return

        openFilesPath = settings["opensave_recent_directory"]
        file = QFileDialog_getSaveFileName(self, translate("Save As"), openFilesPath, format_filter_save)

        mdi_win.save_file(file)

    def exit_program(self):
        r"""
        Instead of closing using exit() this allows us to chain any checks,
        like the confirmation of the close, to the action.
        """
        debug_message("Closing Embroidermodder 2.0.")
        sys.exit()

    def cut_object(self):
        r"""
        Cut acts on objects selected before the action is called.

        If no objects are selected an error is returned.
        """
        debug_message("Cut")

    def copy_object(self):
        r"""
        Copy acts on objects selected before the action is called.

        If no objects are selected an error is returned.
        """
        debug_message("Copy")

    def paste_object(self):
        r"""
        Paste whatever objects are on the clipboard.
        """
        debug_message("Paste")

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
        debug_message("main_redo()")
        #gview = self.active_view
        #if gview:
        #    gview->copy()

        undo_call = ""
        if self.undo_history_position < self.undo_history_length:
            self.undo_history_position += 1
            print("undo_history_position = %d\n", self.undo_history_position)
            print("undo_history_length = %d\n", self.undo_history_length)
            undo_call = self.undo_history[self.undo_history_position]
            # set reverse flag
            undo_call += " -r"
            actuator(undo_call)

    def main_undo(self):
        r"""
        Apply the reverse of the action on the current position
        of the undo_history array.
        """
        debug_message("main_undo()")
        #gview = self.active_view
        #if gview:
        #    gview->paste()
        if self.undo_history_position > 0:
            last = self.undo_history[self.undo_history_position]
            self.undo_history_position -= 1
            print("undo_history_position = %d\n" % self.undo_history_position)
            print("undo_history_length = %d\n" % self.undo_history_length)

            # Create the reverse action from the last action and apply with
            # the main actuator.
            if last[0] == "donothing":
                debug_message("The last action has no undo candidate.")

            self.actuator(last)

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
        r" . "
        debug_message("windowNext()")

    def windowPrevious(self):
        r" . "
        debug_message("windowPrevious()")

    def text_italic(self):
        r" . "
        debug_message("text_italic()")
        settings["text_style_italic"] = not settings["text_style_italic"]

    def textBold(self):
        r" . "
        debug_message("textBold()")
        settings["text_style_bold"] = not settings["text_style_bold"]

    def textStrikeout(self):
        r" . "
        debug_message("textStrikeout()")
        settings["text_style.strikeout"] = not settings["text_style.strikeout"]

    def text_underline(self):
        r" . "
        settings["text_style.underline"] = not settings["text_style.underline"]

    def textOverline(self):
        r" . "
        settings["text_style.overline"] = not settings["text_style.overline"]

    def layer_manager(self):
        r" . "
        debug_message("layerManager()")
        debug_message("Implement layerManager.")
        #LayerManager layman( self,  self)
        #layman.exec()

    def layerPrevious(self):
        r" . "
        debug_message("layerPrevious()")
        debug_message("Implement layerPrevious.")

    def zoom_tealtime(self):
        r" . "
        debug_message("zoomRealtime()")
        debug_message("Implement zoomRealtime.")

    def zoom_previous(self):
        r" . "
        debug_message("zoomPrevious()")
        debug_message("Implement zoomPrevious.")

    def zoom_window(self):
        r" . "
        debug_message("zoom_window()")
        #gview = self.active_view()
        #if gview:
        #    gview->zoom_window()

    def zoom_dynamic(self):
        r" . "
        debug_message("zoomDynamic()")
        debug_message("Implement zoomDynamic.")

    def zoom_scale(self):
        r" . "
        debug_message("zoomScale()")
        debug_message("Implement zoomScale.")

    def zoom_center(self):
        r" . "
        debug_message("zoomCenter()")
        debug_message("Implement zoomCenter.")

    def zoom_in(self):
        r" . "
        debug_message("zoomIn()")

    def zoom_out(self):
        r" . "
        debug_message("zoomOut()")

    def zoom_selected(self):
        r" . "
        debug_message("zoomSelected()")

    def zoom_all(self):
        r" . "
        debug_message("zoomAll()")
        debug_message("Implement zoomAll.")

    def zoom_extents(self):
        r" . "
        debug_message("zoomExtents()")

    def pan_realtime(self):
        r" . "
        debug_message("panrealtime()")

    def pan_point(self):
        r" . "
        debug_message("panpoint()")

    def pan_left(self):
        r" . "
        debug_message("panLeft()")

    def pan_right(self):
        r" . "
        debug_message("panRight()")

    def pan_up(self):
        ""
        debug_message("panUp()")

    def pan_down(self):
        r" . "
        debug_message("panDown()")

    def day_vision(self):
        r""" TODO: Make day vision color settings.
        gview = self->active_view()
        if gview:
            gview->setBackgroundColor("#FFFFFF")
            gview->setCrossHairColor("#000000")
            gview->setGridColor("#000000")
            """
        debug_message("day_vision()")

    def night_vision(self):
        r""" TODO: Make night vision color settings.
        gview = self->active_view()
        if gview:
            gview->setBackgroundColor("#000000")
            gview->setCrossHairColor("#FFFFFF")
            gview->setGridColor("#FFFFFF")
            """
        debug_message("night_vision()")

    def whats_this(self):
        r" . "
        debug_message("whatsselfContextHelp()")

    def makeLayerCurrent(self):
        r" . "
        debug_message("makeLayerCurrent()")

    def layer_selector(self):
        r" . "
        debug_message("main_exit()")

    def main_help(self):
        r" . "
        debug_message("main_help")

    def main_cut(self):
        r" . "
        debug_message("main_cut()")

        gview = self.active_view
        if gview:
            gview.cut()

    def main_copy(self):
        r" . "
        debug_message("main_copy()")

        gview = self.active_view
        if gview:
            gview.copy()

    def main_paste(self):
        r" . "
        debug_message("main_paste()")

        gview = self.active_view
        if gview:
            gview.paste()

    def unlock_all_layers(self):
        r" . "
        debug_message("unlockAllLayers()")

    def hide_all_layers(self):
        r" . "
        debug_message("hideAllLayers()")

    def line_weight_selector(self):
        r" . "
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
        r" . "
        debug_message("StatusBarButton toggleQSnap()")
        settings["snap"] = not settings["snap"]

    def toggle_track(self):
        r" . "
        debug_message("StatusBarButton toggleQTrack()")
        settings["track"] = not settings["track"]

    def toggle_lwt(self):
        r" . "
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

    def build_menu_bar(self):
        r"""
        Build the classic UI dropdown menus using the layout defined
        on file in our 'layout.json'.
        """
        debug_message("build_menu_bar")
        menu_layout = settings["menu_bar"]
        menu_bar = tk.Menu(self.root)
        for menu in menu_layout["order"]:
            debug_message(menu)
            menu_ = tk.Menu(menu_bar, tearoff=0)
            for item in menu_layout[menu]["order"]:
                debug_message(item)
                cmd = menu_layout[menu][item]
                menu_.add_command(
                    label=translate(item),
                    command=lambda: self.actuator(cmd)
                )
            menu_bar.add_cascade(label=translate(menu), menu=menu_)
        self.root.config(menu=menu_bar)

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
                button = tk.Button(
                    self.root,
                    command=lambda: self.actuator(B["command"]),
                    image=settings["tkimg"][button]
                )
                button.grid(row=B["row"], column=B["column"])

    def usage(self):
        r" . "
        print(settings["welcome_message"])
        self.exit_program()

    def application_event(self, event):
        r" . "
        if event.type() == "FileOpen":
            self.open_files_selected(event.file())
            return 1
        # Fall through
        return application_event(event)
