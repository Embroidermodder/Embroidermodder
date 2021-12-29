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
"""

import tkinter as tk

from embroidermodder.utility import translate, settings

def loadIcon(fname):
    return 0

class SettingsDialog():
    """
    TabWidget* tabWidget
    DialogButtonBox* buttonBox
    def addColorsToComboBox(comboBox)
    createTabGeneral()
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
    def comboBoxLanguageCurrentIndexChanged("")
    def comboBoxIconThemeCurrentIndexChanged("")
    def comboBoxIconSizeCurrentIndexChanged(int)
    def checkBoxGeneralMdiBGUseLogoStateChanged(int)
    def chooseGeneralMdiBackgroundLogo()
    def checkBoxGeneralMdiBGUseTextureStateChanged(int)
    def chooseGeneralMdiBackgroundTexture()
    def checkBoxGeneralMdiBGUseColorStateChanged(int)
    def chooseGeneralMdiBackgroundColor()
    def currentGeneralMdiBackgroundColorChanged()
    def checkBoxShowScrollBarsStateChanged(int)
    def spinBoxZoomScaleInValueChanged(double)
    def spinBoxZoomScaleOutValueChanged(double)
    def checkBoxDisableBGStateChanged(int)
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
    def spinBoxDisplaySelectBoxAlphaValueChanged(int)
    def checkBoxCustomFilterStateChanged(int)
    def buttonCustomFilterSelectAllClicked()
    def buttonCustomFilterClearAllClicked()
    def chooseGridColor()
    def currentGridColorChanged()
    def checkBoxGridLoadFromFileStateChanged(int)
    def comboBoxGridTypeCurrentIndexChanged("")
    def chooseRulerColor()
    def currentRulerColorChanged()
    def buttonQSnapSelectAllClicked()
    def buttonQSnapClearAllClicked()
    def comboBoxQSnapLocatorColorCurrentIndexChanged(int)
    def sliderQSnapLocatorSizeValueChanged(int)
    def sliderQSnapApertureSizeValueChanged(int)
    def checkBoxLwtShowLwtStateChanged(int)
    def checkBoxLwtRealRenderStateChanged(int)
    def comboBoxSelectionCoolGripColorCurrentIndexChanged(int)
    def comboBoxSelectionHotGripColorCurrentIndexChanged(int)
    def checkBoxGridCenterOnOriginStateChanged(int)
    def comboBoxRulerMetricCurrentIndexChanged(int)
    def checkBoxGridColorMatchCrossHairStateChanged(int)
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
        tabWidget = tk.TabWidget(self.window)

        #TODO: Add icons to tabs
        tabWidget.addTab(createTabGeneral(), translate("General"))
        tabWidget.addTab(createTabFilesPaths(), translate("Files/Paths"))
        tabWidget.addTab(createTabDisplay(), translate("Display"))
        tabWidget.addTab(createTabOpenSave(), translate("Open/Save"))
        tabWidget.addTab(createTabPrinting(), translate("Printing"))
        tabWidget.addTab(createTabSnap(), translate("Snap"))
        tabWidget.addTab(createTabGridRuler(), translate("Grid/Ruler"))
        tabWidget.addTab(createTabOrthoPolar(), translate("Ortho/Polar"))
        tabWidget.addTab(createTabQuickSnap(), translate("QuickSnap"))
        tabWidget.addTab(createTabQuickTrack(), translate("QuickTrack"))
        tabWidget.addTab(createTabLineWeight(), translate("LineWeight"))
        tabWidget.addTab(createTabSelection(), translate("Selection"))

        for i in range(12):
            if showTab == settings_tab_label[i]:
                tabWidget.setCurrentIndex(i)

        buttonBox = tk.DialogButtonBox(QDialogButtonBox_Ok | tk.DialogButtonBox_Cancel)

        #connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()))
        #connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()))

        vboxLayoutMain = tk.VBoxLayout()
        vboxLayoutMain.addWidget(tabWidget)
        vboxLayoutMain.addWidget(buttonBox)
        setLayout(vboxLayoutMain)

        self.window.title(translate("Settings"))
        self.window.setOverrideCursor("ArrowCursor")

    def createTabGeneral(self):
        " . "
        widget = tk.Widget(self.window)

        #Language
        groupBoxLanguage = tk.GroupBox(translate("Language"), widget)

        labelLanguage = tk.Label(translate("Language (Requires Restart)"), groupBoxLanguage)
        comboBoxLanguage = tk.ComboBox(groupBoxLanguage)
        to_lower(self.dialog.general_language, settings.general_language)
        comboBoxLanguage.addItem("Default")
        comboBoxLanguage.addItem("System")
        comboBoxLanguage.insertSeparator(2)
        trDir = tk.App.applicationDirPath()
        trDir.cd("translations")
        for dirName in trDir.entryList(QDir_Dirs | tk.Dir_NoDotAndDotDot):
            dirName[0] = dirName[0].toUpper()
            comboBoxLanguage.addItem(dirName)

        current = self.dialog.general_language
        current[0] = current[0].toUpper()
        comboBoxLanguage.setCurrentIndex(comboBoxLanguage.findText(current))
        #connect(comboBoxLanguage, SIGNAL(currentIndexChanged("")), this, SLOT(comboBoxLanguageCurrentIndexChanged("")))

        vboxLayoutLanguage = tk.VBoxLayout(groupBoxLanguage)
        vboxLayoutLanguage.addWidget(labelLanguage)
        vboxLayoutLanguage.addWidget(comboBoxLanguage)
        groupBoxLanguage.setLayout(vboxLayoutLanguage)

        #Icons
        groupBoxIcon = tk.GroupBox(translate("Icons"), widget)

        labelIconTheme = tk.Label(translate("Icon Theme"), groupBoxIcon)
        comboBoxIconTheme = tk.ComboBox(groupBoxIcon)
        dir = tk.App.applicationDirPath()
        dir.cd("icons")
        self.dialog.general_icon_theme = settings.general_icon_theme
        for dirName in dir.entryList(QDir_Dirs | tk.Dir_NoDotAndDotDot):
            comboBoxIconTheme.addItem(loadIcon(theme_xpm), dirName)

        comboBoxIconTheme.setCurrentIndex(comboBoxIconTheme.findText(self.dialog.general_icon_theme))
        #connect(comboBoxIconTheme, SIGNAL(currentIndexChanged("")), this, SLOT(comboBoxIconThemeCurrentIndexChanged("")))

        labelIconSize = tk.Label(translate("Icon Size"), groupBoxIcon)
        comboBoxIconSize = tk.ComboBox(groupBoxIcon)
        comboBoxIconSize.addItem(loadIcon("icon16_xpm"), "Very Small", 16)
        comboBoxIconSize.addItem(loadIcon("icon24_xpm"), "Small", 24)
        comboBoxIconSize.addItem(loadIcon("icon32_xpm"), "Medium", 32)
        comboBoxIconSize.addItem(loadIcon("icon48_xpm"), "Large", 48)
        comboBoxIconSize.addItem(loadIcon("icon64_xpm"), "Very Large", 64)
        comboBoxIconSize.addItem(loadIcon("icon128_xpm"), "I'm Blind", 128)
        self.dialog["general_icon_size"] = settings["general_icon_size"]
        comboBoxIconSize.setCurrentIndex(comboBoxIconSize.findData(self.dialog.general_icon_size))
        #connect(comboBoxIconSize, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxIconSizeCurrentIndexChanged(int)))

        vboxLayoutIcon = tk.QVBoxLayout(groupBoxIcon)
        vboxLayoutIcon.addWidget(labelIconTheme)
        vboxLayoutIcon.addWidget(comboBoxIconTheme)
        vboxLayoutIcon.addWidget(labelIconSize)
        vboxLayoutIcon.addWidget(comboBoxIconSize)
        groupBoxIcon.setLayout(vboxLayoutIcon)

        #Mdi Background
        groupBoxMdiBG = tk.GroupBox(self.window, text=translate("Background"))

        checkBoxMdiBGUseLogo = tk.CheckBox(groupBoxMdiBG, text=translate("Use Logo"))
        self.dialog["general_mdi_bg_use_logo"] = settings["general_mdi_bg_use_logo"]
        self.preview["general_mdi_bg_use_logo"] = self.dialog["general_mdi_bg_use_logo"]
        checkBoxMdiBGUseLogo.setChecked(self.preview["general_mdi_bg_use_logo"])
        #connect(checkBoxMdiBGUseLogo, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseLogoStateChanged(int)))

        buttonMdiBGLogo = tk.Button(groupBoxMdiBG, text=translate("Choose"))
        buttonMdiBGLogo.setEnabled(self.dialog["general_mdi_bg_use_logo"])
        self.dialog["general_mdi_bg_logo"] = settings["general_mdi_bg_logo"]
        self.accept["general_mdi_bg_logo"] = self.dialog["general_mdi_bg_logo"]
        #connect(buttonMdiBGLogo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()))
        #connect(checkBoxMdiBGUseLogo, SIGNAL(toggled(int)), buttonMdiBGLogo, SLOT(setEnabled(int)))

        checkBoxMdiBGUseTexture = tk.CheckBox(translate("Use Texture"), groupBoxMdiBG)
        self.dialog["general_mdi_bg_use_texture"] = settings["general_mdi_bg_use_texture"]
        self.preview["general_mdi_bg_use_texture"] = self.dialog["general_mdi_bg_use_texture"]
        checkBoxMdiBGUseTexture.setChecked(self.preview["general_mdi_bg_use_texture"])
        #connect(checkBoxMdiBGUseTexture, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseTextureStateChanged(int)))

        buttonMdiBGTexture = tk.Button(groupBoxMdiBG, text=translate("Choose"))
        buttonMdiBGTexture.setEnabled(self.dialog["general_mdi_bg_use_texture"])
        self.dialog["general_mdi_bg_texture"] = settings["general_mdi_bg_texture"]
        self.accept["general_mdi_bg_texture"] = self.dialog["general_mdi_bg_texture"]
        #connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()))
        #connect(checkBoxMdiBGUseTexture, SIGNAL(toggled(int)), buttonMdiBGTexture, SLOT(setEnabled(int)))

        checkBoxMdiBGUseColor = tk.CheckBox(translate("Use Color"), groupBoxMdiBG)
        self.dialog["general_mdi_bg_use_color"] = settings["general_mdi_bg_use_color"]
        self.preview["general_mdi_bg_use_color"] = self.dialog["general_mdi_bg_use_color"]
        checkBoxMdiBGUseColor.setChecked(preview.general_mdi_bg_use_color)
        #connect(checkBoxMdiBGUseColor, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGeneralMdiBGUseColorStateChanged(int)))

        buttonMdiBGColor = PushButton(translate("Choose"), groupBoxMdiBG)
        buttonMdiBGColor.setEnabled(self.dialog.general_mdi_bg_use_color)
        self.dialog["general_mdi_bg_color"] = settings["general_mdi_bg_color"]
        self.preview["general_mdi_bg_color"] = self.dialog["general_mdi_bg_color"]
        self.accept["general_mdi_bg_color"] = self.dialog["general_mdi_bg_color"]
        mdiBGPix = Image(16,16)
        mdiBGPix.fill(Color(preview.general_mdi_bg_color))
        buttonMdiBGColor.setIcon(QIcon(mdiBGPix))
        #connect(buttonMdiBGColor, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundColor()))
        #connect(checkBoxMdiBGUseColor, SIGNAL(toggled(int)), buttonMdiBGColor, SLOT(setEnabled(int)))

        gridLayoutMdiBG = tk.GridLayout(widget)
        gridLayoutMdiBG.addWidget(checkBoxMdiBGUseLogo, 0, 0, "Align Left")
        gridLayoutMdiBG.addWidget(buttonMdiBGLogo, 0, 1, "Align Right")
        gridLayoutMdiBG.addWidget(checkBoxMdiBGUseTexture, 1, 0, "Align Left")
        gridLayoutMdiBG.addWidget(buttonMdiBGTexture, 1, 1, "Align Right")
        gridLayoutMdiBG.addWidget(checkBoxMdiBGUseColor, 2, 0, "Align Left")
        gridLayoutMdiBG.addWidget(buttonMdiBGColor, 2, 1, "Align Right")
        groupBoxMdiBG.setLayout(gridLayoutMdiBG)

        #Tips
        groupBoxTips = tk.GroupBox(translate("Tips"), widget)

        checkBoxTipOfTheDay = tk.CheckBox(translate("Show Tip of the Day on startup"), groupBoxTips)
        self.dialog.general_tip_of_the_day = settings.general_tip_of_the_day
        checkBoxTipOfTheDay.setChecked(self.dialog.general_tip_of_the_day)
        #connect(checkBoxTipOfTheDay, SIGNAL(stateChanged(int)), this, SLOT(checkBoxTipOfTheDayStateChanged(int)))

        vboxLayoutTips = tk.VBoxLayout(groupBoxTips)
        vboxLayoutTips.addWidget(checkBoxTipOfTheDay)
        groupBoxTips.setLayout(vboxLayoutTips)

        #Help Browser
        groupBoxHelpBrowser = tk.GroupBox(translate("Help Browser"), widget)

        radioButtonSystemHelpBrowser = tk.RadioButton(translate("System"), groupBoxHelpBrowser)
        radioButtonSystemHelpBrowser.setChecked(settings.general_system_help_browser)
        radioButtonCustomHelpBrowser = tk.RadioButton(translate("Custom"), groupBoxHelpBrowser)
        radioButtonCustomHelpBrowser.setChecked(not settings.general_system_help_browser)
        radioButtonCustomHelpBrowser.setEnabled(0); #TODO: finish this

        vboxLayoutHelpBrowser = tk.VBoxLayout(groupBoxHelpBrowser)
        vboxLayoutHelpBrowser.addWidget(radioButtonSystemHelpBrowser)
        vboxLayoutHelpBrowser.addWidget(radioButtonCustomHelpBrowser)
        groupBoxHelpBrowser.setLayout(vboxLayoutHelpBrowser)

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxLanguage)
        vboxLayoutMain.addWidget(groupBoxIcon)
        vboxLayoutMain.addWidget(groupBoxMdiBG)
        vboxLayoutMain.addWidget(groupBoxTips)
        vboxLayoutMain.addWidget(groupBoxHelpBrowser)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabFilesPaths(self):
        widget = tk.Widget(this)

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabDisplay(self):
        " . "
        widget = tk.Widget(this)

        #Rendering
        #TODO: Review OpenGL and Rendering settings for future inclusion
        #
        groupBoxRender = tk.GroupBox(translate("Rendering"), widget)

        checkBoxUseOpenGL = tk.CheckBox(translate("Use OpenGL"), groupBoxRender)
        self.dialog.display_use_opengl = settings.display_use_open_gl
        checkBoxUseOpenGL.setChecked(self.dialog.display_use_opengl)
        #connect(checkBoxUseOpenGL, SIGNAL(stateChanged(int)), this, SLOT(checkBoxUseOpenGLStateChanged(int)))

        checkBoxRenderHintAA = tk.CheckBox(translate("Antialias"), groupBoxRender)
        self.dialog.display_renderhint_aa = settings.display_render_hint_aa
        checkBoxRenderHintAA.setChecked(self.dialog.display_renderhint_aa)
        #connect(checkBoxRenderHintAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintAAStateChanged(int)))

        checkBoxRenderHintTextAA = tk.CheckBox(translate("Antialias Text"), groupBoxRender)
        self.dialog["display_renderhint_text_aa"] = settings["display_render_hint_text_aa"]
        checkBoxRenderHintTextAA.setChecked(self.dialog["display_renderhint_text_aa"])
        #connect(checkBoxRenderHintTextAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintTextAAStateChanged(int)))

        checkBoxRenderHintSmoothPix = tk.CheckBox(translate("Smooth Pixmap"), groupBoxRender)
        self.dialog["display_renderhint_smooth_pix"] = settings["display_render_hint_smooth_pix"]
        checkBoxRenderHintSmoothPix.setChecked(self.dialog.display_renderhint_smooth_pix)
        #connect(checkBoxRenderHintSmoothPix, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintSmoothPixStateChanged(int)))

        checkBoxRenderHintHighAA = tk.CheckBox(translate("High Quality Antialiasing (OpenGL)"), groupBoxRender)
        self.dialog["display_renderhint_high_aa"] = settings["display_render_hint_high_aa"]
        checkBoxRenderHintHighAA.setChecked(self.dialog["display_renderhint_high_aa"])
        #connect(checkBoxRenderHintHighAA, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintHighAAStateChanged(int)))

        checkBoxRenderHintNonCosmetic = tk.CheckBox(translate("Non Cosmetic"), groupBoxRender)
        self.dialog.display_renderhint_noncosmetic = settings.display_render_hint_non_cosmetic
        checkBoxRenderHintNonCosmetic.setChecked(self.dialog.display_renderhint_noncosmetic)
        #connect(checkBoxRenderHintNonCosmetic, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRenderHintNonCosmeticStateChanged(int)))

        vboxLayoutRender = tk.VBoxLayout(groupBoxRender)
        vboxLayoutRender.addWidget(checkBoxUseOpenGL)
        vboxLayoutRender.addWidget(checkBoxRenderHintAA)
        vboxLayoutRender.addWidget(checkBoxRenderHintTextAA)
        vboxLayoutRender.addWidget(checkBoxRenderHintSmoothPix)
        vboxLayoutRender.addWidget(checkBoxRenderHintHighAA)
        vboxLayoutRender.addWidget(checkBoxRenderHintNonCosmetic)
        groupBoxRender.setLayout(vboxLayoutRender)

        #ScrollBars
        groupBoxScrollBars = tk.GroupBox(translate("ScrollBars"), widget)

        checkBoxShowScrollBars = tk.CheckBox(translate("Show ScrollBars"), groupBoxScrollBars)
        self.dialog.display_show_scrollbars = settings.display_show_scrollbars
        self.preview.display_show_scrollbars = self.dialog.display_show_scrollbars
        checkBoxShowScrollBars.setChecked(preview.display_show_scrollbars)
        #connect(checkBoxShowScrollBars, SIGNAL(stateChanged(int)), this, SLOT(checkBoxShowScrollBarsStateChanged(int)))

        labelScrollBarWidget = tk.Label(translate("Perform action when clicking corner widget"), groupBoxScrollBars)
        comboBoxScrollBarWidget = tk.ComboBox(groupBoxScrollBars)
        self.dialog.display_scrollbar_widget_num = settings.display_scrollbar_widget_num
        numActions = self.mw.actionHash.size()
        for i in range(numActions):
            action = self.mw.actionHash.value(i)
            if action:
                comboBoxScrollBarWidget.addItem(action.icon(), action.text().replace("&", ""))

        comboBoxScrollBarWidget.setCurrentIndex(self.dialog.display_scrollbar_widget_num)
        #connect(comboBoxScrollBarWidget, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxScrollBarWidgetCurrentIndexChanged(int)))

        vboxLayoutScrollBars = tk.VBoxLayout(groupBoxScrollBars)
        vboxLayoutScrollBars.addWidget(checkBoxShowScrollBars)
        vboxLayoutScrollBars.addWidget(labelScrollBarWidget)
        vboxLayoutScrollBars.addWidget(comboBoxScrollBarWidget)
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
        spinBoxSelectBoxAlpha = tk.SpinBox(groupBoxColor)
        spinBoxSelectBoxAlpha.setRange(0, 255)
        self.dialog.display_selectbox_alpha = settings.display_selectbox_alpha
        self.preview.display_selectbox_alpha = self.dialog.display_selectbox_alpha
        spinBoxSelectBoxAlpha.setValue(preview.display_selectbox_alpha)
        #connect(spinBoxSelectBoxAlpha, SIGNAL(valueChanged(int)), this, SLOT(spinBoxDisplaySelectBoxAlphaValueChanged(int)))

        gridLayoutColor = tk.GridLayout(widget)
        gridLayoutColor.addWidget(labelCrossHairColor, 0, 0, "Align Left")
        gridLayoutColor.addWidget(buttonCrossHairColor, 0, 1, "Align Right")
        gridLayoutColor.addWidget(labelBGColor, 1, 0, "Align Left")
        gridLayoutColor.addWidget(buttonBGColor, 1, 1, "Align Right")
        gridLayoutColor.addWidget(labelSelectBoxLeftColor, 2, 0, "Align Left")
        gridLayoutColor.addWidget(buttonSelectBoxLeftColor, 2, 1, "Align Right")
        gridLayoutColor.addWidget(labelSelectBoxLeftFill, 3, 0, "Align Left")
        gridLayoutColor.addWidget(buttonSelectBoxLeftFill, 3, 1, "Align Right")
        gridLayoutColor.addWidget(labelSelectBoxRightColor, 4, 0, "Align Left")
        gridLayoutColor.addWidget(buttonSelectBoxRightColor, 4, 1, "Align Right")
        gridLayoutColor.addWidget(labelSelectBoxRightFill, 5, 0, "Align Left")
        gridLayoutColor.addWidget(buttonSelectBoxRightFill, 5, 1, "Align Right")
        gridLayoutColor.addWidget(labelSelectBoxAlpha, 6, 0, "Align Left")
        gridLayoutColor.addWidget(spinBoxSelectBoxAlpha, 6, 1, "Align Right")
        groupBoxColor.setLayout(gridLayoutColor)

        #Zoom
        groupBoxZoom = tk.GroupBox(translate("Zoom"), widget)

        labelZoomScaleIn = tk.Label(translate("Zoom In Scale"), groupBoxZoom)
        spinBoxZoomScaleIn = tk.DoubleSpinBox(groupBoxZoom)
        self.dialog.display_zoomscale_in = settings.display_zoomscale_in
        spinBoxZoomScaleIn.setValue(self.dialog.display_zoomscale_in)
        spinBoxZoomScaleIn.setSingleStep(0.01)
        spinBoxZoomScaleIn.setRange(1.01, 10.00)
        #connect(spinBoxZoomScaleIn, SIGNAL(valueChanged(double)), this, SLOT(spinBoxZoomScaleInValueChanged(double)))

        labelZoomScaleOut = tk.Label(translate("Zoom Out Scale"), groupBoxZoom)
        spinBoxZoomScaleOut = tk.DoubleSpinBox(groupBoxZoom)
        self.dialog.display_zoomscale_out = settings.display_zoomscale_out
        spinBoxZoomScaleOut.setValue(self.dialog.display_zoomscale_out)
        spinBoxZoomScaleOut.setSingleStep(0.01)
        spinBoxZoomScaleOut.setRange(0.01, 0.99)
        #connect(spinBoxZoomScaleOut, SIGNAL(valueChanged(double)), this, SLOT(spinBoxZoomScaleOutValueChanged(double)))

        gridLayoutZoom = tk.GridLayout(groupBoxZoom)
        gridLayoutZoom.addWidget(labelZoomScaleIn, 0, 0, "Align Left")
        gridLayoutZoom.addWidget(spinBoxZoomScaleIn, 0, 1, "Align Right")
        gridLayoutZoom.addWidget(labelZoomScaleOut, 1, 0, "Align Left")
        gridLayoutZoom.addWidget(spinBoxZoomScaleOut, 1, 1, "Align Right")
        groupBoxZoom.setLayout(gridLayoutZoom)

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        #vboxLayoutMain.addWidget(groupBoxRender)
        # TODO: Review OpenGL and Rendering settings for future inclusion
        vboxLayoutMain.addWidget(groupBoxScrollBars)
        vboxLayoutMain.addWidget(groupBoxColor)
        vboxLayoutMain.addWidget(groupBoxZoom)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabOpenSave(self):
        " . "
        " TODO: finish open/save options"
        widget = tk.Widget(this)

        #Custom Filter
        groupBoxCustomFilter = tk.GroupBox(translate("Custom Filter"), widget)
        groupBoxCustomFilter.setEnabled(0); #TODO: Fixup custom filter

        buttonCustomFilterSelectAll = PushButton(translate("Select All"), groupBoxCustomFilter)
        #connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()))
        buttonCustomFilterClearAll = PushButton("Clear All", groupBoxCustomFilter)
        #connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()))
        gridLayoutCustomFilter = tk.GridLayout(groupBoxCustomFilter)

        for i in range(numberOfFormats):
            c = CheckBox(formatTable[i].extension, groupBoxCustomFilter)
            c.setChecked(opensave_custom_filter.contains(QString("*") + formatTable[i].extension, tk.t_CaseInsensitive))
            #connect(c, SIGNAL(stateChanged(int)), this, SLOT(checkBoxCustomFilterStateChanged(int)))
            #connect(this, SIGNAL(buttonCustomFilterSelectAll(int)), c, SLOT(setChecked(int)))
            #connect(this, SIGNAL(buttonCustomFilterClearAll(int)), c, SLOT(setChecked(int)))
            gridLayoutCustomFilter.addWidget(c, i%10, i/10, "Align Left")

        gridLayoutCustomFilter.addWidget(buttonCustomFilterSelectAll, 0, 7, "Align Left")
        gridLayoutCustomFilter.addWidget(buttonCustomFilterClearAll, 1, 7, "Align Left")
        gridLayoutCustomFilter.setColumnStretch(7,1)
        groupBoxCustomFilter.setLayout(gridLayoutCustomFilter)

        if opensave_custom_filter.contains("supported", tk.t_CaseInsensitive):
            buttonCustomFilterSelectAllClicked()

        # Opening
        groupBoxOpening = tk.GroupBox(translate("File Open"), widget)

        comboBoxOpenFormat = tk.ComboBox(groupBoxOpening)

        checkBoxOpenThumbnail = tk.CheckBox(translate("Preview Thumbnails"), groupBoxOpening)
        checkBoxOpenThumbnail.setChecked(0)

        # TODO: Add a button to clear the recent history.

        labelRecentMaxFiles = tk.Label(translate("Number of recently accessed files to show"), groupBoxOpening)
        spinBoxRecentMaxFiles = tk.SpinBox(groupBoxOpening)
        spinBoxRecentMaxFiles.setRange(0, 10)
        self.dialog.opensave_recent_max_files = settings.opensave_recent_max_files
        spinBoxRecentMaxFiles.setValue(self.dialog.opensave_recent_max_files)
        #connect(spinBoxRecentMaxFiles, SIGNAL(valueChanged(int)), this, SLOT(spinBoxRecentMaxFilesValueChanged(int)))

        frameRecent = tk.Frame(groupBoxOpening)
        gridLayoutRecent = tk.GridLayout(frameRecent)
        gridLayoutRecent.addWidget(labelRecentMaxFiles, 0, 0, "AlignLeft")
        gridLayoutRecent.addWidget(spinBoxRecentMaxFiles, 0, 1, "Align Right")
        frameRecent.setLayout(gridLayoutRecent)

        vboxLayoutOpening = tk.VBoxLayout(groupBoxOpening)
        vboxLayoutOpening.addWidget(comboBoxOpenFormat)
        vboxLayoutOpening.addWidget(checkBoxOpenThumbnail)
        vboxLayoutOpening.addWidget(frameRecent)
        groupBoxOpening.setLayout(vboxLayoutOpening)

        #Saving
        groupBoxSaving = tk.GroupBox(translate("File Save"), widget)

        comboBoxSaveFormat = tk.ComboBox(groupBoxSaving)

        checkBoxSaveThumbnail = tk.CheckBox(translate("Save Thumbnails"), groupBoxSaving)
        checkBoxSaveThumbnail.setChecked(0)

        checkBoxAutoSave = tk.CheckBox(translate("AutoSave"), groupBoxSaving)
        checkBoxAutoSave.setChecked(0)

        vboxLayoutSaving = tk.VBoxLayout(groupBoxSaving)
        vboxLayoutSaving.addWidget(comboBoxSaveFormat)
        vboxLayoutSaving.addWidget(checkBoxSaveThumbnail)
        vboxLayoutSaving.addWidget(checkBoxAutoSave)
        groupBoxSaving.setLayout(vboxLayoutSaving)

        #Trimming
        groupBoxTrim = tk.GroupBox(translate("Trimming"), widget)

        labelTrimDstNumJumps = tk.Label(translate("DST Only: Minimum number of jumps to trim"), groupBoxTrim)
        spinBoxTrimDstNumJumps = tk.SpinBox(groupBoxTrim)
        spinBoxTrimDstNumJumps.setRange(1, 20)
        self.dialog.opensave_trim_dst_num_jumps = settings.opensave_trim_dst_num_jumps
        spinBoxTrimDstNumJumps.setValue(self.dialog.opensave_trim_dst_num_jumps)
        #connect(spinBoxTrimDstNumJumps, SIGNAL(valueChanged(int)), this, SLOT(spinBoxTrimDstNumJumpsValueChanged(int)))

        frameTrimDstNumJumps = tk.Frame(groupBoxTrim)
        gridLayoutTrimDstNumJumps = tk.GridLayout(frameTrimDstNumJumps)
        gridLayoutTrimDstNumJumps.addWidget(labelTrimDstNumJumps, 0, 0, "Align Left")
        gridLayoutTrimDstNumJumps.addWidget(spinBoxTrimDstNumJumps, 0, 1, "Align Right")
        frameTrimDstNumJumps.setLayout(gridLayoutTrimDstNumJumps)

        vboxLayoutTrim = tk.VBoxLayout(groupBoxTrim)
        vboxLayoutTrim.addWidget(frameTrimDstNumJumps)
        groupBoxTrim.setLayout(vboxLayoutTrim)

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxCustomFilter)
        vboxLayoutMain.addWidget(groupBoxOpening)
        vboxLayoutMain.addWidget(groupBoxSaving)
        vboxLayoutMain.addWidget(groupBoxTrim)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabPrinting(self):
        " . "
        widget = tk.Widget(this)

        # Default Printer
        groupBoxDefaultPrinter = tk.GroupBox(translate("Default Printer"), widget)

        radioButtonUseSame = tk.RadioButton(translate("Use as default device"), groupBoxDefaultPrinter)
        radioButtonUseSame.setChecked(not settings.printing_use_last_device)
        radioButtonUseLast = tk.RadioButton(translate("Use last used device"), groupBoxDefaultPrinter)
        radioButtonUseLast.setChecked(settings.printing_use_last_device)

        comboBoxDefaultDevice = tk.ComboBox(groupBoxDefaultPrinter)
        listAvailPrinters = tk.PrinterInfo_availablePrinters()
        for info in listAvailPrinters:
            comboBoxDefaultDevice.addItem(loadIcon(print_xpm), info.printerName())

        vboxLayoutDefaultPrinter = tk.VBoxLayout(groupBoxDefaultPrinter)
        vboxLayoutDefaultPrinter.addWidget(radioButtonUseSame)
        vboxLayoutDefaultPrinter.addWidget(comboBoxDefaultDevice)
        vboxLayoutDefaultPrinter.addWidget(radioButtonUseLast)
        groupBoxDefaultPrinter.setLayout(vboxLayoutDefaultPrinter)

        # Save Ink
        groupBoxSaveInk = tk.GroupBox(translate("Save Ink"), widget)

        checkBoxDisableBG = tk.CheckBox(translate("Disable Background"), groupBoxSaveInk)
        self.dialog.printing_disable_bg = settings.printing_disable_bg
        checkBoxDisableBG.setChecked(self.dialog.printing_disable_bg)
        #connect(checkBoxDisableBG, SIGNAL(stateChanged(int)), this, SLOT(checkBoxDisableBGStateChanged(int)))

        vboxLayoutSaveInk = tk.VBoxLayout(groupBoxSaveInk)
        vboxLayoutSaveInk.addWidget(checkBoxDisableBG)
        groupBoxSaveInk.setLayout(vboxLayoutSaveInk)

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxDefaultPrinter)
        vboxLayoutMain.addWidget(groupBoxSaveInk)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)


        scrollArea = tk.ScrollArea(this)
        #scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget);
        return scrollArea

    def createTabSnap():
        widget = tk.Widget(this)

        #TODO: finish this

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabGridRuler():
        widget = tk.Widget(this)

        #Grid Misc
        groupBoxGridMisc = tk.GroupBox(translate("Grid Misc"), widget)

        checkBoxGridShowOnLoad = tk.CheckBox(translate("Initially show grid when loading a file"), groupBoxGridMisc)
        self.dialog.grid_show_on_load = settings.grid_show_on_load
        checkBoxGridShowOnLoad.setChecked(self.dialog.grid_show_on_load)
        #connect(checkBoxGridShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOnLoadStateChanged(int)))

        checkBoxGridShowOrigin = tk.CheckBox(translate("Show the origin when the grid is enabled"), groupBoxGridMisc)
        self.dialog.grid_show_origin = settings.grid_show_origin
        checkBoxGridShowOrigin.setChecked(self.dialog.grid_show_origin)
        #connect(checkBoxGridShowOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridShowOriginStateChanged(int)))

        gridLayoutGridMisc = tk.GridLayout(widget)
        gridLayoutGridMisc.addWidget(checkBoxGridShowOnLoad, 0, 0, "Align Left")
        gridLayoutGridMisc.addWidget(checkBoxGridShowOrigin, 1, 0, "Align Left")
        groupBoxGridMisc.setLayout(gridLayoutGridMisc)

        #Grid Color
        groupBoxGridColor = tk.GroupBox(translate("Grid Color"), widget)

        checkBoxGridColorMatchCrossHair = tk.CheckBox(translate("Match grid color to crosshair color"), groupBoxGridColor)
        self.dialog.grid_color_match_crosshair = settings.grid_color_match_crosshair
        checkBoxGridColorMatchCrossHair.setChecked(self.dialog.grid_color_match_crosshair)
        #connect(checkBoxGridColorMatchCrossHair, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridColorMatchCrossHairStateChanged(int)))

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

        labelGridColor.setEnabled(not self.dialog.grid_color_match_crosshair)
        buttonGridColor.setEnabled(not self.dialog.grid_color_match_crosshair)

        gridLayoutGridColor = tk.GridLayout(widget)
        gridLayoutGridColor.addWidget(checkBoxGridColorMatchCrossHair, 0, 0, "Align Left")
        gridLayoutGridColor.addWidget(labelGridColor, 1, 0, "Align Left")
        gridLayoutGridColor.addWidget(buttonGridColor, 1, 1, "Align Right")
        groupBoxGridColor.setLayout(gridLayoutGridColor)

        #Grid Geometry
        groupBoxGridGeom = tk.GroupBox(translate("Grid Geometry"), widget)

        checkBoxGridLoadFromFile = tk.CheckBox(translate("Set grid size from opened file"), groupBoxGridGeom)
        self.dialog["grid_load_from_file"] = settings["grid_load_from_file"]
        checkBoxGridLoadFromFile.setChecked(self.dialog["grid_load_from_file"])
        #connect(checkBoxGridLoadFromFile, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridLoadFromFileStateChanged(int)))

        labelGridType = tk.Label(translate("Grid Type"), groupBoxGridGeom)
        labelGridType.setObjectName("labelGridType")
        comboBoxGridType = tk.ComboBox(groupBoxGridGeom)
        comboBoxGridType.setObjectName("comboBoxGridType")
        comboBoxGridType.addItem("Rectangular")
        comboBoxGridType.addItem("Circular")
        comboBoxGridType.addItem("Isometric")
        strcpy(self.dialog.grid_type, settings.grid_type)
        comboBoxGridType.setCurrentIndex(comboBoxGridType.findText(self.dialog.grid_type))
        # #connect(comboBoxGridType, SIGNAL(currentIndexChanged("")), this, SLOT(comboBoxGridTypeCurrentIndexChanged("")))

        checkBoxGridCenterOnOrigin = tk.CheckBox(translate("Center the grid on the origin"), groupBoxGridGeom)
        checkBoxGridCenterOnOrigin.setObjectName("checkBoxGridCenterOnOrigin")
        self.dialog["grid_center_on_origin"] = settings["grid_center_on_origin"]
        checkBoxGridCenterOnOrigin.setChecked(self.dialog["grid_center_on_origin"])
        #connect(checkBoxGridCenterOnOrigin, SIGNAL(stateChanged(int)), this, SLOT(checkBoxGridCenterOnOriginStateChanged(int)))

        labelGridCenterX = tk.Label(translate("Grid Center X"), groupBoxGridGeom)
        labelGridCenterX.setObjectName("labelGridCenterX")
        spinBoxGridCenterX = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridCenterX.setObjectName("spinBoxGridCenterX")
        self.dialog.grid_center.x = settings.grid_center.x
        spinBoxGridCenterX.setSingleStep(1.000)
        spinBoxGridCenterX.setRange(-1000.000, 1000.000)
        spinBoxGridCenterX.setValue(self.dialog.grid_center.x)
        #connect(spinBoxGridCenterX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridCenterXValueChanged(double)))

        labelGridCenterY = tk.Label(translate("Grid Center Y"), groupBoxGridGeom)
        labelGridCenterY.setObjectName("labelGridCenterY")
        spinBoxGridCenterY = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridCenterY.setObjectName("spinBoxGridCenterY")
        self.dialog.grid_center.y = settings.grid_center.y
        spinBoxGridCenterY.setSingleStep(1.000)
        spinBoxGridCenterY.setRange(-1000.000, 1000.000)
        spinBoxGridCenterY.setValue(self.dialog.grid_center.y)
        #connect(spinBoxGridCenterY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridCenterYValueChanged(double)))

        labelGridSizeX = tk.Label(translate("Grid Size X"), groupBoxGridGeom)
        labelGridSizeX.setObjectName("labelGridSizeX")
        spinBoxGridSizeX = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSizeX.setObjectName("spinBoxGridSizeX")
        self.dialog.grid_size.x = settings.grid_size.x
        spinBoxGridSizeX.setSingleStep(1.000)
        spinBoxGridSizeX.setRange(1.000, 1000.000)
        spinBoxGridSizeX.setValue(self.dialog.grid_size.x)
        #connect(spinBoxGridSizeX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeXValueChanged(double)))

        labelGridSizeY = tk.Label(translate("Grid Size Y"), groupBoxGridGeom)
        labelGridSizeY.setObjectName("labelGridSizeY")
        spinBoxGridSizeY = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSizeY.setObjectName("spinBoxGridSizeY")
        self.dialog.grid_size.y = settings.grid_size.y
        spinBoxGridSizeY.setSingleStep(1.000)
        spinBoxGridSizeY.setRange(1.000, 1000.000)
        spinBoxGridSizeY.setValue(self.dialog.grid_size.y)
        #connect(spinBoxGridSizeY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeYValueChanged(double)))

        labelGridSpacingX = tk.Label(translate("Grid Spacing X"), groupBoxGridGeom)
        labelGridSpacingX.setObjectName("labelGridSpacingX")
        spinBoxGridSpacingX = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSpacingX.setObjectName("spinBoxGridSpacingX")
        self.dialog.grid_spacing.x = settings.grid_spacing.x
        spinBoxGridSpacingX.setSingleStep(1.000)
        spinBoxGridSpacingX.setRange(0.001, 1000.000)
        spinBoxGridSpacingX.setValue(self.dialog.grid_spacing.x)
        #connect(spinBoxGridSpacingX, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingXValueChanged(double)))

        labelGridSpacingY = tk.Label(translate("Grid Spacing Y"), groupBoxGridGeom)
        labelGridSpacingY.setObjectName("labelGridSpacingY")
        spinBoxGridSpacingY = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSpacingY.setObjectName("spinBoxGridSpacingY")
        self.dialog.grid_spacing.y = settings.grid_spacing.y
        spinBoxGridSpacingY.setSingleStep(1.000)
        spinBoxGridSpacingY.setRange(0.001, 1000.000)
        spinBoxGridSpacingY.setValue(self.dialog.grid_spacing.y)
        #connect(spinBoxGridSpacingY, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingYValueChanged(double)))

        labelGridSizeRadius = tk.Label(translate("Grid Size Radius"), groupBoxGridGeom)
        labelGridSizeRadius.setObjectName("labelGridSizeRadius")
        spinBoxGridSizeRadius = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSizeRadius.setObjectName("spinBoxGridSizeRadius")
        self.dialog.grid_size_radius = settings.grid_size_radius
        spinBoxGridSizeRadius.setSingleStep(1.000)
        spinBoxGridSizeRadius.setRange(1.000, 1000.000)
        spinBoxGridSizeRadius.setValue(self.dialog.grid_size_radius)
        #connect(spinBoxGridSizeRadius, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSizeRadiusValueChanged(double)))

        labelGridSpacingRadius = tk.Label(translate("Grid Spacing Radius"), groupBoxGridGeom)
        labelGridSpacingRadius.setObjectName("labelGridSpacingRadius")
        spinBoxGridSpacingRadius = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSpacingRadius.setObjectName("spinBoxGridSpacingRadius")
        self.dialog.grid_spacing_radius = settings.grid_spacing_radius
        spinBoxGridSpacingRadius.setSingleStep(1.000)
        spinBoxGridSpacingRadius.setRange(0.001, 1000.000)
        spinBoxGridSpacingRadius.setValue(self.dialog.grid_spacing_radius)
        #connect(spinBoxGridSpacingRadius, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingRadiusValueChanged(double)))

        labelGridSpacingAngle = tk.Label(translate("Grid Spacing Angle"), groupBoxGridGeom)
        labelGridSpacingAngle.setObjectName("labelGridSpacingAngle")
        spinBoxGridSpacingAngle = tk.DoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSpacingAngle.setObjectName("spinBoxGridSpacingAngle")
        self.dialog.grid_spacing_angle = settings.grid_spacing_angle
        spinBoxGridSpacingAngle.setSingleStep(1.000)
        spinBoxGridSpacingAngle.setRange(0.001, 1000.000)
        spinBoxGridSpacingAngle.setValue(self.dialog.grid_spacing_angle)
        #connect(spinBoxGridSpacingAngle, SIGNAL(valueChanged(double)), this, SLOT(spinBoxGridSpacingAngleValueChanged(double)))

        labelGridType.setEnabled(not self.dialog["grid_load_from_file"])
        comboBoxGridType.setEnabled(not self.dialog["grid_load_from_file"])
        checkBoxGridCenterOnOrigin.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridCenterX.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridCenterX.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridCenterY.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridCenterY.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridSizeX.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridSizeX.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridSizeY.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridSizeY.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridSpacingX.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridSpacingX.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridSpacingY.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridSpacingY.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridSizeRadius.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridSizeRadius.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridSpacingRadius.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridSpacingRadius.setEnabled(not self.dialog["grid_load_from_file"])
        labelGridSpacingAngle.setEnabled(not self.dialog["grid_load_from_file"])
        spinBoxGridSpacingAngle.setEnabled(not self.dialog["grid_load_from_file"])

        visibility = 0
        if self.dialog.grid_type == "Circular":
            visibility = 1
        labelGridSizeX.setVisible(not visibility)
        spinBoxGridSizeX.setVisible(not visibility)
        labelGridSizeY.setVisible(not visibility)
        spinBoxGridSizeY.setVisible(not visibility)
        labelGridSpacingX.setVisible(not visibility)
        spinBoxGridSpacingX.setVisible(not visibility)
        labelGridSpacingY.setVisible(not visibility)
        spinBoxGridSpacingY.setVisible(not visibility)
        labelGridSizeRadius.setVisible(visibility)
        spinBoxGridSizeRadius.setVisible(visibility)
        labelGridSpacingRadius.setVisible(visibility)
        spinBoxGridSpacingRadius.setVisible(visibility)
        labelGridSpacingAngle.setVisible(visibility)
        spinBoxGridSpacingAngle.setVisible(visibility)

        gridLayoutGridGeom = tk.GridLayout(groupBoxGridGeom)
        gridLayoutGridGeom.addWidget(checkBoxGridLoadFromFile, 0, 0, "Align Left")
        gridLayoutGridGeom.addWidget(labelGridType, 1, 0, "Align Left")
        gridLayoutGridGeom.addWidget(comboBoxGridType, 1, 1, "Align Right")
        gridLayoutGridGeom.addWidget(checkBoxGridCenterOnOrigin, 2, 0, "Align Left")
        gridLayoutGridGeom.addWidget(labelGridCenterX, 3, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridCenterX, 3, 1, "Align Right")
        gridLayoutGridGeom.addWidget(labelGridCenterY, 4, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridCenterY, 4, 1, "Align Right")
        gridLayoutGridGeom.addWidget(labelGridSizeX, 5, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridSizeX, 5, 1, "Align Right")
        gridLayoutGridGeom.addWidget(labelGridSizeY, 6, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridSizeY, 6, 1, "Align Right")
        gridLayoutGridGeom.addWidget(labelGridSpacingX, 7, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridSpacingX, 7, 1, "Align Right")
        gridLayoutGridGeom.addWidget(labelGridSpacingY, 8, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridSpacingY, 8, 1, "Align Right")
        gridLayoutGridGeom.addWidget(labelGridSizeRadius, 9, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridSizeRadius, 9, 1, "Align Right")
        gridLayoutGridGeom.addWidget(labelGridSpacingRadius, 10, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridSpacingRadius, 10, 1, "Align Right")
        gridLayoutGridGeom.addWidget(labelGridSpacingAngle, 11, 0, "Align Left")
        gridLayoutGridGeom.addWidget(spinBoxGridSpacingAngle, 11, 1, "Align Right")
        groupBoxGridGeom.setLayout(gridLayoutGridGeom)

        #Ruler Misc
        groupBoxRulerMisc = tk.GroupBox(translate("Ruler Misc"), widget)

        checkBoxRulerShowOnLoad = tk.CheckBox(translate("Initially show ruler when loading a file"), groupBoxRulerMisc)
        self.dialog.ruler_show_on_load = settings.ruler_show_on_load
        checkBoxRulerShowOnLoad.setChecked(self.dialog.ruler_show_on_load)
        #connect(checkBoxRulerShowOnLoad, SIGNAL(stateChanged(int)), this, SLOT(checkBoxRulerShowOnLoadStateChanged(int)))

        labelRulerMetric = tk.Label(translate("Ruler Units"), groupBoxRulerMisc)
        comboBoxRulerMetric = tk.ComboBox(groupBoxRulerMisc)
        comboBoxRulerMetric.addItem("Imperial", 0)
        comboBoxRulerMetric.addItem("Metric", 1)
        self.dialog.ruler_metric = settings.ruler_metric
        comboBoxRulerMetric.setCurrentIndex(comboBoxRulerMetric.findData(self.dialog.ruler_metric))
        #connect(comboBoxRulerMetric, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRulerMetricCurrentIndexChanged(int)))

        gridLayoutRulerMisc = tk.GridLayout(widget)
        gridLayoutRulerMisc.addWidget(checkBoxRulerShowOnLoad, 0, 0, "Align Left")
        gridLayoutRulerMisc.addWidget(labelRulerMetric, 1, 0, "Align Left")
        gridLayoutRulerMisc.addWidget(comboBoxRulerMetric, 1, 1, "Align Right")
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
        gridLayoutRulerColor.addWidget(labelRulerColor, 1, 0, "Align Left")
        gridLayoutRulerColor.addWidget(buttonRulerColor, 1, 1, "Align Right")
        groupBoxRulerColor.setLayout(gridLayoutRulerColor)

        #Ruler Geometry
        groupBoxRulerGeom = tk.GroupBox(translate("Ruler Geometry"), widget)

        labelRulerPixelSize = tk.Label(translate("Ruler Pixel Size"), groupBoxRulerGeom)
        labelRulerPixelSize.setObjectName("labelRulerPixelSize")
        spinBoxRulerPixelSize = tk.DoubleSpinBox(groupBoxRulerGeom)
        spinBoxRulerPixelSize.setObjectName("spinBoxRulerPixelSize")
        self.dialog.ruler_pixel_size = settings.ruler_pixel_size
        spinBoxRulerPixelSize.setSingleStep(1.000)
        spinBoxRulerPixelSize.setRange(20.000, 100.000)
        spinBoxRulerPixelSize.setValue(self.dialog.ruler_pixel_size)
        #connect(spinBoxRulerPixelSize, SIGNAL(valueChanged(double)), this, SLOT(spinBoxRulerPixelSizeValueChanged(double)))

        gridLayoutRulerGeom = tk.GridLayout(groupBoxRulerGeom)
        gridLayoutRulerGeom.addWidget(labelRulerPixelSize, 0, 0, "Align Left")
        gridLayoutRulerGeom.addWidget(spinBoxRulerPixelSize, 0, 1, "Align Right")
        groupBoxRulerGeom.setLayout(gridLayoutRulerGeom)

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxGridMisc)
        vboxLayoutMain.addWidget(groupBoxGridColor)
        vboxLayoutMain.addWidget(groupBoxGridGeom)
        vboxLayoutMain.addWidget(groupBoxRulerMisc)
        vboxLayoutMain.addWidget(groupBoxRulerColor)
        vboxLayoutMain.addWidget(groupBoxRulerGeom)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabOrthoPolar():
        widget = tk.Widget(this)

        #TODO: finish this

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def make_check_box(label, checked, icon, f, x, y):
        " Originally a macro for constructing tk.t checkboxes. "
        c = tk.CheckBox(translate(label), groupBoxQSnapLoc)
        c.setChecked(settings.checked)
        c.setIcon(loadIcon(icon))
        #connect(c, SIGNAL(stateChanged(int)), this, SLOT(f(int)))
        #connect(this, SIGNAL(buttonQSnapSelectAll(int)), c, SLOT(setChecked(int)))
        #connect(this, SIGNAL(buttonQSnapClearAll(int)), c, SLOT(setChecked(int)))
        gridLayoutQSnap.addWidget(c, x, y, "Align Left")
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

        make_check_box("Endpoint", tk.snap_endpoint, locator_snaptoendpoint_xpm, checkBoxQSnapEndPointStateChanged, 0, 0)
        make_check_box("Midpoint", tk.snap_midpoint, locator_snaptomidpoint_xpm, checkBoxQSnapMidPointStateChanged, 1, 0)
        make_check_box("Center", tk.snap_center, locator_snaptocenter_xpm, checkBoxQSnapCenterStateChanged, 2, 0)
        make_check_box("Node", tk.snap_node, locator_snaptonode_xpm, checkBoxQSnapNodeStateChanged, 3, 0)
        make_check_box("Quadrant", tk.snap_quadrant, locator_snaptoquadrant_xpm, checkBoxQSnapQuadrantStateChanged, 4, 0)
        make_check_box("Intersection", tk.snap_intersection, locator_snaptointersection_xpm, checkBoxQSnapIntersectionStateChanged, 5, 0)
        make_check_box("Extension", tk.snap_extension, locator_snaptoextension_xpm, checkBoxQSnapExtensionStateChanged, 6, 0)
        make_check_box("Insertion", tk.snap_insertion, locator_snaptoinsert_xpm, checkBoxQSnapInsertionStateChanged, 0, 1)
        make_check_box("Perpendicular", tk.snap_perpendicular, locator_snaptoperpendicular_xpm, checkBoxQSnapPerpendicularStateChanged, 1, 1)
        make_check_box("Tangent", tk.snap_tangent, locator_snaptotangent_xpm, checkBoxQSnapTangentStateChanged, 2, 1)
        make_check_box("Nearest", tk.snap_nearest, locator_snaptonearest_xpm, checkBoxQSnapNearestStateChanged, 3, 1)
        make_check_box("Apparent Intersection", tk.snap_apparent, locator_snaptoapparentintersection_xpm, checkBoxQSnapApparentIntersectionStateChanged, 4, 1)
        make_check_box("Parallel", tk.snap_parallel, locator_snaptoparallel_xpm, checkBoxQSnapParallelStateChanged, 5, 1)

        gridLayoutQSnap.addWidget(buttonQSnapSelectAll, 0, 2, "Align Left")
        gridLayoutQSnap.addWidget(buttonQSnapClearAll, 1, 2, "Align Left")
        gridLayoutQSnap.setColumnStretch(2,1)
        groupBoxQSnapLoc.setLayout(gridLayoutQSnap)

        #QSnap Visual Config
        groupBoxQSnapVisual = tk.GroupBox(translate("Visual Configuration"), widget)

        labelQSnapLocColor = tk.Label(translate("Locator Color"), groupBoxQSnapVisual)
        comboBoxQSnapLocColor = tk.ComboBox(groupBoxQSnapVisual)
        addColorsToComboBox(comboBoxQSnapLocColor)
        self.dialog.qsnap_locator_color = settings.qsnap_locator_color
        comboBoxQSnapLocColor.setCurrentIndex(comboBoxQSnapLocColor.findData(self.dialog.qsnap_locator_color))
        #connect(comboBoxQSnapLocColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxQSnapLocatorColorCurrentIndexChanged(int)))

        labelQSnapLocSize = tk.Label(translate("Locator Size"), groupBoxQSnapVisual)
        sliderQSnapLocSize = tk.Slider(Qt_Horizontal, groupBoxQSnapVisual)
        sliderQSnapLocSize.setRange(1,20)
        self.dialog.qsnap_locator_size = settings.qsnap_locator_size
        sliderQSnapLocSize.setValue(self.dialog.qsnap_locator_size)
        #connect(sliderQSnapLocSize, SIGNAL(valueChanged(int)), this, SLOT(sliderQSnapLocatorSizeValueChanged(int)))

        vboxLayoutQSnapVisual = tk.VBoxLayout(groupBoxQSnapVisual)
        vboxLayoutQSnapVisual.addWidget(labelQSnapLocColor)
        vboxLayoutQSnapVisual.addWidget(comboBoxQSnapLocColor)
        vboxLayoutQSnapVisual.addWidget(labelQSnapLocSize)
        vboxLayoutQSnapVisual.addWidget(sliderQSnapLocSize)
        groupBoxQSnapVisual.setLayout(vboxLayoutQSnapVisual)

        #QSnap Sensitivity Config
        groupBoxQSnapSensitivity = tk.GroupBox(translate("Sensitivity"), widget)

        labelQSnapApertureSize = tk.Label(translate("Aperture Size"), groupBoxQSnapSensitivity)
        sliderQSnapApertureSize = tk.Slider(Qt_Horizontal, groupBoxQSnapSensitivity)
        sliderQSnapApertureSize.setRange(1,20)
        self.dialog.qsnap_aperture_size = settings.qsnap_aperture_size
        sliderQSnapApertureSize.setValue(self.dialog.qsnap_aperture_size)
        #connect(sliderQSnapApertureSize, SIGNAL(valueChanged(int)), this, SLOT(sliderQSnapApertureSizeValueChanged(int)))

        vboxLayoutQSnapSensitivity = tk.VBoxLayout(groupBoxQSnapSensitivity)
        vboxLayoutQSnapSensitivity.addWidget(labelQSnapApertureSize)
        vboxLayoutQSnapSensitivity.addWidget(sliderQSnapApertureSize)
        groupBoxQSnapSensitivity.setLayout(vboxLayoutQSnapSensitivity)

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxQSnapLoc)
        vboxLayoutMain.addWidget(groupBoxQSnapVisual)
        vboxLayoutMain.addWidget(groupBoxQSnapSensitivity)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    #undef make_check_box

    def createTabQuickTrack():
        widget = tk.Widget(this)

        # TODO: finish this

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabLineWeight():
        widget = tk.Widget(this)

        # TODO: finish this

        # Misc
        groupBoxLwtMisc = tk.GroupBox(translate("LineWeight Misc"), widget)

        s = self.mw.activeScene()

        checkBoxShowLwt = tk.CheckBox(translate("Show LineWeight"), groupBoxLwtMisc)
        if s:
            self.dialog.lwt_show_lwt = s.property("ENABLE_LWT").toBool()
        else:
            self.dialog.lwt_show_lwt = settings.lwt_show_lwt

        self.preview.lwt_show_lwt = self.dialog.lwt_show_lwt
        checkBoxShowLwt.setChecked(preview.lwt_show_lwt)
        #connect(checkBoxShowLwt, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtShowLwtStateChanged(int)))

        checkBoxRealRender = tk.CheckBox(translate("RealRender"), groupBoxLwtMisc)
        checkBoxRealRender.setObjectName("checkBoxRealRender")
        if s:
            self.dialog.lwt_real_render = s.property("ENABLE_REAL").toBool()
        else:
            self.dialog.lwt_real_render = settings.lwt_real_render

        self.preview.lwt_real_render = self.dialog.lwt_real_render
        checkBoxRealRender.setChecked(preview.lwt_real_render)
        #connect(checkBoxRealRender, SIGNAL(stateChanged(int)), this, SLOT(checkBoxLwtRealRenderStateChanged(int)))
        checkBoxRealRender.setEnabled(self.dialog.lwt_show_lwt)

        labelDefaultLwt = tk.Label(translate("Default weight"), groupBoxLwtMisc)
        labelDefaultLwt.setEnabled(0); # TODO: remove later
        comboBoxDefaultLwt = tk.ComboBox(groupBoxLwtMisc)
        self.dialog.lwt_default_lwt = settings.lwt_default_lwt
        # TODO: populate the comboBox and set the initial value
        comboBoxDefaultLwt.addItem("".setNum(self.dialog.lwt_default_lwt, 'F', 2).append(" mm"), self.dialog.lwt_default_lwt)
        comboBoxDefaultLwt.setEnabled(0); # TODO: remove later

        vboxLayoutLwtMisc = tk.VBoxLayout(groupBoxLwtMisc)
        vboxLayoutLwtMisc.addWidget(checkBoxShowLwt)
        vboxLayoutLwtMisc.addWidget(checkBoxRealRender)
        vboxLayoutLwtMisc.addWidget(labelDefaultLwt)
        vboxLayoutLwtMisc.addWidget(comboBoxDefaultLwt)
        groupBoxLwtMisc.setLayout(vboxLayoutLwtMisc)

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxLwtMisc)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabSelection():
        widget = tk.Widget(this)

        # Selection Modes
        groupBoxSelectionModes = tk.GroupBox(translate("Modes"), widget)

        checkBoxSelectionModePickFirst = tk.CheckBox(translate("Allow Preselection (PickFirst)"), groupBoxSelectionModes)
        self.dialog.selection_mode_pickfirst = settings.selection_mode_pickfirst
        checkBoxSelectionModePickFirst.setChecked(self.dialog.selection_mode_pickfirst)
        checkBoxSelectionModePickFirst.setChecked(1); checkBoxSelectionModePickFirst.setEnabled(0); # TODO: Remove this line when Post-selection is available
        #connect(checkBoxSelectionModePickFirst, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickFirstStateChanged(int)))

        checkBoxSelectionModePickAdd = tk.CheckBox(translate("Add to Selection (PickAdd)"), groupBoxSelectionModes)
        self.dialog.selection_mode_pickadd = settings.selection_mode_pickadd
        checkBoxSelectionModePickAdd.setChecked(self.dialog.selection_mode_pickadd)
        #connect(checkBoxSelectionModePickAdd, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickAddStateChanged(int)))

        checkBoxSelectionModePickDrag = tk.CheckBox(translate("Drag to Select (PickDrag)"), groupBoxSelectionModes)
        self.dialog.selection_mode_pickdrag = settings.selection_mode_pickdrag
        checkBoxSelectionModePickDrag.setChecked(self.dialog.selection_mode_pickdrag)
        checkBoxSelectionModePickDrag.setChecked(0); checkBoxSelectionModePickDrag.setEnabled(0); #TODO: Remove this line when this functionality is available
        #connect(checkBoxSelectionModePickDrag, SIGNAL(stateChanged(int)), this, SLOT(checkBoxSelectionModePickDragStateChanged(int)))

        vboxLayoutSelectionModes = tk.VBoxLayout(groupBoxSelectionModes)
        vboxLayoutSelectionModes.addWidget(checkBoxSelectionModePickFirst)
        vboxLayoutSelectionModes.addWidget(checkBoxSelectionModePickAdd)
        vboxLayoutSelectionModes.addWidget(checkBoxSelectionModePickDrag)
        groupBoxSelectionModes.setLayout(vboxLayoutSelectionModes)

        #Selection Colors
        groupBoxSelectionColors = tk.GroupBox(translate("Colors"), widget)

        labelCoolGripColor = tk.Label(translate("Cool Grip (Unselected)"), groupBoxSelectionColors)
        comboBoxCoolGripColor = tk.ComboBox(groupBoxSelectionColors)
        addColorsToComboBox(comboBoxCoolGripColor)
        self.dialog.selection_coolgrip_color = settings.selection_coolgrip_color
        comboBoxCoolGripColor.setCurrentIndex(comboBoxCoolGripColor.findData(self.dialog.selection_coolgrip_color))
        #connect(comboBoxCoolGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionCoolGripColorCurrentIndexChanged(int)))

        labelHotGripColor = tk.Label(translate("Hot Grip (Selected)"), groupBoxSelectionColors)
        comboBoxHotGripColor = tk.ComboBox(groupBoxSelectionColors)
        addColorsToComboBox(comboBoxHotGripColor)
        self.dialog.selection_hotgrip_color = settings.selection_hotgrip_color
        comboBoxHotGripColor.setCurrentIndex(comboBoxHotGripColor.findData(self.dialog.selection_hotgrip_color))
        #connect(comboBoxHotGripColor, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSelectionHotGripColorCurrentIndexChanged(int)))

        vboxLayoutSelectionColors = tk.VBoxLayout(groupBoxSelectionColors)
        vboxLayoutSelectionColors.addWidget(labelCoolGripColor)
        vboxLayoutSelectionColors.addWidget(comboBoxCoolGripColor)
        vboxLayoutSelectionColors.addWidget(labelHotGripColor)
        vboxLayoutSelectionColors.addWidget(comboBoxHotGripColor)
        groupBoxSelectionColors.setLayout(vboxLayoutSelectionColors)

        #Selection Sizes
        groupBoxSelectionSizes = tk.GroupBox(translate("Sizes"), widget)

        labelSelectionGripSize = tk.Label(translate("Grip Size"), groupBoxSelectionSizes)
        sliderSelectionGripSize = tk.Slider(Qt_Horizontal, groupBoxSelectionSizes)
        sliderSelectionGripSize.setRange(1,20)
        self.dialog.selection_grip_size = settings.selection_grip_size
        sliderSelectionGripSize.setValue(self.dialog.selection_grip_size)
        #connect(sliderSelectionGripSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionGripSizeValueChanged(int)))

        labelSelectionPickBoxSize = tk.Label(translate("Pickbox Size"), groupBoxSelectionSizes)
        sliderSelectionPickBoxSize = tk.Slider(Qt_Horizontal, groupBoxSelectionSizes)
        sliderSelectionPickBoxSize.setRange(1,20)
        self.dialog.selection_pickbox_size = settings.selection_pickbox_size
        sliderSelectionPickBoxSize.setValue(self.dialog.selection_pickbox_size)
        #connect(sliderSelectionPickBoxSize, SIGNAL(valueChanged(int)), this, SLOT(sliderSelectionPickBoxSizeValueChanged(int)))

        vboxLayoutSelectionSizes = tk.VBoxLayout(groupBoxSelectionSizes)
        vboxLayoutSelectionSizes.addWidget(labelSelectionGripSize)
        vboxLayoutSelectionSizes.addWidget(sliderSelectionGripSize)
        vboxLayoutSelectionSizes.addWidget(labelSelectionPickBoxSize)
        vboxLayoutSelectionSizes.addWidget(sliderSelectionPickBoxSize)
        groupBoxSelectionSizes.setLayout(vboxLayoutSelectionSizes)

        #Widget Layout
        vboxLayoutMain = tk.VBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxSelectionModes)
        vboxLayoutMain.addWidget(groupBoxSelectionColors)
        vboxLayoutMain.addWidget(groupBoxSelectionSizes)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = tk.ScrollArea(this)
        scrollArea.setWidgetResizable(1)
        scrollArea.setWidget(widget)
        return scrollArea

    def addColorsToComboBox(comboBox):
        comboBox.addItem(loadIcon(colorred_xpm), translate("Red"), tk.Rgb(255, 0, 0))
        comboBox.addItem(loadIcon(coloryellow_xpm), translate("Yellow"), tk.Rgb(255,255, 0))
        comboBox.addItem(loadIcon(colorgreen_xpm), translate("Green"), tk.Rgb(  0,255, 0))
        comboBox.addItem(loadIcon(colorcyan_xpm), translate("Cyan"), tk.Rgb(  0,255,255))
        comboBox.addItem(loadIcon(colorblue_xpm), translate("Blue"), tk.Rgb(  0, 0,255))
        comboBox.addItem(loadIcon(colormagenta_xpm), translate("Magenta"), tk.Rgb(255, 0,255))
        comboBox.addItem(loadIcon(colorwhite_xpm), translate("White"), tk.Rgb(255,255,255))
        # TODO: Add Other... so the user can select custom colors

    def comboBoxLanguageCurrentIndexChanged(self, lang):
        " . "
        self.dialog.general_language = lang.toLower().toLocal8Bit().constData()

    def comboBoxIconThemeCurrentIndexChanged(self, theme):
        " . "
        strcpy(self.dialog.general_icon_theme, theme.toLocal8Bit().constData())

    def comboBoxIconSizeCurrentIndexChanged(self, index):
        " . "
        comboBox = sender()
        if comboBox:
            ok = 0
            self.dialog.general_icon_size, ok = comboBox.itemData(index).toUInt()
            if not ok:
                self.dialog.general_icon_size = 16
        else:
            self.dialog.general_icon_size = 16

    def checkBoxGeneralMdiBGUseLogoStateChanged(self, checked):
        " . "
        self.preview.general_mdi_bg_use_logo = checked
        self.mw.mdiArea.useBackgroundLogo(checked)

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
            self.mw.mdiArea.setBackgroundLogo(accept_.general_mdi_bg_logo)

    def checkBoxGeneralMdiBGUseTextureStateChanged(checked):
        " . "
        self.preview.general_mdi_bg_use_texture = checked
        self.mw.mdiArea.useBackgroundTexture(checked)

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
            self.mw.mdiArea.setBackgroundTexture(accept_.general_mdi_bg_texture)

    def checkBoxGeneralMdiBGUseColorStateChanged(checked):
        self.preview.general_mdi_bg_use_color = checked
        self.mw.mdiArea.useBackgroundColor(checked)

    def chooseGeneralMdiBackgroundColor():
        button = sender()
        if button:
            colorDialog = ColorDialog(Color(accept_.general_mdi_bg_color), this)
            #connect(colorDialog, SIGNAL(currentColorChanged()), this, SLOT(currentGeneralMdiBackgroundColorChanged()))
            colorDialog.exec()

            if colorDialog.result() == "Accepted":
                self.accept.general_mdi_bg_color = colorDialog.selectedColor().rgb()
                pix = Image(16,16)
                pix.fill(Color(accept_.general_mdi_bg_color))
                button.setIcon(pix)
                self.mw.mdiArea.setBackgroundColor(Color(accept_.general_mdi_bg_color))

            else:
                self.mw.mdiArea.setBackgroundColor(Color(self.dialog.general_mdi_bg_color))

    def currentGeneralMdiBackgroundColorChanged(self, color):
        self.preview.general_mdi_bg_color = color.rgb()
        self.mw.mdiArea.setBackgroundColor(Color(preview.general_mdi_bg_color))

    """
    lambda method constructor?

    def checkBoxTipOfTheDayStateChanged(self, )
    #
    check_func(checkBoxTipOfTheDayStateChanged, general_tip_of_the_day)
    check_func(checkBoxUseOpenGLStateChanged, display_use_opengl)
    check_func(checkBoxRenderHintAAStateChanged, display_renderhint_aa)
    check_func(checkBoxRenderHintTextAAStateChanged, display_renderhint_text_aa)
    check_func(checkBoxRenderHintSmoothPixStateChanged, display_renderhint_smooth_pix)
    check_func(checkBoxRenderHintHighAAStateChanged, display_renderhint_high_aa)
    check_func(checkBoxRenderHintNonCosmeticStateChanged, display_renderhint_noncosmetic)
    """

    def checkBoxShowScrollBarsStateChanged(checked):
        " . "
        self.preview.display_show_scrollbars = checked
        self.mw.updateAllViewScrollBars(preview.display_show_scrollbars)

    def spinBoxZoomScaleInValueChanged(value):
        self.dialog.display_zoomscale_in = value

    def spinBoxZoomScaleOutValueChanged(value):
        self.dialog.display_zoomscale_out = value

    def checkBoxDisableBGStateChanged(checked):
        self.dialog.printing_disable_bg = checked

    def chooseDisplayCrossHairColor():
        button = sender()
        if button:
            colorDialog = ColorDialog(Color(accept_.display_crosshair_color), this)
            #connect(colorDialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplayCrossHairColorChanged()))
            colorDialog.exec()

            if colorDialog.result() == "Accepted":
                self.accept.display_crosshair_color = colorDialog.selectedColor().rgb()
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
            colorDialog = ColorDialog(Color(accept_.display_bg_color), this)
            #connect(colorDialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplayBackgroundColorChanged()))
            colorDialog.exec()

            if colorDialog.result() == "Accepted":
                self.accept.display_bg_color = colorDialog.selectedColor().rgb()
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
            colorDialog = ColorDialog(Color(accept_["display_selectbox_left_color"]), this)
            #connect(colorDialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxLeftColorChanged()))
            colorDialog.exec()

            if colorDialog.result() == tk.Dialog_Accepted:
                self.accept["display_selectbox_left_color"] = colorDialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_["display_selectbox_left_color"]))
                button.setIcon(pix)
                self.mw.updateAllViewSelectBoxColors(accept_["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept.display_selectbox_right_fill,
                    self.preview.display_selectbox_alpha)
            else:
                self.mw.updateAllViewSelectBoxColors(self.dialog["display_selectbox_left_color"],
                    self.dialog["display_selectbox_left_fill"],
                    self.dialog["display_selectbox_right_color"],
                    self.dialog.display_selectbox_right_fill,
                                                    self.preview.display_selectbox_alpha)

    def currentDisplaySelectBoxLeftColorChanged(self, color):
        self.preview["display_selectbox_left_color"] = color.rgb()
        self.mw.updateAllViewSelectBoxColors(preview["display_selectbox_left_color"],
                                            self.preview["display_selectbox_left_fill"],
                                            self.preview["display_selectbox_right_color"],
                                            self.preview.display_selectbox_right_fill,
                                            self.preview.display_selectbox_alpha)

    def chooseDisplaySelectBoxLeftFill(self):
        button = sender()
        if button:
            colorDialog = ColorDialog(Color(accept_["display_selectbox_left_fill"]), this)
            #connect(colorDialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxLeftFillChanged()))
            colorDialog.exec()

            if colorDialog.result() == "Accepted":
                self.accept["display_selectbox_left_fill"] = colorDialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_["display_selectbox_left_fill"]))
                button.setIcon(pix)
                self.mw.updateAllViewSelectBoxColors(
                    self.accept["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept.display_selectbox_right_fill,
                    self.preview.display_selectbox_alpha)

            else:
                self.mw.updateAllViewSelectBoxColors(
                    self.dialog["display_selectbox_left_color"],
                    self.dialog["display_selectbox_left_fill"],
                    self.dialog["display_selectbox_right_color"],
                    self.dialog.display_selectbox_right_fill,
                    self.preview.display_selectbox_alpha)

    def currentDisplaySelectBoxLeftFillChanged(color):
        self.preview["display_selectbox_left_fill"] = color.rgb()
        self.mw.updateAllViewSelectBoxColors(preview["display_selectbox_left_color"],
            self.preview["display_selectbox_left_fill"],
            self.preview["display_selectbox_right_color"],
            self.preview.display_selectbox_right_fill,
            self.preview.display_selectbox_alpha)

    def chooseDisplaySelectBoxRightColor():
        button = sender()
        if button:
            colorDialog = ColorDialog(Color(accept_["display_selectbox_right_color"]), this)
            #connect(colorDialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxRightColorChanged()))
            colorDialog.exec()

            if colorDialog.result() == "Accepted":
                self.accept["display_selectbox_right_color"] = colorDialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_["display_selectbox_right_color"]))
                button.setIcon(pix)
                self.mw.updateAllViewSelectBoxColors(
                    self.accept["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept.display_selectbox_right_fill,
                    self.preview.display_selectbox_alpha)

            else:
                self.mw.updateAllViewSelectBoxColors(
                    self.dialog["display_selectbox_left_color"],
                    self.dialog["display_selectbox_left_fill"],
                    self.dialog["display_selectbox_right_color"],
                    self.dialog.display_selectbox_right_fill,
                    self.preview.display_selectbox_alpha)

    def currentDisplaySelectBoxRightColorChanged(color):
        self.preview["display_selectbox_right_color"] = color.rgb()
        self.mw.updateAllViewSelectBoxColors(preview["display_selectbox_left_color"],
            self.preview["display_selectbox_left_fill"],
            self.preview["display_selectbox_right_color"],
            self.preview.display_selectbox_right_fill,
            self.preview.display_selectbox_alpha)

    def chooseDisplaySelectBoxRightFill():
        button = sender()
        if button:
            colorDialog = ColorDialog(Color(accept_.display_selectbox_right_fill), this)
            #connect(colorDialog, SIGNAL(currentColorChanged()), this, SLOT(currentDisplaySelectBoxRightFillChanged()))
            colorDialog.exec()

            if colorDialog.result() == "Accepted":
                self.accept.display_selectbox_right_fill = colorDialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_.display_selectbox_right_fill))
                button.setIcon(pix)
                self.mw.updateAllViewSelectBoxColors(accept_["display_selectbox_left_color"],
                    self.accept["display_selectbox_left_fill"],
                    self.accept["display_selectbox_right_color"],
                    self.accept.display_selectbox_right_fill,
                    self.preview.display_selectbox_alpha)
            
            else:
                self.mw.updateAllViewSelectBoxColors(self.dialog["display_selectbox_left_color"],
                                                    self.dialog["display_selectbox_left_fill"],
                                                    self.dialog["display_selectbox_right_color"],
                                                    self.dialog.display_selectbox_right_fill,
                                                    self.preview.display_selectbox_alpha)

    def currentDisplaySelectBoxRightFillChanged(self, color):
        self.preview.display_selectbox_right_fill = color.rgb()
        self.mw.updateAllViewSelectBoxColors(
            self.preview["display_selectbox_left_color"],
            self.preview["display_selectbox_left_fill"],
            self.preview["display_selectbox_right_color"],
            self.preview.display_selectbox_right_fill,
            self.preview.display_selectbox_alpha)

    def spinBoxDisplaySelectBoxAlphaValueChanged(self, value):
        " . "
        self.preview.display_selectbox_alpha = value
        self.mw.updateAllViewSelectBoxColors(
            self.accept["display_selectbox_left_color"],
            self.accept["display_selectbox_left_fill"],
            self.accept["display_selectbox_right_color"],
            self.accept["display_selectbox_right_fill"],
            self.preview["display_selectbox_alpha"])

    def checkBoxCustomFilterStateChanged(self, checked):
        " . "
        checkBox = sender()
        if checkBox:
            format = checkBox.text()
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

    def checkBoxGridColorMatchCrossHairStateChanged(self, checked):
        " . "
        self.dialog.grid_color_match_crosshair = checked
        if self.dialog.grid_color_match_crosshair:
            self.mw.updateAllViewGridColors(accept_.display_crosshair_color)
        
        else:
            self.mw.updateAllViewGridColors(accept_.grid_color)

        senderObj = sender()
        if not senderObj:
            return

        parent = senderObj.parent()
        if not parent:
            return

        labelGridColor = parent.findChild("labelGridColor")
        if labelGridColor:
            labelGridColor.setEnabled(not self.dialog.grid_color_match_crosshair)

        buttonGridColor = parent.findChild("buttonGridColor")
        if buttonGridColor:
            buttonGridColor.setEnabled(not self.dialog.grid_color_match_crosshair)

    def checkBoxTipOfTheDayStateChanged(checked):
        dialog["general_tip_of_the_day"] = checked

    def checkBoxUseOpenGLStateChanged(checked):
        dialog["display_use_opengl"] = checked

    def checkBoxRenderHintAAStateChanged(checked):
        dialog["display_renderhint_aa"] = checked

    def checkBoxRenderHintTextAAStateChanged(checked):
        dialog.display_renderhint_text_aa = checked

    def checkBoxRenderHintSmoothPixStateChanged(checked):
        dialog.display_renderhint_smooth_pix = checked

    def checkBoxRenderHintHighAAStateChanged(checked):
        dialog["display_renderhint_high_aa"] = checked

    def checkBoxRenderHintNonCosmeticStateChanged(checked):
        dialog["display_renderhint_noncosmetic"] = checked

    def comboBoxScrollBarWidgetCurrentIndexChanged(index):
        dialog["display_scrollbar_widget_num"] = index

    def spinBoxZoomScaleInValueChanged(value):
        dialog["display_zoomscale_in"] = value

    def spinBoxZoomScaleOutValueChanged(value):
        dialog["display_zoomscale_out"] = value

    def checkBoxDisableBGStateChanged(checked):
        dialog["printing_disable_bg"] = checked

    def spinBoxRecentMaxFilesValueChanged(value):
        dialog["opensave_recent_max_files"] = value

    def spinBoxTrimDstNumJumpsValueChanged(value):
        dialog["opensave_trim_dst_num_jumps"] = value

    def checkBoxGridShowOnLoadStateChanged(checked):
        dialog["grid_show_on_load"] = checked

    def checkBoxGridShowOriginStateChanged(checked):
        dialog["grid_show_origin"] = checked

    def spinBoxRulerPixelSizeValueChanged(value):
        dialog["ruler_pixel_size"] = value

    def checkBoxQSnapEndPointStateChanged(checked):
        dialog["qsnap_endpoint"] = checked

    def checkBoxQSnapMidPointStateChanged(checked):
        dialog.qsnap_midpoint = checked

    def checkBoxQSnapCenterStateChanged(checked):
        dialog.qsnap_center = checked

    def checkBoxQSnapNodeStateChanged(checked):
        dialog.qsnap_node = checked

    def checkBoxQSnapQuadrantStateChanged(checked):
        dialog.qsnap_quadrant = checked

    def checkBoxQSnapIntersectionStateChanged(checked):
        dialog.qsnap_intersection = checked

    def checkBoxQSnapExtensionStateChanged(checked):
        dialog.qsnap_extension = checked

    def checkBoxQSnapInsertionStateChanged(checked):
        dialog.qsnap_insertion = checked

    def checkBoxQSnapPerpendicularStateChanged(checked):
        dialog.qsnap_perpendicular = checked

    def checkBoxQSnapTangentStateChanged(checked):
        dialog.qsnap_tangent = checked

    def checkBoxQSnapNearestStateChanged(checked):
        dialog.qsnap_nearest = checked

    def checkBoxQSnapApparentStateChanged(checked):
        dialog.qsnap_apparent = checked

    def checkBoxQSnapParallelStateChanged(checked):
        dialog.qsnap_parallel = checked

    def checkBoxSelectionModePickFirstStateChanged(checked):
        dialog["selection_mode_pickfirst"] = checked

    def checkBoxSelectionModePickAddStateChanged(checked):
        dialog["selection_mode_pickadd"] = checked

    def checkBoxSelectionModePickDragStateChanged(checked):
        dialog["selection_mode_pickdrag"] = checked

    def sliderSelectionGripSizeValueChanged(value):
        dialog["selection_grip_size"] = value

    def sliderSelectionPickBoxSizeValueChanged(value):
        dialog.selection_pickbox_size = value

    def spinBoxGridCenterXValueChanged(value):
        dialog.grid_center.x = value

    def spinBoxGridCenterYValueChanged(value):
        dialog.grid_center.y = value

    def spinBoxGridSizeXValueChanged(value):
        dialog.grid_size.x = value

    def spinBoxGridSizeYValueChanged(value):
        dialog.grid_size.y = value

    def spinBoxGridSpacingXValueChanged(value):
        dialog.grid_spacing.x = value

    def spinBoxGridSpacingYValueChanged(value):
        dialog.grid_spacing.y = value

    def spinBoxGridSizeRadiusValueChanged(value):
        dialog.grid_size_radius = value

    def spinBoxGridSpacingRadiusValueChanged(value):
        dialog.grid_spacing_radius = value

    def spinBoxGridSpacingAngleValueChanged(value):
        dialog.grid_spacing_angle = value

    def checkBoxRulerShowOnLoadStateChanged(checked):
        dialog.ruler_show_on_load = checked

    def chooseGridColor(self):
        button = sender()
        if button:
            colorDialog = ColorDialog(Color(accept_.grid_color), this)
            #connect(colorDialog, SIGNAL(currentColorChanged()), this, SLOT(currentGridColorChanged()))
            colorDialog.exec()

            if colorDialog.result() == "Accepted":
                self.accept.grid_color = colorDialog.selectedColor().rgb()
                pix = Image(16, 16)
                pix.fill(Color(accept_.grid_color))
                button.setIcon(pix)
                self.mw.updateAllViewGridColors(accept_.grid_color)
            
            else:
                self.mw.updateAllViewGridColors(self.dialog.grid_color)

    def currentGridColorChanged(self, color):
        self.preview.grid_color = color.rgb()
        self.mw.updateAllViewGridColors(preview.grid_color)

    def checkBoxGridLoadFromFileStateChanged(self, checked):
        " . "
        self.dialog["grid_load_from_file"] = checked

        senderObj = sender()
        if not senderObj:
            return

        parent = senderObj.parent()
        if not parent:
            return

        labelGridType = parent.findChild("labelGridType")
        if labelGridType:
            labelGridType.setEnabled(not self.dialog["grid_load_from_file"])

        comboBoxGridType = parent.findChild("comboBoxGridType")
        if comboBoxGridType:
            comboBoxGridType.setEnabled(not self.dialog["grid_load_from_file"])

        checkBoxGridCenterOnOrigin = parent.findChild("checkBoxGridCenterOnOrigin")
        if checkBoxGridCenterOnOrigin:
            checkBoxGridCenterOnOrigin.setEnabled(not self.dialog["grid_load_from_file"])

        labelGridCenterX = parent.findChild("labelGridCenterX")
        if labelGridCenterX:
            labelGridCenterX.setEnabled(not self.dialog["grid_load_from_file"] and not self.dialog["grid_center_on_origin"])

        spinBoxGridCenterX = parent.findChild("spinBoxGridCenterX")
        if spinBoxGridCenterX:
            spinBoxGridCenterX.setEnabled(not self.dialog["grid_load_from_file"] and not self.dialog["grid_center_on_origin"])

        labelGridCenterY = parent.findChild("labelGridCenterY")
        if labelGridCenterY:
            labelGridCenterY.setEnabled(not self.dialog["grid_load_from_file"] and not self.dialog["grid_center_on_origin"])

        spinBoxGridCenterY = parent.findChild("spinBoxGridCenterY")
        if spinBoxGridCenterY:
            spinBoxGridCenterY.setEnabled(not self.dialog["grid_load_from_file"] and not self.dialog["grid_center_on_origin"])

        labelGridSizeX = parent.findChild("labelGridSizeX")
        if labelGridSizeX:
            labelGridSizeX.setEnabled(not self.dialog["grid_load_from_file"])

        spinBoxGridSizeX = parent.findChild("spinBoxGridSizeX")
        if spinBoxGridSizeX:
            spinBoxGridSizeX.setEnabled(not self.dialog["grid_load_from_file"])

        labelGridSizeY = parent.findChild("labelGridSizeY")
        if labelGridSizeY:
            labelGridSizeY.setEnabled(not self.dialog["grid_load_from_file"])

        spinBoxGridSizeY = parent.findChild("spinBoxGridSizeY")
        if spinBoxGridSizeY:
            spinBoxGridSizeY.setEnabled(not self.dialog["grid_load_from_file"])

        labelGridSpacingX = parent.findChild("labelGridSpacingX")
        if labelGridSpacingX:
            labelGridSpacingX.setEnabled(not self.dialog["grid_load_from_file"])

        spinBoxGridSpacingX = parent.findChild("spinBoxGridSpacingX")
        if spinBoxGridSpacingX:
            spinBoxGridSpacingX.setEnabled(not self.dialog["grid_load_from_file"])

        labelGridSpacingY = parent.findChild("labelGridSpacingY")
        if labelGridSpacingY:
            labelGridSpacingY.setEnabled(not self.dialog["grid_load_from_file"])

        spinBoxGridSpacingY = parent.findChild("spinBoxGridSpacingY")
        if spinBoxGridSpacingY:
            spinBoxGridSpacingY.setEnabled(not self.dialog["grid_load_from_file"])

        labelGridSizeRadius = parent.findChild("labelGridSizeRadius")
        if labelGridSizeRadius:
            labelGridSizeRadius.setEnabled(not self.dialog["grid_load_from_file"])

        spinBoxGridSizeRadius = parent.findChild("spinBoxGridSizeRadius")
        if spinBoxGridSizeRadius:
            spinBoxGridSizeRadius.setEnabled(not self.dialog["grid_load_from_file"])

        labelGridSpacingRadius = parent.findChild("labelGridSpacingRadius")
        if labelGridSpacingRadius:
            labelGridSpacingRadius.setEnabled(not self.dialog["grid_load_from_file"])

        spinBoxGridSpacingRadius = parent.findChild("spinBoxGridSpacingRadius")
        if spinBoxGridSpacingRadius:
            spinBoxGridSpacingRadius.setEnabled(not self.dialog["grid_load_from_file"])

        labelGridSpacingAngle = parent.findChild("labelGridSpacingAngle")
        if labelGridSpacingAngle:
            labelGridSpacingAngle.setEnabled(not self.dialog["grid_load_from_file"])

        spinBoxGridSpacingAngle = parent.findChild("spinBoxGridSpacingAngle")
        if spinBoxGridSpacingAngle:
            spinBoxGridSpacingAngle.setEnabled(not self.dialog["grid_load_from_file"])

    def comboBoxGridTypeCurrentIndexChanged(self, type_):
        " . "
        self.dialog.grid_type = type_

        senderObj = sender()
        if not senderObj:
            return

        parent = senderObj.parent()
        if not parent:
            return

        visibility = False
        if type_ == "Circular":
            visibility = True

        labelGridSizeX = parent.findChild("labelGridSizeX")
        if labelGridSizeX:
            labelGridSizeX.setVisible(not visibility)

        spinBoxGridSizeX = parent.findChild("spinBoxGridSizeX")
        if spinBoxGridSizeX:
            spinBoxGridSizeX.setVisible(not visibility)

        labelGridSizeY = parent.findChild("labelGridSizeY")
        if labelGridSizeY:
            labelGridSizeY.setVisible(not visibility)

        spinBoxGridSizeY = parent.findChild("spinBoxGridSizeY")
        if spinBoxGridSizeY:
            spinBoxGridSizeY.setVisible(not visibility)

        labelGridSpacingX = parent.findChild("labelGridSpacingX")
        if labelGridSpacingX:
            labelGridSpacingX.setVisible(not visibility)

        spinBoxGridSpacingX = parent.findChild("spinBoxGridSpacingX")
        if spinBoxGridSpacingX:
            spinBoxGridSpacingX.setVisible(not visibility)

        labelGridSpacingY = parent.findChild("labelGridSpacingY")
        if labelGridSpacingY:
            labelGridSpacingY.setVisible(not visibility)

        spinBoxGridSpacingY = parent.findChild("spinBoxGridSpacingY")
        if spinBoxGridSpacingY:
            spinBoxGridSpacingY.setVisible(not visibility)

        labelGridSizeRadius = parent.findChild("labelGridSizeRadius")
        if labelGridSizeRadius:
            labelGridSizeRadius.setVisible(visibility)

        spinBoxGridSizeRadius = parent.findChild("spinBoxGridSizeRadius")
        if spinBoxGridSizeRadius:
            spinBoxGridSizeRadius.setVisible(visibility)

        labelGridSpacingRadius = parent.findChild("labelGridSpacingRadius")
        if labelGridSpacingRadius:
            labelGridSpacingRadius.setVisible(visibility)

        spinBoxGridSpacingRadius = parent.findChild("spinBoxGridSpacingRadius")
        if spinBoxGridSpacingRadius:
            spinBoxGridSpacingRadius.setVisible(visibility)

        labelGridSpacingAngle = parent.findChild("labelGridSpacingAngle")
        if labelGridSpacingAngle:
            labelGridSpacingAngle.setVisible(visibility)

        spinBoxGridSpacingAngle = parent.findChild("spinBoxGridSpacingAngle")
        if spinBoxGridSpacingAngle:
            spinBoxGridSpacingAngle.setVisible(visibility)

    def checkBoxGridCenterOnOriginStateChanged(self, checked):
        " . "
        self.dialog["grid_center_on_origin"] = checked

        senderObj = sender()
        if not senderObj:
            return

        parent = senderObj.parent()
        if not parent:
            return

        labelGridCenterX = parent.findChild("labelGridCenterX")
        if labelGridCenterX:
            labelGridCenterX.setEnabled(not self.dialog["grid_center_on_origin"])

        spinBoxGridCenterX = parent.findChild("spinBoxGridCenterX")
        if spinBoxGridCenterX:
            spinBoxGridCenterX.setEnabled(not self.dialog["grid_center_on_origin"])

        labelGridCenterY = parent.findChild("labelGridCenterY")
        if labelGridCenterY:
            labelGridCenterY.setEnabled(not self.dialog["grid_center_on_origin"])

        spinBoxGridCenterY = parent.findChild("spinBoxGridCenterY")
        if spinBoxGridCenterY:
            spinBoxGridCenterY.setEnabled(not self.dialog["grid_center_on_origin"])

    def comboBoxRulerMetricCurrentIndexChanged(self, index):
        " . "
        comboBox = sender()
        if comboBox:
            ok = False
            self.dialog.ruler_metric = comboBox.itemData(index).toBool()
        else:
            self.dialog.ruler_metric = True

    def chooseRulerColor(self):
        " . "
        button = sender()
        if button:
            colorDialog = ColorDialog(
                Color(accept_.ruler_color), this)
            # connect(colorDialog, SIGNAL(currentColorChanged()),
            # this, SLOT(currentRulerColorChanged()))
            colorDialog.exec()

            if colorDialog.result() == "QDialog_Accepted":
                self.accept.ruler_color = colorDialog.selectedColor().rgb()
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
    # comboBoxes in general
    # Currently comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
    #        comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
    #        comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
    # are all similar except the self.dialog. variable being worked on and the
    # tk.Variant.

    def comboBoxQSnapLocatorColorCurrentIndexChanged(self, index):
        " . "
        # TODO: Alert user if color matched the display bg color
        comboBox = sender()
        defaultColor = tk.Rgb(255,255,0)
        # Yellow
        if comboBox:
            self.dialog.qsnap_locator_color, ok = comboBox.itemData(index).toUInt()
            if not ok:
                self.dialog.qsnap_locator_color = defaultColor
        else:
            self.dialog.qsnap_locator_color = defaultColor

    def sliderQSnapLocatorSizeValueChanged(self, value):
        " . "
        self.dialog["qsnap_locator_size"] = value

    def sliderQSnapApertureSizeValueChanged(self, value):
        " . "
        self.dialog["qsnap_aperture_size"] = value

    def checkBoxLwtShowLwtStateChanged(self, checked):
        " . "
        self.preview.lwt_show_lwt = checked
        if self.preview.lwt_show_lwt:
            enableLwt()
        else:
            disableLwt()

        senderObj = sender()
        if senderObj:
            parent = senderObj.parent()
            if parent:
                checkBoxRealRender = parent.findChild("checkBoxRealRender")
                if checkBoxRealRender:
                    checkBoxRealRender.setEnabled(preview.lwt_show_lwt)

    def checkBoxLwtRealRenderStateChanged(checked):
        " . "
        self.preview["lwt_real_render"] = checked
        if self.preview["lwt_real_render"]:
            enableReal()
        else:
            disableReal()

    def comboBoxSelectionCoolGripColorCurrentIndexChanged(self, index):
        " . "
        # TODO: Alert user if color matched the display bg color
        comboBox = sender()
        defaultColor = tk.Rgb(0,0,255)
        # Blue
        if comboBox:
            self.dialog.selection_coolgrip_color, ok = comboBox.itemData(index).toUInt()
            if not ok:
                self.dialog.selection_coolgrip_color = defaultColor
        else:
            self.dialog.selection_coolgrip_color = defaultColor

    def comboBoxSelectionHotGripColorCurrentIndexChanged(self, index):
        " . "
        # TODO: Alert user if color matched the display bg color
        comboBox = sender()
        defaultColor = tk.Rgb(255,0,0)
        # Red
        if comboBox:
            self.dialog.selection_hotgrip_color, ok = comboBox.itemData(index).toUInt()
            if not ok:
                self.dialog.selection_hotgrip_color = defaultColor
        else:
            self.dialog.selection_hotgrip_color = defaultColor

    def accept_changes(self):
        " . "
        for k in self.preview.keys():
            self.dialog[k] = self.preview[k]
        for k in self.accept.keys():
            self.dialog[k] = self.preview[k]

        if self.dialog["grid_color_match_crosshair"]:
            self.dialog["grid_color"] = self.accept["display_crosshair_color"]

        # Make sure the user sees the changes applied immediately.
        self.mw.mdiArea.useBackgroundLogo(self.dialog["general_mdi_bg_use_logo"])
        self.mw.mdiArea.useBackgroundTexture(self.dialog["general_mdi_bg_use_texture"])
        self.mw.mdiArea.useBackgroundColor(self.dialog["general_mdi_bg_use_color"])
        self.mw.mdiArea.setBackgroundLogo(self.dialog["general_mdi_bg_logo"])
        self.mw.mdiArea.setBackgroundTexture(self.dialog["general_mdi_bg_texture"])
        self.mw.mdiArea.setBackgroundColor(self.dialog["general_mdi_bg_color"])
        self.mw.iconResize(self.dialog.general_icon_size)
        self.mw.updateAllViewScrollBars(self.dialog.display_show_scrollbars)
        self.mw.updateAllViewCrossHairColors(self.dialog.display_crosshair_color)
        self.mw.updateAllViewBackgroundColors(self.dialog.display_bg_color)
        self.mw.updateAllViewSelectBoxColors(
            self.dialog["display_selectbox_left_color"],
            self.dialog["display_selectbox_left_fill"],
            self.dialog["display_selectbox_right_color"],
            self.dialog["display_selectbox_right_fill"],
            self.dialog["display_selectbox_alpha"])
        self.mw.updateAllViewGridColors(self.dialog["grid_color"])
        self.mw.updateAllViewRulerColors(self.dialog["ruler_color"])

        if self.dialog["lwt_show_lwt"]:
            enableLwt()
        else:
            disableLwt()

        if self.dialog["lwt_real_render"]:
            enableReal()
        else:
            disableReal()

        self.mw.updatePickAddMode(self.dialog.selection_mode_pickadd)

        self.mw.writeSettings()
        accept()

    def reject_changes(self):
        """
        TODO: inform the user if they have changed settings

        Update the view since the user must accept the self.preview
        """
        self.mw.mdiArea.useBackgroundLogo(self.dialog.general_mdi_bg_use_logo)
        self.mw.mdiArea.useBackgroundTexture(self.dialog.general_mdi_bg_use_texture)
        self.mw.mdiArea.useBackgroundColor(self.dialog.general_mdi_bg_use_color)
        self.mw.mdiArea.setBackgroundLogo(self.dialog.general_mdi_bg_logo)
        self.mw.mdiArea.setBackgroundTexture(self.dialog.general_mdi_bg_texture)
        self.mw.mdiArea.setBackgroundColor(self.dialog.general_mdi_bg_color)
        self.mw.updateAllViewScrollBars(self.dialog.display_show_scrollbars)
        self.mw.updateAllViewCrossHairColors(self.dialog.display_crosshair_color)
        self.mw.updateAllViewBackgroundColors(self.dialog.display_bg_color)
        self.mw.updateAllViewSelectBoxColors(
            self.dialog["display_selectbox_left_color"],
            self.dialog["display_selectbox_left_fill"],
            self.dialog["display_selectbox_right_color"],
            self.dialog["display_selectbox_right_fill"],
            self.dialog["display_selectbox_alpha"])
        self.mw.updateAllViewGridColors(self.dialog.grid_color)
        self.mw.updateAllViewRulerColors(self.dialog.ruler_color)

        if self.dialog["lwt_show_lwt"]:
            enableLwt()
        else:
            disableLwt()

        if self.dialog["lwt_real_render"]:
            enableReal()
        else:
            disableReal()

        reject()
