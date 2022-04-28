#!/usr/bin/env python3

r"""
    Embroidermodder 2.

    ------------------------------------------------------------

    Copyright 2013-2022 The Embroidermodder Team
    Embroidermodder 2 is Open Source Software.
    See LICENSE for licensing terms.

    ------------------------------------------------------------

    To make the undo history easier to manage we use a dict for
    keeping all the action information together.

    For more commentary on this file see the Settings Dialog sections of the
    dev_notes.md file.
"""

import tkinter as tk

from embroidermodder.utility import translate, settings


def loadIcon(fname):
    return 0


class SettingsDialog():
    """
    tab_widget* tab_widget
    self.dialogButtonBox* buttonBox
    def addColorsTocombo_box(combo_box)
    create_tab_general()
    createTabFilesPaths()
    createTabDisplay()
    createTabOpenSave()
    createTabPrinting()
    createTabSnap()
    createTabGridRuler()
    createTabOrthoPolar()
    createTabQuickSnap()
    createTabQuickTrack()
    createTabLineWeight()
    createTabSelection()
    def combo_boxLanguageCurrent_index_changed("")
    def combo_boxIconThemeCurrent_index_changed("")
    def combo_box_icon_sizeCurrent_index_changed(int)
    def check_boxGeneralMdiBGUseLogo_state_changed(int)
    def chooseGeneralMdiBackgroundLogo()
    def check_boxGeneralMdiBGUseTexture_state_changed(int)
    def chooseGeneralMdiBackgroundTexture()
    def check_boxGeneralMdiBGUseColor_state_changed(int)
    def chooseGeneralMdiBackgroundColor()
    def currentGeneralMdiBackgroundColorChanged()
    def check_boxShowScrollBars_state_changed(int)
    def spin_boxZoomScaleIn_value_changed(double)
    def spin_boxZoomScaleOut_value_changed(double)
    def check_boxDisableBG_state_changed(int)
    def chooseDisplayCrossHairColor()
    def currentDisplayCrossHairColorChanged()
    def chooseDisplayBackgroundColor()
    def currentDisplayBackgroundColorChanged()
    def chooseDisplaySelectBoxLeftColor()
    def currentDisplaySelectBoxLeftColorChanged()
    def chooseDisplaySelectBoxLeftFill()
    def currentDisplaySelectBoxLeftFillChanged()
    def chooseDisplaySelectBoxRightColor()
    def currentDisplaySelectBoxRightColorChanged()
    def chooseDisplaySelectBoxRightFill()
    def currentDisplaySelectBoxRightFillChanged()
    def spin_boxDisplaySelectBoxAlpha_value_changed(int)
    def check_boxCustomFilter_state_changed(int)
    def buttonCustomFilterSelectAllClicked()
    def buttonCustomFilterClearAllClicked()
    def chooseGridColor()
    def currentGridColorChanged()
    def check_box_grid_load_from_file_state_changed(int)
    def combo_box_grid_typeCurrent_index_changed("")
    def chooseRulerColor()
    def currentRulerColorChanged()
    def buttonQSnapSelectAllClicked()
    def buttonQSnapClearAllClicked()
    def combo_boxQSnapLocatorColorCurrent_index_changed(int)
    def sliderQSnapLocatorSize_value_changed(int)
    def sliderQSnapApertureSize_value_changed(int)
    def check_boxLwtShowLwt_state_changed(int)
    def check_boxLwtRealRender_state_changed(int)
    def combo_boxSelectionCoolGripColorCurrent_index_changed(int)
    def combo_boxSelectionHotGripColorCurrent_index_changed(int)
    def check_box_grid_center_on_origin_state_changed(int)
    def combo_boxRulerMetricCurrent_index_changed(int)
    def check_boxGridColorMatchCrossHair_state_changed(int)
    def buttonCustomFilterSelectAll(int)
    def buttonCustomFilterClearAll(int)
    def buttonQSnapSelectAll(int)
    def buttonQSnapClearAll(int)
    """
    def __init__(self, mw, showTab="General"):
        r"""
        .
        """
        self.mw = mw
        self.accept = settings.copy()
        self.dialog = settings.copy()
        self.preview = settings.copy()

        self.window = tk.Tk()
        self.window.size(750,550)
        tab_widget = tk.tab_widget(self.window)

        #TODO: Add icons to tabs
        tab_widget.add_tab(create_tab_general(), translate("General"))
        tab_widget.add_tab(createTabFilesPaths(), translate("Files/Paths"))
        tab_widget.add_tab(createTabDisplay(), translate("Display"))
        tab_widget.add_tab(createTabOpenSave(), translate("Open/Save"))
        tab_widget.add_tab(createTabPrinting(), translate("Printing"))
        tab_widget.add_tab(createTabSnap(), translate("Snap"))
        tab_widget.add_tab(createTabGridRuler(), translate("Grid/Ruler"))
        tab_widget.add_tab(createTabOrthoPolar(), translate("Ortho/Polar"))
        tab_widget.add_tab(createTabQuickSnap(), translate("QuickSnap"))
        tab_widget.add_tab(createTabQuickTrack(), translate("QuickTrack"))
        tab_widget.add_tab(createTabLineWeight(), translate("LineWeight"))
        tab_widget.add_tab(createTabSelection(), translate("Selection"))

        for i in range(12):
            if showTab == settings_tab_label[i]:
                tab_widget.setCurrentIndex(i)

        buttonBox = tk.DialogButtonBox(QDialogButtonBox_Ok | tk.DialogButtonBox_Cancel)

        #connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()))
        #connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()))

        vbox_layout_main = tk.VBoxLayout()
        vbox_layout_main.add_widget(tab_widget)
        vbox_layout_main.add_widget(buttonBox)
        setLayout(vbox_layout_main)

        self.window.title(translate("Settings"))
        self.window.setOverrideCursor("ArrowCursor")

    def create_tab_general(self):
        " . "
        widget = tk.Widget(self.window)

        #Language
        groupBoxLanguage = tk.GroupBox(translate("Language"), widget)

        labelLanguage = tk.Label(translate("Language (Requires Restart)"), groupBoxLanguage)
        combo_boxLanguage = tk.combo_box(groupBoxLanguage)
        to_lower(self.dialog.general_language, settings.general_language)
        combo_boxLanguage.add_item("Default")
        combo_boxLanguage.add_item("System")
        combo_boxLanguage.insertSeparator(2)
        trDir = tk.App.applicationDirPath()
        trDir.cd("translations")
        for dirName in trDir.entryList(QDir_Dirs | tk.Dir_NoDotAndDotDot):
            dirName[0] = dirName[0].toUpper()
            combo_boxLanguage.add_item(dirName)

        current = self.dialog.general_language
        current[0] = current[0].toUpper()
        combo_boxLanguage.setCurrentIndex(combo_boxLanguage.findText(current))
        #connect(combo_boxLanguage, SIGNAL(current_index_changed("")), this, SLOT(combo_boxLanguageCurrent_index_changed("")))

        vboxLayoutLanguage = tk.VBoxLayout(groupBoxLanguage)
        vboxLayoutLanguage.add_widget(labelLanguage)
        vboxLayoutLanguage.add_widget(combo_boxLanguage)
        groupBoxLanguage.setLayout(vboxLayoutLanguage)

        #Icons
        groupBoxIcon = tk.GroupBox(translate("Icons"), widget)

        labelIconTheme = tk.Label(translate("Icon Theme"), groupBoxIcon)
        combo_boxIconTheme = tk.combo_box(groupBoxIcon)
        dir = tk.App.applicationDirPath()
        dir.cd("icons")
        self.dialog.general_icon_theme = settings.general_icon_theme
        for dirName in dir.entryList(QDir_Dirs | tk.Dir_NoDotAndDotDot):
            combo_boxIconTheme.add_item(loadIcon(theme_xpm), dirName)

        combo_boxIconTheme.setCurrentIndex(combo_boxIconTheme.findText(self.dialog.general_icon_theme))
        #connect(combo_boxIconTheme, SIGNAL(current_index_changed("")), this, SLOT(combo_boxIconThemeCurrent_index_changed("")))

        labelIconSize = tk.Label(translate("Icon Size"), groupBoxIcon)
        combo_box_icon_size = tk.combo_box(groupBoxIcon)
        combo_box_icon_size.add_item(loadIcon("icon16_xpm"), "Very Small", 16)
        combo_box_icon_size.add_item(loadIcon("icon24_xpm"), "Small", 24)
        combo_box_icon_size.add_item(loadIcon("icon32_xpm"), "Medium", 32)
        combo_box_icon_size.add_item(loadIcon("icon48_xpm"), "Large", 48)
        combo_box_icon_size.add_item(loadIcon("icon64_xpm"), "Very Large", 64)
        combo_box_icon_size.add_item(loadIcon("icon128_xpm"), "I'm Blind", 128)
        self.dialog["general_icon_size"] = settings["general_icon_size"]
        combo_box_icon_size.setCurrentIndex(combo_box_icon_size.findData(self.dialog.general_icon_size))
        #connect(combo_box_icon_size, SIGNAL(current_index_changed(int)), this, SLOT(combo_box_icon_sizeCurrent_index_changed(int)))

        vbox_layout_icon = tk.QVBoxLayout(groupBoxIcon)
        vbox_layout_icon.add_widget(labelIconTheme)
        vbox_layout_icon.add_widget(combo_boxIconTheme)
        vbox_layout_icon.add_widget(labelIconSize)
        vbox_layout_icon.add_widget(combo_box_icon_size)
        groupBoxIcon.setLayout(vbox_layout_icon)

        #Mdi Background
        groupBoxMdiBG = tk.GroupBox(self.window, text=translate("Background"))

        check_box_mdi_bg_use_logo = tk.check_box(groupBoxMdiBG, text=translate("Use Logo"))
        self.dialog["general_mdi_bg_use_logo"] = settings["general_mdi_bg_use_logo"]
        self.preview["general_mdi_bg_use_logo"] = self.dialog["general_mdi_bg_use_logo"]
        check_box_mdi_bg_use_logo.setChecked(self.preview["general_mdi_bg_use_logo"])
        #connect(check_box_mdi_bg_use_logo, SIGNAL(_state_changed(int)), this, SLOT(check_boxGeneralMdiBGUseLogo_state_changed(int)))

        button_mdi_bg_logo = tk.Button(groupBoxMdiBG, text=translate("Choose"))
        button_mdi_bg_logo.set_enabled(self.dialog["general_mdi_bg_use_logo"])
        self.dialog["general_mdi_bg_logo"] = settings["general_mdi_bg_logo"]
        self.accept["general_mdi_bg_logo"] = self.dialog["general_mdi_bg_logo"]
        #connect(button_mdi_bg_logo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()))
        #connect(check_box_mdi_bg_use_logo, SIGNAL(toggled(int)), button_mdi_bg_logo, SLOT(set_enabled(int)))

        check_box_mdi_bg_use_texture = tk.check_box(translate("Use Texture"), groupBoxMdiBG)
        self.dialog["general_mdi_bg_use_texture"] = settings["general_mdi_bg_use_texture"]
        self.preview["general_mdi_bg_use_texture"] = self.dialog["general_mdi_bg_use_texture"]
        check_box_mdi_bg_use_texture.setChecked(self.preview["general_mdi_bg_use_texture"])
        #connect(check_box_mdi_bg_use_texture, SIGNAL(_state_changed(int)), this, SLOT(check_boxGeneralMdiBGUseTexture_state_changed(int)))

        buttonMdiBGTexture = tk.Button(groupBoxMdiBG, text=translate("Choose"))
        buttonMdiBGTexture.set_enabled(self.dialog["general_mdi_bg_use_texture"])
        self.dialog["general_mdi_bg_texture"] = settings["general_mdi_bg_texture"]
        self.accept["general_mdi_bg_texture"] = self.dialog["general_mdi_bg_texture"]
        #connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()))
        #connect(check_box_mdi_bg_use_texture, SIGNAL(toggled(int)), buttonMdiBGTexture, SLOT(set_enabled(int)))

        check_boxMdiBGUseColor = tk.check_box(translate("Use Color"), groupBoxMdiBG)
        self.dialog["general_mdi_bg_use_color"] = settings["general_mdi_bg_use_color"]
        self.preview["general_mdi_bg_use_color"] = self.dialog["general_mdi_bg_use_color"]
        check_boxMdiBGUseColor.setChecked(preview.general_mdi_bg_use_color)
        #connect(check_boxMdiBGUseColor, SIGNAL(_state_changed(int)), this, SLOT(check_boxGeneralMdiBGUseColor_state_changed(int)))

        buttonMdiBGColor = PushButton(translate("Choose"), groupBoxMdiBG)
        buttonMdiBGColor.set_enabled(self.dialog.general_mdi_bg_use_color)
        self.dialog["general_mdi_bg_color"] = settings["general_mdi_bg_color"]
        self.preview["general_mdi_bg_color"] = self.dialog["general_mdi_bg_color"]
        self.accept["general_mdi_bg_color"] = self.dialog["general_mdi_bg_color"]
        mdiBGPix = Image(16,16)
        mdiBGPix.fill(Color(preview.general_mdi_bg_color))
        buttonMdiBGColor.setIcon(QIcon(mdiBGPix))
        #connect(buttonMdiBGColor, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundColor()))
        #connect(check_boxMdiBGUseColor, SIGNAL(toggled(int)), buttonMdiBGColor, SLOT(set_enabled(int)))

        gridLayoutMdiBG = tk.GridLayout(widget)
        gridLayoutMdiBG.add_widget(check_box_mdi_bg_use_logo, 0, 0, "Align Left")
        gridLayoutMdiBG.add_widget(button_mdi_bg_logo, 0, 1, "Align Right")
        gridLayoutMdiBG.add_widget(check_box_mdi_bg_use_texture, 1, 0, "Align Left")
        gridLayoutMdiBG.add_widget(buttonMdiBGTexture, 1, 1, "Align Right")
        gridLayoutMdiBG.add_widget(check_boxMdiBGUseColor, 2, 0, "Align Left")
        gridLayoutMdiBG.add_widget(buttonMdiBGColor, 2, 1, "Align Right")
        groupBoxMdiBG.setLayout(gridLayoutMdiBG)

        #Tips
        groupBoxTips = tk.GroupBox(translate("Tips"), widget)

        check_boxTipOfTheDay = tk.check_box(translate("Show Tip of the Day on startup"), groupBoxTips)
        self.dialog.general_tip_of_the_day = settings.general_tip_of_the_day
        check_boxTipOfTheDay.setChecked(self.dialog.general_tip_of_the_day)
        #connect(check_boxTipOfTheDay, SIGNAL(_state_changed(int)), this, SLOT(check_boxTipOfTheDay_state_changed(int)))

        vboxLayoutTips = tk.VBoxLayout(groupBoxTips)
        vboxLayoutTips.add_widget(check_boxTipOfTheDay)
        groupBoxTips.setLayout(vboxLayoutTips)

        #Help Browser
        groupBoxHelpBrowser = tk.GroupBox(translate("Help Browser"), widget)

        radio_button_system_help_browser = tk.RadioButton(translate("System"), groupBoxHelpBrowser)
        radio_button_system_help_browser.setChecked(settings.general_system_help_browser)
        radio_button_custom_help_browser = tk.RadioButton(translate("Custom"), groupBoxHelpBrowser)
        radio_button_custom_help_browser.setChecked(not settings.general_system_help_browser)
        radio_button_custom_help_browser.set_enabled(0); #TODO: finish this

        vboxLayoutHelpBrowser = tk.VBoxLayout(groupBoxHelpBrowser)
        vboxLayoutHelpBrowser.add_widget(radio_button_system_help_browser)
        vboxLayoutHelpBrowser.add_widget(radio_button_custom_help_browser)
        groupBoxHelpBrowser.setLayout(vboxLayoutHelpBrowser)

        #Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        vbox_layout_main.add_widget(groupBoxLanguage)
        vbox_layout_main.add_widget(groupBoxIcon)
        vbox_layout_main.add_widget(groupBoxMdiBG)
        vbox_layout_main.add_widget(groupBoxTips)
        vbox_layout_main.add_widget(groupBoxHelpBrowser)
        vbox_layout_main.addStretch(1)
        widget.setLayout(vbox_layout_main)

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def createTabFilesPaths(self):
        widget = tk.Widget(this)

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def createTabDisplay(self):
        " . "
        widget = tk.Widget(this)

        #Rendering
        #TODO: Review OpenGL and Rendering settings for future inclusion
        #
        groupBoxRender = tk.GroupBox(translate("Rendering"), widget)

        check_boxUseOpenGL = tk.check_box(translate("Use OpenGL"), groupBoxRender)
        self.dialog.display_use_opengl = settings.display_use_open_gl
        check_boxUseOpenGL.setChecked(self.dialog.display_use_opengl)
        #connect(check_boxUseOpenGL, SIGNAL(_state_changed(int)), this, SLOT(check_boxUseOpenGL_state_changed(int)))

        check_boxRenderHintAA = tk.check_box(translate("Antialias"), groupBoxRender)
        self.dialog.display_renderhint_aa = settings.display_render_hint_aa
        check_boxRenderHintAA.setChecked(self.dialog.display_renderhint_aa)
        #connect(check_boxRenderHintAA, SIGNAL(_state_changed(int)), this, SLOT(check_boxRenderHintAA_state_changed(int)))

        check_boxRenderHintTextAA = tk.check_box(translate("Antialias Text"), groupBoxRender)
        self.dialog["display_renderhint_text_aa"] = settings["display_render_hint_text_aa"]
        check_boxRenderHintTextAA.setChecked(self.dialog["display_renderhint_text_aa"])
        #connect(check_boxRenderHintTextAA, SIGNAL(_state_changed(int)), this, SLOT(check_boxRenderHintTextAA_state_changed(int)))

        check_boxRenderHintSmoothPix = tk.check_box(translate("Smooth Pixmap"), groupBoxRender)
        self.dialog["display_renderhint_smooth_pix"] = settings["display_render_hint_smooth_pix"]
        check_boxRenderHintSmoothPix.setChecked(self.dialog.display_renderhint_smooth_pix)
        #connect(check_boxRenderHintSmoothPix, SIGNAL(_state_changed(int)), this, SLOT(check_boxRenderHintSmoothPix_state_changed(int)))

        check_boxRenderHintHighAA = tk.check_box(translate("High Quality Antialiasing (OpenGL)"), groupBoxRender)
        self.dialog["display_renderhint_high_aa"] = settings["display_render_hint_high_aa"]
        check_boxRenderHintHighAA.setChecked(self.dialog["display_renderhint_high_aa"])
        #connect(check_boxRenderHintHighAA, SIGNAL(_state_changed(int)), this, SLOT(check_boxRenderHintHighAA_state_changed(int)))

        check_boxRenderHintNonCosmetic = tk.check_box(translate("Non Cosmetic"), groupBoxRender)
        self.dialog.display_renderhint_noncosmetic = settings.display_render_hint_non_cosmetic
        check_boxRenderHintNonCosmetic.setChecked(self.dialog.display_renderhint_noncosmetic)
        #connect(check_boxRenderHintNonCosmetic, SIGNAL(_state_changed(int)), this, SLOT(check_boxRenderHintNonCosmetic_state_changed(int)))

        vboxLayoutRender = tk.VBoxLayout(groupBoxRender)
        vboxLayoutRender.add_widget(check_boxUseOpenGL)
        vboxLayoutRender.add_widget(check_boxRenderHintAA)
        vboxLayoutRender.add_widget(check_boxRenderHintTextAA)
        vboxLayoutRender.add_widget(check_boxRenderHintSmoothPix)
        vboxLayoutRender.add_widget(check_boxRenderHintHighAA)
        vboxLayoutRender.add_widget(check_boxRenderHintNonCosmetic)
        groupBoxRender.setLayout(vboxLayoutRender)

        #ScrollBars
        groupBoxScrollBars = tk.GroupBox(translate("ScrollBars"), widget)

        check_boxShowScrollBars = tk.check_box(translate("Show ScrollBars"), groupBoxScrollBars)
        self.dialog.display_show_scrollbars = settings.display_show_scrollbars
        self.preview.display_show_scrollbars = self.dialog.display_show_scrollbars
        check_boxShowScrollBars.setChecked(preview.display_show_scrollbars)
        #connect(check_boxShowScrollBars, SIGNAL(_state_changed(int)), this, SLOT(check_boxShowScrollBars_state_changed(int)))

        labelScrollBarWidget = tk.Label(translate("Perform action when clicking corner widget"), groupBoxScrollBars)
        combo_boxScrollBarWidget = tk.combo_box(groupBoxScrollBars)
        self.dialog.display_scrollbar_widget_num = settings.display_scrollbar_widget_num
        numActions = self.mw.actionHash.size()
        for i in range(numActions):
            action = self.mw.actionHash.value(i)
            if action:
                combo_boxScrollBarWidget.add_item(action.icon(), action.text().replace("&", ""))

        combo_boxScrollBarWidget.setCurrentIndex(self.dialog.display_scrollbar_widget_num)
        #connect(combo_boxScrollBarWidget, SIGNAL(current_index_changed(int)), this, SLOT(combo_boxScrollBarWidgetCurrent_index_changed(int)))

        vboxLayoutScrollBars = tk.VBoxLayout(groupBoxScrollBars)
        vboxLayoutScrollBars.add_widget(check_boxShowScrollBars)
        vboxLayoutScrollBars.add_widget(labelScrollBarWidget)
        vboxLayoutScrollBars.add_widget(combo_boxScrollBarWidget)
        groupBoxScrollBars.setLayout(vboxLayoutScrollBars)

        #Colors
        groupBoxColor = tk.GroupBox(translate("Colors"), widget)

        labelCrossHairColor = tk.Label(translate("Crosshair Color"), groupBoxColor)
        buttonCrossHairColor = PushButton(translate("Choose"), groupBoxColor)
        self.dialog.display_crosshair_color = settings.display_crosshair_color
        self.preview.display_crosshair_color = self.dialog.display_crosshair_color
        self.accept.display_crosshair_color = self.dialog.display_crosshair_color
        crosshairPix = (16,16)
        crosshairPix.fill(Color(preview.display_crosshair_color))
        buttonCrossHairColor.setIcon(QIcon(crosshairPix))
        #connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()))

        labelBGColor = tk.Label(translate("Background Color"), groupBoxColor)
        buttonBGColor = PushButton(translate("Choose"), groupBoxColor)
        self.dialog.display_bg_color = settings.display_bg_color
        self.preview.display_bg_color = self.dialog.display_bg_color
        self.accept.display_bg_color = self.dialog.display_bg_color
        bgPix = Image(16,16)
        bgPix.fill(Color(preview.display_bg_color))
        buttonBGColor.setIcon(QIcon(bgPix))
        #connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()))

        labelSelectBoxLeftColor = tk.Label(translate("Selection Box Color (Crossing)"), groupBoxColor)
        buttonSelectBoxLeftColor = PushButton(translate("Choose"), groupBoxColor)
        self.dialog["display_selectbox_left_color"] = settings["display_selectbox_left_color"]
        self.preview["display_selectbox_left_color"] = self.dialog["display_selectbox_left_color"]
        self.accept["display_selectbox_left_color"] = self.dialog["display_selectbox_left_color"]
        sBoxLCPix = Image(16,16)
        sBoxLCPix.fill(Color(preview["display_selectbox_left_color"]))
        buttonSelectBoxLeftColor.setIcon(QIcon(sBoxLCPix))
        #connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()))

        labelSelectBoxLeftFill = tk.Label(translate("Selection Box Fill (Crossing)"), groupBoxColor)
        buttonSelectBoxLeftFill = PushButton(translate("Choose"), groupBoxColor)
        self.dialog["display_selectbox_left_fill"] = settings["display_selectbox_left_fill"]
        self.preview["display_selectbox_left_fill"] = self.dialog["display_selectbox_left_fill"]
        self.accept["display_selectbox_left_fill"] = self.dialog["display_selectbox_left_fill"]
        sBoxLFPix = Image(16,16)
        sBoxLFPix.fill(Color(preview["display_selectbox_left_fill"]))
        buttonSelectBoxLeftFill.setIcon(QIcon(sBoxLFPix))
        #connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()))

        labelSelectBoxRightColor = tk.Label(translate("Selection Box Color (Window)"), groupBoxColor)
        buttonSelectBoxRightColor = tk.PushButton(translate("Choose"), groupBoxColor)
        self.dialog["display_selectbox_right_color"] = settings["display_selectbox_right_color"]
        self.preview["display_selectbox_right_color"] = self.dialog["display_selectbox_right_color"]
        self.accept["display_selectbox_right_color"] = self.dialog["display_selectbox_right_color"]
        sBoxRCPix = Image(16,16)
        sBoxRCPix.fill(Color(preview["display_selectbox_right_color"]))
        buttonSelectBoxRightColor.setIcon(QIcon(sBoxRCPix))
        #connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()))

        labelSelectBoxRightFill = tk.Label(translate("Selection Box Fill (Window)"), groupBoxColor)
        buttonSelectBoxRightFill = PushButton(translate("Choose"), groupBoxColor)
        self.dialog.display_selectbox_right_fill = settings.display_selectbox_right_fill
        self.preview.display_selectbox_right_fill = self.dialog.display_selectbox_right_fill
        self.accept.display_selectbox_right_fill = self.dialog.display_selectbox_right_fill
        sBoxRFPix = Image(16,16)
        sBoxRFPix.fill(Color(preview.display_selectbox_right_fill))
        buttonSelectBoxRightFill.setIcon(QIcon(sBoxRFPix))
        #connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()))

        labelSelectBoxAlpha = tk.Label(translate("Selection Box Fill Alpha"), groupBoxColor)
        spin_boxSelectBoxAlpha = tk.spin_box(groupBoxColor)
        spin_boxSelectBoxAlpha.setRange(0, 255)
        self.dialog.display_selectbox_alpha = settings.display_selectbox_alpha
        self.preview.display_selectbox_alpha = self.dialog.display_selectbox_alpha
        spin_boxSelectBoxAlpha.setValue(preview.display_selectbox_alpha)
        #connect(spin_boxSelectBoxAlpha, SIGNAL(_value_changed(int)), this, SLOT(spin_boxDisplaySelectBoxAlpha_value_changed(int)))

        gridLayoutColor = tk.GridLayout(widget)
        gridLayoutColor.add_widget(labelCrossHairColor, 0, 0, "Align Left")
        gridLayoutColor.add_widget(buttonCrossHairColor, 0, 1, "Align Right")
        gridLayoutColor.add_widget(labelBGColor, 1, 0, "Align Left")
        gridLayoutColor.add_widget(buttonBGColor, 1, 1, "Align Right")
        gridLayoutColor.add_widget(labelSelectBoxLeftColor, 2, 0, "Align Left")
        gridLayoutColor.add_widget(buttonSelectBoxLeftColor, 2, 1, "Align Right")
        gridLayoutColor.add_widget(labelSelectBoxLeftFill, 3, 0, "Align Left")
        gridLayoutColor.add_widget(buttonSelectBoxLeftFill, 3, 1, "Align Right")
        gridLayoutColor.add_widget(labelSelectBoxRightColor, 4, 0, "Align Left")
        gridLayoutColor.add_widget(buttonSelectBoxRightColor, 4, 1, "Align Right")
        gridLayoutColor.add_widget(labelSelectBoxRightFill, 5, 0, "Align Left")
        gridLayoutColor.add_widget(buttonSelectBoxRightFill, 5, 1, "Align Right")
        gridLayoutColor.add_widget(labelSelectBoxAlpha, 6, 0, "Align Left")
        gridLayoutColor.add_widget(spin_boxSelectBoxAlpha, 6, 1, "Align Right")
        groupBoxColor.setLayout(gridLayoutColor)

        #Zoom
        groupBoxZoom = tk.GroupBox(translate("Zoom"), widget)

        labelZoomScaleIn = tk.Label(translate("Zoom In Scale"), groupBoxZoom)
        spin_boxZoomScaleIn = tk.Doublespin_box(groupBoxZoom)
        self.dialog.display_zoomscale_in = settings.display_zoomscale_in
        spin_boxZoomScaleIn.setValue(self.dialog.display_zoomscale_in)
        spin_boxZoomScaleIn.setSingleStep(0.01)
        spin_boxZoomScaleIn.setRange(1.01, 10.00)
        #connect(spin_boxZoomScaleIn, SIGNAL(_value_changed(double)), this, SLOT(spin_boxZoomScaleIn_value_changed(double)))

        labelZoomScaleOut = tk.Label(translate("Zoom Out Scale"), groupBoxZoom)
        spin_boxZoomScaleOut = tk.Doublespin_box(groupBoxZoom)
        self.dialog.display_zoomscale_out = settings.display_zoomscale_out
        spin_boxZoomScaleOut.setValue(self.dialog.display_zoomscale_out)
        spin_boxZoomScaleOut.setSingleStep(0.01)
        spin_boxZoomScaleOut.setRange(0.01, 0.99)
        #connect(spin_boxZoomScaleOut, SIGNAL(_value_changed(double)), this, SLOT(spin_boxZoomScaleOut_value_changed(double)))

        gridLayoutZoom = tk.GridLayout(groupBoxZoom)
        gridLayoutZoom.add_widget(labelZoomScaleIn, 0, 0, "Align Left")
        gridLayoutZoom.add_widget(spin_boxZoomScaleIn, 0, 1, "Align Right")
        gridLayoutZoom.add_widget(labelZoomScaleOut, 1, 0, "Align Left")
        gridLayoutZoom.add_widget(spin_boxZoomScaleOut, 1, 1, "Align Right")
        groupBoxZoom.setLayout(gridLayoutZoom)

        #Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        #vbox_layout_main.add_widget(groupBoxRender)
        # TODO: Review OpenGL and Rendering settings for future inclusion
        vbox_layout_main.add_widget(groupBoxScrollBars)
        vbox_layout_main.add_widget(groupBoxColor)
        vbox_layout_main.add_widget(groupBoxZoom)
        vbox_layout_main.addStretch(1)
        widget.setLayout(vbox_layout_main)

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def createTabOpenSave(self):
        " . "
        " TODO: finish open/save options"
        widget = tk.Widget(this)

        #Custom Filter
        groupBoxCustomFilter = tk.GroupBox(translate("Custom Filter"), widget)
        groupBoxCustomFilter.set_enabled(0); #TODO: Fixup custom filter

        buttonCustomFilterSelectAll = PushButton(translate("Select All"), groupBoxCustomFilter)
        #connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()))
        buttonCustomFilterClearAll = PushButton("Clear All", groupBoxCustomFilter)
        #connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()))
        gridLayoutCustomFilter = tk.GridLayout(groupBoxCustomFilter)

        for i in range(numberOfFormats):
            c = check_box(formatTable[i].extension, groupBoxCustomFilter)
            c.setChecked(opensave_custom_filter.contains(QString("*") + formatTable[i].extension, tk.t_CaseInsensitive))
            #connect(c, SIGNAL(_state_changed(int)), this, SLOT(check_boxCustomFilter_state_changed(int)))
            #connect(this, SIGNAL(buttonCustomFilterSelectAll(int)), c, SLOT(setChecked(int)))
            #connect(this, SIGNAL(buttonCustomFilterClearAll(int)), c, SLOT(setChecked(int)))
            gridLayoutCustomFilter.add_widget(c, i%10, i/10, "Align Left")

        gridLayoutCustomFilter.add_widget(buttonCustomFilterSelectAll, 0, 7, "Align Left")
        gridLayoutCustomFilter.add_widget(buttonCustomFilterClearAll, 1, 7, "Align Left")
        gridLayoutCustomFilter.setColumnStretch(7,1)
        groupBoxCustomFilter.setLayout(gridLayoutCustomFilter)

        if opensave_custom_filter.contains("supported", tk.t_CaseInsensitive):
            buttonCustomFilterSelectAllClicked()

        # Opening
        groupBoxOpening = tk.GroupBox(translate("File Open"), widget)

        combo_boxOpenFormat = tk.combo_box(groupBoxOpening)

        check_boxOpenThumbnail = tk.check_box(translate("Preview Thumbnails"), groupBoxOpening)
        check_boxOpenThumbnail.setChecked(0)

        # TODO: Add a button to clear the recent history.

        labelRecentMaxFiles = tk.Label(translate("Number of recently accessed files to show"), groupBoxOpening)
        spin_boxRecentMaxFiles = tk.spin_box(groupBoxOpening)
        spin_boxRecentMaxFiles.setRange(0, 10)
        self.dialog.opensave_recent_max_files = settings.opensave_recent_max_files
        spin_boxRecentMaxFiles.setValue(self.dialog.opensave_recent_max_files)
        #connect(spin_boxRecentMaxFiles, SIGNAL(_value_changed(int)), this, SLOT(spin_boxRecentMaxFiles_value_changed(int)))

        frameRecent = tk.Frame(groupBoxOpening)
        gridLayoutRecent = tk.GridLayout(frameRecent)
        gridLayoutRecent.add_widget(labelRecentMaxFiles, 0, 0, "AlignLeft")
        gridLayoutRecent.add_widget(spin_boxRecentMaxFiles, 0, 1, "Align Right")
        frameRecent.setLayout(gridLayoutRecent)

        vboxLayoutOpening = tk.VBoxLayout(groupBoxOpening)
        vboxLayoutOpening.add_widget(combo_boxOpenFormat)
        vboxLayoutOpening.add_widget(check_boxOpenThumbnail)
        vboxLayoutOpening.add_widget(frameRecent)
        groupBoxOpening.setLayout(vboxLayoutOpening)

        #Saving
        groupBoxSaving = tk.GroupBox(translate("File Save"), widget)

        combo_boxSaveFormat = tk.combo_box(groupBoxSaving)

        check_boxSaveThumbnail = tk.check_box(translate("Save Thumbnails"), groupBoxSaving)
        check_boxSaveThumbnail.setChecked(0)

        check_boxAutoSave = tk.check_box(translate("AutoSave"), groupBoxSaving)
        check_boxAutoSave.setChecked(0)

        vboxLayoutSaving = tk.VBoxLayout(groupBoxSaving)
        vboxLayoutSaving.add_widget(combo_boxSaveFormat)
        vboxLayoutSaving.add_widget(check_boxSaveThumbnail)
        vboxLayoutSaving.add_widget(check_boxAutoSave)
        groupBoxSaving.setLayout(vboxLayoutSaving)

        #Trimming
        groupBoxTrim = tk.GroupBox(translate("Trimming"), widget)

        labelTrimDstNumJumps = tk.Label(translate("DST Only: Minimum number of jumps to trim"), groupBoxTrim)
        spin_boxTrimDstNumJumps = tk.spin_box(groupBoxTrim)
        spin_boxTrimDstNumJumps.setRange(1, 20)
        self.dialog.opensave_trim_dst_num_jumps = settings.opensave_trim_dst_num_jumps
        spin_boxTrimDstNumJumps.setValue(self.dialog.opensave_trim_dst_num_jumps)
        #connect(spin_boxTrimDstNumJumps, SIGNAL(_value_changed(int)), this, SLOT(spin_boxTrimDstNumJumps_value_changed(int)))

        frameTrimDstNumJumps = tk.Frame(groupBoxTrim)
        gridLayoutTrimDstNumJumps = tk.GridLayout(frameTrimDstNumJumps)
        gridLayoutTrimDstNumJumps.add_widget(labelTrimDstNumJumps, 0, 0, "Align Left")
        gridLayoutTrimDstNumJumps.add_widget(spin_boxTrimDstNumJumps, 0, 1, "Align Right")
        frameTrimDstNumJumps.setLayout(gridLayoutTrimDstNumJumps)

        vboxLayoutTrim = tk.VBoxLayout(groupBoxTrim)
        vboxLayoutTrim.add_widget(frameTrimDstNumJumps)
        groupBoxTrim.setLayout(vboxLayoutTrim)

        #Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        vbox_layout_main.add_widget(groupBoxCustomFilter)
        vbox_layout_main.add_widget(groupBoxOpening)
        vbox_layout_main.add_widget(groupBoxSaving)
        vbox_layout_main.add_widget(groupBoxTrim)
        vbox_layout_main.addStretch(1)
        widget.setLayout(vbox_layout_main)

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def createTabPrinting(self):
        " . "
        widget = tk.Widget(this)

        # Default Printer
        groupBoxDefaultPrinter = tk.GroupBox(translate("Default Printer"), widget)

        radioButtonUseSame = tk.RadioButton(translate("Use as default device"), groupBoxDefaultPrinter)
        radioButtonUseSame.setChecked(not settings.printing_use_last_device)
        radioButtonUseLast = tk.RadioButton(translate("Use last used device"), groupBoxDefaultPrinter)
        radioButtonUseLast.setChecked(settings.printing_use_last_device)

        combo_boxDefaultDevice = tk.combo_box(groupBoxDefaultPrinter)
        listAvailPrinters = tk.PrinterInfo_availablePrinters()
        for info in listAvailPrinters:
            combo_boxDefaultDevice.add_item(loadIcon(print_xpm), info.printerName())

        vboxLayoutDefaultPrinter = tk.VBoxLayout(groupBoxDefaultPrinter)
        vboxLayoutDefaultPrinter.add_widget(radioButtonUseSame)
        vboxLayoutDefaultPrinter.add_widget(combo_boxDefaultDevice)
        vboxLayoutDefaultPrinter.add_widget(radioButtonUseLast)
        groupBoxDefaultPrinter.setLayout(vboxLayoutDefaultPrinter)

        # Save Ink
        groupBoxSaveInk = tk.GroupBox(translate("Save Ink"), widget)

        check_boxDisableBG = tk.check_box(translate("Disable Background"), groupBoxSaveInk)
        self.dialog.printing_disable_bg = settings.printing_disable_bg
        check_boxDisableBG.setChecked(self.dialog.printing_disable_bg)
        #connect(check_boxDisableBG, SIGNAL(_state_changed(int)), this, SLOT(check_boxDisableBG_state_changed(int)))

        vboxLayoutSaveInk = tk.VBoxLayout(groupBoxSaveInk)
        vboxLayoutSaveInk.add_widget(check_boxDisableBG)
        groupBoxSaveInk.setLayout(vboxLayoutSaveInk)

        #Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        vbox_layout_main.add_widget(groupBoxDefaultPrinter)
        vbox_layout_main.add_widget(groupBoxSaveInk)
        vbox_layout_main.addStretch(1)
        widget.setLayout(vbox_layout_main)


        scroll_area = tk.scroll_area(this)
        #scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget);
        return scroll_area

    def createTabSnap():
        widget = tk.Widget(this)

        #TODO: finish this

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def createTabGridRuler():
        widget = tk.Widget(this)

        #Grid Misc
        groupBoxGridMisc = tk.GroupBox(translate("Grid Misc"), widget)

        check_boxGridShowOnLoad = tk.check_box(translate("Initially show grid when loading a file"), groupBoxGridMisc)
        self.dialog.grid_show_on_load = settings.grid_show_on_load
        check_boxGridShowOnLoad.setChecked(self.dialog.grid_show_on_load)
        #connect(check_boxGridShowOnLoad, SIGNAL(_state_changed(int)), this, SLOT(check_boxGridShowOnLoad_state_changed(int)))

        check_boxGridShowOrigin = tk.check_box(translate("Show the origin when the grid is enabled"), groupBoxGridMisc)
        self.dialog.grid_show_origin = settings.grid_show_origin
        check_boxGridShowOrigin.setChecked(self.dialog.grid_show_origin)
        #connect(check_boxGridShowOrigin, SIGNAL(_state_changed(int)), this, SLOT(check_boxGridShowOrigin_state_changed(int)))

        gridLayoutGridMisc = tk.GridLayout(widget)
        gridLayoutGridMisc.add_widget(check_boxGridShowOnLoad, 0, 0, "Align Left")
        gridLayoutGridMisc.add_widget(check_boxGridShowOrigin, 1, 0, "Align Left")
        groupBoxGridMisc.setLayout(gridLayoutGridMisc)

        #Grid Color
        groupBoxGridColor = tk.GroupBox(translate("Grid Color"), widget)

        check_boxGridColorMatchCrossHair = tk.check_box(translate("Match grid color to crosshair color"), groupBoxGridColor)
        self.dialog.grid_color_match_crosshair = settings.grid_color_match_crosshair
        check_boxGridColorMatchCrossHair.setChecked(self.dialog.grid_color_match_crosshair)
        #connect(check_boxGridColorMatchCrossHair, SIGNAL(_state_changed(int)), this, SLOT(check_boxGridColorMatchCrossHair_state_changed(int)))

        labelGridColor = tk.Label(translate("Grid Color"), groupBoxGridColor)
        labelGridColor.setObjectName("labelGridColor")
        buttonGridColor = PushButton(translate("Choose"), groupBoxGridColor)
        buttonGridColor.setObjectName("buttonGridColor")
        if self.dialog.grid_color_match_crosshair:
            self.dialog.grid_color = settings.display_crosshair_color
        else:
            self.dialog.grid_color = settings.grid_color
        self.preview.grid_color = self.dialog.grid_color
        self.accept.grid_color = self.dialog.grid_color
        gridPix = Image(16,16)
        gridPix.fill(Color(preview.grid_color))
        buttonGridColor.setIcon(QIcon(gridPix))
        #connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()))

        labelGridColor.set_enabled(not self.dialog.grid_color_match_crosshair)
        buttonGridColor.set_enabled(not self.dialog.grid_color_match_crosshair)

        gridLayoutGridColor = tk.GridLayout(widget)
        gridLayoutGridColor.add_widget(check_boxGridColorMatchCrossHair, 0, 0, "Align Left")
        gridLayoutGridColor.add_widget(labelGridColor, 1, 0, "Align Left")
        gridLayoutGridColor.add_widget(buttonGridColor, 1, 1, "Align Right")
        groupBoxGridColor.setLayout(gridLayoutGridColor)

        #Grid Geometry
        groupBoxGridGeom = tk.GroupBox(translate("Grid Geometry"), widget)

        check_box_grid_load_from_file = tk.check_box(translate("Set grid size from opened file"), groupBoxGridGeom)
        self.dialog["grid_load_from_file"] = settings["grid_load_from_file"]
        check_box_grid_load_from_file.setChecked(self.dialog["grid_load_from_file"])
        #connect(check_box_grid_load_from_file, SIGNAL(_state_changed(int)), this, SLOT(check_box_grid_load_from_file_state_changed(int)))

        label_grid_type = tk.Label(translate("Grid Type"), groupBoxGridGeom)
        label_grid_type.setObjectName("label_grid_type")
        combo_box_grid_type = tk.combo_box(groupBoxGridGeom)
        combo_box_grid_type.setObjectName("combo_box_grid_type")
        combo_box_grid_type.add_item("Rectangular")
        combo_box_grid_type.add_item("Circular")
        combo_box_grid_type.add_item("Isometric")
        strcpy(self.dialog.grid_type, settings.grid_type)
        combo_box_grid_type.setCurrentIndex(combo_box_grid_type.findText(self.dialog.grid_type))
        # #connect(combo_box_grid_type, SIGNAL(current_index_changed("")), this, SLOT(combo_box_grid_typeCurrent_index_changed("")))

        check_box_grid_center_on_origin = tk.check_box(translate("Center the grid on the origin"), groupBoxGridGeom)
        check_box_grid_center_on_origin.setObjectName("check_box_grid_center_on_origin")
        self.dialog["grid_center_on_origin"] = settings["grid_center_on_origin"]
        check_box_grid_center_on_origin.setChecked(self.dialog["grid_center_on_origin"])
        #connect(check_box_grid_center_on_origin, SIGNAL(_state_changed(int)), this, SLOT(check_box_grid_center_on_origin_state_changed(int)))

        label_grid_center_x = tk.Label(translate("Grid Center X"), groupBoxGridGeom)
        label_grid_center_x.setObjectName("label_grid_center_x")
        spin_boxGridCenterX = tk.Doublespin_box(groupBoxGridGeom)
        spin_boxGridCenterX.setObjectName("spin_boxGridCenterX")
        self.dialog.grid_center.x = settings.grid_center.x
        spin_boxGridCenterX.setSingleStep(1.000)
        spin_boxGridCenterX.setRange(-1000.000, 1000.000)
        spin_boxGridCenterX.setValue(self.dialog.grid_center.x)
        #connect(spin_boxGridCenterX, SIGNAL(_value_changed(double)), this, SLOT(spin_boxGridCenterX_value_changed(double)))

        label_grid_center_y = tk.Label(translate("Grid Center Y"), groupBoxGridGeom)
        label_grid_center_y.setObjectName("label_grid_center_y")
        spin_box_grid_center_y = tk.Doublespin_box(groupBoxGridGeom)
        spin_box_grid_center_y.setObjectName("spin_box_grid_center_y")
        self.dialog.grid_center.y = settings.grid_center.y
        spin_box_grid_center_y.setSingleStep(1.000)
        spin_box_grid_center_y.setRange(-1000.000, 1000.000)
        spin_box_grid_center_y.setValue(self.dialog.grid_center.y)
        #connect(spin_box_grid_center_y, SIGNAL(_value_changed(double)), this, SLOT(spin_box_grid_center_y_value_changed(double)))

        label_grid_sizeX = tk.Label(translate("Grid Size X"), groupBoxGridGeom)
        label_grid_sizeX.setObjectName("label_grid_sizeX")
        spin_box_grid_sizeX = tk.Doublespin_box(groupBoxGridGeom)
        spin_box_grid_sizeX.setObjectName("spin_box_grid_sizeX")
        self.dialog.grid_size.x = settings.grid_size.x
        spin_box_grid_sizeX.setSingleStep(1.000)
        spin_box_grid_sizeX.setRange(1.000, 1000.000)
        spin_box_grid_sizeX.setValue(self.dialog.grid_size.x)
        #connect(spin_box_grid_sizeX, SIGNAL(_value_changed(double)), this, SLOT(spin_box_grid_sizeX_value_changed(double)))

        label_grid_sizeY = tk.Label(translate("Grid Size Y"), groupBoxGridGeom)
        label_grid_sizeY.setObjectName("label_grid_sizeY")
        spin_box_grid_sizeY = tk.Doublespin_box(groupBoxGridGeom)
        spin_box_grid_sizeY.setObjectName("spin_box_grid_sizeY")
        self.dialog.grid_size.y = settings.grid_size.y
        spin_box_grid_sizeY.setSingleStep(1.000)
        spin_box_grid_sizeY.setRange(1.000, 1000.000)
        spin_box_grid_sizeY.setValue(self.dialog.grid_size.y)
        #connect(spin_box_grid_sizeY, SIGNAL(_value_changed(double)), this, SLOT(spin_box_grid_sizeY_value_changed(double)))

        labelGridSpacingX = tk.Label(translate("Grid Spacing X"), groupBoxGridGeom)
        labelGridSpacingX.setObjectName("labelGridSpacingX")
        spin_boxGridSpacingX = tk.Doublespin_box(groupBoxGridGeom)
        spin_boxGridSpacingX.setObjectName("spin_boxGridSpacingX")
        self.dialog.grid_spacing.x = settings.grid_spacing.x
        spin_boxGridSpacingX.setSingleStep(1.000)
        spin_boxGridSpacingX.setRange(0.001, 1000.000)
        spin_boxGridSpacingX.setValue(self.dialog.grid_spacing.x)
        #connect(spin_boxGridSpacingX, SIGNAL(_value_changed(double)), this, SLOT(spin_boxGridSpacingX_value_changed(double)))

        labelGridSpacingY = tk.Label(translate("Grid Spacing Y"), groupBoxGridGeom)
        labelGridSpacingY.setObjectName("labelGridSpacingY")
        spin_boxGridSpacingY = tk.Doublespin_box(groupBoxGridGeom)
        spin_boxGridSpacingY.setObjectName("spin_boxGridSpacingY")
        self.dialog.grid_spacing.y = settings.grid_spacing.y
        spin_boxGridSpacingY.setSingleStep(1.000)
        spin_boxGridSpacingY.setRange(0.001, 1000.000)
        spin_boxGridSpacingY.setValue(self.dialog.grid_spacing.y)
        #connect(spin_boxGridSpacingY, SIGNAL(_value_changed(double)), this, SLOT(spin_boxGridSpacingY_value_changed(double)))

        label_grid_sizeRadius = tk.Label(translate("Grid Size Radius"), groupBoxGridGeom)
        label_grid_sizeRadius.setObjectName("label_grid_sizeRadius")
        spin_box_grid_sizeRadius = tk.Doublespin_box(groupBoxGridGeom)
        spin_box_grid_sizeRadius.setObjectName("spin_box_grid_sizeRadius")
        self.dialog.grid_size_radius = settings.grid_size_radius
        spin_box_grid_sizeRadius.setSingleStep(1.000)
        spin_box_grid_sizeRadius.setRange(1.000, 1000.000)
        spin_box_grid_sizeRadius.setValue(self.dialog.grid_size_radius)
        #connect(spin_box_grid_sizeRadius, SIGNAL(_value_changed(double)), this, SLOT(spin_box_grid_sizeRadius_value_changed(double)))

        labelGridSpacingRadius = tk.Label(translate("Grid Spacing Radius"), groupBoxGridGeom)
        labelGridSpacingRadius.setObjectName("labelGridSpacingRadius")
        spin_boxGridSpacingRadius = tk.Doublespin_box(groupBoxGridGeom)
        spin_boxGridSpacingRadius.setObjectName("spin_boxGridSpacingRadius")
        self.dialog.grid_spacing_radius = settings.grid_spacing_radius
        spin_boxGridSpacingRadius.setSingleStep(1.000)
        spin_boxGridSpacingRadius.setRange(0.001, 1000.000)
        spin_boxGridSpacingRadius.setValue(self.dialog.grid_spacing_radius)
        #connect(spin_boxGridSpacingRadius, SIGNAL(_value_changed(double)), this, SLOT(spin_boxGridSpacingRadius_value_changed(double)))

        labelGridSpacingAngle = tk.Label(translate("Grid Spacing Angle"), groupBoxGridGeom)
        labelGridSpacingAngle.setObjectName("labelGridSpacingAngle")
        spin_boxGridSpacingAngle = tk.Doublespin_box(groupBoxGridGeom)
        spin_boxGridSpacingAngle.setObjectName("spin_boxGridSpacingAngle")
        self.dialog.grid_spacing_angle = settings.grid_spacing_angle
        spin_boxGridSpacingAngle.setSingleStep(1.000)
        spin_boxGridSpacingAngle.setRange(0.001, 1000.000)
        spin_boxGridSpacingAngle.setValue(self.dialog.grid_spacing_angle)
        #connect(spin_boxGridSpacingAngle, SIGNAL(_value_changed(double)), this, SLOT(spin_boxGridSpacingAngle_value_changed(double)))

        label_grid_type.set_enabled(not self.dialog["grid_load_from_file"])
        combo_box_grid_type.set_enabled(not self.dialog["grid_load_from_file"])
        check_box_grid_center_on_origin.set_enabled(not self.dialog["grid_load_from_file"])
        label_grid_center_x.set_enabled(not self.dialog["grid_load_from_file"])
        spin_boxGridCenterX.set_enabled(not self.dialog["grid_load_from_file"])
        label_grid_center_y.set_enabled(not self.dialog["grid_load_from_file"])
        spin_box_grid_center_y.set_enabled(not self.dialog["grid_load_from_file"])
        label_grid_sizeX.set_enabled(not self.dialog["grid_load_from_file"])
        spin_box_grid_sizeX.set_enabled(not self.dialog["grid_load_from_file"])
        label_grid_sizeY.set_enabled(not self.dialog["grid_load_from_file"])
        spin_box_grid_sizeY.set_enabled(not self.dialog["grid_load_from_file"])
        labelGridSpacingX.set_enabled(not self.dialog["grid_load_from_file"])
        spin_boxGridSpacingX.set_enabled(not self.dialog["grid_load_from_file"])
        labelGridSpacingY.set_enabled(not self.dialog["grid_load_from_file"])
        spin_boxGridSpacingY.set_enabled(not self.dialog["grid_load_from_file"])
        label_grid_sizeRadius.set_enabled(not self.dialog["grid_load_from_file"])
        spin_box_grid_sizeRadius.set_enabled(not self.dialog["grid_load_from_file"])
        labelGridSpacingRadius.set_enabled(not self.dialog["grid_load_from_file"])
        spin_boxGridSpacingRadius.set_enabled(not self.dialog["grid_load_from_file"])
        labelGridSpacingAngle.set_enabled(not self.dialog["grid_load_from_file"])
        spin_boxGridSpacingAngle.set_enabled(not self.dialog["grid_load_from_file"])

        visibility = 0
        if self.dialog.grid_type == "Circular":
            visibility = 1
        label_grid_sizeX.set_visible(not visibility)
        spin_box_grid_sizeX.set_visible(not visibility)
        label_grid_sizeY.set_visible(not visibility)
        spin_box_grid_sizeY.set_visible(not visibility)
        labelGridSpacingX.set_visible(not visibility)
        spin_boxGridSpacingX.set_visible(not visibility)
        labelGridSpacingY.set_visible(not visibility)
        spin_boxGridSpacingY.set_visible(not visibility)
        label_grid_sizeRadius.set_visible(visibility)
        spin_box_grid_sizeRadius.set_visible(visibility)
        labelGridSpacingRadius.set_visible(visibility)
        spin_boxGridSpacingRadius.set_visible(visibility)
        labelGridSpacingAngle.set_visible(visibility)
        spin_boxGridSpacingAngle.set_visible(visibility)

        gridLayoutGridGeom = tk.GridLayout(groupBoxGridGeom)
        gridLayoutGridGeom.add_widget(check_box_grid_load_from_file, 0, 0, "Align Left")
        gridLayoutGridGeom.add_widget(label_grid_type, 1, 0, "Align Left")
        gridLayoutGridGeom.add_widget(combo_box_grid_type, 1, 1, "Align Right")
        gridLayoutGridGeom.add_widget(check_box_grid_center_on_origin, 2, 0, "Align Left")
        gridLayoutGridGeom.add_widget(label_grid_center_x, 3, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_boxGridCenterX, 3, 1, "Align Right")
        gridLayoutGridGeom.add_widget(label_grid_center_y, 4, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_box_grid_center_y, 4, 1, "Align Right")
        gridLayoutGridGeom.add_widget(label_grid_sizeX, 5, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_box_grid_sizeX, 5, 1, "Align Right")
        gridLayoutGridGeom.add_widget(label_grid_sizeY, 6, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_box_grid_sizeY, 6, 1, "Align Right")
        gridLayoutGridGeom.add_widget(labelGridSpacingX, 7, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_boxGridSpacingX, 7, 1, "Align Right")
        gridLayoutGridGeom.add_widget(labelGridSpacingY, 8, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_boxGridSpacingY, 8, 1, "Align Right")
        gridLayoutGridGeom.add_widget(label_grid_sizeRadius, 9, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_box_grid_sizeRadius, 9, 1, "Align Right")
        gridLayoutGridGeom.add_widget(labelGridSpacingRadius, 10, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_boxGridSpacingRadius, 10, 1, "Align Right")
        gridLayoutGridGeom.add_widget(labelGridSpacingAngle, 11, 0, "Align Left")
        gridLayoutGridGeom.add_widget(spin_boxGridSpacingAngle, 11, 1, "Align Right")
        groupBoxGridGeom.setLayout(gridLayoutGridGeom)

        #Ruler Misc
        groupBoxRulerMisc = tk.GroupBox(translate("Ruler Misc"), widget)

        check_boxRulerShowOnLoad = tk.check_box(translate("Initially show ruler when loading a file"), groupBoxRulerMisc)
        self.dialog.ruler_show_on_load = settings.ruler_show_on_load
        check_boxRulerShowOnLoad.setChecked(self.dialog.ruler_show_on_load)
        #connect(check_boxRulerShowOnLoad, SIGNAL(_state_changed(int)), this, SLOT(check_boxRulerShowOnLoad_state_changed(int)))

        labelRulerMetric = tk.Label(translate("Ruler Units"), groupBoxRulerMisc)
        combo_boxRulerMetric = tk.combo_box(groupBoxRulerMisc)
        combo_boxRulerMetric.add_item("Imperial", 0)
        combo_boxRulerMetric.add_item("Metric", 1)
        self.dialog.ruler_metric = settings.ruler_metric
        combo_boxRulerMetric.setCurrentIndex(combo_boxRulerMetric.findData(self.dialog.ruler_metric))
        #connect(combo_boxRulerMetric, SIGNAL(current_index_changed(int)), this, SLOT(combo_boxRulerMetricCurrent_index_changed(int)))

        gridLayoutRulerMisc = tk.GridLayout(widget)
        gridLayoutRulerMisc.add_widget(check_boxRulerShowOnLoad, 0, 0, "Align Left")
        gridLayoutRulerMisc.add_widget(labelRulerMetric, 1, 0, "Align Left")
        gridLayoutRulerMisc.add_widget(combo_boxRulerMetric, 1, 1, "Align Right")
        groupBoxRulerMisc.setLayout(gridLayoutRulerMisc)

        #Ruler Color
        groupBoxRulerColor = tk.GroupBox(translate("Ruler Color"), widget)

        labelRulerColor = tk.Label(translate("Ruler Color"), groupBoxRulerColor)
        labelRulerColor.setObjectName("labelRulerColor")
        buttonRulerColor = PushButton(translate("Choose"), groupBoxRulerColor)
        buttonRulerColor.setObjectName("buttonRulerColor")
        self.dialog.ruler_color = settings.ruler_color
        self.preview.ruler_color = self.dialog.ruler_color
        self.accept.ruler_color = self.dialog.ruler_color
        rulerPix = Image(16,16)
        rulerPix.fill(Color(preview.ruler_color))
        buttonRulerColor.setIcon(QIcon(rulerPix))
        #connect(buttonRulerColor, SIGNAL(clicked()), this, SLOT(chooseRulerColor()))

        gridLayoutRulerColor = tk.GridLayout(widget)
        gridLayoutRulerColor.add_widget(labelRulerColor, 1, 0, "Align Left")
        gridLayoutRulerColor.add_widget(buttonRulerColor, 1, 1, "Align Right")
        groupBoxRulerColor.setLayout(gridLayoutRulerColor)

        #Ruler Geometry
        groupBoxRulerGeom = tk.GroupBox(translate("Ruler Geometry"), widget)

        labelRulerPixelSize = tk.Label(translate("Ruler Pixel Size"), groupBoxRulerGeom)
        labelRulerPixelSize.setObjectName("labelRulerPixelSize")
        spin_boxRulerPixelSize = tk.Doublespin_box(groupBoxRulerGeom)
        spin_boxRulerPixelSize.setObjectName("spin_boxRulerPixelSize")
        self.dialog.ruler_pixel_size = settings.ruler_pixel_size
        spin_boxRulerPixelSize.setSingleStep(1.000)
        spin_boxRulerPixelSize.setRange(20.000, 100.000)
        spin_boxRulerPixelSize.setValue(self.dialog.ruler_pixel_size)
        #connect(spin_boxRulerPixelSize, SIGNAL(_value_changed(double)), this, SLOT(spin_boxRulerPixelSize_value_changed(double)))

        gridLayoutRulerGeom = tk.GridLayout(groupBoxRulerGeom)
        gridLayoutRulerGeom.add_widget(labelRulerPixelSize, 0, 0, "Align Left")
        gridLayoutRulerGeom.add_widget(spin_boxRulerPixelSize, 0, 1, "Align Right")
        groupBoxRulerGeom.setLayout(gridLayoutRulerGeom)

        #Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        vbox_layout_main.add_widget(groupBoxGridMisc)
        vbox_layout_main.add_widget(groupBoxGridColor)
        vbox_layout_main.add_widget(groupBoxGridGeom)
        vbox_layout_main.add_widget(groupBoxRulerMisc)
        vbox_layout_main.add_widget(groupBoxRulerColor)
        vbox_layout_main.add_widget(groupBoxRulerGeom)
        vbox_layout_main.addStretch(1)
        widget.setLayout(vbox_layout_main)

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def createTabOrthoPolar():
        widget = tk.Widget(this)

        #TODO: finish this

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def make_check_box(label, checked, icon, f, x, y):
        " Originally a macro for constructing tk.t check_boxes. "
        c = tk.check_box(translate(label), groupBoxQSnapLoc)
        c.setChecked(settings.checked)
        c.setIcon(loadIcon(icon))
        #connect(c, SIGNAL(_state_changed(int)), this, SLOT(f(int)))
        #connect(this, SIGNAL(buttonQSnapSelectAll(int)), c, SLOT(setChecked(int)))
        #connect(this, SIGNAL(buttonQSnapClearAll(int)), c, SLOT(setChecked(int)))
        gridLayoutQSnap.add_widget(c, x, y, "Align Left")
        self.dialog.checked = settings.checked

    def createTabQuickSnap():
        widget = tk.Widget(this)

        #QSnap Locators
        groupBoxQSnapLoc = tk.GroupBox(translate("Locators Used"), widget)
        buttonQSnapSelectAll = PushButton(translate("Select All"), groupBoxQSnapLoc)
        buttonQSnapClearAll = PushButton(translate("Clear All"), groupBoxQSnapLoc)
        gridLayoutQSnap = tk.GridLayout(groupBoxQSnapLoc)

        #connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(buttonQSnapSelectAllClicked()))
        #connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()))

        make_check_box("Endpoint", tk.snap_endpoint, locator_snaptoendpoint_xpm, check_boxQSnapEndPoint_state_changed, 0, 0)
        make_check_box("Midpoint", tk.snap_midpoint, locator_snaptomidpoint_xpm, check_boxQSnapMidPoint_state_changed, 1, 0)
        make_check_box("Center", tk.snap_center, locator_snaptocenter_xpm, check_boxQSnapCenter_state_changed, 2, 0)
        make_check_box("Node", tk.snap_node, locator_snaptonode_xpm, check_boxQSnapNode_state_changed, 3, 0)
        make_check_box("Quadrant", tk.snap_quadrant, locator_snaptoquadrant_xpm, check_boxQSnapQuadrant_state_changed, 4, 0)
        make_check_box("Intersection", tk.snap_intersection, locator_snaptointersection_xpm, check_boxQSnapIntersection_state_changed, 5, 0)
        make_check_box("Extension", tk.snap_extension, locator_snaptoextension_xpm, check_boxQSnapExtension_state_changed, 6, 0)
        make_check_box("Insertion", tk.snap_insertion, locator_snaptoinsert_xpm, check_boxQSnapInsertion_state_changed, 0, 1)
        make_check_box("Perpendicular", tk.snap_perpendicular, locator_snaptoperpendicular_xpm, check_boxQSnapPerpendicular_state_changed, 1, 1)
        make_check_box("Tangent", tk.snap_tangent, locator_snaptotangent_xpm, check_boxQSnapTangent_state_changed, 2, 1)
        make_check_box("Nearest", tk.snap_nearest, locator_snaptonearest_xpm, check_boxQSnapNearest_state_changed, 3, 1)
        make_check_box("Apparent Intersection", tk.snap_apparent, locator_snaptoapparentintersection_xpm, check_boxQSnapApparentIntersection_state_changed, 4, 1)
        make_check_box("Parallel", tk.snap_parallel, locator_snaptoparallel_xpm, check_boxQSnapParallel_state_changed, 5, 1)

        gridLayoutQSnap.add_widget(buttonQSnapSelectAll, 0, 2, "Align Left")
        gridLayoutQSnap.add_widget(buttonQSnapClearAll, 1, 2, "Align Left")
        gridLayoutQSnap.setColumnStretch(2,1)
        groupBoxQSnapLoc.setLayout(gridLayoutQSnap)

        #QSnap Visual Config
        groupBoxQSnapVisual = tk.GroupBox(translate("Visual Configuration"), widget)

        labelQSnapLocColor = tk.Label(translate("Locator Color"), groupBoxQSnapVisual)
        combo_boxQSnapLocColor = tk.combo_box(groupBoxQSnapVisual)
        addColorsTocombo_box(combo_boxQSnapLocColor)
        self.dialog.qsnap_locator_color = settings.qsnap_locator_color
        combo_boxQSnapLocColor.setCurrentIndex(combo_boxQSnapLocColor.findData(self.dialog.qsnap_locator_color))
        #connect(combo_boxQSnapLocColor, SIGNAL(current_index_changed(int)), this, SLOT(combo_boxQSnapLocatorColorCurrent_index_changed(int)))

        labelQSnapLocSize = tk.Label(translate("Locator Size"), groupBoxQSnapVisual)
        sliderQSnapLocSize = tk.Slider(Qt_Horizontal, groupBoxQSnapVisual)
        sliderQSnapLocSize.setRange(1,20)
        self.dialog.qsnap_locator_size = settings.qsnap_locator_size
        sliderQSnapLocSize.setValue(self.dialog.qsnap_locator_size)
        #connect(sliderQSnapLocSize, SIGNAL(_value_changed(int)), this, SLOT(sliderQSnapLocatorSize_value_changed(int)))

        vboxLayoutQSnapVisual = tk.VBoxLayout(groupBoxQSnapVisual)
        vboxLayoutQSnapVisual.add_widget(labelQSnapLocColor)
        vboxLayoutQSnapVisual.add_widget(combo_boxQSnapLocColor)
        vboxLayoutQSnapVisual.add_widget(labelQSnapLocSize)
        vboxLayoutQSnapVisual.add_widget(sliderQSnapLocSize)
        groupBoxQSnapVisual.setLayout(vboxLayoutQSnapVisual)

        #QSnap Sensitivity Config
        groupBoxQSnapSensitivity = tk.GroupBox(translate("Sensitivity"), widget)

        labelQSnapApertureSize = tk.Label(translate("Aperture Size"), groupBoxQSnapSensitivity)
        sliderQSnapApertureSize = tk.Slider(Qt_Horizontal, groupBoxQSnapSensitivity)
        sliderQSnapApertureSize.setRange(1,20)
        self.dialog.qsnap_aperture_size = settings.qsnap_aperture_size
        sliderQSnapApertureSize.setValue(self.dialog.qsnap_aperture_size)
        #connect(sliderQSnapApertureSize, SIGNAL(_value_changed(int)), this, SLOT(sliderQSnapApertureSize_value_changed(int)))

        vboxLayoutQSnapSensitivity = tk.VBoxLayout(groupBoxQSnapSensitivity)
        vboxLayoutQSnapSensitivity.add_widget(labelQSnapApertureSize)
        vboxLayoutQSnapSensitivity.add_widget(sliderQSnapApertureSize)
        groupBoxQSnapSensitivity.setLayout(vboxLayoutQSnapSensitivity)

        #Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        vbox_layout_main.add_widget(groupBoxQSnapLoc)
        vbox_layout_main.add_widget(groupBoxQSnapVisual)
        vbox_layout_main.add_widget(groupBoxQSnapSensitivity)
        vbox_layout_main.addStretch(1)
        widget.setLayout(vbox_layout_main)

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    #undef make_check_box

    def createTabQuickTrack():
        widget = tk.Widget(this)

        # TODO: finish this

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def createTabLineWeight():
        widget = tk.Widget(this)

        # TODO: finish this

        # Misc
        groupBoxLwtMisc = tk.GroupBox(translate("LineWeight Misc"), widget)

        s = self.mw.activeScene()

        check_boxShowLwt = tk.check_box(translate("Show LineWeight"), groupBoxLwtMisc)
        if s:
            self.dialog.lwt_show_lwt = s.property("ENABLE_LWT").toBool()
        else:
            self.dialog.lwt_show_lwt = settings.lwt_show_lwt

        self.preview.lwt_show_lwt = self.dialog.lwt_show_lwt
        check_boxShowLwt.setChecked(preview.lwt_show_lwt)
        #connect(check_boxShowLwt, SIGNAL(_state_changed(int)), this, SLOT(check_boxLwtShowLwt_state_changed(int)))

        check_boxRealRender = tk.check_box(translate("RealRender"), groupBoxLwtMisc)
        check_boxRealRender.setObjectName("check_boxRealRender")
        if s:
            self.dialog.lwt_real_render = s.property("ENABLE_REAL").toBool()
        else:
            self.dialog.lwt_real_render = settings.lwt_real_render

        self.preview.lwt_real_render = self.dialog.lwt_real_render
        check_boxRealRender.setChecked(preview.lwt_real_render)
        #connect(check_boxRealRender, SIGNAL(_state_changed(int)), this, SLOT(check_boxLwtRealRender_state_changed(int)))
        check_boxRealRender.set_enabled(self.dialog.lwt_show_lwt)

        labelDefaultLwt = tk.Label(translate("Default weight"), groupBoxLwtMisc)
        labelDefaultLwt.set_enabled(0); # TODO: remove later
        combo_boxDefaultLwt = tk.combo_box(groupBoxLwtMisc)
        self.dialog.lwt_default_lwt = settings.lwt_default_lwt
        # TODO: populate the combo_box and set the initial value
        combo_boxDefaultLwt.add_item("".setNum(self.dialog.lwt_default_lwt, 'F', 2).append(" mm"), self.dialog.lwt_default_lwt)
        combo_boxDefaultLwt.set_enabled(0); # TODO: remove later

        vboxLayoutLwtMisc = tk.VBoxLayout(groupBoxLwtMisc)
        vboxLayoutLwtMisc.add_widget(check_boxShowLwt)
        vboxLayoutLwtMisc.add_widget(check_boxRealRender)
        vboxLayoutLwtMisc.add_widget(labelDefaultLwt)
        vboxLayoutLwtMisc.add_widget(combo_boxDefaultLwt)
        groupBoxLwtMisc.setLayout(vboxLayoutLwtMisc)

        #Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        vbox_layout_main.add_widget(groupBoxLwtMisc)
        vbox_layout_main.addStretch(1)
        widget.setLayout(vbox_layout_main)

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def createTabSelection():
        widget = tk.Widget(this)

        # Selection Modes
        groupBoxSelectionModes = tk.GroupBox(translate("Modes"), widget)

        check_boxSelectionModePickFirst = tk.check_box(translate("Allow Preselection (PickFirst)"), groupBoxSelectionModes)
        self.dialog.selection_mode_pickfirst = settings.selection_mode_pickfirst
        check_boxSelectionModePickFirst.setChecked(self.dialog.selection_mode_pickfirst)
        check_boxSelectionModePickFirst.setChecked(1); check_boxSelectionModePickFirst.set_enabled(0); # TODO: Remove this line when Post-selection is available
        #connect(check_boxSelectionModePickFirst, SIGNAL(_state_changed(int)), this, SLOT(check_boxSelectionModePickFirst_state_changed(int)))

        check_boxSelectionModePickAdd = tk.check_box(translate("Add to Selection (PickAdd)"), groupBoxSelectionModes)
        self.dialog.selection_mode_pickadd = settings.selection_mode_pickadd
        check_boxSelectionModePickAdd.setChecked(self.dialog.selection_mode_pickadd)
        #connect(check_boxSelectionModePickAdd, SIGNAL(_state_changed(int)), this, SLOT(check_boxSelectionModePickAdd_state_changed(int)))

        check_boxSelectionModePickDrag = tk.check_box(translate("Drag to Select (PickDrag)"), groupBoxSelectionModes)
        self.dialog.selection_mode_pickdrag = settings.selection_mode_pickdrag
        check_boxSelectionModePickDrag.setChecked(self.dialog.selection_mode_pickdrag)
        check_boxSelectionModePickDrag.setChecked(0); check_boxSelectionModePickDrag.set_enabled(0); #TODO: Remove this line when this functionality is available
        #connect(check_boxSelectionModePickDrag, SIGNAL(_state_changed(int)), this, SLOT(check_boxSelectionModePickDrag_state_changed(int)))

        vboxLayoutSelectionModes = tk.VBoxLayout(groupBoxSelectionModes)
        vboxLayoutSelectionModes.add_widget(check_boxSelectionModePickFirst)
        vboxLayoutSelectionModes.add_widget(check_boxSelectionModePickAdd)
        vboxLayoutSelectionModes.add_widget(check_boxSelectionModePickDrag)
        groupBoxSelectionModes.setLayout(vboxLayoutSelectionModes)

        #Selection Colors
        groupBoxSelectionColors = tk.GroupBox(translate("Colors"), widget)

        labelCoolGripColor = tk.Label(translate("Cool Grip (Unselected)"), groupBoxSelectionColors)
        combo_boxCoolGripColor = tk.combo_box(groupBoxSelectionColors)
        addColorsTocombo_box(combo_boxCoolGripColor)
        self.dialog.selection_coolgrip_color = settings.selection_coolgrip_color
        combo_boxCoolGripColor.setCurrentIndex(combo_boxCoolGripColor.findData(self.dialog.selection_coolgrip_color))
        #connect(combo_boxCoolGripColor, SIGNAL(current_index_changed(int)), this, SLOT(combo_boxSelectionCoolGripColorCurrent_index_changed(int)))

        labelHotGripColor = tk.Label(translate("Hot Grip (Selected)"), groupBoxSelectionColors)
        combo_boxHotGripColor = tk.combo_box(groupBoxSelectionColors)
        addColorsTocombo_box(combo_boxHotGripColor)
        self.dialog.selection_hotgrip_color = settings.selection_hotgrip_color
        combo_boxHotGripColor.setCurrentIndex(combo_boxHotGripColor.findData(self.dialog.selection_hotgrip_color))
        #connect(combo_boxHotGripColor, SIGNAL(current_index_changed(int)), this, SLOT(combo_boxSelectionHotGripColorCurrent_index_changed(int)))

        vboxLayoutSelectionColors = tk.VBoxLayout(groupBoxSelectionColors)
        vboxLayoutSelectionColors.add_widget(labelCoolGripColor)
        vboxLayoutSelectionColors.add_widget(combo_boxCoolGripColor)
        vboxLayoutSelectionColors.add_widget(labelHotGripColor)
        vboxLayoutSelectionColors.add_widget(combo_boxHotGripColor)
        groupBoxSelectionColors.setLayout(vboxLayoutSelectionColors)

        #Selection Sizes
        groupBoxSelectionSizes = tk.GroupBox(translate("Sizes"), widget)

        labelSelectionGripSize = tk.Label(translate("Grip Size"), groupBoxSelectionSizes)
        sliderSelectionGripSize = tk.Slider(Qt_Horizontal, groupBoxSelectionSizes)
        sliderSelectionGripSize.setRange(1,20)
        self.dialog.selection_grip_size = settings.selection_grip_size
        sliderSelectionGripSize.setValue(self.dialog.selection_grip_size)
        #connect(sliderSelectionGripSize, SIGNAL(_value_changed(int)), this, SLOT(sliderSelectionGripSize_value_changed(int)))

        labelSelectionPickBoxSize = tk.Label(translate("Pickbox Size"), groupBoxSelectionSizes)
        sliderSelectionPickBoxSize = tk.Slider(Qt_Horizontal, groupBoxSelectionSizes)
        sliderSelectionPickBoxSize.setRange(1,20)
        self.dialog.selection_pickbox_size = settings.selection_pickbox_size
        sliderSelectionPickBoxSize.setValue(self.dialog.selection_pickbox_size)
        #connect(sliderSelectionPickBoxSize, SIGNAL(_value_changed(int)), this, SLOT(sliderSelectionPickBoxSize_value_changed(int)))

        vboxLayoutSelectionSizes = tk.VBoxLayout(groupBoxSelectionSizes)
        vboxLayoutSelectionSizes.add_widget(labelSelectionGripSize)
        vboxLayoutSelectionSizes.add_widget(sliderSelectionGripSize)
        vboxLayoutSelectionSizes.add_widget(labelSelectionPickBoxSize)
        vboxLayoutSelectionSizes.add_widget(sliderSelectionPickBoxSize)
        groupBoxSelectionSizes.setLayout(vboxLayoutSelectionSizes)

        #Widget Layout
        vbox_layout_main = tk.VBoxLayout(widget)
        vbox_layout_main.add_widget(groupBoxSelectionModes)
        vbox_layout_main.add_widget(groupBoxSelectionColors)
        vbox_layout_main.add_widget(groupBoxSelectionSizes)
        vbox_layout_main.addStretch(1)
        widget.setLayout(vbox_layout_main)

        scroll_area = tk.scroll_area(this)
        scroll_area.setWidgetResizable(1)
        scroll_area.setWidget(widget)
        return scroll_area

    def addColorsTocombo_box(combo_box):
        combo_box.add_item(loadIcon(colorred_xpm), translate("Red"), tk.Rgb(255, 0, 0))
        combo_box.add_item(loadIcon(coloryellow_xpm), translate("Yellow"), tk.Rgb(255,255, 0))
        combo_box.add_item(loadIcon(colorgreen_xpm), translate("Green"), tk.Rgb(  0,255, 0))
        combo_box.add_item(loadIcon(colorcyan_xpm), translate("Cyan"), tk.Rgb(  0,255,255))
        combo_box.add_item(loadIcon(colorblue_xpm), translate("Blue"), tk.Rgb(  0, 0,255))
        combo_box.add_item(loadIcon(colormagenta_xpm), translate("Magenta"), tk.Rgb(255, 0,255))
        combo_box.add_item(loadIcon(colorwhite_xpm), translate("White"), tk.Rgb(255,255,255))
        # TODO: Add Other... so the user can select custom colors

    def combo_boxLanguageCurrent_index_changed(self, lang):
        " . "
        self.dialog.general_language = lang.toLower().toLocal8Bit().constData()

    def combo_boxIconThemeCurrent_index_changed(self, theme):
        " . "
        strcpy(self.dialog.general_icon_theme, theme.toLocal8Bit().constData())

    def combo_box_icon_sizeCurrent_index_changed(self, index):
        " . "
        combo_box = sender()
        if combo_box:
            ok = 0
            self.dialog.general_icon_size, ok = combo_box.itemData(index).toUInt()
            if not ok:
                self.dialog.general_icon_size = 16
        else:
            self.dialog.general_icon_size = 16

    def check_boxGeneralMdiBGUseLogo_state_changed(self, checked):
        " . "
        self.preview.general_mdi_bg_use_logo = checked
        self.mw.mdi_area.useBackgroundLogo(checked)

    def chooseGeneralMdiBackgroundLogo(self):
        " . "
        button = sender()
        if button:
            selectedImage = tk.FileDialog_getOpenFileName(this, translate("Open File"),
                            tk.StandardPaths_writableLocation(QStandardPaths_PicturesLocation),
                            translate("Images (*.bmp *.png *.jpg)"))

            if selectedImage != "":
                self.accept.general_mdi_bg_logo = selectedImage

            #Update immediately so it can be self.previewed
            self.mw.mdi_area.setBackgroundLogo(accept_.general_mdi_bg_logo)

    def check_boxGeneralMdiBGUseTexture_state_changed(self, checked):
        " . "
        self.preview.general_mdi_bg_use_texture = checked
        self.mw.mdi_area.useBackgroundTexture(checked)

    def chooseGeneralMdiBackgroundTexture(self):
        " . "
        button = sender()
        if button:
            selectedImage = tk.FileDialog_getOpenFileName(this, translate("Open File"),
            tk.StandardPaths_writableLocation(QStandardPaths_PicturesLocation),
                            translate("Images (*.bmp *.png *.jpg)"))

            if selectedImage != "":
                self.accept.general_mdi_bg_texture = selectedImage

            #Update immediately so it can be self.previewed
            self.mw.mdi_area.setBackgroundTexture(accept_.general_mdi_bg_texture)

    def check_boxGeneralMdiBGUseColor_state_changed(self, checked):
        self.preview.general_mdi_bg_use_color = checked
        self.mw.mdi_area.useBackgroundColor(checked)

    def chooseGeneralMdiBackgroundColor():
        button = sender()
        if button:
            color_dialog = color_dialog(Color(accept_.general_mdi_bg_color), this)
            #connect(color_dialog, SIGNAL(currentColorChanged()), this, SLOT(currentGeneralMdiBackgroundColorChanged()))
            color_dialog.exec()

            if color_dialog.result() == "Accepted":
                self.accept.general_mdi_bg_color = color_dialog.selectedColor().rgb()
                pix = Image(16,16)
                pix.fill(Color(accept_.general_mdi_bg_color))
                button.setIcon(pix)
                self.mw.mdi_area.setBackgroundColor(Color(accept_.general_mdi_bg_color))

            else:
                self.mw.mdi_area.setBackgroundColor(Color(self.dialog.general_mdi_bg_color))

    def currentGeneralMdiBackgroundColorChanged(self, color):
        self.preview.general_mdi_bg_color = color.rgb()
        self.mw.mdi_area.setBackgroundColor(Color(preview.general_mdi_bg_color))

    """
    lambda method constructor?

    def check_boxTipOfTheDay_state_changed(self, )
    #
    check_func(check_boxTipOfTheDay_state_changed, general_tip_of_the_day)
    check_func(check_boxUseOpenGL_state_changed, display_use_opengl)
    check_func(check_boxRenderHintAA_state_changed, display_renderhint_aa)
    check_func(check_boxRenderHintTextAA_state_changed, display_renderhint_text_aa)
    check_func(check_boxRenderHintSmoothPix_state_changed, display_renderhint_smooth_pix)
    check_func(check_boxRenderHintHighAA_state_changed, display_renderhint_high_aa)
    check_func(check_boxRenderHintNonCosmetic_state_changed, display_renderhint_noncosmetic)
    """

    def check_boxShowScrollBars_state_changed(self, checked):
        " . "
        self.preview.display_show_scrollbars = checked
        self.mw.updateAllViewScrollBars(preview.display_show_scrollbars)

    def spin_boxZoomScaleIn_value_changed(self, value):
        self.dialog.display_zoomscale_in = value

    def spin_boxZoomScaleOut_value_changed(self, value):
        self.dialog.display_zoomscale_out = value

    def check_boxDisableBG_state_changed(self, checked):
        self.dialog.printing_disable_bg = checked

    def chooseDisplayCrossHairColor():
        button = sender()
        if button:
            color_dialog = color_dialog(Color(accept_.display_crosshair_color), this)
            #connect(color_dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplayCrossHairColorChanged()))
            color_dialog.exec()

            if color_dialog.result() == "Accepted":
                self.accept.display_crosshair_color = color_dialog.selectedColor().rgb()
                pix = Image(16,16)
                pix.fill(Color(accept_.display_crosshair_color))
                button.setIcon(pix)
                self.mw.updateAllViewCrossHairColors(accept_.display_crosshair_color)
            else:
                self.mw.updateAllViewCrossHairColors(self.dialog.display_crosshair_color)

    def currentDisplayCrossHairColorChanged(color):
        self.preview.display_crosshair_color = color.rgb()
        self.mw.updateAllViewCrossHairColors(preview.display_crosshair_color)

    def chooseDisplayBackgroundColor():
        button = sender()
        if button:
            color_dialog = color_dialog(Color(accept_.display_bg_color), this)
            #connect(color_dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplayBackgroundColorChanged()))
            color_dialog.exec()

            if color_dialog.result() == "Accepted":
                self.accept.display_bg_color = color_dialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_.display_bg_color))
                button.setIcon(pix)
                self.mw.updateAllViewBackgroundColors(accept_.display_bg_color)
            else:
                self.mw.updateAllViewBackgroundColors(self.dialog.display_bg_color)

    def currentDisplayBackgroundColorChanged(color):
        self.preview.display_bg_color = color.rgb()
        self.mw.updateAllViewBackgroundColors(preview.display_bg_color)

    def chooseDisplaySelectBoxLeftColor():
        button = sender()
        if button:
            color_dialog = color_dialog(Color(accept_["display_selectbox_left_color"]), this)
            #connect(color_dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxLeftColorChanged()))
            color_dialog.exec()

            if color_dialog.result() == tk.Dialog_Accepted:
                self.accept["display_selectbox_left_color"] = color_dialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_["display_selectbox_left_color"]))
                button.setIcon(pix)
                self.mw.update_all_view_select_box_colors(accept_["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept.display_selectbox_right_fill,
                    self.preview.display_selectbox_alpha)
            else:
                self.mw.update_all_view_select_box_colors(
                    self.dialog["display_selectbox_left_color"],
                    self.dialog["display_selectbox_left_fill"],
                    self.dialog["display_selectbox_right_color"],
                    self.dialog.display_selectbox_right_fill,
                    self.preview.display_selectbox_alpha)

    def currentDisplaySelectBoxLeftColorChanged(self, color):
        self.preview["display_selectbox_left_color"] = color.rgb()
        self.mw.update_all_view_select_box_colors(
            self.preview["display_selectbox_left_color"],
            self.preview["display_selectbox_left_fill"],
            self.preview["display_selectbox_right_color"],
            self.preview["display_selectbox_right_fill"],
            self.preview["display_selectbox_alpha"])

    def chooseDisplaySelectBoxLeftFill(self):
        button = sender()
        if button:
            color_dialog = color_dialog(Color(self.accept["display_selectbox_left_fill"]), this)
            #connect(color_dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxLeftFillChanged()))
            color_dialog.exec()

            if color_dialog.result() == "Accepted":
                self.accept["display_selectbox_left_fill"] = color_dialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(self.accept["display_selectbox_left_fill"]))
                button.setIcon(pix)
                self.mw.update_all_view_select_box_colors(
                    self.accept["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept["display_selectbox_right_fill"],
                    self.preview["display_selectbox_alpha"])

            else:
                self.mw.update_all_view_select_box_colors(
                    self.dialog["display_selectbox_left_color"],
                    self.dialog["display_selectbox_left_fill"],
                    self.dialog["display_selectbox_right_color"],
                    self.dialog["display_selectbox_right_fill"],
                    self.preview["display_selectbox_alpha"])

    def currentDisplaySelectBoxLeftFillChanged(color):
        self.preview["display_selectbox_left_fill"] = color.rgb()
        self.mw.update_all_view_select_box_colors(
            self.preview["display_selectbox_left_color"],
            self.preview["display_selectbox_left_fill"],
            self.preview["display_selectbox_right_color"],
            self.preview["display_selectbox_right_fill"],
            self.preview["display_selectbox_alpha"])

    def chooseDisplaySelectBoxRightColor():
        button = sender()
        if button:
            color_dialog = color_dialog(Color(accept_["display_selectbox_right_color"]), this)
            #connect(color_dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxRightColorChanged()))
            color_dialog.exec()

            if color_dialog.result() == "Accepted":
                self.accept["display_selectbox_right_color"] = color_dialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(self.accept["display_selectbox_right_color"]))
                button.setIcon(pix)
                self.mw.update_all_view_select_box_colors(
                    self.accept["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept["display_selectbox_right_fill"],
                    self.preview["display_selectbox_alpha"])

            else:
                self.mw.update_all_view_select_box_colors(
                    self.dialog["display_selectbox_left_color"],
                    self.dialog["display_selectbox_left_fill"],
                    self.dialog["display_selectbox_right_color"],
                    self.dialog["display_selectbox_right_fill"],
                    self.preview["display_selectbox_alpha"])

    def currentDisplaySelectBoxRightColorChanged(color):
        self.preview["display_selectbox_right_color"] = color.rgb()
        self.mw.update_all_view_select_box_colors(
            self.preview["display_selectbox_left_color"],
            self.preview["display_selectbox_left_fill"],
            self.preview["display_selectbox_right_color"],
            self.preview["display_selectbox_right_fill"],
            self.preview["display_selectbox_alpha"])

    def chooseDisplaySelectBoxRightFill():
        button = sender()
        if button:
            color_dialog = color_dialog(Color(accept_.display_selectbox_right_fill), this)
            #connect(color_dialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxRightFillChanged()))
            color_dialog.exec()

            if color_dialog.result() == "Accepted":
                self.accept.display_selectbox_right_fill = color_dialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_.display_selectbox_right_fill))
                button.setIcon(pix)
                self.mw.update_all_view_select_box_colors(
                    self.accept["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept["display_selectbox_right_fill"],
                    self.preview["display_selectbox_alpha"])

            else:
                self.mw.update_all_view_select_box_colors(
                    self.dialog["display_selectbox_left_color"],
                    self.dialog["display_selectbox_left_fill"],
                    self.dialog["display_selectbox_right_color"],
                    self.dialog["display_selectbox_right_fill"],
                    self.preview["display_selectbox_alpha"])

    def currentDisplaySelectBoxRightFillChanged(self, color):
        self.preview.display_selectbox_right_fill = color.rgb()
        self.mw.update_all_view_select_box_colors(
            self.preview["display_selectbox_left_color"],
            self.preview["display_selectbox_left_fill"],
            self.preview["display_selectbox_right_color"],
            self.preview["display_selectbox_right_fill"],
            self.preview["display_selectbox_alpha"])

    def spin_boxDisplaySelectBoxAlpha_value_changed(self, value):
        " . "
        self.preview.display_selectbox_alpha = value
        self.mw.update_all_view_select_box_colors(
            self.accept["display_selectbox_left_color"],
            self.accept["display_selectbox_left_fill"],
            self.accept["display_selectbox_right_color"],
            self.accept["display_selectbox_right_fill"],
            self.preview["display_selectbox_alpha"])

    def check_boxCustomFilter_state_changed(self, checked):
        " . "
        check_box = sender()
        if check_box:
            format = check_box.text()
            debug_message("CustomFilter: %s %d", tk.Printable(format), checked)
            if checked:
                opensave_custom_filter.append(" *." + format.toLower())
            else:
                opensave_custom_filter.remove("*." + format, tk.t_CaseInsensitive)
            #dialog.opensave_custom_filter = checked; #TODO

    def buttonCustomFilterSelectAllClicked(self):
        " . "
        buttonCustomFilterSelectAll(1)
        opensave_custom_filter = "supported"

    def buttonCustomFilterClearAllClicked(self):
        " . "
        buttonCustomFilterClearAll(0)
        opensave_custom_filter.clear()

    def check_boxGridColorMatchCrossHair_state_changed(self, checked):
        " . "
        self.dialog.grid_color_match_crosshair = checked
        if self.dialog.grid_color_match_crosshair:
            self.mw.updateAllViewGridColors(accept_.display_crosshair_color)

        else:
            self.mw.updateAllViewGridColors(accept_.grid_color)

        sender_obj = sender()
        if not sender_obj:
            return

        parent = sender_obj.parent()
        if not parent:
            return

        labelGridColor = parent.find_child("labelGridColor")
        if labelGridColor:
            labelGridColor.set_enabled(not self.dialog.grid_color_match_crosshair)

        buttonGridColor = parent.find_child("buttonGridColor")
        if buttonGridColor:
            buttonGridColor.set_enabled(not self.dialog.grid_color_match_crosshair)

    def checkBoxTipOfTheDayStateChanged(self, checked):
        r" . "
        self.dialog["general_tip_of_the_day"] = checked

    def checkBoxUseOpenGLStateChanged(self, checked):
        r" . "
        self.dialog["display_use_opengl"] = checked

    def checkBoxRenderHintAAStateChanged(self, checked):
        r" . "
        self.dialog["display_renderhint_aa"] = checked

    def checkBoxRenderHintTextAAStateChanged(self, checked):
        r" . "
        self.dialog["display_renderhint_text_aa"] = checked

    def checkBoxRenderHintSmoothPixStateChanged(self, checked):
        r" . "
        self.dialog["display_renderhint_smooth_pix"] = checked

    def checkBoxRenderHintHighAAStateChanged(self, checked):
        r" . "
        self.dialog["display_renderhint_high_aa"] = checked

    def checkBoxRenderHintNonCosmeticStateChanged(self, checked):
        r" . "
        self.dialog["display_renderhint_noncosmetic"] = checked

    def comboBoxScrollBarWidgetCurrentIndexChanged(index):
        r" . "
        self.dialog["display_scrollbar_widget_num"] = index

    def spinBoxZoomScaleInValueChanged(self, value):
        r" . "
        self.dialog["display_zoomscale_in"] = value

    def spinBoxZoomScaleOutValueChanged(self, value):
        r" . "
        self.dialog["display_zoomscale_out"] = value

    def checkBoxDisableBGStateChanged(self, checked):
        r" . "
        self.dialog["printing_disable_bg"] = checked

    def spinBoxRecentMaxFilesValueChanged(self, value):
        r" . "
        self.dialog["opensave_recent_max_files"] = value

    def spinBoxTrimDstNumJumpsValueChanged(self, value):
        r" . "
        self.dialog["opensave_trim_dst_num_jumps"] = value

    def checkBoxGridShowOnLoadStateChanged(self, checked):
        r" . "
        self.dialog["grid_show_on_load"] = checked

    def checkBoxGridShowOriginStateChanged(self, checked):
        r" . "
        self.dialog["grid_show_origin"] = checked

    def spinBoxRulerPixelSizeValueChanged(self, value):
        r" . "
        self.dialog["ruler_pixel_size"] = value

    def checkBoxQSnapEndPointStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_endpoint"] = checked

    def checkBoxQSnapMidPointStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_midpoint"] = checked

    def checkBoxQSnapCenterStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_center"] = checked

    def checkBoxQSnapNodeStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_node"] = checked

    def checkBoxQSnapQuadrantStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_quadrant"] = checked

    def checkBoxQSnapIntersectionStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_intersection"] = checked

    def checkBoxQSnapExtensionStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_extension"] = checked

    def checkBoxQSnapInsertionStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_insertion"] = checked

    def checkBoxQSnapPerpendicularStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_perpendicular"] = checked

    def checkBoxQSnapTangentStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_tangent"] = checked

    def checkBoxQSnapNearestStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_nearest"] = checked

    def checkBoxQSnapApparentStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_apparent"] = checked

    def checkBoxQSnapParallelStateChanged(self, checked):
        r" . "
        self.dialog["qsnap_parallel"] = checked

    def checkBoxSelectionModePickFirstStateChanged(self, checked):
        r" . "
        self.dialog["selection_mode_pickfirst"] = checked

    def checkBoxSelectionModePickAddStateChanged(self, checked):
        r" . "
        self.dialog["selection_mode_pickadd"] = checked

    def checkBoxSelectionModePickDragStateChanged(self, checked):
        r" . "
        self.dialog["selection_mode_pickdrag"] = checked

    def sliderSelectionGripSizeValueChanged(self, value):
        r" . "
        self.dialog["selection_grip_size"] = value

    def sliderSelectionPickBoxSizeValueChanged(self, value):
        r" . "
        self.dialog["selection_pickbox_size"] = value

    def spinBoxGridCenterXValueChanged(self, value):
        r" . "
        self.dialog["grid_center.x"] = value

    def spinBoxGridCenterYValueChanged(self, value):
        r" . "
        self.dialog["grid_center.y"] = value

    def spinBoxGridSizeXValueChanged(self, value):
        r" . "
        self.dialog["grid_size.x"] = value

    def spinBoxGridSizeYValueChanged(self, value):
        r" . "
        self.dialog["grid_size.y"] = value

    def spinBoxGridSpacingXValueChanged(self, value):
        r" . "
        self.dialog["grid_spacing.x"] = value

    def spinBoxGridSpacingYValueChanged(self, value):
        r" . "
        self.dialog["grid_spacing.y"] = value

    def spinBoxGridSizeRadiusValueChanged(self, value):
        r" . "
        self.dialog["grid_size_radius"] = value

    def spinBoxGridSpacingRadiusValueChanged(self, value):
        r" . "
        self.dialog["grid_spacing_radius"] = value

    def spinBoxGridSpacingAngleValueChanged(self, value):
        r" . "
        self.dialog["grid_spacing_angle"] = value

    def checkBoxRulerShowOnLoadStateChanged(self, checked):
        r" . "
        self.dialog["ruler_show_on_load"] = checked

    def check_boxTipOfTheDay_state_changed(self, checked):
        r" . "
        self.dialog["general_tip_of_the_day"] = checked

    def check_boxUseOpenGL_state_changed(self, checked):
        r" . "
        self.dialog["display_use_opengl"] = checked

    def check_boxRenderHintAA_state_changed(self, checked):
        r" . "
        self.dialog["display_renderhint_aa"] = checked

    def check_boxRenderHintTextAA_state_changed(self, checked):
        r" . "
        self.dialog["display_renderhint_text_aa"] = checked

    def check_boxRenderHintSmoothPix_state_changed(self, checked):
        r" . "
        self.dialog["display_renderhint_smooth_pix"] = checked

    def check_box_render_hintHighAA_state_changed(self, checked):
        r" . "
        self.dialog["display_renderhint_high_aa"] = checked

    def check_box_renderHintNonCosmetic_state_changed(self, checked):
        r" . "
        self.dialog["display_renderhint_noncosmetic"] = checked

    def combo_box_scroll_bar_widget_current_index_changed(index):
        r" . "
        self.dialog["display_scrollbar_widget_num"] = index

    def spin_boxZoomScaleIn_value_changed(self, value):
        self.dialog["display_zoomscale_in"] = value

    def spin_boxZoomScaleOut_value_changed(self, value):
        r" . "
        self.dialog["display_zoomscale_out"] = value

    def check_boxDisableBG_state_changed(self, checked):
        r" . "
        self.dialog["printing_disable_bg"] = checked

    def spin_boxRecentMaxFiles_value_changed(self, value):
        r" . "
        self.dialog["opensave_recent_max_files"] = value

    def spin_boxTrimDstNumJumps_value_changed(self, value):
        r" . "
        self.dialog["opensave_trim_dst_num_jumps"] = value

    def check_boxGridShowOnLoad_state_changed(self, checked):
        r" . "
        self.dialog["grid_show_on_load"] = checked

    def check_boxGridShowOrigin_state_changed(self, checked):
        r" . "
        self.dialog["grid_show_origin"] = checked

    def spin_boxRulerPixelSize_value_changed(self, value):
        r" . "
        self.dialog["ruler_pixel_size"] = value

    def check_boxQSnapEndPoint_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_endpoint"] = checked

    def check_boxQSnapMidPoint_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_midpoint"] = checked

    def check_boxQSnapCenter_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_center"] = checked

    def check_boxQSnapNode_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_node"] = checked

    def check_boxQSnapQuadrant_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_quadrant"] = checked

    def check_boxQSnapIntersection_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_intersection"] = checked

    def check_boxQSnapExtension_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_extension"] = checked

    def check_boxQSnapInsertion_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_insertion"] = checked

    def check_boxQSnapPerpendicular_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_perpendicular"] = checked

    def check_boxQSnapTangent_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_tangent"] = checked

    def check_boxQSnap_nearest_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_nearest"] = checked

    def check_boxQSnap_apparent_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_apparent"] = checked

    def check_boxQSnapParallel_state_changed(self, checked):
        r" . "
        self.dialog["qsnap_parallel"] = checked

    def check_boxSelectionModePickFirst_state_changed(self, checked):
        r" . "
        self.dialog["selection_mode_pickfirst"] = checked

    def check_boxSelectionModePickAdd_state_changed(self, checked):
        r" . "
        self.dialog["selection_mode_pickadd"] = checked

    def check_boxSelectionModePickDrag_state_changed(self, checked):
        r" . "
        self.dialog["selection_mode_pickdrag"] = checked

    def sliderSelectionGripSize_value_changed(self, value):
        r" . "
        self.dialog["selection_grip_size"] = value

    def sliderSelectionPickBoxSize_value_changed(self, value):
        r" . "
        self.dialog["selection_pickbox_size"] = value

    def spin_boxGridCenterX_value_changed(self, value):
        r" . "
        self.dialog["grid_center"]["x"] = value

    def spin_box_grid_center_y_value_changed(self, value):
        r" . "
        self.dialog["grid_center"]["y"] = value

    def spin_box_grid_size_x_value_changed(self, value):
        r" . "
        self.dialog["grid_size"]["x"] = value

    def spin_box_grid_size_y_value_changed(self, value):
        r" . "
        self.dialog["grid_size"]["y"] = value

    def spin_box_grid_spacing_x_value_changed(self, value):
        r" . "
        self.dialog["grid_spacing.x"] = value

    def spin_box_grid_spacing_y_value_changed(self, value):
        r" . "
        self.dialog["grid_spacing.y"] = value

    def spin_box_grid_size_radius_value_changed(self, value):
        r" . "
        self.dialog["grid_size_radius"] = value

    def spin_box_grid_spacing_radius_value_changed(self, value):
        r" . "
        self.dialog["grid_spacing_radius"] = value

    def spin_boxGridSpacingAngle_value_changed(self, value):
        r" . "
        self.dialog["grid_spacing_angle"] = value

    def check_boxRulerShowOnLoad_state_changed(self, checked):
        r" . "
        self.dialog["ruler_show_on_load"] = checked

    def chooseGridColor(self):
        r" . "
        button = sender()
        if button:
            color_dialog = color_dialog(Color(accept_.grid_color), this)
            #connect(color_dialog, SIGNAL(currentColorChanged()), this, SLOT(currentGridColorChanged()))
            color_dialog.exec()

            if color_dialog.result() == "Accepted":
                self.accept.grid_color = color_dialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_.grid_color))
                button.setIcon(pix)
                self.mw.updateAllViewGridColors(accept_.grid_color)

            else:
                self.mw.updateAllViewGridColors(self.dialog.grid_color)

    def currentGridColorChanged(self, color):
        r" . "
        self.preview.grid_color = color.rgb()
        self.mw.updateAllViewGridColors(preview.grid_color)

    def check_box_grid_load_from_file_state_changed(self, checked):
        r" . "
        self.dialog["grid_load_from_file"] = checked

        sender_obj = sender()
        if not sender_obj:
            return

        parent = sender_obj.parent()
        if not parent:
            return

        label_grid_type = parent.find_child("label_grid_type")
        if label_grid_type:
            label_grid_type.set_enabled(not self.dialog["grid_load_from_file"])

        combo_box_grid_type = parent.find_child("combo_box_grid_type")
        if combo_box_grid_type:
            combo_box_grid_type.set_enabled(not self.dialog["grid_load_from_file"])

        check_box_grid_center_on_origin = parent.find_child("check_box_grid_center_on_origin")
        if check_box_grid_center_on_origin:
            check_box_grid_center_on_origin.set_enabled(not self.dialog["grid_load_from_file"])

        label_grid_center_x = parent.find_child("label_grid_center_x")
        if label_grid_center_x:
            label_grid_center_x.set_enabled(not self.dialog["grid_load_from_file"] and not self.dialog["grid_center_on_origin"])

        spin_boxGridCenterX = parent.find_child("spin_boxGridCenterX")
        if spin_boxGridCenterX:
            spin_boxGridCenterX.set_enabled(not self.dialog["grid_load_from_file"] and not self.dialog["grid_center_on_origin"])

        label_grid_center_y = parent.find_child("label_grid_center_y")
        if label_grid_center_y:
            label_grid_center_y.set_enabled(not self.dialog["grid_load_from_file"] and not self.dialog["grid_center_on_origin"])

        spin_box_grid_center_y = parent.find_child("spin_box_grid_center_y")
        if spin_box_grid_center_y:
            spin_box_grid_center_y.set_enabled(not self.dialog["grid_load_from_file"] and not self.dialog["grid_center_on_origin"])

        label_grid_sizeX = parent.find_child("label_grid_sizeX")
        if label_grid_sizeX:
            label_grid_sizeX.set_enabled(not self.dialog["grid_load_from_file"])

        spin_box_grid_sizeX = parent.find_child("spin_box_grid_sizeX")
        if spin_box_grid_sizeX:
            spin_box_grid_sizeX.set_enabled(not self.dialog["grid_load_from_file"])

        label_grid_sizeY = parent.find_child("label_grid_sizeY")
        if label_grid_sizeY:
            label_grid_sizeY.set_enabled(not self.dialog["grid_load_from_file"])

        spin_box_grid_sizeY = parent.find_child("spin_box_grid_sizeY")
        if spin_box_grid_sizeY:
            spin_box_grid_sizeY.set_enabled(not self.dialog["grid_load_from_file"])

        labelGridSpacingX = parent.find_child("labelGridSpacingX")
        if labelGridSpacingX:
            labelGridSpacingX.set_enabled(not self.dialog["grid_load_from_file"])

        spin_boxGridSpacingX = parent.find_child("spin_boxGridSpacingX")
        if spin_boxGridSpacingX:
            spin_boxGridSpacingX.set_enabled(not self.dialog["grid_load_from_file"])

        labelGridSpacingY = parent.find_child("labelGridSpacingY")
        if labelGridSpacingY:
            labelGridSpacingY.set_enabled(not self.dialog["grid_load_from_file"])

        spin_boxGridSpacingY = parent.find_child("spin_boxGridSpacingY")
        if spin_boxGridSpacingY:
            spin_boxGridSpacingY.set_enabled(not self.dialog["grid_load_from_file"])

        label_grid_sizeRadius = parent.find_child("label_grid_sizeRadius")
        if label_grid_sizeRadius:
            label_grid_sizeRadius.set_enabled(not self.dialog["grid_load_from_file"])

        spin_box_grid_sizeRadius = parent.find_child("spin_box_grid_sizeRadius")
        if spin_box_grid_sizeRadius:
            spin_box_grid_sizeRadius.set_enabled(not self.dialog["grid_load_from_file"])

        labelGridSpacingRadius = parent.find_child("labelGridSpacingRadius")
        if labelGridSpacingRadius:
            labelGridSpacingRadius.set_enabled(not self.dialog["grid_load_from_file"])

        spin_boxGridSpacingRadius = parent.find_child("spin_boxGridSpacingRadius")
        if spin_boxGridSpacingRadius:
            spin_boxGridSpacingRadius.set_enabled(not self.dialog["grid_load_from_file"])

        labelGridSpacingAngle = parent.find_child("labelGridSpacingAngle")
        if labelGridSpacingAngle:
            labelGridSpacingAngle.set_enabled(not self.dialog["grid_load_from_file"])

        spin_boxGridSpacingAngle = parent.find_child("spin_boxGridSpacingAngle")
        if spin_boxGridSpacingAngle:
            spin_boxGridSpacingAngle.set_enabled(not self.dialog["grid_load_from_file"])

    def combo_box_grid_typeCurrent_index_changed(self, type_):
        r" . "
        self.dialog["grid_type"] = type_

        sender_obj = sender()
        if not sender_obj:
            return

        parent = sender_obj.parent()
        if not parent:
            return

        visibility = False
        if type_ == "Circular":
            visibility = True

        label_grid_size_x = parent.find_child("label_grid_sizeX")
        if label_grid_size_x:
            label_grid_size_x.set_visible(not visibility)

        spin_box_grid_sizeX = parent.find_child("spin_box_grid_sizeX")
        if spin_box_grid_sizeX:
            spin_box_grid_sizeX.set_visible(not visibility)

        label_grid_sizeY = parent.find_child("label_grid_sizeY")
        if label_grid_sizeY:
            label_grid_sizeY.set_visible(not visibility)

        spin_box_grid_sizeY = parent.find_child("spin_box_grid_sizeY")
        if spin_box_grid_sizeY:
            spin_box_grid_sizeY.set_visible(not visibility)

        labelGridSpacingX = parent.find_child("labelGridSpacingX")
        if labelGridSpacingX:
            labelGridSpacingX.set_visible(not visibility)

        spin_boxGridSpacingX = parent.find_child("spin_boxGridSpacingX")
        if spin_boxGridSpacingX:
            spin_boxGridSpacingX.set_visible(not visibility)

        labelGridSpacingY = parent.find_child("labelGridSpacingY")
        if labelGridSpacingY:
            labelGridSpacingY.set_visible(not visibility)

        spin_boxGridSpacingY = parent.find_child("spin_boxGridSpacingY")
        if spin_boxGridSpacingY:
            spin_boxGridSpacingY.set_visible(not visibility)

        label_grid_sizeRadius = parent.find_child("label_grid_sizeRadius")
        if label_grid_sizeRadius:
            label_grid_sizeRadius.set_visible(visibility)

        spin_box_grid_sizeRadius = parent.find_child("spin_box_grid_sizeRadius")
        if spin_box_grid_sizeRadius:
            spin_box_grid_sizeRadius.set_visible(visibility)

        labelGridSpacingRadius = parent.find_child("labelGridSpacingRadius")
        if labelGridSpacingRadius:
            labelGridSpacingRadius.set_visible(visibility)

        spin_boxGridSpacingRadius = parent.find_child("spin_boxGridSpacingRadius")
        if spin_boxGridSpacingRadius:
            spin_boxGridSpacingRadius.set_visible(visibility)

        labelGridSpacingAngle = parent.find_child("labelGridSpacingAngle")
        if labelGridSpacingAngle:
            labelGridSpacingAngle.set_visible(visibility)

        spin_boxGridSpacingAngle = parent.find_child("spin_boxGridSpacingAngle")
        if spin_boxGridSpacingAngle:
            spin_boxGridSpacingAngle.set_visible(visibility)

    def check_box_grid_center_on_origin_state_changed(self, checked):
        r" . "
        self.dialog["grid_center_on_origin"] = checked

        sender_obj = sender()
        if not sender_obj:
            return

        parent = sender_obj.parent()
        if not parent:
            return

        label_grid_center_x = parent.find_child("label_grid_center_x")
        if label_grid_center_x:
            label_grid_center_x.set_enabled(not self.dialog["grid_center_on_origin"])

        spin_boxGridCenterX = parent.find_child("spin_boxGridCenterX")
        if spin_boxGridCenterX:
            spin_boxGridCenterX.set_enabled(not self.dialog["grid_center_on_origin"])

        label_grid_center_y = parent.find_child("label_grid_center_y")
        if label_grid_center_y:
            label_grid_center_y.set_enabled(not self.dialog["grid_center_on_origin"])

        spin_box_grid_center_y = parent.find_child("spin_box_grid_center_y")
        if spin_box_grid_center_y:
            spin_box_grid_center_y.set_enabled(not self.dialog["grid_center_on_origin"])

    def combo_box_ruler_metric_current__index_changed(self, index):
        " . "
        combo_box = sender()
        if combo_box:
            ok = False
            self.dialog.ruler_metric = combo_box.itemData(index).toBool()
        else:
            self.dialog.ruler_metric = True

    def chooseRulerColor(self):
        " . "
        button = sender()
        if button:
            color_dialog = color_dialog(
                Color(accept_.ruler_color), this)
            # connect(color_dialog, SIGNAL(currentColorChanged()),
            # this, SLOT(currentRulerColorChanged()))
            color_dialog.exec()

            if color_dialog.result() == "QDialog_Accepted":
                self.accept.ruler_color = color_dialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_.ruler_color))
                button.setIcon(pix)
                self.mw.updateAllViewRulerColors(accept_.ruler_color)

            else:
                self.mw.updateAllViewRulerColors(self.dialog.ruler_color)

    def currentRulerColorChanged(color):
        " . "
        self.preview.ruler_color = color.rgb()
        self.mw.updateAllViewRulerColors(preview.ruler_color)

    def buttonQSnapSelectAllClicked():
        " . "
        buttonQSnapSelectAll(1)

    def buttonQSnapClearAllClicked():
        " . "
        buttonQSnapClearAll(0)

    #
    # TODO:
    # Figure out how to abstract the slot in a way that it can be used for
    # combo_boxes in general
    # Currently combo_boxQSnapLocatorColorCurrent_index_changed(int index)
    #        combo_boxSelectionCoolGripColorCurrent_index_changed(int index)
    #        combo_boxSelectionHotGripColorCurrent_index_changed(int index)
    # are all similar except the self.dialog. variable being worked on and the
    # tk.Variant.

    def combo_boxQSnapLocatorColorCurrent_index_changed(self, index):
        " . "
        # TODO: Alert user if color matched the display bg color
        combo_box = sender()
        default_color = tk.Rgb(255,255,0)
        # Yellow
        if combo_box:
            self.dialog.qsnap_locator_color, ok = combo_box.itemData(index).toUInt()
            if not ok:
                self.dialog.qsnap_locator_color = default_color
        else:
            self.dialog.qsnap_locator_color = default_color

    def sliderQSnap_locator_size_value_changed(self, value):
        " . "
        self.dialog["qsnap_locator_size"] = value

    def sliderQSnap_aperture_size_value_changed(self, value):
        " . "
        self.dialog["qsnap_aperture_size"] = value

    def check_boxLwtShowLwt_state_changed(self, checked):
        " . "
        self.preview.lwt_show_lwt = checked
        if self.preview.lwt_show_lwt:
            enable_lwt()
        else:
            disableLwt()

        sender_obj = sender()
        if sender_obj:
            parent = sender_obj.parent()
            if parent:
                check_boxRealRender = parent.find_child("check_boxRealRender")
                if check_boxRealRender:
                    check_boxRealRender.set_enabled(preview.lwt_show_lwt)

    def check_boxLwtRealRender_state_changed(self, checked):
        " . "
        self.preview["lwt_real_render"] = checked
        if self.preview["lwt_real_render"]:
            enableReal()
        else:
            disableReal()

    def combo_boxSelectionCoolGripColorCurrent_index_changed(self, index):
        " . "
        # TODO: Alert user if color matched the display bg color
        combo_box = sender()
        default_color = tk.Rgb(0,0,255)
        # Blue
        if combo_box:
            self.dialog["selection_coolgrip_color"], ok = combo_box.item_data(index).toUInt()
            if not ok:
                self.dialog["selection_coolgrip_color"] = default_color
        else:
            self.dialog["selection_coolgrip_color"] = default_color

    def combo_box_selectionHotGripColorCurrent_index_changed(self, index):
        " . "
        # TODO: Alert user if color matched the display bg color
        combo_box = sender()
        default_color = tk.Rgb(255,0,0)
        # Red
        if combo_box:
            self.dialog["selection_hotgrip_color"], ok = combo_box.item_data(index).toUInt()
            if not ok:
                self.dialog["selection_hotgrip_color"] = default_color
        else:
            self.dialog["selection_hotgrip_color"] = default_color

    def accept_changes(self):
        " . "
        for k in self.preview.keys():
            self.dialog[k] = self.preview[k]
        for k in self.accept.keys():
            self.dialog[k] = self.preview[k]

        if self.dialog["grid_color_match_crosshair"]:
            self.dialog["grid_color"] = self.accept["display_crosshair_color"]

        # Make sure the user sees the changes applied immediately.
        self.mw.mdi_area.use_background_logo(self.dialog["general_mdi_bg_use_logo"])
        self.mw.mdi_area.use_background_texture(self.dialog["general_mdi_bg_use_texture"])
        self.mw.mdi_area.use_background_color(self.dialog["general_mdi_bg_use_color"])
        self.mw.mdi_area.set_background_logo(self.dialog["general_mdi_bg_logo"])
        self.mw.mdi_area.set_background_texture(self.dialog["general_mdi_bg_texture"])
        self.mw.mdi_area.set_background_color(self.dialog["general_mdi_bg_color"])
        self.mw.icon_resize(self.dialog["general_icon_size"])
        self.mw.update_all_view_scrollBars(self.dialog["display_show_scrollbars"])
        self.mw.update_all_view_cross_hair_colors(self.dialog["display_crosshair_color"])
        self.mw.update_all_view_background_colors(self.dialog["display_bg_color"])
        self.mw.update_all_view_select_box_colors(
            self.dialog["display_selectbox_left_color"],
            self.dialog["display_selectbox_left_fill"],
            self.dialog["display_selectbox_right_color"],
            self.dialog["display_selectbox_right_fill"],
            self.dialog["display_selectbox_alpha"])
        self.mw.update_all_view_grid_colors(self.dialog["grid_color"])
        self.mw.update_all_view_ruler_colors(self.dialog["ruler_color"])

        if self.dialog["lwt_show_lwt"]:
            enable_lwt()
        else:
            disableLwt()

        if self.dialog["lwt_real_render"]:
            enableReal()
        else:
            disableReal()

        self.mw.update_pick_add_mode(self.dialog["selection_mode_pickadd"])

        self.mw.writeSettings()
        accept()

    def reject_changes(self):
        """
        TODO: inform the user if they have changed settings

        Update the view since the user must accept the self.preview
        """
        self.mw.mdi_area.use_background_logo(self.dialog["general_mdi_bg_use_logo"])
        self.mw.mdi_area.use_background_texture(self.dialog["general_mdi_bg_use_texture"])
        self.mw.mdi_area.use_background_color(self.dialog["general_mdi_bg_use_color"])
        self.mw.mdi_area.set_background_logo(self.dialog["general_mdi_bg_logo"])
        self.mw.mdi_area.set_background_texture(self.dialog["general_mdi_bg_texture"])
        self.mw.mdi_area.set_background_color(self.dialog["general_mdi_bg_color"])
        self.mw.update_all_view_scroll_bars(self.dialog["display_show_scrollbars"])
        self.mw.update_all_view_cross_hair_colors(self.dialog["display_crosshair_color"])
        self.mw.update_all_view_background_colors(self.dialog["display_bg_color"])
        self.mw.update_all_view_select_box_colors(
            self.dialog["display_selectbox_left_color"],
            self.dialog["display_selectbox_left_fill"],
            self.dialog["display_selectbox_right_color"],
            self.dialog["display_selectbox_right_fill"],
            self.dialog["display_selectbox_alpha"])
        self.mw.update_all_view_grid_colors(self.dialog["grid_color"])
        self.mw.update_all_view_ruler_colors(self.dialog["ruler_color"])

        if self.dialog["lwt_show_lwt"]:
            enable_lwt()
        else:
            disableLwt()

        if self.dialog["lwt_real_render"]:
            enableReal()
        else:
            disableReal()

        reject()
