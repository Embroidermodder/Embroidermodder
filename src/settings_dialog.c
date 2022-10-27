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
 *  Settings Dialog.
 *
 */

#include "em2.h"

/* 
class settings-dialog-action()
    tab-widget* tab-widget
    dialogButtonBox* buttonBox
*/

/* showTab is the tab index, use the TAB-GENERAL style defines.
 */
void
settings_dialog_init(int showTab)
{
    printf("showTab: %d\n", showTab);
    /* mw = mw;
    accept = copy();
    dialog = copy();
    preview = copy();

    window = tk.Tk();
    window.size(750,550);
    tab_widget = tk.tab-widget(window);
    */

    /* TODO: Add icons to tabs */
    /* tab-widget.add-tab(create-tab-general(), translate("General"));
    tab_widget.add-tab(createTabFilesPaths(), translate("Files/Paths"));
    tab_widget.add-tab(createTabDisplay(), translate("Display"));
    tab_widget.add-tab(createTabOpenSave(), translate("Open/Save"));
    tab_widget.add-tab(createTabPrinting(), translate("Printing"));
    tab_widget.add-tab(createTabSnap(), translate("Snap"));
    tab_widget.add-tab(createTabGridRuler(), translate("Grid/Ruler"));
    tab_widget.add-tab(createTabOrthoPolar(), translate("Ortho/Polar"));
    tab_widget.add-tab(createTabQuickSnap(), translate("QuickSnap"));
    tab-widget.add-tab(createTabQuickTrack(), translate("QuickTrack"));
    tab-widget.add-tab(createTabLineWeight(), translate("LineWeight"));
    tab-widget.add-tab(createTabSelection(), translate("Selection"));

    for (i=0; i<12; i++) {
        if (showTab == settings_tab_label[i]) {
            tab-widget.set_current_index(i);
        }
    }

    buttonBox = tk.DialogButtonBox(QDialogButtonBox-Ok | tk.DialogButtonBox-Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(rejectChanges()));

    vbox_layout_main = tk.VBoxLayout();
    vbox_layout_main, add_widget(tab-widget);
    vbox_layout_main, add_widget(buttonBox);
    setLayout(vbox_layout_main);

    window.title(translate("Settings"));
    window.setOverrideCursor("ArrowCursor");
    */
}

void
create_tab_general(void)
{
    /* widget = tk.Widget(window); */

    /* Language */
    /*
    groupboxLanguage = tk.groupbox(translate("Language"), widget);

    labelLanguage = tk.Label(translate("Language (Requires Restart)"), groupboxLanguage);
    comboboxLanguage = tk.combobox(groupboxLanguage);
    to-lower(dialog-general-language, general-language);
    comboboxLanguage.add_item("Default");
    comboboxLanguage.add_item("System");
    comboboxLanguage.insertSeparator(2);
    trDir = tk.App.applicationDirPath();
    trDir.cd("translations");
    for dirName in trDir.entryList(QDir-Dirs | tk.Dir-NoDotAndDotDot)
        dirName[0] = dirName[0].toUpper();
        comboboxLanguage.add_item(dirName);

    current = dialog-general-language;
    current[0] = current[0].toUpper();
    comboboxLanguage.set_current_index(comboboxLanguage.findText(current));
    connect(comboboxLanguage, SIGNAL(current-index_changed("")), this, SLOT(comboboxLanguageCurrent-index_changed("")));

    vboxLayoutLanguage = tk.VBoxLayout(groupboxLanguage);
    vboxLayoutLanguage, add_widget(labelLanguage);
    vboxLayoutLanguage, add_widget(comboboxLanguage);
    groupboxLanguage.setLayout(vboxLayoutLanguage); */

    /* Icons */ /*
    groupboxIcon = tk.groupbox(translate("Icons"), widget);

    labelIconTheme = tk.Label(translate("Icon Theme"), groupboxIcon);
    comboboxIconTheme = tk.combobox(groupboxIcon);
    dir = tk.App.applicationDirPath();
    dir.cd("icons");
    dialog-general-icon-theme = general-icon-theme;
    for dirName in dir.entryList(QDir-Dirs | tk.Dir-NoDotAndDotDot)
        comboboxIconTheme.add_item(load-icon(theme-xpm), dirName);

    comboboxIconTheme.set_current_index(comboboxIconTheme.findText(dialog-general-icon-theme));
    connect(comboboxIconTheme, SIGNAL(current-index_changed("")), this, SLOT(comboboxIconThemeCurrent-index_changed("")));

    labelIconSize = tk.Label(translate("Icon Size"), groupboxIcon);
    combobox-icon-size = tk.combobox(groupboxIcon);
    combobox-icon-size.add_item(load-icon("icon16-xpm"), "Very Small", 16);
    combobox-icon-size.add_item(load-icon("icon24-xpm"), "Small", 24);
    combobox-icon-size.add_item(load-icon("icon32-xpm"), "Medium", 32);
    combobox-icon-size.add_item(load-icon("icon48-xpm"), "Large", 48);
    combobox-icon-size.add_item(load-icon("icon64-xpm"), "Very Large", 64);
    combobox-icon-size.add_item(load-icon("icon128-xpm"), "I'm Blind", 128);
    dialog-general-icon-size = general-icon-size;
    combobox-icon-size.set_current_index(combobox-icon-size.findData(dialog-general-icon-size));
    connect(combobox-icon-size, SIGNAL(current-index_changed(int)), this, SLOT(combobox-icon-sizeCurrent-index_changed(int)));

    vbox-layout-icon = tk.QVBoxLayout(groupboxIcon);
    vbox-layout-icon, add_widget(labelIconTheme);
    vbox-layout-icon, add_widget(comboboxIconTheme);
    vbox-layout-icon, add_widget(labelIconSize);
    vbox-layout-icon, add_widget(combobox-icon-size);
    groupboxIcon.setLayout(vbox-layout-icon); */

    /* Mdi Background */ /*
    groupboxMdiBG = tk.groupbox(window, text=translate("Background"));

    checkbox-mdi-bg-use-logo = tk.checkbox(groupboxMdiBG, text=translate("Use Logo"));
    dialog-general-mdi-bg-use-logo = general-mdi-bg-use-logo;
    preview["general-mdi-bg-use-logo = dialog-general-mdi-bg-use-logo;
    checkbox-mdi-bg-use-logo.set_checked(preview["general-mdi-bg-use-logo);
    connect(checkbox-mdi-bg-use-logo, SIGNAL(-state_changed(int)), this, SLOT(checkboxGeneralMdiBGUseLogo-state_changed(int)));

    button-mdi-bg-logo = tk.Button(groupboxMdiBG, text=translate("Choose"));
    button-mdi-bg-logo.setEnabled(dialog-general-mdi-bg-use-logo);
    dialog-general-mdi-bg-logo = general-mdi-bg-logo;
    accept["general-mdi-bg-logo = dialog-general-mdi-bg-logo;
    connect(button-mdi-bg-logo, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundLogo()));
    connect(checkbox-mdi-bg-use-logo, SIGNAL(toggled(int)), button-mdi-bg-logo, SLOT(setEnabled(int)));

    checkbox-mdi-bg-use-texture = tk.checkbox(translate("Use Texture"), groupboxMdiBG);
    dialog-general-mdi-bg-use-texture = general-mdi-bg-use-texture;
    preview["general-mdi-bg-use-texture = dialog-general-mdi-bg-use-texture;
    checkbox-mdi-bg-use-texture.set_checked(preview["general-mdi-bg-use-texture);
    connect(checkbox-mdi-bg-use-texture, SIGNAL(-state_changed(int)), this, SLOT(checkboxGeneralMdiBGUseTexture-state_changed(int)));

    buttonMdiBGTexture = tk.Button(groupboxMdiBG, text=translate("Choose"))
    buttonMdiBGTexture.setEnabled(dialog-general-mdi-bg-use-texture)
    dialog-general-mdi-bg-texture = general-mdi-bg-texture
    accept["general-mdi-bg-texture = dialog-general-mdi-bg-texture
    connect(buttonMdiBGTexture, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundTexture()))
    connect(checkbox-mdi-bg-use-texture, SIGNAL(toggled(int)), buttonMdiBGTexture, SLOT(setEnabled(int)))

    checkboxMdiBGUseColor = tk.checkbox(translate("Use Color"), groupboxMdiBG)
    dialog-general-mdi-bg-use-color = general-mdi-bg-use-color
    preview["general-mdi-bg-use-color = dialog-general-mdi-bg-use-color
    checkboxMdiBGUseColor.set_checked(preview.general-mdi-bg-use-color)
    connect(checkboxMdiBGUseColor, SIGNAL(-state_changed(int)), this, SLOT(checkboxGeneralMdiBGUseColor-state_changed(int)))

    buttonMdiBGColor = PushButton(translate("Choose"), groupboxMdiBG)
    buttonMdiBGColor.setEnabled(dialog-general-mdi-bg-use-color)
    dialog-general-mdi-bg-color = general-mdi-bg-color
    preview["general-mdi-bg-color = dialog-general-mdi-bg-color
    accept["general-mdi-bg-color = dialog-general-mdi-bg-color
    mdiBGPix = Image(16,16)
    mdiBGPix.fill(Color(preview.general-mdi-bg-color))
    buttonMdiBGColor.setIcon(QIcon(mdiBGPix))
    connect(buttonMdiBGColor, SIGNAL(clicked()), this, SLOT(chooseGeneralMdiBackgroundColor()))
    connect(checkboxMdiBGUseColor, SIGNAL(toggled(int)), buttonMdiBGColor, SLOT(setEnabled(int)))

    gridLayoutMdiBG = tk.GridLayout(widget)
    gridLayoutMdiBG, add_widget(checkbox-mdi-bg-use-logo, 0, 0, "Align Left")
    gridLayoutMdiBG, add_widget(button-mdi-bg-logo, 0, 1, "Align Right")
    gridLayoutMdiBG, add_widget(checkbox-mdi-bg-use-texture, 1, 0, "Align Left")
    gridLayoutMdiBG, add_widget(buttonMdiBGTexture, 1, 1, "Align Right")
    gridLayoutMdiBG, add_widget(checkboxMdiBGUseColor, 2, 0, "Align Left")
    gridLayoutMdiBG, add_widget(buttonMdiBGColor, 2, 1, "Align Right")
    groupboxMdiBG.setLayout(gridLayoutMdiBG); */

    /* Tips */ /*
    groupboxTips = tk.groupbox(translate("Tips"), widget)

    checkboxTipOfTheDay = tk.checkbox(translate("Show Tip of the Day on startup"), groupboxTips)
    dialog-general-tip-of-the-day = general-tip-of-the-day
    checkboxTipOfTheDay.set_checked(dialog-general-tip-of-the-day)
    connect(checkboxTipOfTheDay, SIGNAL(-state_changed(int)), this, SLOT(checkboxTipOfTheDay-state_changed(int)))

    vboxLayoutTips = tk.VBoxLayout(groupboxTips)
    vboxLayoutTips, add_widget(checkboxTipOfTheDay)
    groupboxTips.setLayout(vboxLayoutTips); */

    /* Help Browser */ /*
    groupboxHelpBrowser = tk.groupbox(translate("Help Browser"), widget)

    radio-button-system-help-browser = tk.RadioButton(translate("System"), groupboxHelpBrowser)
    radio-button-system-help-browser.set_checked(general-system-help-browser)
    radio-button-custom-help-browser = tk.RadioButton(translate("Custom"), groupboxHelpBrowser)
    radio-button-custom-help-browser.set_checked(!general-system-help-browser)
    radio-button-custom-help-browser.setEnabled(0) TODO: finish this */

    /*
    vboxLayoutHelpBrowser = tk.VBoxLayout(groupboxHelpBrowser)
    vboxLayoutHelpBrowser, add_widget(radio-button-system-help-browser)
    vboxLayoutHelpBrowser, add_widget(radio-button-custom-help-browser)
    groupboxHelpBrowser.setLayout(vboxLayoutHelpBrowser)
*/
    /* Widget Layout */ /*
    vbox_layout_main = tk.VBoxLayout(widget)
    vbox_layout_main, add_widget(groupboxLanguage)
    vbox_layout_main, add_widget(groupboxIcon)
    vbox_layout_main, add_widget(groupboxMdiBG)
    vbox_layout_main, add_widget(groupboxTips)
    vbox_layout_main, add_widget(groupboxHelpBrowser)
    vbox_layout_main.addStretch(1)
    widget.setLayout(vbox_layout_main)

    scroll_area = tk.scroll_area(this)
    scroll_area.setWidgetResizable(1)
    scroll_area.setWidget(widget)
    return scroll_area */
}

void
create_tab_files_paths(void)
{
    /* widget = tk.Widget(this)

    scroll_area = tk.scroll_area(this)
    scroll_area.setWidgetResizable(1)
    scroll_area.setWidget(widget)
    return scroll_area */
}

void
create_tab_display(void)
{
    /*
    widget = tk.Widget(this) */

    /* Rendering
     * TODO: Review OpenGL and Rendering settings for future inclusion
     */ /*
    groupboxRender = tk.groupbox(translate("Rendering"), widget)

    checkboxUseOpenGL = tk.checkbox(translate("Use OpenGL"), groupboxRender)
    int-dialog[INT-DISPLAY-USE-OPENGL] = int-setting[INT-DISPLAY-USE-OPENGL]
    checkboxUseOpenGL.set_checked(int-dialog[INT-DISPLAY-USE-OPENGL)
    connect(checkboxUseOpenGL, SIGNAL(-state_changed(int)), this, SLOT(checkboxUseOpenGL-state_changed(int)))

    checkboxRenderHintAA = tk.checkbox(translate("Antialias"), groupboxRender)
    dialog_display_renderhint-aa = display-render-hint-aa
    checkboxRenderHintAA.set_checked(dialog_display_renderhint-aa)
    connect(checkboxRenderHintAA, SIGNAL(-state_changed(int)), this, SLOT(checkboxRenderHintAA-state_changed(int)))

    checkboxRenderHintTextAA = tk.checkbox(translate("Antialias Text"), groupboxRender)
    dialog_display_renderhint-text-aa = display-render-hint-text-aa
    checkboxRenderHintTextAA.set_checked(dialog_display_renderhint-text-aa)
    connect(checkboxRenderHintTextAA, SIGNAL(-state_changed(int)), this, SLOT(checkboxRenderHintTextAA-state_changed(int)))

    checkboxRenderHintSmoothPix = tk.checkbox(translate("Smooth Pixmap"), groupboxRender)
    dialog_display_renderhint-smooth-pix = display-render-hint-smooth-pix
    checkboxRenderHintSmoothPix.set_checked(dialog_display_renderhint-smooth-pix)
    connect(checkboxRenderHintSmoothPix, SIGNAL(-state_changed(int)), this, SLOT(checkboxRenderHintSmoothPix-state_changed(int)))

    checkboxRenderHintHighAA = tk.checkbox(translate("High Quality Antialiasing (OpenGL)"), groupboxRender)
    dialog_display_renderhint-high-aa = display-render-hint-high-aa
    checkboxRenderHintHighAA.set_checked(dialog_display_renderhint-high-aa)
    connect(checkboxRenderHintHighAA, SIGNAL(-state_changed(int)), this, SLOT(checkboxRenderHintHighAA-state_changed(int)))

    checkboxRenderHintNonCosmetic = tk.checkbox(translate("Non Cosmetic"), groupboxRender)
    dialog_display_renderhint-noncosmetic = display-render-hint-non-cosmetic
    checkboxRenderHintNonCosmetic.set_checked(dialog_display_renderhint-noncosmetic)
    connect(checkboxRenderHintNonCosmetic, SIGNAL(-state_changed(int)), this, SLOT(checkboxRenderHintNonCosmetic-state_changed(int)))

    vboxLayoutRender = tk.VBoxLayout(groupboxRender)
    vboxLayoutRender, add_widget(checkboxUseOpenGL)
    vboxLayoutRender, add_widget(checkboxRenderHintAA)
    vboxLayoutRender, add_widget(checkboxRenderHintTextAA)
    vboxLayoutRender, add_widget(checkboxRenderHintSmoothPix)
    vboxLayoutRender, add_widget(checkboxRenderHintHighAA)
    vboxLayoutRender, add_widget(checkboxRenderHintNonCosmetic)
    groupboxRender.setLayout(vboxLayoutRender) */

    /* ScrollBars */ /*
    groupboxScrollBars = tk.groupbox(translate("ScrollBars"), widget)

    checkboxShowScrollBars = tk.checkbox(translate("Show ScrollBars"), groupboxScrollBars)
    dialog_display_show-scrollbars = display-show-scrollbars
    preview.display-show-scrollbars = dialog_display_show-scrollbars
    checkboxShowScrollBars.set_checked(preview.display-show-scrollbars)
    connect(checkboxShowScrollBars, SIGNAL(-state_changed(int)), this, SLOT(checkboxShowScrollBars-state_changed(int)))

    labelScrollBarWidget = tk.Label(translate("Perform action when clicking corner widget"), groupboxScrollBars)
    comboboxScrollBarWidget = tk.combobox(groupboxScrollBars)
    dialog_display_scrollbar-widget-num = display-scrollbar-widget-num
    numActions = actionHash.size();
    for (i=0; i<numActions; i++) {
        action = actionHash.value(i);
        if (action) {
            comboboxScrollBarWidget.add_item(action.icon(), action.text().replace("&", ""));
        }
    }

    comboboxScrollBarWidget.set_current_index(dialog_display_scrollbar-widget-num);
    connect(comboboxScrollBarWidget, SIGNAL(current-index_changed(int)), this, SLOT(comboboxScrollBarWidgetCurrent-index_changed(int)));

    vbox_layout_scrollbars = tk.VBoxLayout(groupboxScrollBars);
    vbox_layout_scrollbars, add_widget(checkboxShowScrollBars);
    vbox_layout_scrollbars, add_widget(labelScrollBarWidget);
    vbox_layout_scrollbars, add_widget(comboboxScrollBarWidget);
    groupboxScrollBars.setLayout(vbox_layout_scrollbars);
    */

    /* Colors */ /*
    groupboxColor = tk.groupbox(translate("Colors"), widget);

    labelCrossHairColor = tk.Label(translate("Crosshair Color"), groupboxColor);
    buttonCrossHairColor = PushButton(translate("Choose"), groupboxColor);
    dialog_display_crosshair-color = display-crosshair-color;
    preview.display-crosshair-color = dialog_display_crosshair-color;
    accept.display-crosshair-color = dialog_display_crosshair-color;
    crosshairPix = (16,16);
    crosshairPix.fill(Color(preview.display-crosshair-color));
    buttonCrossHairColor.setIcon(QIcon(crosshairPix));
    connect(buttonCrossHairColor, SIGNAL(clicked()), this, SLOT(chooseDisplayCrossHairColor()));

    labelBGColor = tk.Label(translate("Background Color"), groupboxColor);
    buttonBGColor = PushButton(translate("Choose"), groupboxColor);
    dialog_display_bg-color = display-bg-color;
    preview.display-bg-color = dialog_display_bg-color;
    accept.display-bg-color = dialog_display_bg-color;
    bgPix = Image(16,16);
    bgPix.fill(Color(preview.display-bg-color));
    buttonBGColor.setIcon(QIcon(bgPix));
    connect(buttonBGColor, SIGNAL(clicked()), this, SLOT(chooseDisplayBackgroundColor()));

    labelSelectBoxLeftColor = tk.Label(translate("Selection Box Color (Crossing)"), groupboxColor);
    buttonSelectBoxLeftColor = PushButton(translate("Choose"), groupboxColor);
    dialog_display_selectbox_left-color = display-selectbox-left-color;
    preview["display-selectbox-left-color = dialog_display_selectbox_left-color;
    accept["display-selectbox-left-color = dialog_display_selectbox_left-color;
    sBoxLCPix = Image(16,16);
    sBoxLCPix.fill(Color(preview["display-selectbox-left-color));
    buttonSelectBoxLeftColor.setIcon(QIcon(sBoxLCPix));
    connect(buttonSelectBoxLeftColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftColor()));

    label_select_box_left_fill = tk.Label(translate("Selection Box Fill (Crossing)"), groupboxColor);
    buttonSelectBoxLeftFill = PushButton(translate("Choose"), groupboxColor);
    copy_seting("dialog-display_selectbox-left-fill",
        "display-selectbox-left-fill");
    copy_seting("preview-display-selectbox-left-fill",
        "dialog-display-selectbox-left-fill");
    accept-display-selectbox-left-fill = dialog_display_selectbox_left-fill;
    sBoxLFPix = Image(16,16);
    sBoxLFPix.fill(Color(preview["display-selectbox-left-fill));
    buttonSelectBoxLeftFill.setIcon(QIcon(sBoxLFPix));
    connect(buttonSelectBoxLeftFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxLeftFill()));

    labelSelectBoxRightColor = tk.Label(translate("Selection Box Color (Window)"), groupboxColor);
    buttonSelectBoxRightColor = tk.PushButton(translate("Choose"), groupboxColor);
    dialog_display_selectbox_right-color = display-selectbox-right-color;
    preview["display-selectbox-right-color = dialog_display_selectbox_right-color;
    accept["display-selectbox-right-color = dialog_display_selectbox_right-color;
    sBoxRCPix = Image(16,16);
    sBoxRCPix.fill(Color(preview["display-selectbox-right-color));
    buttonSelectBoxRightColor.setIcon(QIcon(sBoxRCPix));
    connect(buttonSelectBoxRightColor, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightColor()));

    labelSelectBoxRightFill = tk.Label(translate("Selection Box Fill (Window)"), groupboxColor);
    buttonSelectBoxRightFill = PushButton(translate("Choose"), groupboxColor);
    dialog_display_selectbox_right-fill = display-selectbox-right-fill;
    preview.display-selectbox-right-fill = dialog_display_selectbox_right-fill;
    accept.display-selectbox-right-fill = dialog_display_selectbox_right-fill;
    sBoxRFPix = Image(16,16);
    sBoxRFPix.fill(Color(preview.display-selectbox-right-fill));
    buttonSelectBoxRightFill.setIcon(QIcon(sBoxRFPix));
    connect(buttonSelectBoxRightFill, SIGNAL(clicked()), this, SLOT(chooseDisplaySelectBoxRightFill()));

    labelSelectBoxAlpha = tk.Label(translate("Selection Box Fill Alpha"), groupboxColor);
    spinboxSelectBoxAlpha = tk.spinbox(groupboxColor);
    spinboxSelectBoxAlpha.setRange(0, 255);
    dialog_display_selectbox_alpha = display-selectbox-alpha;
    preview.display-selectbox-alpha = dialog_display_selectbox_alpha;
    spinboxSelectBoxAlpha.setValue(preview.display-selectbox-alpha);
    connect(spinboxSelectBoxAlpha, SIGNAL(_value_changed(int)), this, SLOT(spinboxDisplaySelectBoxAlpha_value_changed(int)));

    grid_layout_color = tk.GridLayout(widget);
    add_widget(grid_layout_color, labelCrossHairColor, 0, 0, "Align Left");
    add_widget(grid_layout_color, buttonCrossHairColor, 0, 1, "Align Right");
    add_widget(grid_layout_color, labelBGColor, 1, 0, "Align Left");
    add_widget(grid_layout_color, buttonBGColor, 1, 1, "Align Right");
    add_widget(grid_layout_color, labelSelectBoxLeftColor, 2, 0, "Align Left");
    add_widget(grid_layout_color, buttonSelectBoxLeftColor, 2, 1, "Align Right");
    add_widget(grid_layout_color, label_select_box_left_fill, 3, 0, "Align Left");
    add_widget(grid_layout_color, buttonSelectBoxLeftFill, 3, 1, "Align Right");
    grid_layout_color, add_widget(labelSelectBoxRightColor, 4, 0, "Align Left");
    grid_layout_color, add_widget(buttonSelectBoxRightColor, 4, 1, "Align Right");
    grid_layout_color, add_widget(labelSelectBoxRightFill, 5, 0, "Align Left");
    grid_layout_color, add_widget(buttonSelectBoxRightFill, 5, 1, "Align Right");
    grid_layout_color, add_widget(labelSelectBoxAlpha, 6, 0, "Align Left");
    grid_layout_color, add_widget(spinboxSelectBoxAlpha, 6, 1, "Align Right");
    groupboxColor.setLayout(grid_layout_color); */

    /* Zoom */
    /*
    groupbox_zoom = tk.groupbox(translate("Zoom"), widget);

    label_zoom_scale_action_in = tk.Label(translate("Zoom In Scale"), groupbox_zoom);
    spinbox_zoom_scale_action_in = tk.Doublespinbox(groupbox_zoom);
    dialog_display_zoom-scale-action-in = display-zoom-scale-action-in;
    spinbox_zoom_scale_action_in.setValue(dialog_display_zoom-scale-action-in);
    spinbox_zoom_scale_action_in.setSingleStep(0.01);
    spinbox_zoom_scale_action_in.setRange(1.01, 10.00);
    connect(spinbox_zoom_scale_action_in, SIGNAL(_value_changed(double)), this, SLOT(spinbox_zoom_scale_action_in_value_changed(double)));

    labelzoom-scale-actionOut = tk.Label(translate("Zoom Out Scale"), groupbox_zoom);
    spinboxzoom-scale-actionOut = tk.Doublespinbox(groupbox_zoom);
    dialog_display_zoom-scale-action-out = display-zoom-scale-action-out;
    spinboxzoom-scale-actionOut.setValue(dialog_display_zoom-scale-action-out);
    spinboxzoom-scale-actionOut.setSingleStep(0.01);
    spinboxzoom-scale-actionOut.setRange(0.01, 0.99);
    connect(spinboxzoom-scale-actionOut, SIGNAL(_value_changed(double)), this, SLOT(spinboxzoom-scale-actionOut_value_changed(double)));

    grid_layout_zoom = tk.GridLayout(groupbox_zoom);
    add_widget(grid_layout_zoom, label_zoom_scale_action_in, 0, 0, "Align Left");
    add_widget(grid_layout_zoom, spinbox_zoom_scale_action_in, 0, 1, "Align Right");
    add_widget(grid_layout_zoom, labelzoom-scale-actionOut, 1, 0, "Align Left");
    add_widget(grid_layout_zoom, spinboxzoom-scale-actionOut, 1, 1, "Align Right");
    groupbox_zoom.setLayout(grid_layout_zoom);
    */

    /* Widget Layout */
    /*
    vbox_layout_main = tk.VBoxLayout(widget);
    #vbox_layout_main, add_widget(groupboxRender);
    # TODO: Review OpenGL and Rendering settings for future inclusion
    vbox_layout_main, add_widget(groupboxScrollBars);
    vbox_layout_main, add_widget(groupboxColor);
    vbox_layout_main, add_widget(groupbox_zoom);
    vbox_layout_main.addStretch(1);
    widget.setLayout(vbox_layout_main);

    scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

/* TODO: finish open/save options */
void
create_tab_open_save(void)
{
    int i;
    /* widget = tk.Widget(this); */

    /* Custom Filter */
    /*
    groupboxCustomFilter = tk.groupbox(translate("Custom Filter"), widget);
    groupboxCustomFilter.setEnabled(0);
    */
    /* TODO: Fixup custom filter */

    /* buttonCustomFilterSelectAll = PushButton(translate("Select All"), groupboxCustomFilter);
    connect(buttonCustomFilterSelectAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterSelectAllClicked()));
    buttonCustomFilterClearAll = PushButton("Clear All", groupboxCustomFilter);
    connect(buttonCustomFilterClearAll, SIGNAL(clicked()), this, SLOT(buttonCustomFilterClearAllClicked()));
    grid_layout_custom_filter = tk.GridLayout(groupboxCustomFilter); */

    for (i=0; i<numberOfFormats; i++) {
        /*
        c = checkbox(formatTable[i].extension, groupboxCustomFilter);
        c.set_checked(opensave-custom-filter.contains(QString("*") + formatTable[i].extension, tk.t-CaseInsensitive));
        connect(c, SIGNAL(-state_changed(int)), this, SLOT(checkboxCustomFilter-state_changed(int)));
        connect(this, SIGNAL(buttonCustomFilterSelectAll(int)), c, SLOT(set_checked(int)));
        connect(this, SIGNAL(buttonCustomFilterClearAll(int)), c, SLOT(set_checked(int)));
        add_widget(grid_layout_custom_filter,
            c, i%10, i/10, "Align Left");
        */
    }

    /*
    add_widget(grid_layout_custom_filter,
        buttonCustomFilterSelectAll, 0, 7, "Align Left");
    add_widget(grid_layout_custom_filter,
        buttonCustomFilterClearAll, 1, 7, "Align Left");
    grid_layout_custom_filter.setColumnStretch(7,1);
    setLayout(groupboxCustomFilter,
        grid_layout_custom_filter);

    if (opensave-custom-filter.contains("supported", tk.t-CaseInsensitive)) {
        buttonCustomFilterSelectAllClicked()
    } */

    /* Opening */ /*
    groupboxOpening = tk.groupbox(translate("File Open"), widget)

    comboboxOpenFormat = tk.combobox(groupboxOpening);

    checkboxOpenThumbnail = tk.checkbox(translate("Preview Thumbnails"), groupboxOpening);
    checkboxOpenThumbnail.set_checked(0); */

    /* TODO: Add a button to clear the recent history. */
    /*
    labelRecentMaxFiles = tk.Label(translate("Number of recently accessed files to show"), groupboxOpening);
    spinboxRecentMaxFiles = tk.spinbox(groupboxOpening);
    spinboxRecentMaxFiles.setRange(0, 10);
    dialog-opensave-recent-max-files = opensave-recent-max-files;
    spinboxRecentMaxFiles.setValue(dialog-opensave-recent-max-files);
    connect(spinboxRecentMaxFiles, SIGNAL(_value_changed(int)), this, SLOT(spinboxRecentMaxFiles_value_changed(int)));

    frameRecent = tk.Frame(groupboxOpening);
    grid_layout_recent = tk.GridLayout(frameRecent);
    add_widget(grid_layout_recent,
        labelRecentMaxFiles, 0, 0, "AlignLeft");
    add_widget(grid_layout_recent,
        spinboxRecentMaxFiles, 0, 1, "Align Right");
    frameRecent.setLayout(grid_layout_recent);

    vboxLayoutOpening = tk.VBoxLayout(groupboxOpening);
    vboxLayoutOpening, add_widget(comboboxOpenFormat);
    vboxLayoutOpening, add_widget(checkboxOpenThumbnail);
    vboxLayoutOpening, add_widget(frameRecent);
    groupboxOpening.setLayout(vboxLayoutOpening);

    #Saving
    groupboxSaving = tk.groupbox(translate("File Save"), widget);

    comboboxSaveFormat = tk.combobox(groupboxSaving);

    checkboxSaveThumbnail = tk.checkbox(translate("Save Thumbnails"), groupboxSaving);
    checkboxSaveThumbnail.set_checked(0);

    checkboxAutoSave = tk.checkbox(translate("AutoSave"), groupboxSaving);
    checkboxAutoSave.set_checked(0);

    vboxLayoutSaving = tk.VBoxLayout(groupboxSaving);
    vboxLayoutSaving, add_widget(comboboxSaveFormat);
    vboxLayoutSaving, add_widget(checkboxSaveThumbnail);
    vboxLayoutSaving, add_widget(checkboxAutoSave);
    groupboxSaving.setLayout(vboxLayoutSaving);

    #Trimming
    groupboxTrim = tk.groupbox(translate("Trimming"), widget);

    labelTrimDstNumJumps = tk.Label(translate("DST Only: Minimum number of jumps to trim"), groupboxTrim);
    spinboxTrimDstNumJumps = tk.spinbox(groupboxTrim);
    spinboxTrimDstNumJumps.setRange(1, 20);
    dialog-opensave-trim-dst-num-jumps = opensave-trim-dst-num-jumps;
    spinboxTrimDstNumJumps.setValue(dialog-opensave-trim-dst-num-jumps);
    connect(spinboxTrimDstNumJumps, SIGNAL(_value_changed(int)), this, SLOT(spinboxTrimDstNumJumps_value_changed(int)));

    frameTrimDstNumJumps = tk.Frame(groupboxTrim);
    gridLayoutTrimDstNumJumps = tk.GridLayout(frameTrimDstNumJumps);
    gridLayoutTrimDstNumJumps, add_widget(labelTrimDstNumJumps, 0, 0, "Align Left");
    gridLayoutTrimDstNumJumps, add_widget(spinboxTrimDstNumJumps, 0, 1, "Align Right");
    frameTrimDstNumJumps.setLayout(gridLayoutTrimDstNumJumps);

    vboxLayoutTrim = tk.VBoxLayout(groupboxTrim);
    vboxLayoutTrim, add_widget(frameTrimDstNumJumps);
    groupboxTrim.setLayout(vboxLayoutTrim);

    #Widget Layout
    vbox_layout_main = tk.VBoxLayout(widget);
    vbox_layout_main, add_widget(groupboxCustomFilter);
    vbox_layout_main, add_widget(groupboxOpening);
    vbox_layout_main, add_widget(groupboxSaving);
    vbox_layout_main, add_widget(groupboxTrim);
    vbox_layout_main.addStretch(1);
    widget.setLayout(vbox_layout_main);

    scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

void
create_tab_printing(void)
{
    /*
    widget = tk.Widget(this);

    # Default Printer
    groupboxDefaultPrinter = tk.groupbox(translate("Default Printer"), widget);

    radioButtonUseSame = tk.RadioButton(translate("Use as default device"), groupboxDefaultPrinter);
    radioButtonUseSame.set_checked(!printing-use-last-device);
    radioButtonUseLast = tk.RadioButton(translate("Use last used device"), groupboxDefaultPrinter);
    radioButtonUseLast.set_checked(printing-use-last-device);

    comboboxDefaultDevice = tk.combobox(groupboxDefaultPrinter);
    listAvailPrinters = tk.PrinterInfo-availablePrinters();
    for info in listAvailPrinters:
        comboboxDefaultDevice.add_item(load-icon(print-xpm), info.printerName());

    vboxLayoutDefaultPrinter = tk.VBoxLayout(groupboxDefaultPrinter);
    vboxLayoutDefaultPrinter, add_widget(radioButtonUseSame);
    vboxLayoutDefaultPrinter, add_widget(comboboxDefaultDevice);
    vboxLayoutDefaultPrinter, add_widget(radioButtonUseLast);
    groupboxDefaultPrinter.setLayout(vboxLayoutDefaultPrinter);

    # Save Ink
    groupboxSaveInk = tk.groupbox(translate("Save Ink"), widget);

    checkboxDisableBG = tk.checkbox(translate("Disable Background"), groupboxSaveInk);
    dialog-printing-disable-bg = printing-disable-bg;
    checkboxDisableBG.set_checked(dialog-printing-disable-bg);
    connect(checkboxDisableBG, SIGNAL(-state_changed(int)), this, SLOT(checkboxDisableBG-state_changed(int)));

    vboxLayoutSaveInk = tk.VBoxLayout(groupboxSaveInk);
    vboxLayoutSaveInk, add_widget(checkboxDisableBG);
    groupboxSaveInk.setLayout(vboxLayoutSaveInk); */

    /* Widget Layout */ /*
    vbox_layout_main = tk.VBoxLayout(widget);
    vbox_layout_main, add_widget(groupboxDefaultPrinter);
    vbox_layout_main, add_widget(groupboxSaveInk);
    vbox_layout_main.addStretch(1);
    widget.setLayout(vbox_layout_main);

    scroll_area = tk.scroll_area(this);
    #scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

void
copy_setting(Window *w, char *to, char *from)
{
    set_str(w, to, get_str(w, from));
}

void
create_tab_snap(void)
{
    /*
    widget = tk.Widget(this); */

    /* TODO: finish this */

    /* scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

void
create_tab_grid_ruler(void)
{
    /*
    create_settings_tab(grid_ruler_settings);
    widget = tk.Widget(this);

    #Grid Color
    groupboxGridColor = tk.groupbox(translate("Grid Color"), widget);

    labelGridColor = tk.Label(translate("Grid Color"), groupboxGridColor);
    labelGridColor.setObjectName("labelGridColor");
    buttonGridColor = PushButton(translate("Choose"), groupboxGridColor);
    buttonGridColor.setObjectName("buttonGridColor");
    if dialogGridcolor-match-crosshair:
        dialogGridcolor = display-crosshair-color;
    else {
        dialogGridcolor = grid-color;
    preview.grid-color = dialogGridcolor;
    accept.grid-color = dialogGridcolor;
    gridPix = Image(16,16);
    gridPix.fill(Color(preview.grid-color));
    buttonGridColor.setIcon(QIcon(gridPix));
    connect(buttonGridColor, SIGNAL(clicked()), this, SLOT(chooseGridColor()));

    labelGridColor.setEnabled(!dialogGridcolor-match-crosshair);
    buttonGridColor.setEnabled(!dialogGridcolor-match-crosshair);

    gridLayoutGridColor = tk.GridLayout(widget);
    gridLayoutGridColor, add_widget(checkboxGridColorMatchCrossHair, 0, 0, "Align Left");
    gridLayoutGridColor, add_widget(labelGridColor, 1, 0, "Align Left");
    gridLayoutGridColor, add_widget(buttonGridColor, 1, 1, "Align Right");
    groupboxGridColor.setLayout(gridLayoutGridColor);

    #Grid Geometry
    groupboxGridGeom = tk.groupbox(translate("Grid Geometry"), widget);

    checkboxGridload-from-file = tk.checkbox(translate("Set grid size from opened file"), groupboxGridGeom);
    dialogGridload-from-file = grid-load-from-file;
    checkboxGridload-from-file.set_checked(dialogGridload-from-file);
    connect(checkboxGridload-from-file, SIGNAL(-state_changed(int)), this, SLOT(checkboxGridload-from-file-state_changed(int)))

    labelGridtype = tk.Label(translate("Grid Type"), groupboxGridGeom);
    labelGridtype.setObjectName("labelGridtype");
    combobox_grid_type = tk.combobox(groupboxGridGeom);
    combobox_grid_type.setObjectName("combobox_grid_type");
    add_item(combobox_grid_type, "Rectangular");
    add_item(combobox_grid_type, "Circular");
    add_item(combobox_grid_type, "Isometric");
    strcpy(dialogGridtype, grid-type);
    set_current_index(combobox_grid_type, combobox_grid_type.findText(dialogGridtype));
    connect(combobox_grid_type, SIGNAL(current-index_changed("")), this, SLOT(combobox_grid_typeCurrent-index_changed("")));

    checkbox_grid_center_on_origin = tk.checkbox(translate("Center the grid on the origin"), groupboxGridGeom);
    setObjectName(checkbox_grid_center_on_origin,
        "checkbox_grid_center_on_origin");
    copy_setting("dialog-grid-center-on-origin", "grid-center-on-origin");
    set_checked(checkbox_grid_center_on_origin, dialog_grid_center_on_origin);
    connect(checkbox_grid_center_on_origin, SIGNAL(-state_changed(int)), this, SLOT(checkbox_grid_center_on_origin-state_changed(int)));

    label_grid_center_x = tk.Label(translate("Grid Center X"), groupboxGridGeom);
    label_grid_center_x.setObjectName("label_grid_center_x");
    spinboxGridCenterX = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridCenterX.setObjectName("spinboxGridCenterX");
    dialogGridcenter.x = grid-center.x;
    spinboxGridCenterX.setSingleStep(1.000);
    spinboxGridCenterX.setRange(-1000.000, 1000.000);
    spinboxGridCenterX.setValue(dialogGridcenter.x);
    connect(spinboxGridCenterX, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridCenterX_value_changed(double)));

    labelGridcenter-y = tk.Label(translate("Grid Center Y"), groupboxGridGeom);
    labelGridcenter-y.setObjectName("labelGridcenter-y");
    spinboxGridcenter-y = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridcenter-y.setObjectName("spinboxGridcenter-y");
    dialogGridcenter.y = grid-center.y;
    spinboxGridcenter-y.setSingleStep(1.000);
    spinboxGridcenter-y.setRange(-1000.000, 1000.000);
    spinboxGridcenter-y.setValue(dialogGridcenter.y);
    connect(spinboxGridcenter-y, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridcenter-y_value_changed(double)));

    labelGridsizeX = tk.Label(translate("Grid Size X"), groupboxGridGeom);
    labelGridsizeX.setObjectName("labelGridsizeX");
    spinboxGridsizeX = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridsizeX.setObjectName("spinboxGridsizeX");
    dialogGridsize.x = grid-size.x;
    spinboxGridsizeX.setSingleStep(1.000);
    spinboxGridsizeX.setRange(1.000, 1000.000);
    spinboxGridsizeX.setValue(dialogGridsize.x);
    connect(spinboxGridsizeX, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridsizeX_value_changed(double)));

    labelGridsizeY = tk.Label(translate("Grid Size Y"), groupboxGridGeom);
    labelGridsizeY.setObjectName("labelGridsizeY");
    spinboxGridsizeY = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridsizeY.setObjectName("spinboxGridsizeY");
    dialogGridsize.y = grid-size.y;
    spinboxGridsizeY.setSingleStep(1.000);
    spinboxGridsizeY.setRange(1.000, 1000.000);
    spinboxGridsizeY.setValue(dialogGridsize.y);
    connect(spinboxGridsizeY, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridsizeY_value_changed(double)));

    labelGridSpacingX = tk.Label(translate("Grid Spacing X"), groupboxGridGeom);
    labelGridSpacingX.setObjectName("labelGridSpacingX");
    spinboxGridSpacingX = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridSpacingX.setObjectName("spinboxGridSpacingX");
    dialogGridspacing.x = grid-spacing.x;
    spinboxGridSpacingX.setSingleStep(1.000);
    spinboxGridSpacingX.setRange(0.001, 1000.000);
    spinboxGridSpacingX.setValue(dialogGridspacing.x);
    connect(spinboxGridSpacingX, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridSpacingX_value_changed(double)));

    labelGridSpacingY = tk.Label(translate("Grid Spacing Y"), groupboxGridGeom);
    labelGridSpacingY.setObjectName("labelGridSpacingY");
    spinboxGridSpacingY = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridSpacingY.setObjectName("spinboxGridSpacingY");
    dialogGridspacing.y = grid-spacing.y;
    spinboxGridSpacingY.setSingleStep(1.000);
    spinboxGridSpacingY.setRange(0.001, 1000.000);
    spinboxGridSpacingY.setValue(dialogGridspacing.y);
    connect(spinboxGridSpacingY, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridSpacingY_value_changed(double)));

    labelGridsizeRadius = tk.Label(translate("Grid Size Radius"), groupboxGridGeom);
    labelGridsizeRadius.setObjectName("labelGridsizeRadius");
    spinboxGridsizeRadius = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridsizeRadius.setObjectName("spinboxGridsizeRadius");
    dialogGridsize-radius = grid-size-radius;
    spinboxGridsizeRadius.setSingleStep(1.000);
    spinboxGridsizeRadius.setRange(1.000, 1000.000);
    spinboxGridsizeRadius.setValue(dialogGridsize-radius);
    connect(spinboxGridsizeRadius, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridsizeRadius_value_changed(double)));

    labelGridSpacingRadius = tk.Label(translate("Grid Spacing Radius"), groupboxGridGeom);
    labelGridSpacingRadius.setObjectName("labelGridSpacingRadius");
    spinboxGridSpacingRadius = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridSpacingRadius.setObjectName("spinboxGridSpacingRadius");
    dialogGridspacing-radius = grid-spacing-radius;
    spinboxGridSpacingRadius.setSingleStep(1.000);
    spinboxGridSpacingRadius.setRange(0.001, 1000.000);
    spinboxGridSpacingRadius.setValue(dialogGridspacing-radius);
    connect(spinboxGridSpacingRadius, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridSpacingRadius_value_changed(double)));

    labelGridSpacingAngle = tk.Label(translate("Grid Spacing Angle"), groupboxGridGeom);
    labelGridSpacingAngle.setObjectName("labelGridSpacingAngle");
    spinboxGridSpacingAngle = tk.Doublespinbox(groupboxGridGeom);
    spinboxGridSpacingAngle.setObjectName("spinboxGridSpacingAngle");
    dialogGridspacing-angle = grid-spacing-angle;
    spinboxGridSpacingAngle.setSingleStep(1.000);
    spinboxGridSpacingAngle.setRange(0.001, 1000.000);
    spinboxGridSpacingAngle.setValue(dialogGridspacing-angle);
    connect(spinboxGridSpacingAngle, SIGNAL(_value_changed(double)), this, SLOT(spinboxGridSpacingAngle_value_changed(double)));

    labelGridtype.setEnabled(!dialogGridloadFromFile);
    combobox_grid_type.setEnabled(!dialogGridloadFromFile);
    checkbox_grid_center_on_origin.setEnabled(!dialogGridload-from-file);
    label_grid_center_x.setEnabled(!dialogGridload-from-file);
    spinboxGridCenterX.setEnabled(!dialogGridload-from-file);
    labelGridcenter-y.setEnabled(!dialogGridload-from-file);
    spinboxGridcenter-y.setEnabled(!dialogGridload-from-file);
    labelGridsizeX.setEnabled(!dialogGridload-from-file);
    spinboxGridsizeX.setEnabled(!dialogGridload-from-file);
    labelGridsizeY.setEnabled(!dialogGridload-from-file);
    spinboxGridsizeY.setEnabled(!dialogGridload-from-file);
    labelGridSpacingX.setEnabled(!dialogGridload-from-file);
    spinboxGridSpacingX.setEnabled(!dialogGridload-from-file);
    labelGridSpacingY.setEnabled(!dialogGridload-from-file);
    spinboxGridSpacingY.setEnabled(!dialogGridload-from-file);
    labelGridsizeRadius.setEnabled(!dialogGridload-from-file);
    spinboxGridsizeRadius.setEnabled(!dialogGridload-from-file);
    labelGridSpacingRadius.setEnabled(!dialogGridload-from-file);
    spinboxGridSpacingRadius.setEnabled(!dialogGridload-from-file);
    labelGridSpacingAngle.setEnabled(!dialogGridload-from-file);
    spinboxGridSpacingAngle.setEnabled(!dialogGridload-from-file);

    int visibility = 0;
    if (dialogGridtype == "Circular") {
        visibility = 1;
    }
    labelGridsizeX.visible = !visibility;
    spinboxGridsizeX.visible = !visibility;
    labelGridsizeY.visible = !visibility;
    spinboxGridsizeY.visible = !visibility;
    labelGridSpacingX.visible = !visibility;
    spinboxGridSpacingX.visible = !visibility;
    labelGridSpacingY.visible = !visibility;
    spinboxGridSpacingY.visible = !visibility;
    labelGridsizeRadius.visible = visibility;
    spinboxGridsizeRadius.visible = visibility;
    labelGridSpacingRadius.visible = visibility;
    spinboxGridSpacingRadius.visible = visibility;
    labelGridSpacingAngle.visible = visibility;
    spinboxGridSpacingAngle.visible = visibility;

    gridLayoutGridGeom = tk.GridLayout(groupboxGridGeom);
    add_widget(gridLayoutGridGeom, checkboxGridload-from-file, 0, 0, "Align Left");
    add_widget(gridLayoutGridGeom, labelGridtype, 1, 0, "Align Left");
    add_widget(gridLayoutGridGeom, combobox_grid_type, 1, 1, "Align Right");
    add_widget(gridLayoutGridGeom, checkbox_grid_center_on_origin, 2, 0, "Align Left");
    add_widget(gridLayoutGridGeom, label_grid_center_x, 3, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridCenterX, 3, 1, "Align Right");
    add_widget(gridLayoutGridGeom, labelGridcenter-y, 4, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridcenter-y, 4, 1, "Align Right");
    add_widget(gridLayoutGridGeom, labelGridsizeX, 5, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridsizeX, 5, 1, "Align Right");
    add_widget(gridLayoutGridGeom, labelGridsizeY, 6, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridsizeY, 6, 1, "Align Right");
    add_widget(gridLayoutGridGeom, labelGridSpacingX, 7, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridSpacingX, 7, 1, "Align Right");
    add_widget(gridLayoutGridGeom, labelGridSpacingY, 8, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridSpacingY, 8, 1, "Align Right");
    add_widget(gridLayoutGridGeom, labelGridsizeRadius, 9, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridsizeRadius, 9, 1, "Align Right");
    add_widget(gridLayoutGridGeom, labelGridSpacingRadius, 10, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridSpacingRadius, 10, 1, "Align Right");
    add_widget(gridLayoutGridGeom, labelGridSpacingAngle, 11, 0, "Align Left");
    add_widget(gridLayoutGridGeom, spinboxGridSpacingAngle, 11, 1, "Align Right");
    groupboxGridGeom.setLayout(gridLayoutGridGeom);

    #Ruler Misc
    groupboxRulerMisc = tk.groupbox(translate("Ruler Misc"), widget);

    checkboxRulerShowOnLoad = tk.checkbox(translate("Initially show ruler when loading a file"), groupboxRulerMisc);
    dialog-ruler-show-on-load = ruler-show-on-load;
    checkboxRulerShowOnLoad.set_checked(dialog-ruler-show-on-load);
    connect(checkboxRulerShowOnLoad, SIGNAL(-state_changed(int)), this, SLOT(checkboxRulerShowOnLoad-state_changed(int)));

    labelRulerMetric = tk.Label(translate("Ruler Units"), groupboxRulerMisc);
    comboboxRulerMetric = tk.combobox(groupboxRulerMisc);
    comboboxRulerMetric.add_item("Imperial", 0);
    comboboxRulerMetric.add_item("Metric", 1);
    dialog-ruler-metric = ruler-metric;
    comboboxRulerMetric.set_current_index(comboboxRulerMetric.findData(dialog-ruler-metric));
    connect(comboboxRulerMetric, SIGNAL(current-index_changed(int)), this, SLOT(comboboxRulerMetricCurrent-index_changed(int)));

    gridLayoutRulerMisc = tk.GridLayout(widget);
    gridLayoutRulerMisc, add_widget(checkboxRulerShowOnLoad, 0, 0, "Align Left");
    gridLayoutRulerMisc, add_widget(labelRulerMetric, 1, 0, "Align Left");
    gridLayoutRulerMisc, add_widget(comboboxRulerMetric, 1, 1, "Align Right");
    groupboxRulerMisc.setLayout(gridLayoutRulerMisc);

    #Ruler Color
    groupboxRulerColor = tk.groupbox(translate("Ruler Color"), widget);

    labelRulerColor = tk.Label(translate("Ruler Color"), groupboxRulerColor);
    labelRulerColor.setObjectName("labelRulerColor");
    buttonRulerColor = PushButton(translate("Choose"), groupboxRulerColor);
    buttonRulerColor.setObjectName("buttonRulerColor");
    set_int("dialog-ruler-color", get_int("ruler-color"));
    set_int("preview.ruler-color", get_int("dialog-ruler-color"));
    accept.ruler-color = dialog-ruler-color;
    rulerPix = Image(16,16);
    rulerPix.fill(Color(preview.ruler-color));
    buttonRulerColor.setIcon(QIcon(rulerPix));
    connect(buttonRulerColor, SIGNAL(clicked()), this, SLOT(chooseRulerColor()));

    gridLayoutRulerColor = tk.GridLayout(widget);
    gridLayoutRulerColor, add_widget(labelRulerColor, 1, 0, "Align Left");
    gridLayoutRulerColor, add_widget(buttonRulerColor, 1, 1, "Align Right");
    groupboxRulerColor.setLayout(gridLayoutRulerColor);

    #Ruler Geometry
    groupboxRulerGeom = tk.groupbox(translate("Ruler Geometry"), widget);

    labelRulerPixelSize = tk.Label(translate("Ruler Pixel Size"), groupboxRulerGeom);
    labelRulerPixelSize.setObjectName("labelRulerPixelSize");
    spinboxRulerPixelSize = tk.Doublespinbox(groupboxRulerGeom);
    spinboxRulerPixelSize.setObjectName("spinboxRulerPixelSize");
    dialog-ruler-pixel-size = ruler-pixel-size;
    spinboxRulerPixelSize.setSingleStep(1.000);
    spinboxRulerPixelSize.setRange(20.000, 100.000);
    spinboxRulerPixelSize.setValue(dialog-ruler-pixel-size);
    connect(spinboxRulerPixelSize, SIGNAL(_value_changed(double)), this, SLOT(spinboxRulerPixelSize_value_changed(double)));

    gridLayoutRulerGeom = tk.GridLayout(groupboxRulerGeom);
    gridLayoutRulerGeom, add_widget(labelRulerPixelSize, 0, 0, "Align Left");
    gridLayoutRulerGeom, add_widget(spinboxRulerPixelSize, 0, 1, "Align Right");
    groupboxRulerGeom.setLayout(gridLayoutRulerGeom);

    /* Widget Layout */
    /*
    vbox_layout_main = tk.VBoxLayout(widget);
    vbox_layout_main, add_widget(groupboxGridMisc);
    vbox_layout_main, add_widget(groupboxGridColor);
    vbox_layout_main, add_widget(groupboxGridGeom);
    vbox_layout_main, add_widget(groupboxRulerMisc);
    vbox_layout_main, add_widget(groupboxRulerColor);
    vbox_layout_main, add_widget(groupboxRulerGeom);
    vbox_layout_main.addStretch(1);
    widget.setLayout(vbox_layout_main);

    scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

void
create_tab_ortho_polar(void)
{
    /* widget = tk.Widget(this); */

    /* TODO: finish this */

    /* scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

/* Originally a macro for constructing checkboxes.
 */
void
make_checkbox(char *label, int checked, char *icon, int f, int x, int y)
{
    /*
    c = tk.checkbox(translate(label), groupboxQSnapLoc);
    c.set_checked(checked);
    c.setIcon(load-icon(icon));
    connect(c, SIGNAL(-state_changed(int)), this, SLOT(f(int)));
    connect(this, SIGNAL(buttonQSnapSelectAll(int)), c, SLOT(set_checked(int)));
    connect(this, SIGNAL(buttonQSnapClearAll(int)), c, SLOT(set_checked(int)));
    gridLayoutQSnap, add_widget(c, x, y, "Align Left");
    dialog-checked = checked;
    */
}

void
create_tab_quick_snap(void)
{
    /*
    widget = tk.Widget(this);

    #QSnap Locators
    groupboxQSnapLoc = tk.groupbox(translate("Locators Used"), widget);
    buttonQSnapSelectAll = PushButton(translate("Select All"), groupboxQSnapLoc);
    buttonQSnapClearAll = PushButton(translate("Clear All"), groupboxQSnapLoc);
    gridLayoutQSnap = tk.GridLayout(groupboxQSnapLoc);

    connect(buttonQSnapSelectAll, SIGNAL(clicked()), this, SLOT(button_qsnap_select_all_clicked()));
    connect(buttonQSnapClearAll, SIGNAL(clicked()), this, SLOT(buttonQSnapClearAllClicked()));

    make-checkbox("Endpoint", tk.snap-endpoint, locator-snaptoendpoint-xpm, checkboxQSnapEndPoint-state-changed, 0, 0);
    make-checkbox("Midpoint", tk.snap-midpoint, locator-snaptomidpoint-xpm, checkboxQSnapMidPoint-state-changed, 1, 0);
    make-checkbox("Center", tk.snap-center, locator-snaptocenter-xpm, checkboxQSnapCenter-state-changed, 2, 0);
    make-checkbox("Node", tk.snap-node, locator-snaptonode-xpm, checkboxQSnapNode-state-changed, 3, 0);
    make-checkbox("Quadrant", tk.snap-quadrant, locator-snaptoquadrant-xpm, checkboxQSnapQuadrant-state-changed, 4, 0);
    make-checkbox("Intersection", tk.snap-intersection, locator-snaptointersection-xpm, checkboxQSnapIntersection-state-changed, 5, 0);
    make-checkbox("Extension", tk.snap-extension, locator-snaptoextension-xpm, checkboxQSnapExtension-state-changed, 6, 0);
    make-checkbox("Insertion", tk.snap-insertion, locator-snaptoinsert-xpm, checkboxQSnapInsertion-state-changed, 0, 1);
    make-checkbox("Perpendicular", tk.snap-perpendicular, locator-snaptoperpendicular-xpm, checkboxQSnapPerpendicular-state-changed, 1, 1);
    make-checkbox("Tangent", tk.snap-tangent, locator-snaptotangent-xpm, checkboxQSnapTangent-state-changed, 2, 1);
    make-checkbox("Nearest", tk.snap-nearest, locator-snaptonearest-xpm, checkboxQSnapNearest-state-changed, 3, 1);
    make-checkbox("Apparent Intersection", tk.snap-apparent, locator-snaptoapparentintersection-xpm, checkboxQSnapApparentIntersection-state-changed, 4, 1);
    make-checkbox("Parallel", tk.snap-parallel, locator-snaptoparallel-xpm, checkboxQSnapParallel-state-changed, 5, 1);

    gridLayoutQSnap, add_widget(buttonQSnapSelectAll, 0, 2, "Align Left");
    gridLayoutQSnap, add_widget(buttonQSnapClearAll, 1, 2, "Align Left");
    gridLayoutQSnap.setColumnStretch(2,1);
    groupboxQSnapLoc.setLayout(gridLayoutQSnap);

    #QSnap Visual Config
    groupboxQSnapVisual = tk.groupbox(translate("Visual Configuration"), widget);

    labelQSnapLocColor = tk.Label(translate("Locator Color"), groupboxQSnapVisual);
    comboboxQSnapLocColor = tk.combobox(groupboxQSnapVisual);
    addColorsTocombobox(comboboxQSnapLocColor);
    dialog-qsnap-locator-color = qsnap-locator-color;
    comboboxQSnapLocColor.set_current_index(comboboxQSnapLocColor.findData(dialog-qsnap-locator-color));
    connect(comboboxQSnapLocColor, SIGNAL(current-index_changed(int)), this, SLOT(comboboxQSnapLocatorColorCurrent-index_changed(int)));

    labelQSnapLocSize = tk.Label(translate("Locator Size"), groupboxQSnapVisual);
    sliderQSnapLocSize = tk.Slider(Qt-Horizontal, groupboxQSnapVisual);
    sliderQSnapLocSize.setRange(1,20);
    dialog-qsnap-locator-size = qsnap-locator-size;
    sliderQSnapLocSize.setValue(dialog-qsnap-locator-size);
    connect(sliderQSnapLocSize, SIGNAL(_value_changed(int)), this, SLOT(sliderQSnapLocatorSize_value_changed(int)));

    vboxLayoutQSnapVisual = tk.VBoxLayout(groupboxQSnapVisual);
    vboxLayoutQSnapVisual, add_widget(labelQSnapLocColor);
    vboxLayoutQSnapVisual, add_widget(comboboxQSnapLocColor);
    vboxLayoutQSnapVisual, add_widget(labelQSnapLocSize);
    vboxLayoutQSnapVisual, add_widget(sliderQSnapLocSize);
    groupboxQSnapVisual.setLayout(vboxLayoutQSnapVisual);

    #QSnap Sensitivity Config
    groupboxQSnapSensitivity = tk.groupbox(translate("Sensitivity"), widget);

    labelQSnapApertureSize = tk.Label(translate("Aperture Size"), groupboxQSnapSensitivity);
    sliderQSnapApertureSize = tk.Slider(Qt-Horizontal, groupboxQSnapSensitivity);
    sliderQSnapApertureSize.setRange(1,20);
    dialog-qsnap-aperture-size = qsnap-aperture-size;
    sliderQSnapApertureSize.setValue(dialog-qsnap-aperture-size);
    connect(sliderQSnapApertureSize, SIGNAL(_value_changed(int)), this, SLOT(sliderQSnapApertureSize_value_changed(int)));

    vboxLayoutQSnapSensitivity = tk.VBoxLayout(groupboxQSnapSensitivity);
    vboxLayoutQSnapSensitivity, add_widget(labelQSnapApertureSize);
    vboxLayoutQSnapSensitivity, add_widget(sliderQSnapApertureSize);
    groupboxQSnapSensitivity.setLayout(vboxLayoutQSnapSensitivity);
    */

    /* Widget Layout */
    /*
    vbox_layout_main = tk.VBoxLayout(widget);
    vbox_layout_main, add_widget(groupboxQSnapLoc);
    vbox_layout_main, add_widget(groupboxQSnapVisual);
    vbox_layout_main, add_widget(groupboxQSnapSensitivity);
    vbox_layout_main.addStretch(1);
    widget.setLayout(vbox_layout_main);

    scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

/* TODO: finish this */
void
create_tab_quick_track(void)
{
    /*
    widget = tk.Widget(this);

    scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

/* TODO: finish this */
void
create_tab_line_weight(void)
{
    /*
    widget = tk.Widget(this)

 Misc
    groupboxLwtMisc = tk.groupbox(translate("LineWeight Misc"), widget);

    s = (active-scene);

    checkboxShowLwt = tk.checkbox(translate("Show LineWeight"), groupboxLwtMisc);
    if s:
        dialog-lwt-show-lwt = s.property("ENABLE-LWT");
    else
        dialog-lwt-show-lwt = lwt-show-lwt;

    preview.lwt-show-lwt = dialog-lwt-show-lwt;
    checkboxShowLwt.set_checked(preview.lwt-show-lwt);
    connect(checkboxShowLwt, SIGNAL(-state_changed(int)), this, SLOT(checkboxLwtShowLwt-state_changed(int)));

    checkboxRealRender = tk.checkbox(translate("RealRender"), groupboxLwtMisc);
    checkboxRealRender.setObjectName("checkboxRealRender");
    if s:
        dialog-lwt-real-render = s.property("ENABLE-REAL");
    else {
        dialog-lwt-real-render = lwt-real-render;

    preview.lwt-real-render = dialog-lwt-real-render;
    checkboxRealRender.set_checked(preview.lwt-real-render);
    connect(checkboxRealRender, SIGNAL(-state_changed(int)), this, SLOT(checkboxLwtRealRender-state_changed(int)));
    checkboxRealRender.setEnabled(dialog-lwt-show-lwt);

    labelDefaultLwt = tk.Label(translate("Default weight"), groupboxLwtMisc);
    labelDefaultLwt.setEnabled(0) # TODO: remove later;
    comboboxDefaultLwt = tk.combobox(groupboxLwtMisc);
    dialog-lwt-default-lwt = lwt-default-lwt;
    # TODO: populate the combobox and set the initial value;
    comboboxDefaultLwt.add_item("".setNum(dialog-lwt-default-lwt, 'F', 2).append(" mm"), dialog-lwt-default-lwt);
    comboboxDefaultLwt.setEnabled(0); # TODO: remove later

    vboxLayoutLwtMisc = tk.VBoxLayout(groupboxLwtMisc);
    vboxLayoutLwtMisc, add_widget(checkboxShowLwt);
    vboxLayoutLwtMisc, add_widget(checkboxRealRender);
    vboxLayoutLwtMisc, add_widget(labelDefaultLwt);
    vboxLayoutLwtMisc, add_widget(comboboxDefaultLwt);
    groupboxLwtMisc.setLayout(vboxLayoutLwtMisc);

    #Widget Layout
    vbox_layout_main = tk.VBoxLayout(widget);
    vbox_layout_main, add_widget(groupboxLwtMisc);
    vbox_layout_main.addStretch(1);
    widget.setLayout(vbox_layout_main);

    scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

void
create_tab_selection(void)
{
    /* widget = tk.Widget(this); */

    /* Selection Modes */
    /*
    groupboxSelectionModes = tk.groupbox(translate("Modes"), widget);

    checkboxSelectionModePickFirst = tk.checkbox(translate("Allow Preselection (PickFirst)"), groupboxSelectionModes);
    dialog-selection-mode-pickfirst = selection-mode-pickfirst;
    checkboxSelectionModePickFirst.set_checked(dialog-selection-mode-pickfirst);
    checkboxSelectionModePickFirst.set_checked(1) checkboxSelectionModePickFirst.setEnabled(0); # TODO: Remove this line when Post-selection is available
    connect(checkboxSelectionModePickFirst, SIGNAL(-state_changed(int)), this, SLOT(checkboxSelectionModePickFirst-state_changed(int)));

    checkboxSelectionModePickAdd = tk.checkbox(translate("Add to Selection (PickAdd)"), groupboxSelectionModes);
    dialog-selection-mode-pickadd = selection-mode-pickadd;
    checkboxSelectionModePickAdd.set_checked(dialog-selection-mode-pickadd);
    connect(checkboxSelectionModePickAdd, SIGNAL(-state_changed(int)), this, SLOT(checkboxSelectionModePickAdd-state_changed(int)));

    checkboxSelectionModePickDrag = tk.checkbox(translate("Drag to Select (PickDrag)"), groupboxSelectionModes);
    dialog-selection-mode-pickdrag = selection-mode-pickdrag;
    checkboxSelectionModePickDrag.set_checked(dialog-selection-mode-pickdrag);
    checkboxSelectionModePickDrag.set_checked(0) checkboxSelectionModePickDrag.setEnabled(0) #TODO: Remove this line when this functionality is available
    connect(checkboxSelectionModePickDrag, SIGNAL(-state_changed(int)), this, SLOT(checkboxSelectionModePickDrag-state_changed(int)));

    vboxLayoutSelectionModes = tk.VBoxLayout(groupboxSelectionModes);
    vboxLayoutSelectionModes, add_widget(checkboxSelectionModePickFirst);
    vboxLayoutSelectionModes, add_widget(checkboxSelectionModePickAdd);
    vboxLayoutSelectionModes, add_widget(checkboxSelectionModePickDrag);
    groupboxSelectionModes.setLayout(vboxLayoutSelectionModes);

    #Selection Colors
    groupboxSelectionColors = tk.groupbox(translate("Colors"), widget);

    labelCoolGripColor = tk.Label(translate("Cool Grip (Unselected)"), groupboxSelectionColors);
    comboboxCoolGripColor = tk.combobox(groupboxSelectionColors);
    addColorsTocombobox(comboboxCoolGripColor);
    dialog-selection-coolgrip-color = selection-coolgrip-color;
    comboboxCoolGripColor.set_current_index(comboboxCoolGripColor.findData(dialog-selection-coolgrip-color));
    connect(comboboxCoolGripColor, SIGNAL(current-index_changed(int)), this, SLOT(comboboxSelectionCoolGripColorCurrent-index_changed(int)));

    labelHotGripColor = tk.Label(translate("Hot Grip (Selected)"), groupboxSelectionColors);
    comboboxHotGripColor = tk.combobox(groupboxSelectionColors);
    addColorsTocombobox(comboboxHotGripColor);
    dialog-selection-hotgrip-color = selection-hotgrip-color;
    comboboxHotGripColor.set_current_index(comboboxHotGripColor.findData(dialog-selection-hotgrip-color));
    connect(comboboxHotGripColor, SIGNAL(current-index_changed(int)), this, SLOT(comboboxSelectionHotGripColorCurrent-index_changed(int)));

    vboxLayoutSelectionColors = tk.VBoxLayout(groupboxSelectionColors);
    add_widget(vboxLayoutSelectionColors, labelCoolGripColor);
    add_widget(vboxLayoutSelectionColors, comboboxCoolGripColor);
    add_widget(vboxLayoutSelectionColors, labelHotGripColor);
    add_widget(vboxLayoutSelectionColors, comboboxHotGripColor);
    groupboxSelectionColors.layout = vboxLayoutSelectionColors;

    #Selection Sizes
    groupboxSelectionSizes = tk.groupbox(translate("Sizes"), widget);

    labelSelectionGripSize = tk.Label(translate("Grip Size"), groupboxSelectionSizes);
    sliderSelectionGripSize = tk.Slider(Qt-Horizontal, groupboxSelectionSizes);
    sliderSelectionGripSize.setRange(1,20);
    dialog-selection-grip-size = selection-grip-size;
    sliderSelectionGripSize.setValue(dialog-selection-grip-size);
    connect(sliderSelectionGripSize, SIGNAL(_value_changed(int)), this, SLOT(sliderSelectionGripSize_value_changed(int)));

    labelSelectionPickBoxSize = tk.Label(translate("Pickbox Size"), groupboxSelectionSizes);
    sliderSelectionPickBoxSize = tk.Slider(Qt-Horizontal, groupboxSelectionSizes);
    sliderSelectionPickBoxSize.setRange(1,20);
    dialog-selection-pickbox-size = selection-pickbox-size;
    sliderSelectionPickBoxSize.setValue(dialog-selection-pickbox-size);
    connect(sliderSelectionPickBoxSize, SIGNAL(_value_changed(int)), this, SLOT(sliderSelectionPickBoxSize_value_changed(int)));

    vboxLayoutSelectionSizes = tk.VBoxLayout(groupboxSelectionSizes);
    vboxLayoutSelectionSizes, add_widget(labelSelectionGripSize);
    vboxLayoutSelectionSizes, add_widget(sliderSelectionGripSize);
    vboxLayoutSelectionSizes, add_widget(labelSelectionPickBoxSize);
    vboxLayoutSelectionSizes, add_widget(sliderSelectionPickBoxSize);
    groupboxSelectionSizes.setLayout(vboxLayoutSelectionSizes);
    */

    /* Widget Layout */
    /*
    vbox_layout_main = tk.VBoxLayout(widget);
    vbox_layout_main, add_widget(groupboxSelectionModes);
    vbox_layout_main, add_widget(groupboxSelectionColors);
    vbox_layout_main, add_widget(groupboxSelectionSizes);
    vbox_layout_main.addStretch(1);
    widget.setLayout(vbox_layout_main);

    scroll_area = tk.scroll_area(this);
    scroll_area.setWidgetResizable(1);
    scroll_area.setWidget(widget);
    return scroll_area;
    */
}

