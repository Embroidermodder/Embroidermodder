#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
===================================
|module_summary| settings_dialog.py
===================================

TOWRITE

Classes summary:
================

============================ ============================
:class:`~Settings_Dialog`    TOWRITE
============================ ============================

---------------------------------------------------------
"""

#-Imports.---------------------------------------------------------------------
#--Python Imports.
import os

#--PySide/PyQt Imports.
if PYSIDE:
    ## from PySide import QtCore, QtGui
    # or... Improve performace with less dots...
    from PySide.QtCore import qDebug, Qt, SIGNAL, SLOT, QSize, QDir, Signal, Slot
    from PySide.QtGui import qApp, qRgb, QApplication, QDialogButtonBox, QIcon, QImage, QHBoxLayout, \
        QLabel, QPixmap, QPainter, QGroupBox, QComboBox, QScrollArea, QRadioButton, \
        QPushButton, QSpacerItem, QVBoxLayout, QWidget, QWizard, QWizardPage, \
        QGridLayout, QDialog, QTabWidget, QCheckBox, QColor, QColorDialog, \
        QSpinBox, QDoubleSpinBox, QFontComboBox, QFont, QFrame, QPrinterInfo, QSlider, QFileDialog
    try: # Qt4
        from PySide.QtGui import QStandardPaths
    except ImportError: # Qt5
        from PySide.QtGui import QDesktopServices
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    ## from PyQt4 import QtCore, QtGui
    # or... Improve performace with less dots...
    from PyQt4.QtCore import qDebug, Qt, SIGNAL, SLOT, QSize, QDir
    from PyQt4.QtCore import pyqtSignal as Signal
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtGui import qApp, qRgb, QApplication, QDialogButtonBox, QIcon, QImage, QHBoxLayout, \
        QLabel, QPixmap, QPainter, QGroupBox, QComboBox, QScrollArea, QRadioButton, \
        QPushButton, QSpacerItem, QVBoxLayout, QWidget, QWizard, QWizardPage, \
        QGridLayout, QDialog, QTabWidget, QCheckBox, QColor, QColorDialog, \
        QSpinBox, QDoubleSpinBox, QFontComboBox, QFont, QFrame, QPrinterInfo, QSlider, QFileDialog

#--Local Imports.
from object_data import ENABLE_LWT, ENABLE_REAL


gOsSep = os.sep

class Settings_Dialog(QDialog):
    """
    Subclass of `QDialog`_

    Settings dialog for Embroidermodder.

    .. sphinx_generate_methods_summary::
       Settings_Dialog
    """
    def __init__(self, mw, showTab="General", parent=None):
        """
        Default class constructor.

        :param `mw`: The application's MainWindow instance.
        :type `mw`: `QMainWindow`_
        :param `showTab`: The tab to open the dialog up on when opening.
         Can be one of these strings:
         "General", "Files/Paths", "Display", "Prompt", "Open/Save",
         "Printing", "Snap", "Grid/Ruler", "Ortho/Polar", "QuickSnap",
         "QuickTrack", "LineWeight", "Selection"
        :type `showTab`: QString
        :param `parent`: Pointer to a parent widget instance.
        :type `parent`: `QWidget`_
        """
        super(Settings_Dialog, self).__init__(parent)

        self.mainWin = mw
        self.gIconDir = mw.gIconDir
        self.setMinimumSize(750, 550)

        tabWidget = QTabWidget(self)

        #hhhhhhhhhhhhhh
        self.preview_general_mdi_bg_use_logo =       bool()   # bool
        self.preview_general_mdi_bg_use_texture =    bool()   # bool
        self.preview_general_mdi_bg_use_color =      bool()   # bool

        self.accept_general_mdi_bg_logo =            str()    # QString
        self.accept_general_mdi_bg_texture =         str()    # QString
        self.preview_general_mdi_bg_color =          int()    # QRgb
        self.accept_general_mdi_bg_color =           int()    # QRgb

        self.preview_display_show_scrollbars =       bool()   # bool

        self.preview_display_crosshair_color =       int()    # QRgb
        self.accept_display_crosshair_color =        int()    # QRgb
        self.preview_display_bg_color =              int()    # QRgb
        self.accept_display_bg_color =               int()    # QRgb

        self.preview_display_selectbox_left_color =  int()    # QRgb
        self.accept_display_selectbox_left_color =   int()    # QRgb
        self.preview_display_selectbox_left_fill =   int()    # QRgb
        self.accept_display_selectbox_left_fill =    int()    # QRgb
        self.preview_display_selectbox_right_color = int()    # QRgb
        self.accept_display_selectbox_right_color =  int()    # QRgb
        self.preview_display_selectbox_right_fill =  int()    # QRgb
        self.accept_display_selectbox_right_fill =   int()    # QRgb
        self.preview_display_selectbox_alpha =       int()    # quint8

        self.preview_prompt_text_color =             int()    # QRgb
        self.accept_prompt_text_color =              int()    # QRgb

        self.preview_prompt_bg_color =               int()    # QRgb
        self.accept_prompt_bg_color =                int()    # QRgb

        self.preview_prompt_font_family =            str()    # QString
        self.preview_prompt_font_style =             str()    # QString
        self.preview_prompt_font_size =              int()    # quint8

        self.preview_grid_color =                    int()    # QRgb
        self.accept_grid_color =                     int()    # QRgb

        self.preview_ruler_color =                   int()    # QRgb
        self.accept_ruler_color =                    int()    # QRgb

        self.preview_lwt_show_lwt =                  bool()   # bool
        self.preview_lwt_real_render =               bool()   # bool

        self.dialog_general_language =               str()    # QString
        self.dialog_general_icon_theme =             str()    # QString
        self.dialog_general_icon_size =              int()    # int
        self.dialog_general_mdi_bg_use_logo =        bool()   # bool
        self.dialog_general_mdi_bg_use_texture =     bool()   # bool
        self.dialog_general_mdi_bg_use_color =       bool()   # bool
        self.dialog_general_mdi_bg_logo =            str()    # QString
        self.dialog_general_mdi_bg_texture =         str()    # QString
        self.dialog_general_mdi_bg_color =           int()    # QRgb
        self.dialog_general_tip_of_the_day =         bool()   # bool
        self.dialog_general_system_help_browser =    bool()   # bool
        self.dialog_display_use_opengl =             bool()   # bool
        self.dialog_display_renderhint_aa =          bool()   # bool
        self.dialog_display_renderhint_text_aa =     bool()   # bool
        self.dialog_display_renderhint_smooth_pix =  bool()   # bool
        self.dialog_display_renderhint_high_aa =     bool()   # bool
        self.dialog_display_renderhint_noncosmetic = bool()   # bool
        self.dialog_display_show_scrollbars =        bool()   # bool
        self.dialog_display_scrollbar_widget_num =   int()    # int
        self.dialog_display_crosshair_color =        int()    # QRgb
        self.dialog_display_bg_color =               int()    # QRgb
        self.dialog_display_selectbox_left_color =   int()    # QRgb
        self.dialog_display_selectbox_left_fill =    int()    # QRgb
        self.dialog_display_selectbox_right_color =  int()    # QRgb
        self.dialog_display_selectbox_right_fill =   int()    # QRgb
        self.dialog_display_selectbox_alpha =        int()    # quint8
        self.dialog_display_zoomscale_in =           float()  # qreal
        self.dialog_display_zoomscale_out =          float()  # qreal
        self.dialog_display_crosshair_percent =      int()    # quint8
        self.dialog_display_units =                  str()    # QString
        self.dialog_prompt_text_color =              int()    # QRgb
        self.dialog_prompt_bg_color =                int()    # QRgb
        self.dialog_prompt_font_family =             str()    # QString
        self.dialog_prompt_font_style =              str()    # QString
        self.dialog_prompt_font_size =               int()    # quint8
        self.dialog_prompt_save_history =            bool()   # bool
        self.dialog_prompt_save_history_as_html =    bool()   # bool
        self.dialog_prompt_save_history_filename =   str()    # QString
        self.dialog_opensave_custom_filter =         str()    # QString
        self.dialog_opensave_open_format =           str()    # QString
        self.dialog_opensave_open_thumbnail =        bool()   # bool
        self.dialog_opensave_save_format =           str()    # QString
        self.dialog_opensave_save_thumbnail =        bool()   # bool
        self.dialog_opensave_recent_max_files =      int()    # quint8
        self.dialog_opensave_trim_dst_num_jumps =    int()    # quint8
        self.dialog_printing_default_device =        str()    # QString
        self.dialog_printing_use_last_device =       bool()   # bool
        self.dialog_printing_disable_bg =            bool()   # bool
        self.dialog_grid_show_on_load =              bool()   # bool
        self.dialog_grid_show_origin =               bool()   # bool
        self.dialog_grid_color_match_crosshair =     bool()   # bool
        self.dialog_grid_color =                     int()    # QRgb
        self.dialog_grid_load_from_file =            bool()   # bool
        self.dialog_grid_type =                      str()    # QString
        self.dialog_grid_center_on_origin =          bool()   # bool
        self.dialog_grid_center_x =                  float()  # qreal
        self.dialog_grid_center_y =                  float()  # qreal
        self.dialog_grid_size_x =                    float()  # qreal
        self.dialog_grid_size_y =                    float()  # qreal
        self.dialog_grid_spacing_x =                 float()  # qreal
        self.dialog_grid_spacing_y =                 float()  # qreal
        self.dialog_grid_size_radius =               float()  # qreal
        self.dialog_grid_spacing_radius =            float()  # qreal
        self.dialog_grid_spacing_angle =             float()  # qreal
        self.dialog_ruler_show_on_load =             bool()   # bool
        self.dialog_ruler_metric =                   bool()   # bool
        self.dialog_ruler_color =                    int()    # QRgb
        self.dialog_ruler_pixel_size =               int()    # quint8
        self.dialog_qsnap_enabled =                  bool()   # bool
        self.dialog_qsnap_locator_color =            int()    # QRgb
        self.dialog_qsnap_locator_size =             int()    # quint8
        self.dialog_qsnap_aperture_size =            int()    # quint8
        self.dialog_qsnap_endpoint =                 bool()   # bool
        self.dialog_qsnap_midpoint =                 bool()   # bool
        self.dialog_qsnap_center =                   bool()   # bool
        self.dialog_qsnap_node =                     bool()   # bool
        self.dialog_qsnap_quadrant =                 bool()   # bool
        self.dialog_qsnap_intersection =             bool()   # bool
        self.dialog_qsnap_extension =                bool()   # bool
        self.dialog_qsnap_insertion =                bool()   # bool
        self.dialog_qsnap_perpendicular =            bool()   # bool
        self.dialog_qsnap_tangent =                  bool()   # bool
        self.dialog_qsnap_nearest =                  bool()   # bool
        self.dialog_qsnap_apparent =                 bool()   # bool
        self.dialog_qsnap_parallel =                 bool()   # bool
        self.dialog_lwt_show_lwt =                   bool()   # bool
        self.dialog_lwt_real_render =                bool()   # bool
        self.dialog_lwt_default_lwt =                float()  # qreal
        self.dialog_selection_mode_pickfirst =       bool()   # bool
        self.dialog_selection_mode_pickadd =         bool()   # bool
        self.dialog_selection_mode_pickdrag =        bool()   # bool
        self.dialog_selection_coolgrip_color =       int()    # QRgb
        self.dialog_selection_hotgrip_color =        int()    # QRgb
        self.dialog_selection_grip_size =            int()    # quint8
        self.dialog_selection_pickbox_size =         int()    # quint8

        # TODO: Add icons to tabs
        tabWidget.addTab(self.createTabGeneral(),    self.tr("General"))
        tabWidget.addTab(self.createTabFilesPaths(), self.tr("Files/Paths"))
        tabWidget.addTab(self.createTabDisplay(),    self.tr("Display"))
        tabWidget.addTab(self.createTabPrompt(),     self.tr("Prompt"))
        tabWidget.addTab(self.createTabOpenSave(),   self.tr("Open/Save"))
        tabWidget.addTab(self.createTabPrinting(),   self.tr("Printing"))
        tabWidget.addTab(self.createTabSnap(),       self.tr("Snap"))
        tabWidget.addTab(self.createTabGridRuler(),  self.tr("Grid/Ruler"))
        tabWidget.addTab(self.createTabOrthoPolar(), self.tr("Ortho/Polar"))
        tabWidget.addTab(self.createTabQuickSnap(),  self.tr("QuickSnap"))
        tabWidget.addTab(self.createTabQuickTrack(), self.tr("QuickTrack"))
        tabWidget.addTab(self.createTabLineWeight(), self.tr("LineWeight"))
        tabWidget.addTab(self.createTabSelection(),  self.tr("Selection"))

        tabIndex = {
            "General":     0,
            "Files/Path":  1,
            "Display":     2,
            "Prompt":      3,
            "Open/Save":   4,
            "Printing":    5,
            "Snap":        6,
            "Grid/Ruler":  7,
            "Ortho/Polar": 8,
            "QuickSnap":   9,
            "QuickTrack": 10,
            "LineWeight": 11,
            "Selection":  12}
        tabWidget.setCurrentIndex(tabIndex[showTab])

        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)

        buttonBox.accepted.connect(self.acceptChanges)
        buttonBox.rejected.connect(self.rejectChanges)

        vboxLayoutMain = QVBoxLayout(self)
        vboxLayoutMain.addWidget(tabWidget)
        vboxLayoutMain.addWidget(buttonBox)
        self.setLayout(vboxLayoutMain)

        self.setWindowTitle(self.tr("Settings"))

        # QApplication.setOverrideCursor(Qt.ArrowCursor)

    # def __del__(self):
        # QApplication.restoreOverrideCursor()

    def createTabGeneral(self):
        """TOWRITE"""
        widget = QWidget(self)

        # Language
        groupBoxLanguage = QGroupBox(self.tr("Language"), widget)

        labelLanguage = QLabel(self.tr("Language (Requires Restart)"), groupBoxLanguage)
        comboBoxLanguage = QComboBox(groupBoxLanguage)
        self.dialog_general_language = self.mainWin.getSettingsGeneralLanguage().lower()
        comboBoxLanguage.addItem("Default")
        comboBoxLanguage.addItem("System")
        comboBoxLanguage.insertSeparator(2)
        trDir = self.mainWin.gAppDir + gOsSep + "translations"
        for dirName in os.listdir(trDir):
            if os.path.isdir(trDir + gOsSep + dirName):
                dirName = dirName.upper()
                comboBoxLanguage.addItem(dirName)

        current = self.dialog_general_language
        if current == 'default':
            comboBoxLanguage.setCurrentIndex(0)
        elif current == 'system':
            comboBoxLanguage.setCurrentIndex(1)
        else:
            current = current.upper()
            comboBoxLanguage.setCurrentIndex(comboBoxLanguage.findText(current))
        comboBoxLanguage.currentIndexChanged[str].connect(self.comboBoxLanguageCurrentIndexChanged)

        vboxLayoutLanguage = QVBoxLayout(groupBoxLanguage)
        vboxLayoutLanguage.addWidget(labelLanguage)
        vboxLayoutLanguage.addWidget(comboBoxLanguage)
        groupBoxLanguage.setLayout(vboxLayoutLanguage)

        # Icons
        groupBoxIcon = QGroupBox(self.tr("Icons"), widget)

        labelIconTheme = QLabel(self.tr("Icon Theme"), groupBoxIcon)
        comboBoxIconTheme = QComboBox(groupBoxIcon)
        iconDir = self.mainWin.gAppDir + gOsSep + 'icons'
        self.dialog_general_icon_theme = self.mainWin.getSettingsGeneralIconTheme()
        for dirName in os.listdir(iconDir):
            comboBoxIconTheme.addItem(QIcon(iconDir + gOsSep + dirName + gOsSep + "theme.png"), dirName)

        comboBoxIconTheme.setCurrentIndex(comboBoxIconTheme.findText(self.dialog_general_icon_theme))
        comboBoxIconTheme.currentIndexChanged[str].connect(self.comboBoxIconThemeCurrentIndexChanged)

        labelIconSize = QLabel(self.tr("Icon Size"), groupBoxIcon)
        comboBoxIconSize = QComboBox(groupBoxIcon)
        iDir = iconDir + gOsSep + self.dialog_general_icon_theme + gOsSep
        comboBoxIconSize.addItem(QIcon(iDir + "icon16.png"),  "Very Small", 16)
        comboBoxIconSize.addItem(QIcon(iDir + "icon24.png"),  "Small",      24)
        comboBoxIconSize.addItem(QIcon(iDir + "icon32.png"),  "Medium",     32)
        comboBoxIconSize.addItem(QIcon(iDir + "icon48.png"),  "Large",      48)
        comboBoxIconSize.addItem(QIcon(iDir + "icon64.png"),  "Very Large", 64)
        comboBoxIconSize.addItem(QIcon(iDir + "icon128.png"), "I'm Blind", 128)
        self.dialog_general_icon_size = self.mainWin.getSettingsGeneralIconSize()
        comboBoxIconSize.setCurrentIndex(comboBoxIconSize.findData(self.dialog_general_icon_size))
        comboBoxIconSize.currentIndexChanged[int].connect(self.comboBoxIconSizeCurrentIndexChanged)

        vboxLayoutIcon = QVBoxLayout(groupBoxIcon)
        vboxLayoutIcon.addWidget(labelIconTheme)
        vboxLayoutIcon.addWidget(comboBoxIconTheme)
        vboxLayoutIcon.addWidget(labelIconSize)
        vboxLayoutIcon.addWidget(comboBoxIconSize)
        groupBoxIcon.setLayout(vboxLayoutIcon)

        # Mdi Background
        groupBoxMdiBG = QGroupBox(self.tr("Background"), widget)

        checkBoxMdiBGUseLogo = QCheckBox(self.tr("Use Logo"), groupBoxMdiBG)
        self.dialog_general_mdi_bg_use_logo = self.mainWin.getSettingsGeneralMdiBGUseLogo()
        self.preview_general_mdi_bg_use_logo = self.dialog_general_mdi_bg_use_logo
        checkBoxMdiBGUseLogo.setChecked(self.preview_general_mdi_bg_use_logo)
        checkBoxMdiBGUseLogo.stateChanged[int].connect(self.checkBoxGeneralMdiBGUseLogoStateChanged)

        buttonMdiBGLogo = QPushButton(self.tr("Choose"), groupBoxMdiBG)
        buttonMdiBGLogo.setEnabled(self.dialog_general_mdi_bg_use_logo)
        self.dialog_general_mdi_bg_logo = self.mainWin.getSettingsGeneralMdiBGLogo()
        self.accept_general_mdi_bg_logo = self.dialog_general_mdi_bg_logo
        buttonMdiBGLogo.clicked.connect(self.chooseGeneralMdiBackgroundLogo)
        checkBoxMdiBGUseLogo.toggled[bool].connect(buttonMdiBGLogo.setEnabled)

        checkBoxMdiBGUseTexture = QCheckBox(self.tr("Use Texture"), groupBoxMdiBG)
        self.dialog_general_mdi_bg_use_texture = self.mainWin.getSettingsGeneralMdiBGUseTexture()
        self.preview_general_mdi_bg_use_texture = self.dialog_general_mdi_bg_use_texture
        checkBoxMdiBGUseTexture.setChecked(self.preview_general_mdi_bg_use_texture)
        checkBoxMdiBGUseTexture.stateChanged[int].connect(self.checkBoxGeneralMdiBGUseTextureStateChanged)

        buttonMdiBGTexture = QPushButton(self.tr("Choose"), groupBoxMdiBG)
        buttonMdiBGTexture.setEnabled(self.dialog_general_mdi_bg_use_texture)
        self.dialog_general_mdi_bg_texture = self.mainWin.getSettingsGeneralMdiBGTexture()
        self.accept_general_mdi_bg_texture = self.dialog_general_mdi_bg_texture
        buttonMdiBGTexture.clicked.connect(self.chooseGeneralMdiBackgroundTexture)
        checkBoxMdiBGUseTexture.toggled[bool].connect(buttonMdiBGTexture.setEnabled)

        checkBoxMdiBGUseColor = QCheckBox(self.tr("Use Color"), groupBoxMdiBG)
        self.dialog_general_mdi_bg_use_color = self.mainWin.getSettingsGeneralMdiBGUseColor()
        self.preview_general_mdi_bg_use_color = self.dialog_general_mdi_bg_use_color
        checkBoxMdiBGUseColor.setChecked(self.preview_general_mdi_bg_use_color)
        self.connect(checkBoxMdiBGUseColor, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxGeneralMdiBGUseColorStateChanged(int)"))

        buttonMdiBGColor = QPushButton(self.tr("Choose"), groupBoxMdiBG)
        buttonMdiBGColor.setEnabled(self.dialog_general_mdi_bg_use_color)
        self.dialog_general_mdi_bg_color  = self.mainWin.getSettingsGeneralMdiBGColor()
        self.preview_general_mdi_bg_color = self.dialog_general_mdi_bg_color
        self.accept_general_mdi_bg_color  = self.dialog_general_mdi_bg_color
        mdiBGPix = QPixmap(16, 16)
        mdiBGPix.fill(QColor(self.preview_general_mdi_bg_color))
        buttonMdiBGColor.setIcon(QIcon(mdiBGPix))
        self.connect(buttonMdiBGColor, SIGNAL("clicked()"), self, SLOT("chooseGeneralMdiBackgroundColor()"))
        self.connect(checkBoxMdiBGUseColor, SIGNAL("toggled(bool)"), buttonMdiBGColor, SLOT("setEnabled(bool)"))

        gridLayoutMdiBG = QGridLayout(widget)
        gridLayoutMdiBG.addWidget(checkBoxMdiBGUseLogo,    0, 0, Qt.AlignLeft)
        gridLayoutMdiBG.addWidget(buttonMdiBGLogo,         0, 1, Qt.AlignRight)
        gridLayoutMdiBG.addWidget(checkBoxMdiBGUseTexture, 1, 0, Qt.AlignLeft)
        gridLayoutMdiBG.addWidget(buttonMdiBGTexture,      1, 1, Qt.AlignRight)
        gridLayoutMdiBG.addWidget(checkBoxMdiBGUseColor,   2, 0, Qt.AlignLeft)
        gridLayoutMdiBG.addWidget(buttonMdiBGColor,        2, 1, Qt.AlignRight)
        groupBoxMdiBG.setLayout(gridLayoutMdiBG)

        # Tips
        groupBoxTips = QGroupBox(self.tr("Tips"), widget)

        checkBoxTipOfTheDay = QCheckBox(self.tr("Show Tip of the Day on startup"), groupBoxTips)
        self.dialog_general_tip_of_the_day = self.mainWin.getSettingsGeneralTipOfTheDay()
        checkBoxTipOfTheDay.setChecked(self.dialog_general_tip_of_the_day)
        checkBoxTipOfTheDay.stateChanged.connect(self.checkBoxTipOfTheDayStateChanged)

        vboxLayoutTips = QVBoxLayout(groupBoxTips)
        vboxLayoutTips.addWidget(checkBoxTipOfTheDay)
        groupBoxTips.setLayout(vboxLayoutTips)

        # Help Browser
        groupBoxHelpBrowser = QGroupBox(self.tr("Help Browser"), widget)

        radioButtonSystemHelpBrowser = QRadioButton(self.tr("System"), groupBoxHelpBrowser)
        radioButtonSystemHelpBrowser.setChecked(self.mainWin.getSettingsGeneralSystemHelpBrowser())
        radioButtonCustomHelpBrowser = QRadioButton(self.tr("Custom"), groupBoxHelpBrowser)
        radioButtonCustomHelpBrowser.setChecked(not self.mainWin.getSettingsGeneralSystemHelpBrowser())
        radioButtonCustomHelpBrowser.setEnabled(False) # TODO: finish self

        vboxLayoutHelpBrowser = QVBoxLayout(groupBoxHelpBrowser)
        vboxLayoutHelpBrowser.addWidget(radioButtonSystemHelpBrowser)
        vboxLayoutHelpBrowser.addWidget(radioButtonCustomHelpBrowser)
        groupBoxHelpBrowser.setLayout(vboxLayoutHelpBrowser)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxLanguage)
        vboxLayoutMain.addWidget(groupBoxIcon)
        vboxLayoutMain.addWidget(groupBoxMdiBG)
        vboxLayoutMain.addWidget(groupBoxTips)
        vboxLayoutMain.addWidget(groupBoxHelpBrowser)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabFilesPaths(self):
        """TOWRITE"""
        widget = QWidget(self)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabDisplay(self):
        """TOWRITE"""
        widget = QWidget(self)

        # Rendering
        # TODO: Review OpenGL and Rendering settings for future inclusion
        #TODO#COMMENTEDOUT# /*
        #TODO#COMMENTEDOUT# groupBoxRender = QGroupBox(self.tr("Rendering"), widget)
        #TODO#COMMENTEDOUT#
        #TODO#COMMENTEDOUT# checkBoxUseOpenGL = QCheckBox(self.tr("Use OpenGL"), groupBoxRender)
        #TODO#COMMENTEDOUT# self.dialog_display_use_opengl = self.mainWin.getSettingsDisplayUseOpenGL()
        #TODO#COMMENTEDOUT# checkBoxUseOpenGL.setChecked(self.dialog_display_use_opengl)
        #TODO#COMMENTEDOUT# connect(checkBoxUseOpenGL, SIGNAL(stateChanged(int)), self, SLOT(checkBoxUseOpenGLStateChanged(int)))
        #TODO#COMMENTEDOUT#
        #TODO#COMMENTEDOUT# checkBoxRenderHintAA = QCheckBox(self.tr("Antialias"), groupBoxRender)
        #TODO#COMMENTEDOUT# self.dialog_display_renderhint_aa = self.mainWin.getSettingsDisplayRenderHintAA()
        #TODO#COMMENTEDOUT# checkBoxRenderHintAA.setChecked(self.dialog_display_renderhint_aa)
        #TODO#COMMENTEDOUT# connect(checkBoxRenderHintAA, SIGNAL(stateChanged(int)), self, SLOT(checkBoxRenderHintAAStateChanged(int)))
        #TODO#COMMENTEDOUT#
        #TODO#COMMENTEDOUT# checkBoxRenderHintTextAA = QCheckBox(self.tr("Antialias Text"), groupBoxRender)
        #TODO#COMMENTEDOUT# self.dialog_display_renderhint_text_aa = self.mainWin.getSettingsDisplayRenderHintTextAA()
        #TODO#COMMENTEDOUT# checkBoxRenderHintTextAA.setChecked(self.dialog_display_renderhint_text_aa)
        #TODO#COMMENTEDOUT# connect(checkBoxRenderHintTextAA, SIGNAL(stateChanged(int)), self, SLOT(checkBoxRenderHintTextAAStateChanged(int)))
        #TODO#COMMENTEDOUT#
        #TODO#COMMENTEDOUT# checkBoxRenderHintSmoothPix = QCheckBox(self.tr("Smooth Pixmap"), groupBoxRender)
        #TODO#COMMENTEDOUT# self.dialog_display_renderhint_smooth_pix = self.mainWin.getSettingsDisplayRenderHintSmoothPix()
        #TODO#COMMENTEDOUT# checkBoxRenderHintSmoothPix.setChecked(self.dialog_display_renderhint_smooth_pix)
        #TODO#COMMENTEDOUT# connect(checkBoxRenderHintSmoothPix, SIGNAL(stateChanged(int)), self, SLOT(checkBoxRenderHintSmoothPixStateChanged(int)))
        #TODO#COMMENTEDOUT#
        #TODO#COMMENTEDOUT# checkBoxRenderHintHighAA = QCheckBox(self.tr("High Quality Antialiasing (OpenGL)"), groupBoxRender)
        #TODO#COMMENTEDOUT# self.dialog_display_renderhint_high_aa = self.mainWin.getSettingsDisplayRenderHintHighAA()
        #TODO#COMMENTEDOUT# checkBoxRenderHintHighAA.setChecked(self.dialog_display_renderhint_high_aa)
        #TODO#COMMENTEDOUT# connect(checkBoxRenderHintHighAA, SIGNAL(stateChanged(int)), self, SLOT(checkBoxRenderHintHighAAStateChanged(int)))
        #TODO#COMMENTEDOUT#
        #TODO#COMMENTEDOUT# checkBoxRenderHintNonCosmetic = QCheckBox(self.tr("Non Cosmetic"), groupBoxRender)
        #TODO#COMMENTEDOUT# self.dialog_display_renderhint_noncosmetic = self.mainWin.getSettingsDisplayRenderHintNonCosmetic()
        #TODO#COMMENTEDOUT# checkBoxRenderHintNonCosmetic.setChecked(self.dialog_display_renderhint_noncosmetic)
        #TODO#COMMENTEDOUT# connect(checkBoxRenderHintNonCosmetic, SIGNAL(stateChanged(int)), self, SLOT(checkBoxRenderHintNonCosmeticStateChanged(int)))
        #TODO#COMMENTEDOUT#
        #TODO#COMMENTEDOUT# vboxLayoutRender = QVBoxLayout(groupBoxRender)
        #TODO#COMMENTEDOUT# vboxLayoutRender.addWidget(checkBoxUseOpenGL)
        #TODO#COMMENTEDOUT# vboxLayoutRender.addWidget(checkBoxRenderHintAA)
        #TODO#COMMENTEDOUT# vboxLayoutRender.addWidget(checkBoxRenderHintTextAA)
        #TODO#COMMENTEDOUT# vboxLayoutRender.addWidget(checkBoxRenderHintSmoothPix)
        #TODO#COMMENTEDOUT# vboxLayoutRender.addWidget(checkBoxRenderHintHighAA)
        #TODO#COMMENTEDOUT# vboxLayoutRender.addWidget(checkBoxRenderHintNonCosmetic)
        #TODO#COMMENTEDOUT# groupBoxRender.setLayout(vboxLayoutRender)
        #TODO#COMMENTEDOUT# */

        # ScrollBars
        groupBoxScrollBars = QGroupBox(self.tr("ScrollBars"), widget)

        checkBoxShowScrollBars = QCheckBox(self.tr("Show ScrollBars"), groupBoxScrollBars)
        self.dialog_display_show_scrollbars = self.mainWin.getSettingsDisplayShowScrollBars()
        self.preview_display_show_scrollbars = self.dialog_display_show_scrollbars
        checkBoxShowScrollBars.setChecked(self.preview_display_show_scrollbars)
        self.connect(checkBoxShowScrollBars, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxShowScrollBarsStateChanged(int)"))

        labelScrollBarWidget = QLabel(self.tr("Perform action when clicking corner widget"), groupBoxScrollBars)
        comboBoxScrollBarWidget = QComboBox(groupBoxScrollBars)
        self.dialog_display_scrollbar_widget_num = self.mainWin.getSettingsDisplayScrollBarWidgetNum()
        #TODO# int numActions = self.mainWin.actionHash.size()
        #TODO# for(int i = 0 i < numActions i++)
        #TODO#     QAction* action = self.mainWin.actionHash.value(i)
        #TODO#     if action:
        #TODO#         comboBoxScrollBarWidget.addItem(action.icon(), action.text().replace("&", ""))

        comboBoxScrollBarWidget.setCurrentIndex(self.dialog_display_scrollbar_widget_num)
        self.connect(comboBoxScrollBarWidget, SIGNAL("currentIndexChanged(int)"), self, SLOT("comboBoxScrollBarWidgetCurrentIndexChanged(int)"))

        vboxLayoutScrollBars = QVBoxLayout(groupBoxScrollBars)
        vboxLayoutScrollBars.addWidget(checkBoxShowScrollBars)
        vboxLayoutScrollBars.addWidget(labelScrollBarWidget)
        vboxLayoutScrollBars.addWidget(comboBoxScrollBarWidget)
        groupBoxScrollBars.setLayout(vboxLayoutScrollBars)

        # Colors
        groupBoxColor = QGroupBox(self.tr("Colors"), widget)

        labelCrossHairColor = QLabel(self.tr("Crosshair Color"), groupBoxColor)
        buttonCrossHairColor = QPushButton(self.tr("Choose"), groupBoxColor)
        self.dialog_display_crosshair_color  = self.mainWin.getSettingsDisplayCrossHairColor()
        preview_display_crosshair_color = self.dialog_display_crosshair_color
        self.accept_display_crosshair_color  = self.dialog_display_crosshair_color
        crosshairPix = QPixmap(16, 16)
        crosshairPix.fill(QColor(preview_display_crosshair_color))
        buttonCrossHairColor.setIcon(QIcon(crosshairPix))
        self.connect(buttonCrossHairColor, SIGNAL("clicked()"), self, SLOT("chooseDisplayCrossHairColor()"))

        labelBGColor = QLabel(self.tr("Background Color"), groupBoxColor)
        buttonBGColor = QPushButton(self.tr("Choose"), groupBoxColor)
        self.dialog_display_bg_color  = self.mainWin.getSettingsDisplayBGColor()
        preview_display_bg_color = self.dialog_display_bg_color
        self.accept_display_bg_color  = self.dialog_display_bg_color
        bgPix = QPixmap(16, 16)
        bgPix.fill(QColor(preview_display_bg_color))
        buttonBGColor.setIcon(QIcon(bgPix))
        self.connect(buttonBGColor, SIGNAL("clicked()"), self, SLOT("chooseDisplayBackgroundColor()"))

        labelSelectBoxLeftColor = QLabel(self.tr("Selection Box Color (Crossing)"), groupBoxColor)
        buttonSelectBoxLeftColor = QPushButton(self.tr("Choose"), groupBoxColor)
        self.dialog_display_selectbox_left_color  = self.mainWin.getSettingsDisplaySelectBoxLeftColor()
        preview_display_selectbox_left_color = self.dialog_display_selectbox_left_color
        self.accept_display_selectbox_left_color  = self.dialog_display_selectbox_left_color
        sBoxLCPix = QPixmap(16, 16)
        sBoxLCPix.fill(QColor(preview_display_selectbox_left_color))
        buttonSelectBoxLeftColor.setIcon(QIcon(sBoxLCPix))
        self.connect(buttonSelectBoxLeftColor, SIGNAL("clicked()"), self, SLOT("chooseDisplaySelectBoxLeftColor()"))

        labelSelectBoxLeftFill = QLabel(self.tr("Selection Box Fill (Crossing)"), groupBoxColor)
        buttonSelectBoxLeftFill = QPushButton(self.tr("Choose"), groupBoxColor)
        self.dialog_display_selectbox_left_fill  = self.mainWin.getSettingsDisplaySelectBoxLeftFill()
        preview_display_selectbox_left_fill = self.dialog_display_selectbox_left_fill
        self.accept_display_selectbox_left_fill  = self.dialog_display_selectbox_left_fill
        sBoxLFPix = QPixmap(16, 16)
        sBoxLFPix.fill(QColor(preview_display_selectbox_left_fill))
        buttonSelectBoxLeftFill.setIcon(QIcon(sBoxLFPix))
        self.connect(buttonSelectBoxLeftFill, SIGNAL("clicked()"), self, SLOT("chooseDisplaySelectBoxLeftFill()"))

        labelSelectBoxRightColor = QLabel(self.tr("Selection Box Color (Window)"), groupBoxColor)
        buttonSelectBoxRightColor = QPushButton(self.tr("Choose"), groupBoxColor)
        self.dialog_display_selectbox_right_color  = self.mainWin.getSettingsDisplaySelectBoxRightColor()
        preview_display_selectbox_right_color = self.dialog_display_selectbox_right_color
        self.accept_display_selectbox_right_color  = self.dialog_display_selectbox_right_color
        sBoxRCPix = QPixmap(16,16)
        sBoxRCPix.fill(QColor(preview_display_selectbox_right_color))
        buttonSelectBoxRightColor.setIcon(QIcon(sBoxRCPix))
        self.connect(buttonSelectBoxRightColor, SIGNAL("clicked()"), self, SLOT("chooseDisplaySelectBoxRightColor()"))

        labelSelectBoxRightFill = QLabel(self.tr("Selection Box Fill (Window)"), groupBoxColor)
        buttonSelectBoxRightFill = QPushButton(self.tr("Choose"), groupBoxColor)
        self.dialog_display_selectbox_right_fill  = self.mainWin.getSettingsDisplaySelectBoxRightFill()
        preview_display_selectbox_right_fill = self.dialog_display_selectbox_right_fill
        self.accept_display_selectbox_right_fill  = self.dialog_display_selectbox_right_fill
        sBoxRFPix = QPixmap(16, 16)
        sBoxRFPix.fill(QColor(preview_display_selectbox_right_fill))
        buttonSelectBoxRightFill.setIcon(QIcon(sBoxRFPix))
        self.connect(buttonSelectBoxRightFill, SIGNAL("clicked()"), self, SLOT("chooseDisplaySelectBoxRightFill()"))

        labelSelectBoxAlpha = QLabel(self.tr("Selection Box Fill Alpha"), groupBoxColor)
        spinBoxSelectBoxAlpha = QSpinBox(groupBoxColor)
        spinBoxSelectBoxAlpha.setRange(0, 255)
        self.dialog_display_selectbox_alpha = self.mainWin.getSettingsDisplaySelectBoxAlpha()
        self.preview_display_selectbox_alpha = self.dialog_display_selectbox_alpha
        spinBoxSelectBoxAlpha.setValue(self.preview_display_selectbox_alpha)
        self.connect(spinBoxSelectBoxAlpha, SIGNAL("valueChanged(int)"), self, SLOT("spinBoxDisplaySelectBoxAlphaValueChanged(int)"))

        gridLayoutColor = QGridLayout(widget)
        gridLayoutColor.addWidget(labelCrossHairColor,       0, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(buttonCrossHairColor,      0, 1, Qt.AlignRight)
        gridLayoutColor.addWidget(labelBGColor,              1, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(buttonBGColor,             1, 1, Qt.AlignRight)
        gridLayoutColor.addWidget(labelSelectBoxLeftColor,   2, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(buttonSelectBoxLeftColor,  2, 1, Qt.AlignRight)
        gridLayoutColor.addWidget(labelSelectBoxLeftFill,    3, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(buttonSelectBoxLeftFill,   3, 1, Qt.AlignRight)
        gridLayoutColor.addWidget(labelSelectBoxRightColor,  4, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(buttonSelectBoxRightColor, 4, 1, Qt.AlignRight)
        gridLayoutColor.addWidget(labelSelectBoxRightFill,   5, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(buttonSelectBoxRightFill,  5, 1, Qt.AlignRight)
        gridLayoutColor.addWidget(labelSelectBoxAlpha,       6, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(spinBoxSelectBoxAlpha,     6, 1, Qt.AlignRight)
        groupBoxColor.setLayout(gridLayoutColor)

        # Zoom
        groupBoxZoom = QGroupBox(self.tr("Zoom"), widget)

        labelZoomScaleIn = QLabel(self.tr("Zoom In Scale"), groupBoxZoom)
        spinBoxZoomScaleIn = QDoubleSpinBox(groupBoxZoom)
        self.dialog_display_zoomscale_in = self.mainWin.getSettingsDisplayZoomScaleIn()
        spinBoxZoomScaleIn.setValue(self.dialog_display_zoomscale_in)
        spinBoxZoomScaleIn.setSingleStep(0.01)
        spinBoxZoomScaleIn.setRange(1.01, 10.00)
        self.connect(spinBoxZoomScaleIn, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxZoomScaleInValueChanged(double)"))

        labelZoomScaleOut = QLabel(self.tr("Zoom Out Scale"), groupBoxZoom)
        spinBoxZoomScaleOut = QDoubleSpinBox(groupBoxZoom)
        self.dialog_display_zoomscale_out = self.mainWin.getSettingsDisplayZoomScaleOut()
        spinBoxZoomScaleOut.setValue(self.dialog_display_zoomscale_out)
        spinBoxZoomScaleOut.setSingleStep(0.01)
        spinBoxZoomScaleOut.setRange(0.01, 0.99)
        self.connect(spinBoxZoomScaleOut, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxZoomScaleOutValueChanged(double)"))

        gridLayoutZoom = QGridLayout(groupBoxZoom)
        gridLayoutZoom.addWidget(labelZoomScaleIn,    0, 0, Qt.AlignLeft)
        gridLayoutZoom.addWidget(spinBoxZoomScaleIn,  0, 1, Qt.AlignRight)
        gridLayoutZoom.addWidget(labelZoomScaleOut,   1, 0, Qt.AlignLeft)
        gridLayoutZoom.addWidget(spinBoxZoomScaleOut, 1, 1, Qt.AlignRight)
        groupBoxZoom.setLayout(gridLayoutZoom)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        # vboxLayoutMain.addWidget(groupBoxRender) # TODO: Review OpenGL and Rendering settings for future inclusion
        vboxLayoutMain.addWidget(groupBoxScrollBars)
        vboxLayoutMain.addWidget(groupBoxColor)
        vboxLayoutMain.addWidget(groupBoxZoom)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea

    # TODO: finish prompt options
    def createTabPrompt(self):
        """
        :return: TOWRITE
        :rtype: TOWRITE
        """
        widget = QWidget(self)

        # Colors
        groupBoxColor = QGroupBox(self.tr("Colors"), widget)

        labelTextColor = QLabel(self.tr("Text Color"), groupBoxColor)
        buttonTextColor = QPushButton(self.tr("Choose"), groupBoxColor)
        self.dialog_prompt_text_color  = self.mainWin.getSettingsPromptTextColor()
        preview_prompt_text_color = self.dialog_prompt_text_color
        self.accept_prompt_text_color  = self.dialog_prompt_text_color
        pix = QPixmap(16, 16)
        pix.fill(QColor(preview_prompt_text_color))
        buttonTextColor.setIcon(QIcon(pix))
        self.connect(buttonTextColor, SIGNAL("clicked()"), self, SLOT("choosePromptTextColor()"))

        labelBGColor = QLabel(self.tr("Background Color"), groupBoxColor)
        buttonBGColor = QPushButton(self.tr("Choose"), groupBoxColor)
        self.dialog_prompt_bg_color  = self.mainWin.getSettingsPromptBGColor()
        preview_prompt_bg_color = self.dialog_prompt_bg_color
        self.accept_prompt_bg_color  = self.dialog_prompt_bg_color
        pixx = QPixmap(16, 16)
        pixx.fill(QColor(preview_prompt_bg_color))
        buttonBGColor.setIcon(QIcon(pixx))
        self.connect(buttonBGColor, SIGNAL("clicked()"), self, SLOT("choosePromptBackgroundColor()"))

        gridLayoutColor = QGridLayout(widget)
        gridLayoutColor.addWidget(labelTextColor,  0, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(buttonTextColor, 0, 1, Qt.AlignRight)
        gridLayoutColor.addWidget(labelBGColor,    1, 0, Qt.AlignLeft)
        gridLayoutColor.addWidget(buttonBGColor,   1, 1, Qt.AlignRight)
        groupBoxColor.setLayout(gridLayoutColor)

        # TODO: Tweak the Prompt Font ComboBoxes so they work properly
        # Font
        groupBoxFont = QGroupBox(self.tr("Font"), widget)

        labelFontFamily = QLabel(self.tr("Font Family"), groupBoxFont)
        comboBoxFontFamily = QFontComboBox(groupBoxFont)
        self.dialog_prompt_font_family = self.mainWin.getSettingsPromptFontFamily()
        self.preview_prompt_font_family = self.dialog_prompt_font_family
        comboBoxFontFamily.setCurrentFont(QFont(self.preview_prompt_font_family))
        self.connect(comboBoxFontFamily, SIGNAL("currentIndexChanged(QString)"), self, SLOT("comboBoxPromptFontFamilyCurrentIndexChanged(QString)"))
        labelFontStyle = QLabel(self.tr("Font Style"), groupBoxFont)
        comboBoxFontStyle = QComboBox(groupBoxFont)
        comboBoxFontStyle.addItem("Normal")
        comboBoxFontStyle.addItem("Italic")
        #TODO# comboBoxFontStyle.setEditText(self.preview_prompt_font_style)
        self.connect(comboBoxFontStyle, SIGNAL("currentIndexChanged(QString)"), self, SLOT("comboBoxPromptFontStyleCurrentIndexChanged(QString)"))
        labelFontSize = QLabel(self.tr("Font Size"), groupBoxFont)
        spinBoxFontSize = QSpinBox(groupBoxFont)
        spinBoxFontSize.setRange(4, 64)
        self.dialog_prompt_font_size = self.mainWin.getSettingsPromptFontSize()
        self.preview_prompt_font_size = self.dialog_prompt_font_size
        spinBoxFontSize.setValue(self.preview_prompt_font_size)
        self.connect(spinBoxFontSize, SIGNAL("valueChanged(int)"), self, SLOT("spinBoxPromptFontSizeValueChanged(int)"))

        gridLayoutFont = QGridLayout(groupBoxFont)
        gridLayoutFont.addWidget(labelFontFamily,    0, 0, Qt.AlignLeft)
        gridLayoutFont.addWidget(comboBoxFontFamily, 0, 1, Qt.AlignRight)
        gridLayoutFont.addWidget(labelFontStyle,     1, 0, Qt.AlignLeft)
        gridLayoutFont.addWidget(comboBoxFontStyle,  1, 1, Qt.AlignRight)
        gridLayoutFont.addWidget(labelFontSize,      2, 0, Qt.AlignLeft)
        gridLayoutFont.addWidget(spinBoxFontSize,    2, 1, Qt.AlignRight)
        groupBoxFont.setLayout(gridLayoutFont)

        # History
        groupBoxHistory = QGroupBox(self.tr("History"), widget)

        checkBoxPromptSaveHistory = QCheckBox(self.tr("Save History"), groupBoxHistory)
        self.dialog_prompt_save_history = self.mainWin.getSettingsPromptSaveHistory()
        checkBoxPromptSaveHistory.setChecked(self.dialog_prompt_save_history)
        self.connect(checkBoxPromptSaveHistory, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxPromptSaveHistoryStateChanged(int)"))

        checkBoxPromptSaveHistoryAsHtml = QCheckBox(self.tr("Save As HTML"), groupBoxHistory)
        self.dialog_prompt_save_history_as_html = self.mainWin.getSettingsPromptSaveHistoryAsHtml()
        checkBoxPromptSaveHistoryAsHtml.setChecked(self.dialog_prompt_save_history_as_html)
        self.connect(checkBoxPromptSaveHistoryAsHtml, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxPromptSaveHistoryAsHtmlStateChanged(int)"))

        vboxLayoutHistory = QVBoxLayout(groupBoxHistory)
        vboxLayoutHistory.addWidget(checkBoxPromptSaveHistory)
        vboxLayoutHistory.addWidget(checkBoxPromptSaveHistoryAsHtml)
        groupBoxHistory.setLayout(vboxLayoutHistory)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxColor)
        vboxLayoutMain.addWidget(groupBoxFont)
        vboxLayoutMain.addWidget(groupBoxHistory)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea


    # TODO: finish open/save options
    def createTabOpenSave(self):
        """TOWRITE"""
        widget = QWidget(self)

        # Custom Filter
        groupBoxCustomFilter = QGroupBox(self.tr("Custom Filter"), widget)
        groupBoxCustomFilter.setEnabled(False) # TODO: Fixup custom filter

        self.dialog_opensave_custom_filter = self.mainWin.getSettingsCustomFilter()

        checkBoxCustomFilter100 = QCheckBox("100", groupBoxCustomFilter)
        checkBoxCustomFilter100.setChecked("*.100" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilter100, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilter10o = QCheckBox("10o", groupBoxCustomFilter)
        checkBoxCustomFilter10o.setChecked("*.10o" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilter10o, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterART = QCheckBox("ART", groupBoxCustomFilter)
        checkBoxCustomFilterART.setChecked("*.ART" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterART, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterBMC = QCheckBox("BMC", groupBoxCustomFilter)
        checkBoxCustomFilterBMC.setChecked("*.BMC" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterBMC, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterBRO = QCheckBox("BRO", groupBoxCustomFilter)
        checkBoxCustomFilterBRO.setChecked("*.BRO" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterBRO, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterCND = QCheckBox("CND", groupBoxCustomFilter)
        checkBoxCustomFilterCND.setChecked("*.CND" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterCND, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterCOL = QCheckBox("COL", groupBoxCustomFilter)
        checkBoxCustomFilterCOL.setChecked("*.COL" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterCOL, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterCSD = QCheckBox("CSD", groupBoxCustomFilter)
        checkBoxCustomFilterCSD.setChecked("*.CSD" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterCSD, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterCSV = QCheckBox("CSV", groupBoxCustomFilter)
        checkBoxCustomFilterCSV.setChecked("*.CSV" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterCSV, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterDAT = QCheckBox("DAT", groupBoxCustomFilter)
        checkBoxCustomFilterDAT.setChecked("*.DAT" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterDAT, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterDEM = QCheckBox("DEM", groupBoxCustomFilter)
        checkBoxCustomFilterDEM.setChecked("*.DEM" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterDEM, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterDSB = QCheckBox("DSB", groupBoxCustomFilter)
        checkBoxCustomFilterDSB.setChecked("*.DSB" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterDSB, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterDST = QCheckBox("DST", groupBoxCustomFilter)
        checkBoxCustomFilterDST.setChecked("*.DST" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterDST, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterDSZ = QCheckBox("DSZ", groupBoxCustomFilter)
        checkBoxCustomFilterDSZ.setChecked("*.DSZ" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterDSZ, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterDXF = QCheckBox("DXF", groupBoxCustomFilter)
        checkBoxCustomFilterDXF.setChecked("*.DXF" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterDXF, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterEDR = QCheckBox("EDR", groupBoxCustomFilter)
        checkBoxCustomFilterEDR.setChecked("*.EDR" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterEDR, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterEMD = QCheckBox("EMD", groupBoxCustomFilter)
        checkBoxCustomFilterEMD.setChecked("*.EMD" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterEMD, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterEXP = QCheckBox("EXP", groupBoxCustomFilter)
        checkBoxCustomFilterEXP.setChecked("*.EXP" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterEXP, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterEXY = QCheckBox("EXY", groupBoxCustomFilter)
        checkBoxCustomFilterEXY.setChecked("*.EXY" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterEXY, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterEYS = QCheckBox("EYS", groupBoxCustomFilter)
        checkBoxCustomFilterEYS.setChecked("*.EYS" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterEYS, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterFXY = QCheckBox("FXY", groupBoxCustomFilter)
        checkBoxCustomFilterFXY.setChecked("*.FXY" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterFXY, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterGNC = QCheckBox("GNC", groupBoxCustomFilter)
        checkBoxCustomFilterGNC.setChecked("*.GNC" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterGNC, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterGT = QCheckBox("GT", groupBoxCustomFilter)
        checkBoxCustomFilterGT.setChecked("*.GT" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterGT, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterHUS = QCheckBox("HUS", groupBoxCustomFilter)
        checkBoxCustomFilterHUS.setChecked("*.HUS" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterHUS, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterINB = QCheckBox("INB", groupBoxCustomFilter)
        checkBoxCustomFilterINB.setChecked("*.INB" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterINB, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterJEF = QCheckBox("JEF", groupBoxCustomFilter)
        checkBoxCustomFilterJEF.setChecked("*.JEF" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterJEF, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterKSM = QCheckBox("KSM", groupBoxCustomFilter)
        checkBoxCustomFilterKSM.setChecked("*.KSM" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterKSM, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPCD = QCheckBox("PCD", groupBoxCustomFilter)
        checkBoxCustomFilterPCD.setChecked("*.PCD" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPCD, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPCM = QCheckBox("PCM", groupBoxCustomFilter)
        checkBoxCustomFilterPCM.setChecked("*.PCM" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPCM, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPCQ = QCheckBox("PCQ", groupBoxCustomFilter)
        checkBoxCustomFilterPCQ.setChecked("*.PCQ" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPCQ, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPCS = QCheckBox("PCS", groupBoxCustomFilter)
        checkBoxCustomFilterPCS.setChecked("*.PCS" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPCS, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPEC = QCheckBox("PEC", groupBoxCustomFilter)
        checkBoxCustomFilterPEC.setChecked("*.PEC" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPEC, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPEL = QCheckBox("PEL", groupBoxCustomFilter)
        checkBoxCustomFilterPEL.setChecked("*.PEL" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPEL, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPEM = QCheckBox("PEM", groupBoxCustomFilter)
        checkBoxCustomFilterPEM.setChecked("*.PEM" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPEM, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPES = QCheckBox("PES", groupBoxCustomFilter)
        checkBoxCustomFilterPES.setChecked("*.PES" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPES, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPHB = QCheckBox("PHB", groupBoxCustomFilter)
        checkBoxCustomFilterPHB.setChecked("*.PHB" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPHB, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterPHC = QCheckBox("PHC", groupBoxCustomFilter)
        checkBoxCustomFilterPHC.setChecked("*.PHC" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterPHC, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterRGB = QCheckBox("RGB", groupBoxCustomFilter)
        checkBoxCustomFilterRGB.setChecked("*.RGB" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterRGB, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterSEW = QCheckBox("SEW", groupBoxCustomFilter)
        checkBoxCustomFilterSEW.setChecked("*.SEW" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterSEW, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterSHV = QCheckBox("SHV", groupBoxCustomFilter)
        checkBoxCustomFilterSHV.setChecked("*.SHV" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterSHV, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterSST = QCheckBox("SST", groupBoxCustomFilter)
        checkBoxCustomFilterSST.setChecked("*.SST" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterSST, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterSTX = QCheckBox("STX", groupBoxCustomFilter)
        checkBoxCustomFilterSTX.setChecked("*.STX" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterSTX, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterSVG = QCheckBox("SVG", groupBoxCustomFilter)
        checkBoxCustomFilterSVG.setChecked("*.SVG" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterSVG, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterT09 = QCheckBox("T09", groupBoxCustomFilter)
        checkBoxCustomFilterT09.setChecked("*.T09" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterT09, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterTAP = QCheckBox("TAP", groupBoxCustomFilter)
        checkBoxCustomFilterTAP.setChecked("*.TAP" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterTAP, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterTHR = QCheckBox("THR", groupBoxCustomFilter)
        checkBoxCustomFilterTHR.setChecked("*.THR" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterTHR, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterTXT = QCheckBox("TXT", groupBoxCustomFilter)
        checkBoxCustomFilterTXT.setChecked("*.TXT" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterTXT, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterU00 = QCheckBox("U00", groupBoxCustomFilter)
        checkBoxCustomFilterU00.setChecked("*.U00" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterU00, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterU01 = QCheckBox("U01", groupBoxCustomFilter)
        checkBoxCustomFilterU01.setChecked("*.U01" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterU01, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterVIP = QCheckBox("VIP", groupBoxCustomFilter)
        checkBoxCustomFilterVIP.setChecked("*.VIP" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterVIP, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterVP3 = QCheckBox("VP3", groupBoxCustomFilter)
        checkBoxCustomFilterVP3.setChecked("*.VP3" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterVP3, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterXXX = QCheckBox("XXX", groupBoxCustomFilter)
        checkBoxCustomFilterXXX.setChecked("*.XXX" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterXXX, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        checkBoxCustomFilterZSK = QCheckBox("ZSK", groupBoxCustomFilter)
        checkBoxCustomFilterZSK.setChecked("*.ZSK" in self.dialog_opensave_custom_filter)
        self.connect(checkBoxCustomFilterZSK, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxCustomFilterStateChanged(int)"))

        buttonCustomFilterSelectAll = QPushButton(self.tr("Select All"), groupBoxCustomFilter)
        self.connect(buttonCustomFilterSelectAll, SIGNAL("clicked()"), self, SLOT("buttonCustomFilterSelectAllClicked()"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilter100, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilter10o, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterART, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterBMC, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterBRO, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterCND, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterCOL, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterCSD, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterCSV, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterDAT, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterDEM, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterDSB, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterDST, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterDSZ, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterDXF, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterEDR, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterEMD, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterEXP, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterEXY, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterEYS, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterFXY, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterGNC, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterGT,  SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterHUS, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterINB, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterJEF, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterKSM, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPCD, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPCM, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPCQ, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPCS, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPEC, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPEL, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPEM, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPES, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPHB, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterPHC, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterRGB, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterSEW, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterSHV, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterSST, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterSTX, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterSVG, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterT09, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterTAP, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterTHR, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterTXT, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterU00, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterU01, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterVIP, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterVP3, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterXXX, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterSelectAll(bool)"), checkBoxCustomFilterZSK, SLOT("setChecked(bool)"))

        buttonCustomFilterClearAll = QPushButton("Clear All", groupBoxCustomFilter)
        self.connect(buttonCustomFilterClearAll, SIGNAL("clicked()"), self, SLOT("buttonCustomFilterClearAllClicked()"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilter100, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilter10o, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterART, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterBMC, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterBRO, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterCND, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterCOL, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterCSD, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterCSV, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterDAT, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterDEM, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterDSB, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterDST, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterDSZ, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterDXF, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterEDR, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterEMD, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterEXP, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterEXY, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterEYS, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterFXY, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterGNC, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterGT,  SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterHUS, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterINB, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterJEF, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterKSM, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPCD, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPCM, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPCQ, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPCS, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPEC, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPEL, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPEM, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPES, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPHB, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterPHC, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterRGB, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterSEW, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterSHV, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterSST, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterSTX, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterSVG, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterT09, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterTAP, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterTHR, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterTXT, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterU00, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterU01, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterVIP, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterVP3, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterXXX, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonCustomFilterClearAll(bool)"), checkBoxCustomFilterZSK, SLOT("setChecked(bool)"))

        gridLayoutCustomFilter = QGridLayout(groupBoxCustomFilter)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilter100,     0, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilter10o,     1, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterART,     2, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterBMC,     3, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterBRO,     4, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterCND,     5, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterCOL,     6, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterCSD,     7, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterCSV,     8, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterDAT,     9, 0, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterDEM,     0, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterDSB,     1, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterDST,     2, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterDSZ,     3, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterDXF,     4, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterEDR,     5, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterEMD,     6, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterEXP,     7, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterEXY,     8, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterEYS,     9, 1, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterFXY,     0, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterGNC,     1, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterGT,      2, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterHUS,     3, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterINB,     4, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterJEF,     5, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterKSM,     6, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPCD,     7, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPCM,     8, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPCQ,     9, 2, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPCS,     0, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPEC,     1, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPEL,     2, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPEM,     3, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPES,     4, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPHB,     5, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterPHC,     6, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterRGB,     7, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterSEW,     8, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterSHV,     9, 3, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterSST,     0, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterSTX,     1, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterSVG,     2, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterT09,     3, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterTAP,     4, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterTHR,     5, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterTXT,     6, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterU00,     7, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterU01,     8, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterVIP,     9, 4, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterVP3,     0, 5, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterXXX,     1, 5, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(checkBoxCustomFilterZSK,     2, 5, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(buttonCustomFilterSelectAll, 0, 6, Qt.AlignLeft)
        gridLayoutCustomFilter.addWidget(buttonCustomFilterClearAll,  1, 6, Qt.AlignLeft)
        gridLayoutCustomFilter.setColumnStretch(6,1)
        groupBoxCustomFilter.setLayout(gridLayoutCustomFilter)

        #TODO# if "supported" in self.dialog_opensave_custom_filter:
        #TODO#     buttonCustomFilterSelectAllClicked()

        # Opening
        groupBoxOpening = QGroupBox(self.tr("File Open"), widget)

        comboBoxOpenFormat = QComboBox(groupBoxOpening)

        checkBoxOpenThumbnail = QCheckBox(self.tr("Preview Thumbnails"), groupBoxOpening)
        checkBoxOpenThumbnail.setChecked(False)

        # TODO: Add a button to clear the recent history.

        labelRecentMaxFiles = QLabel(self.tr("Number of recently accessed files to show"), groupBoxOpening)
        spinBoxRecentMaxFiles = QSpinBox(groupBoxOpening)
        spinBoxRecentMaxFiles.setRange(0, 10)
        self.dialog_opensave_recent_max_files = self.mainWin.getSettingsRecentMaxFiles()
        spinBoxRecentMaxFiles.setValue(self.dialog_opensave_recent_max_files)
        self.connect(spinBoxRecentMaxFiles, SIGNAL("valueChanged(int)"), self, SLOT("spinBoxRecentMaxFilesValueChanged(int)"))

        frameRecent = QFrame(groupBoxOpening)
        gridLayoutRecent = QGridLayout(frameRecent)
        gridLayoutRecent.addWidget(labelRecentMaxFiles,   0, 0, Qt.AlignLeft)
        gridLayoutRecent.addWidget(spinBoxRecentMaxFiles, 0, 1, Qt.AlignRight)
        frameRecent.setLayout(gridLayoutRecent)

        vboxLayoutOpening = QVBoxLayout(groupBoxOpening)
        vboxLayoutOpening.addWidget(comboBoxOpenFormat)
        vboxLayoutOpening.addWidget(checkBoxOpenThumbnail)
        vboxLayoutOpening.addWidget(frameRecent)
        groupBoxOpening.setLayout(vboxLayoutOpening)

        # Saving
        groupBoxSaving = QGroupBox(self.tr("File Save"), widget)

        comboBoxSaveFormat = QComboBox(groupBoxSaving)

        checkBoxSaveThumbnail = QCheckBox(self.tr("Save Thumbnails"), groupBoxSaving)
        checkBoxSaveThumbnail.setChecked(False)

        checkBoxAutoSave = QCheckBox(self.tr("AutoSave"), groupBoxSaving)
        checkBoxAutoSave.setChecked(False)

        vboxLayoutSaving = QVBoxLayout(groupBoxSaving)
        vboxLayoutSaving.addWidget(comboBoxSaveFormat)
        vboxLayoutSaving.addWidget(checkBoxSaveThumbnail)
        vboxLayoutSaving.addWidget(checkBoxAutoSave)
        groupBoxSaving.setLayout(vboxLayoutSaving)

        # Trimming
        groupBoxTrim = QGroupBox(self.tr("Trimming"), widget)

        labelTrimDstNumJumps = QLabel(self.tr("DST Only: Minimum number of jumps to trim"), groupBoxTrim)
        spinBoxTrimDstNumJumps = QSpinBox(groupBoxTrim)
        spinBoxTrimDstNumJumps.setRange(1, 20)
        self.dialog_opensave_trim_dst_num_jumps = self.mainWin.getSettingsOpenSaveTrimDstNumJumps()
        spinBoxTrimDstNumJumps.setValue(self.dialog_opensave_trim_dst_num_jumps)
        self.connect(spinBoxTrimDstNumJumps, SIGNAL("valueChanged(int)"), self, SLOT("spinBoxTrimDstNumJumpsValueChanged(int)"))

        frameTrimDstNumJumps = QFrame(groupBoxTrim)
        gridLayoutTrimDstNumJumps = QGridLayout(frameTrimDstNumJumps)
        gridLayoutTrimDstNumJumps.addWidget(labelTrimDstNumJumps,   0, 0, Qt.AlignLeft)
        gridLayoutTrimDstNumJumps.addWidget(spinBoxTrimDstNumJumps, 0, 1, Qt.AlignRight)
        frameTrimDstNumJumps.setLayout(gridLayoutTrimDstNumJumps)

        vboxLayoutTrim = QVBoxLayout(groupBoxTrim)
        vboxLayoutTrim.addWidget(frameTrimDstNumJumps)
        groupBoxTrim.setLayout(vboxLayoutTrim)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxCustomFilter)
        vboxLayoutMain.addWidget(groupBoxOpening)
        vboxLayoutMain.addWidget(groupBoxSaving)
        vboxLayoutMain.addWidget(groupBoxTrim)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea


    def createTabPrinting(self):
        """TOWRITE"""
        widget = QWidget(self)

        # Default Printer
        groupBoxDefaultPrinter = QGroupBox(self.tr("Default Printer"), widget)

        radioButtonUseSame = QRadioButton(self.tr("Use as default device"), groupBoxDefaultPrinter)
        radioButtonUseSame.setChecked(not self.mainWin.getSettingsPrintingUseLastDevice())
        radioButtonUseLast = QRadioButton(self.tr("Use last used device"), groupBoxDefaultPrinter)
        radioButtonUseLast.setChecked(self.mainWin.getSettingsPrintingUseLastDevice())

        comboBoxDefaultDevice = QComboBox(groupBoxDefaultPrinter)
        listAvailPrinters = QPrinterInfo.availablePrinters() # QList<QPrinterInfo>
        ## foreach(QPrinterInfo info, listAvailPrinters)
        for info in listAvailPrinters:
            comboBoxDefaultDevice.addItem(QIcon("icons/" + self.mainWin.getSettingsGeneralIconTheme() + "/" + "print" + ".png"), info.printerName())

        vboxLayoutDefaultPrinter = QVBoxLayout(groupBoxDefaultPrinter)
        vboxLayoutDefaultPrinter.addWidget(radioButtonUseSame)
        vboxLayoutDefaultPrinter.addWidget(comboBoxDefaultDevice)
        vboxLayoutDefaultPrinter.addWidget(radioButtonUseLast)
        groupBoxDefaultPrinter.setLayout(vboxLayoutDefaultPrinter)

        # Save Ink
        groupBoxSaveInk = QGroupBox(self.tr("Save Ink"), widget)

        checkBoxDisableBG = QCheckBox(self.tr("Disable Background"), groupBoxSaveInk)
        self.dialog_printing_disable_bg = self.mainWin.getSettingsPrintingDisableBG()
        checkBoxDisableBG.setChecked(self.dialog_printing_disable_bg)
        self.connect(checkBoxDisableBG, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxDisableBGStateChanged(int)"))

        vboxLayoutSaveInk = QVBoxLayout(groupBoxSaveInk)
        vboxLayoutSaveInk.addWidget(checkBoxDisableBG)
        groupBoxSaveInk.setLayout(vboxLayoutSaveInk)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxDefaultPrinter)
        vboxLayoutMain.addWidget(groupBoxSaveInk)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea


    def createTabSnap(self):
        """TOWRITE"""
        widget = QWidget(self)

        # TODO: finish self

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea


    def createTabGridRuler(self):
        """TOWRITE"""
        widget = QWidget(self)

        # Grid Misc
        groupBoxGridMisc = QGroupBox(self.tr("Grid Misc"), widget)

        checkBoxGridShowOnLoad = QCheckBox(self.tr("Initially show grid when loading a file"), groupBoxGridMisc)
        self.dialog_grid_show_on_load = self.mainWin.getSettingsGridShowOnLoad()
        checkBoxGridShowOnLoad.setChecked(self.dialog_grid_show_on_load)
        self.connect(checkBoxGridShowOnLoad, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxGridShowOnLoadStateChanged(int)"))

        checkBoxGridShowOrigin = QCheckBox(self.tr("Show the origin when the grid is enabled"), groupBoxGridMisc)
        self.dialog_grid_show_origin = self.mainWin.getSettingsGridShowOrigin()
        checkBoxGridShowOrigin.setChecked(self.dialog_grid_show_origin)
        self.connect(checkBoxGridShowOrigin, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxGridShowOriginStateChanged(int)"))

        gridLayoutGridMisc = QGridLayout(widget)
        gridLayoutGridMisc.addWidget(checkBoxGridShowOnLoad, 0, 0, Qt.AlignLeft)
        gridLayoutGridMisc.addWidget(checkBoxGridShowOrigin, 1, 0, Qt.AlignLeft)
        groupBoxGridMisc.setLayout(gridLayoutGridMisc)

        # Grid Color
        groupBoxGridColor = QGroupBox(self.tr("Grid Color"), widget)

        checkBoxGridColorMatchCrossHair = QCheckBox(self.tr("Match grid color to crosshair color"), groupBoxGridColor)
        self.dialog_grid_color_match_crosshair = self.mainWin.getSettingsGridColorMatchCrossHair()
        checkBoxGridColorMatchCrossHair.setChecked(self.dialog_grid_color_match_crosshair)
        self.connect(checkBoxGridColorMatchCrossHair, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxGridColorMatchCrossHairStateChanged(int)"))

        labelGridColor = QLabel(self.tr("Grid Color"), groupBoxGridColor)
        labelGridColor.setObjectName("labelGridColor")
        buttonGridColor = QPushButton(self.tr("Choose"), groupBoxGridColor)
        buttonGridColor.setObjectName("buttonGridColor")
        if self.dialog_grid_color_match_crosshair:
            self.dialog_grid_color = self.mainWin.getSettingsDisplayCrossHairColor()
        else:
            self.dialog_grid_color = self.mainWin.getSettingsGridColor()
        preview_grid_color = self.dialog_grid_color
        self.accept_grid_color  = self.dialog_grid_color
        gridPix = QPixmap(16, 16)
        gridPix.fill(QColor(preview_grid_color))
        buttonGridColor.setIcon(QIcon(gridPix))
        self.connect(buttonGridColor, SIGNAL("clicked()"), self, SLOT("chooseGridColor()"))

        labelGridColor.setEnabled(not self.dialog_grid_color_match_crosshair)
        buttonGridColor.setEnabled(not self.dialog_grid_color_match_crosshair)

        gridLayoutGridColor = QGridLayout(widget)
        gridLayoutGridColor.addWidget(checkBoxGridColorMatchCrossHair, 0, 0, Qt.AlignLeft)
        gridLayoutGridColor.addWidget(labelGridColor,                  1, 0, Qt.AlignLeft)
        gridLayoutGridColor.addWidget(buttonGridColor,                 1, 1, Qt.AlignRight)
        groupBoxGridColor.setLayout(gridLayoutGridColor)

        # Grid Geometry
        groupBoxGridGeom = QGroupBox(self.tr("Grid Geometry"), widget)

        checkBoxGridLoadFromFile = QCheckBox(self.tr("Set grid size from opened file"), groupBoxGridGeom)
        self.dialog_grid_load_from_file = self.mainWin.getSettingsGridLoadFromFile()
        checkBoxGridLoadFromFile.setChecked(self.dialog_grid_load_from_file)
        self.connect(checkBoxGridLoadFromFile, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxGridLoadFromFileStateChanged(int)"))

        labelGridType = QLabel(self.tr("Grid Type"), groupBoxGridGeom)
        labelGridType.setObjectName("labelGridType")
        comboBoxGridType = QComboBox(groupBoxGridGeom)
        comboBoxGridType.setObjectName("comboBoxGridType")
        comboBoxGridType.addItem("Rectangular")
        comboBoxGridType.addItem("Circular")
        comboBoxGridType.addItem("Isometric")
        self.dialog_grid_type = self.mainWin.getSettingsGridType()
        comboBoxGridType.setCurrentIndex(comboBoxGridType.findText(self.dialog_grid_type))
        self.connect(comboBoxGridType, SIGNAL("currentIndexChanged(QString)"), self, SLOT("comboBoxGridTypeCurrentIndexChanged(QString)"))

        checkBoxGridCenterOnOrigin = QCheckBox(self.tr("Center the grid on the origin"), groupBoxGridGeom)
        checkBoxGridCenterOnOrigin.setObjectName("checkBoxGridCenterOnOrigin")
        self.dialog_grid_center_on_origin = self.mainWin.getSettingsGridCenterOnOrigin()
        checkBoxGridCenterOnOrigin.setChecked(self.dialog_grid_center_on_origin)
        self.connect(checkBoxGridCenterOnOrigin, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxGridCenterOnOriginStateChanged(int)"))

        labelGridCenterX = QLabel(self.tr("Grid Center X"), groupBoxGridGeom)
        labelGridCenterX.setObjectName("labelGridCenterX")
        spinBoxGridCenterX = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridCenterX.setObjectName("spinBoxGridCenterX")
        self.dialog_grid_center_x = self.mainWin.getSettingsGridCenterX()
        spinBoxGridCenterX.setSingleStep(1.000)
        spinBoxGridCenterX.setRange(-1000.000, 1000.000)
        spinBoxGridCenterX.setValue(self.dialog_grid_center_x)
        self.connect(spinBoxGridCenterX, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridCenterXValueChanged(double)"))

        labelGridCenterY = QLabel(self.tr("Grid Center Y"), groupBoxGridGeom)
        labelGridCenterY.setObjectName("labelGridCenterY")
        spinBoxGridCenterY = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridCenterY.setObjectName("spinBoxGridCenterY")
        self.dialog_grid_center_y = self.mainWin.getSettingsGridCenterY()
        spinBoxGridCenterY.setSingleStep(1.000)
        spinBoxGridCenterY.setRange(-1000.000, 1000.000)
        spinBoxGridCenterY.setValue(self.dialog_grid_center_y)
        self.connect(spinBoxGridCenterY, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridCenterYValueChanged(double)"))

        labelGridSizeX = QLabel(self.tr("Grid Size X"), groupBoxGridGeom)
        labelGridSizeX.setObjectName("labelGridSizeX")
        spinBoxGridSizeX = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSizeX.setObjectName("spinBoxGridSizeX")
        self.dialog_grid_size_x = self.mainWin.getSettingsGridSizeX()
        spinBoxGridSizeX.setSingleStep(1.000)
        spinBoxGridSizeX.setRange(1.000, 1000.000)
        spinBoxGridSizeX.setValue(self.dialog_grid_size_x)
        self.connect(spinBoxGridSizeX, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridSizeXValueChanged(double)"))

        labelGridSizeY = QLabel(self.tr("Grid Size Y"), groupBoxGridGeom)
        labelGridSizeY.setObjectName("labelGridSizeY")
        spinBoxGridSizeY = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSizeY.setObjectName("spinBoxGridSizeY")
        self.dialog_grid_size_y = self.mainWin.getSettingsGridSizeY()
        spinBoxGridSizeY.setSingleStep(1.000)
        spinBoxGridSizeY.setRange(1.000, 1000.000)
        spinBoxGridSizeY.setValue(self.dialog_grid_size_y)
        self.connect(spinBoxGridSizeY, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridSizeYValueChanged(double)"))

        labelGridSpacingX = QLabel(self.tr("Grid Spacing X"), groupBoxGridGeom)
        labelGridSpacingX.setObjectName("labelGridSpacingX")
        spinBoxGridSpacingX = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSpacingX.setObjectName("spinBoxGridSpacingX")
        self.dialog_grid_spacing_x = self.mainWin.getSettingsGridSpacingX()
        spinBoxGridSpacingX.setSingleStep(1.000)
        spinBoxGridSpacingX.setRange(0.001, 1000.000)
        spinBoxGridSpacingX.setValue(self.dialog_grid_spacing_x)
        self.connect(spinBoxGridSpacingX, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridSpacingXValueChanged(double)"))

        labelGridSpacingY = QLabel(self.tr("Grid Spacing Y"), groupBoxGridGeom)
        labelGridSpacingY.setObjectName("labelGridSpacingY")
        spinBoxGridSpacingY = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSpacingY.setObjectName("spinBoxGridSpacingY")
        self.dialog_grid_spacing_y = self.mainWin.getSettingsGridSpacingY()
        spinBoxGridSpacingY.setSingleStep(1.000)
        spinBoxGridSpacingY.setRange(0.001, 1000.000)
        spinBoxGridSpacingY.setValue(self.dialog_grid_spacing_y)
        self.connect(spinBoxGridSpacingY, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridSpacingYValueChanged(double)"))

        labelGridSizeRadius = QLabel(self.tr("Grid Size Radius"), groupBoxGridGeom)
        labelGridSizeRadius.setObjectName("labelGridSizeRadius")
        spinBoxGridSizeRadius = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSizeRadius.setObjectName("spinBoxGridSizeRadius")
        self.dialog_grid_size_radius = self.mainWin.getSettingsGridSizeRadius()
        spinBoxGridSizeRadius.setSingleStep(1.000)
        spinBoxGridSizeRadius.setRange(1.000, 1000.000)
        spinBoxGridSizeRadius.setValue(self.dialog_grid_size_radius)
        self.connect(spinBoxGridSizeRadius, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridSizeRadiusValueChanged(double)"))

        labelGridSpacingRadius = QLabel(self.tr("Grid Spacing Radius"), groupBoxGridGeom)
        labelGridSpacingRadius.setObjectName("labelGridSpacingRadius")
        spinBoxGridSpacingRadius = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSpacingRadius.setObjectName("spinBoxGridSpacingRadius")
        self.dialog_grid_spacing_radius = self.mainWin.getSettingsGridSpacingRadius()
        spinBoxGridSpacingRadius.setSingleStep(1.000)
        spinBoxGridSpacingRadius.setRange(0.001, 1000.000)
        spinBoxGridSpacingRadius.setValue(self.dialog_grid_spacing_radius)
        self.connect(spinBoxGridSpacingRadius, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridSpacingRadiusValueChanged(double)"))

        labelGridSpacingAngle = QLabel(self.tr("Grid Spacing Angle"), groupBoxGridGeom)
        labelGridSpacingAngle.setObjectName("labelGridSpacingAngle")
        spinBoxGridSpacingAngle = QDoubleSpinBox(groupBoxGridGeom)
        spinBoxGridSpacingAngle.setObjectName("spinBoxGridSpacingAngle")
        self.dialog_grid_spacing_angle = self.mainWin.getSettingsGridSpacingAngle()
        spinBoxGridSpacingAngle.setSingleStep(1.000)
        spinBoxGridSpacingAngle.setRange(0.001, 1000.000)
        spinBoxGridSpacingAngle.setValue(self.dialog_grid_spacing_angle)
        self.connect(spinBoxGridSpacingAngle, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxGridSpacingAngleValueChanged(double)"))

        labelGridType.setEnabled(not self.dialog_grid_load_from_file)
        comboBoxGridType.setEnabled(not self.dialog_grid_load_from_file)
        checkBoxGridCenterOnOrigin.setEnabled(not self.dialog_grid_load_from_file)
        labelGridCenterX.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridCenterX.setEnabled(not self.dialog_grid_load_from_file)
        labelGridCenterY.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridCenterY.setEnabled(not self.dialog_grid_load_from_file)
        labelGridSizeX.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridSizeX.setEnabled(not self.dialog_grid_load_from_file)
        labelGridSizeY.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridSizeY.setEnabled(not self.dialog_grid_load_from_file)
        labelGridSpacingX.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridSpacingX.setEnabled(not self.dialog_grid_load_from_file)
        labelGridSpacingY.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridSpacingY.setEnabled(not self.dialog_grid_load_from_file)
        labelGridSizeRadius.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridSizeRadius.setEnabled(not self.dialog_grid_load_from_file)
        labelGridSpacingRadius.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridSpacingRadius.setEnabled(not self.dialog_grid_load_from_file)
        labelGridSpacingAngle.setEnabled(not self.dialog_grid_load_from_file)
        spinBoxGridSpacingAngle.setEnabled(not self.dialog_grid_load_from_file)

        visibility = False
        if self.dialog_grid_type == "Circular":
            visibility = True
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

        gridLayoutGridGeom = QGridLayout(groupBoxGridGeom)
        gridLayoutGridGeom.addWidget(checkBoxGridLoadFromFile,   0, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(labelGridType,              1, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(comboBoxGridType,           1, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(checkBoxGridCenterOnOrigin, 2, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(labelGridCenterX,           3, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridCenterX,         3, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(labelGridCenterY,           4, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridCenterY,         4, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(labelGridSizeX,             5, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridSizeX,           5, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(labelGridSizeY,             6, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridSizeY,           6, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(labelGridSpacingX,          7, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridSpacingX,        7, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(labelGridSpacingY,          8, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridSpacingY,        8, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(labelGridSizeRadius,        9, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridSizeRadius,      9, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(labelGridSpacingRadius,    10, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridSpacingRadius,  10, 1, Qt.AlignRight)
        gridLayoutGridGeom.addWidget(labelGridSpacingAngle,     11, 0, Qt.AlignLeft)
        gridLayoutGridGeom.addWidget(spinBoxGridSpacingAngle,   11, 1, Qt.AlignRight)
        groupBoxGridGeom.setLayout(gridLayoutGridGeom)

        # Ruler Misc
        groupBoxRulerMisc = QGroupBox(self.tr("Ruler Misc"), widget)

        checkBoxRulerShowOnLoad = QCheckBox(self.tr("Initially show ruler when loading a file"), groupBoxRulerMisc)
        self.dialog_ruler_show_on_load = self.mainWin.getSettingsRulerShowOnLoad()
        checkBoxRulerShowOnLoad.setChecked(self.dialog_ruler_show_on_load)
        self.connect(checkBoxRulerShowOnLoad, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxRulerShowOnLoadStateChanged(int)"))

        labelRulerMetric = QLabel(self.tr("Ruler Units"), groupBoxRulerMisc)
        comboBoxRulerMetric = QComboBox(groupBoxRulerMisc)
        comboBoxRulerMetric.addItem("Imperial", False)
        comboBoxRulerMetric.addItem("Metric",   True)
        self.dialog_ruler_metric = self.mainWin.getSettingsRulerMetric()
        comboBoxRulerMetric.setCurrentIndex(comboBoxRulerMetric.findData(self.dialog_ruler_metric))
        self.connect(comboBoxRulerMetric, SIGNAL("currentIndexChanged(int)"), self, SLOT("comboBoxRulerMetricCurrentIndexChanged(int)"))

        gridLayoutRulerMisc = QGridLayout(widget)
        gridLayoutRulerMisc.addWidget(checkBoxRulerShowOnLoad, 0, 0, Qt.AlignLeft)
        gridLayoutRulerMisc.addWidget(labelRulerMetric,        1, 0, Qt.AlignLeft)
        gridLayoutRulerMisc.addWidget(comboBoxRulerMetric,     1, 1, Qt.AlignRight)
        groupBoxRulerMisc.setLayout(gridLayoutRulerMisc)

        # Ruler Color
        groupBoxRulerColor = QGroupBox(self.tr("Ruler Color"), widget)

        labelRulerColor = QLabel(self.tr("Ruler Color"), groupBoxRulerColor)
        labelRulerColor.setObjectName("labelRulerColor")
        buttonRulerColor = QPushButton(self.tr("Choose"), groupBoxRulerColor)
        buttonRulerColor.setObjectName("buttonRulerColor")
        self.dialog_ruler_color = self.mainWin.getSettingsRulerColor()
        preview_ruler_color = self.dialog_ruler_color
        self.accept_ruler_color  = self.dialog_ruler_color
        rulerPix = QPixmap(16, 16)
        rulerPix.fill(QColor(preview_ruler_color))
        buttonRulerColor.setIcon(QIcon(rulerPix))
        self.connect(buttonRulerColor, SIGNAL("clicked()"), self, SLOT("chooseRulerColor()"))

        gridLayoutRulerColor = QGridLayout(widget)
        gridLayoutRulerColor.addWidget(labelRulerColor,  1, 0, Qt.AlignLeft)
        gridLayoutRulerColor.addWidget(buttonRulerColor, 1, 1, Qt.AlignRight)
        groupBoxRulerColor.setLayout(gridLayoutRulerColor)

        # Ruler Geometry
        groupBoxRulerGeom = QGroupBox(self.tr("Ruler Geometry"), widget)

        labelRulerPixelSize = QLabel(self.tr("Ruler Pixel Size"), groupBoxRulerGeom)
        labelRulerPixelSize.setObjectName("labelRulerPixelSize")
        spinBoxRulerPixelSize = QDoubleSpinBox(groupBoxRulerGeom)
        spinBoxRulerPixelSize.setObjectName("spinBoxRulerPixelSize")
        self.dialog_ruler_pixel_size = self.mainWin.getSettingsRulerPixelSize()
        spinBoxRulerPixelSize.setSingleStep(1.000)
        spinBoxRulerPixelSize.setRange(20.000, 100.000)
        spinBoxRulerPixelSize.setValue(self.dialog_ruler_pixel_size)
        self.connect(spinBoxRulerPixelSize, SIGNAL("valueChanged(double)"), self, SLOT("spinBoxRulerPixelSizeValueChanged(double)"))

        gridLayoutRulerGeom = QGridLayout(groupBoxRulerGeom)
        gridLayoutRulerGeom.addWidget(labelRulerPixelSize,   0, 0, Qt.AlignLeft)
        gridLayoutRulerGeom.addWidget(spinBoxRulerPixelSize, 0, 1, Qt.AlignRight)
        groupBoxRulerGeom.setLayout(gridLayoutRulerGeom)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxGridMisc)
        vboxLayoutMain.addWidget(groupBoxGridColor)
        vboxLayoutMain.addWidget(groupBoxGridGeom)
        vboxLayoutMain.addWidget(groupBoxRulerMisc)
        vboxLayoutMain.addWidget(groupBoxRulerColor)
        vboxLayoutMain.addWidget(groupBoxRulerGeom)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabOrthoPolar(self):
        """TOWRITE"""
        widget = QWidget(self)

        # TODO: finish self

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabQuickSnap(self):
        """TOWRITE"""
        widget = QWidget(self)

        iconTheme = self.mainWin.getSettingsGeneralIconTheme() # QString

        # QSnap Locators
        groupBoxQSnapLoc = QGroupBox(self.tr("Locators Used"), widget)

        checkBoxQSnapEndPoint = QCheckBox(self.tr("Endpoint"), groupBoxQSnapLoc)
        self.dialog_qsnap_endpoint = self.mainWin.getSettingsQSnapEndPoint()
        checkBoxQSnapEndPoint.setChecked(self.dialog_qsnap_endpoint)
        checkBoxQSnapEndPoint.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoendpoint" + ".png"))
        self.connect(checkBoxQSnapEndPoint, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapEndPointStateChanged(int)"))

        checkBoxQSnapMidPoint = QCheckBox(self.tr("Midpoint"), groupBoxQSnapLoc)
        self.dialog_qsnap_midpoint = self.mainWin.getSettingsQSnapMidPoint()
        checkBoxQSnapMidPoint.setChecked(self.dialog_qsnap_midpoint)
        checkBoxQSnapMidPoint.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptomidpoint" + ".png"))
        self.connect(checkBoxQSnapMidPoint, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapMidPointStateChanged(int)"))

        checkBoxQSnapCenter = QCheckBox(self.tr("Center"), groupBoxQSnapLoc)
        self.dialog_qsnap_center = self.mainWin.getSettingsQSnapCenter()
        checkBoxQSnapCenter.setChecked(self.dialog_qsnap_center)
        checkBoxQSnapCenter.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptocenter" + ".png"))
        self.connect(checkBoxQSnapCenter, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapCenterStateChanged(int)"))

        checkBoxQSnapNode = QCheckBox(self.tr("Node"), groupBoxQSnapLoc)
        self.dialog_qsnap_node = self.mainWin.getSettingsQSnapNode()
        checkBoxQSnapNode.setChecked(self.dialog_qsnap_node)
        checkBoxQSnapNode.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptonode" + ".png"))
        self.connect(checkBoxQSnapNode, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapNodeStateChanged(int)"))

        checkBoxQSnapQuadrant = QCheckBox(self.tr("Quadrant"), groupBoxQSnapLoc)
        self.dialog_qsnap_quadrant = self.mainWin.getSettingsQSnapQuadrant()
        checkBoxQSnapQuadrant.setChecked(self.dialog_qsnap_quadrant)
        checkBoxQSnapQuadrant.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoquadrant" + ".png"))
        self.connect(checkBoxQSnapQuadrant, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapQuadrantStateChanged(int)"))

        checkBoxQSnapIntersection = QCheckBox(self.tr("Intersection"), groupBoxQSnapLoc)
        self.dialog_qsnap_intersection = self.mainWin.getSettingsQSnapIntersection()
        checkBoxQSnapIntersection.setChecked(self.dialog_qsnap_intersection)
        checkBoxQSnapIntersection.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptointersection" + ".png"))
        self.connect(checkBoxQSnapIntersection, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapIntersectionStateChanged(int)"))

        checkBoxQSnapExtension = QCheckBox(self.tr("Extension"), groupBoxQSnapLoc)
        self.dialog_qsnap_extension = self.mainWin.getSettingsQSnapExtension()
        checkBoxQSnapExtension.setChecked(self.dialog_qsnap_extension)
        checkBoxQSnapExtension.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoextension" + ".png"))
        self.connect(checkBoxQSnapExtension, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapExtensionStateChanged(int)"))

        checkBoxQSnapInsertion = QCheckBox(self.tr("Insertion"), groupBoxQSnapLoc)
        self.dialog_qsnap_insertion = self.mainWin.getSettingsQSnapInsertion()
        checkBoxQSnapInsertion.setChecked(self.dialog_qsnap_insertion)
        checkBoxQSnapInsertion.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoinsert" + ".png"))
        self.connect(checkBoxQSnapInsertion, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapInsertionStateChanged(int)"))

        checkBoxQSnapPerpendicular = QCheckBox(self.tr("Perpendicular"), groupBoxQSnapLoc)
        self.dialog_qsnap_perpendicular = self.mainWin.getSettingsQSnapPerpendicular()
        checkBoxQSnapPerpendicular.setChecked(self.dialog_qsnap_perpendicular)
        checkBoxQSnapPerpendicular.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoperpendicular" + ".png"))
        self.connect(checkBoxQSnapPerpendicular, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapPerpendicularStateChanged(int)"))

        checkBoxQSnapTangent = QCheckBox(self.tr("Tangent"), groupBoxQSnapLoc)
        self.dialog_qsnap_tangent = self.mainWin.getSettingsQSnapTangent()
        checkBoxQSnapTangent.setChecked(self.dialog_qsnap_tangent)
        checkBoxQSnapTangent.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptotangent" + ".png"))
        self.connect(checkBoxQSnapTangent, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapTangentStateChanged(int)"))

        checkBoxQSnapNearest = QCheckBox(self.tr("Nearest"), groupBoxQSnapLoc)
        self.dialog_qsnap_nearest = self.mainWin.getSettingsQSnapNearest()
        checkBoxQSnapNearest.setChecked(self.dialog_qsnap_nearest)
        checkBoxQSnapNearest.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptonearest" + ".png"))
        self.connect(checkBoxQSnapNearest, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapNearestStateChanged(int)"))

        checkBoxQSnapApparent = QCheckBox(self.tr("Apparent Intersection"), groupBoxQSnapLoc)
        self.dialog_qsnap_apparent = self.mainWin.getSettingsQSnapApparent()
        checkBoxQSnapApparent.setChecked(self.dialog_qsnap_apparent)
        checkBoxQSnapApparent.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoapparentintersection" + ".png"))
        self.connect(checkBoxQSnapApparent, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapApparentStateChanged(int)"))

        checkBoxQSnapParallel = QCheckBox(self.tr("Parallel"), groupBoxQSnapLoc)
        self.dialog_qsnap_parallel = self.mainWin.getSettingsQSnapParallel()
        checkBoxQSnapParallel.setChecked(self.dialog_qsnap_parallel)
        checkBoxQSnapParallel.setIcon(QIcon("icons/" + iconTheme + "/" + "locator-snaptoparallel" + ".png"))
        self.connect(checkBoxQSnapParallel, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxQSnapParallelStateChanged(int)"))

        buttonQSnapSelectAll = QPushButton(self.tr("Select All"), groupBoxQSnapLoc)
        self.connect(buttonQSnapSelectAll, SIGNAL("clicked()"), self, SLOT("buttonQSnapSelectAllClicked()"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapEndPoint,      SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapMidPoint,      SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapCenter,        SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapNode,          SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapQuadrant,      SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapIntersection,  SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapExtension,     SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapInsertion,     SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapPerpendicular, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapTangent,       SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapNearest,       SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapApparent,      SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapSelectAll(bool)"), checkBoxQSnapParallel,      SLOT("setChecked(bool)"))

        buttonQSnapClearAll = QPushButton(self.tr("Clear All"), groupBoxQSnapLoc)
        self.connect(buttonQSnapClearAll, SIGNAL("clicked()"), self, SLOT("buttonQSnapClearAllClicked()"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapEndPoint,      SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapMidPoint,      SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapCenter,        SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapNode,          SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapQuadrant,      SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapIntersection,  SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapExtension,     SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapInsertion,     SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapPerpendicular, SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapTangent,       SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapNearest,       SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapApparent,      SLOT("setChecked(bool)"))
        self.connect(self, SIGNAL("buttonQSnapClearAll(bool)"), checkBoxQSnapParallel,      SLOT("setChecked(bool)"))

        gridLayoutQSnap = QGridLayout(groupBoxQSnapLoc)
        gridLayoutQSnap.addWidget(checkBoxQSnapEndPoint,      0, 0, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapMidPoint,      1, 0, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapCenter,        2, 0, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapNode,          3, 0, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapQuadrant,      4, 0, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapIntersection,  5, 0, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapExtension,     6, 0, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapInsertion,     0, 1, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapPerpendicular, 1, 1, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapTangent,       2, 1, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapNearest,       3, 1, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapApparent,      4, 1, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(checkBoxQSnapParallel,      5, 1, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(buttonQSnapSelectAll,       0, 2, Qt.AlignLeft)
        gridLayoutQSnap.addWidget(buttonQSnapClearAll,        1, 2, Qt.AlignLeft)
        gridLayoutQSnap.setColumnStretch(2,1)
        groupBoxQSnapLoc.setLayout(gridLayoutQSnap)

        # QSnap Visual Config
        groupBoxQSnapVisual = QGroupBox(self.tr("Visual Configuration"), widget)

        labelQSnapLocColor = QLabel(self.tr("Locator Color"), groupBoxQSnapVisual)
        comboBoxQSnapLocColor = QComboBox(groupBoxQSnapVisual)
        #TODO# self.addColorsToComboBox(comboBoxQSnapLocColor)
        self.dialog_qsnap_locator_color = self.mainWin.getSettingsQSnapLocatorColor()
        comboBoxQSnapLocColor.setCurrentIndex(comboBoxQSnapLocColor.findData(self.dialog_qsnap_locator_color))
        self.connect(comboBoxQSnapLocColor, SIGNAL("currentIndexChanged(int)"), self, SLOT("comboBoxQSnapLocatorColorCurrentIndexChanged(int)"))

        labelQSnapLocSize = QLabel(self.tr("Locator Size"), groupBoxQSnapVisual)
        sliderQSnapLocSize = QSlider(Qt.Horizontal, groupBoxQSnapVisual)
        sliderQSnapLocSize.setRange(1, 20)
        self.dialog_qsnap_locator_size = self.mainWin.getSettingsQSnapLocatorSize()
        sliderQSnapLocSize.setValue(self.dialog_qsnap_locator_size)
        self.connect(sliderQSnapLocSize, SIGNAL("valueChanged(int)"), self, SLOT("sliderQSnapLocatorSizeValueChanged(int)"))

        vboxLayoutQSnapVisual = QVBoxLayout(groupBoxQSnapVisual)
        vboxLayoutQSnapVisual.addWidget(labelQSnapLocColor)
        vboxLayoutQSnapVisual.addWidget(comboBoxQSnapLocColor)
        vboxLayoutQSnapVisual.addWidget(labelQSnapLocSize)
        vboxLayoutQSnapVisual.addWidget(sliderQSnapLocSize)
        groupBoxQSnapVisual.setLayout(vboxLayoutQSnapVisual)

        # QSnap Sensitivity Config
        groupBoxQSnapSensitivity = QGroupBox(self.tr("Sensitivity"), widget)

        labelQSnapApertureSize = QLabel(self.tr("Aperture Size"), groupBoxQSnapSensitivity)
        sliderQSnapApertureSize = QSlider(Qt.Horizontal, groupBoxQSnapSensitivity)
        sliderQSnapApertureSize.setRange(1,20)
        self.dialog_qsnap_aperture_size = self.mainWin.getSettingsQSnapApertureSize()
        sliderQSnapApertureSize.setValue(self.dialog_qsnap_aperture_size)
        self.connect(sliderQSnapApertureSize, SIGNAL("valueChanged(int)"), self, SLOT("sliderQSnapApertureSizeValueChanged(int)"))

        vboxLayoutQSnapSensitivity = QVBoxLayout(groupBoxQSnapSensitivity)
        vboxLayoutQSnapSensitivity.addWidget(labelQSnapApertureSize)
        vboxLayoutQSnapSensitivity.addWidget(sliderQSnapApertureSize)
        groupBoxQSnapSensitivity.setLayout(vboxLayoutQSnapSensitivity)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxQSnapLoc)
        vboxLayoutMain.addWidget(groupBoxQSnapVisual)
        vboxLayoutMain.addWidget(groupBoxQSnapSensitivity)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabQuickTrack(self):
        """TOWRITE"""
        widget = QWidget(self)

        # TODO: finish self

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea

    def createTabLineWeight(self):
        """TOWRITE"""
        widget = QWidget(self)

        # TODO: finish self

        # Misc
        groupBoxLwtMisc = QGroupBox(self.tr("LineWeight Misc"), widget)

        s = None #TODO/FIXME#
        #s = self.mainWin.activeScene() # QGraphicsScene*

        checkBoxShowLwt = QCheckBox(self.tr("Show LineWeight"), groupBoxLwtMisc)
        if s:
            self.dialog_lwt_show_lwt = s.property(ENABLE_LWT)  # .toBool()
        else:
            self.dialog_lwt_show_lwt = self.mainWin.getSettingsLwtShowLwt()
        self.preview_lwt_show_lwt = self.dialog_lwt_show_lwt
        checkBoxShowLwt.setChecked(self.preview_lwt_show_lwt)
        self.connect(checkBoxShowLwt, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxLwtShowLwtStateChanged(int)"))

        checkBoxRealRender = QCheckBox(self.tr("RealRender"), groupBoxLwtMisc)
        checkBoxRealRender.setObjectName("checkBoxRealRender")
        if s:
            self.dialog_lwt_real_render = s.property(ENABLE_REAL)  # .toBool()
        else:
            self.dialog_lwt_real_render = self.mainWin.getSettingsLwtRealRender()
        self.preview_lwt_real_render = self.dialog_lwt_real_render
        checkBoxRealRender.setChecked(self.preview_lwt_real_render)
        self.connect(checkBoxRealRender, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxLwtRealRenderStateChanged(int)"))
        checkBoxRealRender.setEnabled(self.dialog_lwt_show_lwt)

        labelDefaultLwt = QLabel(self.tr("Default weight"), groupBoxLwtMisc)
        labelDefaultLwt.setEnabled(False) # TODO: remove later
        comboBoxDefaultLwt = QComboBox(groupBoxLwtMisc)
        self.dialog_lwt_default_lwt = self.mainWin.getSettingsLwtDefaultLwt()
        # TODO: populate the comboBox and set the initial value
        #TODO# comboBoxDefaultLwt.addItem(QString().setNum(self.dialog_lwt_default_lwt, 'F', 2).append(" mm"), self.dialog_lwt_default_lwt)
        comboBoxDefaultLwt.setEnabled(False) # TODO: remove later

        vboxLayoutLwtMisc = QVBoxLayout(groupBoxLwtMisc)
        vboxLayoutLwtMisc.addWidget(checkBoxShowLwt)
        vboxLayoutLwtMisc.addWidget(checkBoxRealRender)
        vboxLayoutLwtMisc.addWidget(labelDefaultLwt)
        vboxLayoutLwtMisc.addWidget(comboBoxDefaultLwt)
        groupBoxLwtMisc.setLayout(vboxLayoutLwtMisc)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxLwtMisc)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea


    def createTabSelection(self):
        """TOWRITE"""
        widget = QWidget(self)

        # Selection Modes
        groupBoxSelectionModes = QGroupBox(self.tr("Modes"), widget)

        checkBoxSelectionModePickFirst = QCheckBox(self.tr("Allow Preselection (PickFirst)"), groupBoxSelectionModes)
        self.dialog_selection_mode_pickfirst = self.mainWin.getSettingsSelectionModePickFirst()
        checkBoxSelectionModePickFirst.setChecked(self.dialog_selection_mode_pickfirst)
        checkBoxSelectionModePickFirst.setChecked(True)
        checkBoxSelectionModePickFirst.setEnabled(False) # TODO: Remove self line when Post-selection is available
        self.connect(checkBoxSelectionModePickFirst, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxSelectionModePickFirstStateChanged(int)"))

        checkBoxSelectionModePickAdd = QCheckBox(self.tr("Add to Selection (PickAdd)"), groupBoxSelectionModes)
        self.dialog_selection_mode_pickadd = self.mainWin.getSettingsSelectionModePickAdd()
        checkBoxSelectionModePickAdd.setChecked(self.dialog_selection_mode_pickadd)
        self.connect(checkBoxSelectionModePickAdd, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxSelectionModePickAddStateChanged(int)"))

        checkBoxSelectionModePickDrag = QCheckBox(self.tr("Drag to Select (PickDrag)"), groupBoxSelectionModes)
        self.dialog_selection_mode_pickdrag = self.mainWin.getSettingsSelectionModePickDrag()
        checkBoxSelectionModePickDrag.setChecked(self.dialog_selection_mode_pickdrag)
        checkBoxSelectionModePickDrag.setChecked(False)
        checkBoxSelectionModePickDrag.setEnabled(False) # TODO: Remove self line when self functionality is available
        self.connect(checkBoxSelectionModePickDrag, SIGNAL("stateChanged(int)"), self, SLOT("checkBoxSelectionModePickDragStateChanged(int)"))

        vboxLayoutSelectionModes = QVBoxLayout(groupBoxSelectionModes)
        vboxLayoutSelectionModes.addWidget(checkBoxSelectionModePickFirst)
        vboxLayoutSelectionModes.addWidget(checkBoxSelectionModePickAdd)
        vboxLayoutSelectionModes.addWidget(checkBoxSelectionModePickDrag)
        groupBoxSelectionModes.setLayout(vboxLayoutSelectionModes)

        # Selection Colors
        groupBoxSelectionColors = QGroupBox(self.tr("Colors"), widget)

        labelCoolGripColor = QLabel(self.tr("Cool Grip (Unselected)"), groupBoxSelectionColors)
        comboBoxCoolGripColor = QComboBox(groupBoxSelectionColors)
        self.addColorsToComboBox(comboBoxCoolGripColor)
        self.dialog_selection_coolgrip_color = self.mainWin.getSettingsSelectionCoolGripColor()
        comboBoxCoolGripColor.setCurrentIndex(comboBoxCoolGripColor.findData(self.dialog_selection_coolgrip_color))
        self.connect(comboBoxCoolGripColor, SIGNAL("currentIndexChanged(int)"), self, SLOT("comboBoxSelectionCoolGripColorCurrentIndexChanged(int)"))

        labelHotGripColor = QLabel(self.tr("Hot Grip (Selected)"), groupBoxSelectionColors)
        comboBoxHotGripColor = QComboBox(groupBoxSelectionColors)
        self.addColorsToComboBox(comboBoxHotGripColor)
        self.dialog_selection_hotgrip_color = self.mainWin.getSettingsSelectionHotGripColor()
        comboBoxHotGripColor.setCurrentIndex(comboBoxHotGripColor.findData(self.dialog_selection_hotgrip_color))
        self.connect(comboBoxHotGripColor, SIGNAL("currentIndexChanged(int)"), self, SLOT("comboBoxSelectionHotGripColorCurrentIndexChanged(int)"))

        vboxLayoutSelectionColors = QVBoxLayout(groupBoxSelectionColors)
        vboxLayoutSelectionColors.addWidget(labelCoolGripColor)
        vboxLayoutSelectionColors.addWidget(comboBoxCoolGripColor)
        vboxLayoutSelectionColors.addWidget(labelHotGripColor)
        vboxLayoutSelectionColors.addWidget(comboBoxHotGripColor)
        groupBoxSelectionColors.setLayout(vboxLayoutSelectionColors)

        # Selection Sizes
        groupBoxSelectionSizes = QGroupBox(self.tr("Sizes"), widget)

        labelSelectionGripSize = QLabel(self.tr("Grip Size"), groupBoxSelectionSizes)
        sliderSelectionGripSize = QSlider(Qt.Horizontal, groupBoxSelectionSizes)
        sliderSelectionGripSize.setRange(1,20)
        self.dialog_selection_grip_size = self.mainWin.getSettingsSelectionGripSize()
        sliderSelectionGripSize.setValue(self.dialog_selection_grip_size)
        self.connect(sliderSelectionGripSize, SIGNAL("valueChanged(int)"), self, SLOT("sliderSelectionGripSizeValueChanged(int)"))

        labelSelectionPickBoxSize = QLabel(self.tr("Pickbox Size"), groupBoxSelectionSizes)
        sliderSelectionPickBoxSize = QSlider(Qt.Horizontal, groupBoxSelectionSizes)
        sliderSelectionPickBoxSize.setRange(1,20)
        self.dialog_selection_pickbox_size = self.mainWin.getSettingsSelectionPickBoxSize()
        sliderSelectionPickBoxSize.setValue(self.dialog_selection_pickbox_size)
        self.connect(sliderSelectionPickBoxSize, SIGNAL("valueChanged(int)"), self, SLOT("sliderSelectionPickBoxSizeValueChanged(int)"))

        vboxLayoutSelectionSizes = QVBoxLayout(groupBoxSelectionSizes)
        vboxLayoutSelectionSizes.addWidget(labelSelectionGripSize)
        vboxLayoutSelectionSizes.addWidget(sliderSelectionGripSize)
        vboxLayoutSelectionSizes.addWidget(labelSelectionPickBoxSize)
        vboxLayoutSelectionSizes.addWidget(sliderSelectionPickBoxSize)
        groupBoxSelectionSizes.setLayout(vboxLayoutSelectionSizes)

        # Widget Layout
        vboxLayoutMain = QVBoxLayout(widget)
        vboxLayoutMain.addWidget(groupBoxSelectionModes)
        vboxLayoutMain.addWidget(groupBoxSelectionColors)
        vboxLayoutMain.addWidget(groupBoxSelectionSizes)
        vboxLayoutMain.addStretch(1)
        widget.setLayout(vboxLayoutMain)

        scrollArea = QScrollArea(self)
        scrollArea.setWidgetResizable(True)
        scrollArea.setWidget(widget)
        return scrollArea

    def addColorsToComboBox(self, comboBox):
        """
        TOWRITE

        :param `comboBox`: TOWRITE
        :type `comboBox`: `QComboBox`_
        """
        iconTheme = self.mainWin.getSettingsGeneralIconTheme() # QString
        iconDir = self.mainWin.gIconDir
        comboBox.addItem(QIcon(iconDir + gOsSep + iconTheme + gOsSep + "colorred" + ".png"),     self.tr("Red"),     qRgb(255,  0,  0))
        comboBox.addItem(QIcon(iconDir + gOsSep + iconTheme + gOsSep + "coloryellow" + ".png"),  self.tr("Yellow"),  qRgb(255,255,  0))
        comboBox.addItem(QIcon(iconDir + gOsSep + iconTheme + gOsSep + "colorgreen" + ".png"),   self.tr("Green"),   qRgb(  0,255,  0))
        comboBox.addItem(QIcon(iconDir + gOsSep + iconTheme + gOsSep + "colorcyan" + ".png"),    self.tr("Cyan"),    qRgb(  0,255,255))
        comboBox.addItem(QIcon(iconDir + gOsSep + iconTheme + gOsSep + "colorblue" + ".png"),    self.tr("Blue"),    qRgb(  0,  0,255))
        comboBox.addItem(QIcon(iconDir + gOsSep + iconTheme + gOsSep + "colormagenta" + ".png"), self.tr("Magenta"), qRgb(255,  0,255))
        comboBox.addItem(QIcon(iconDir + gOsSep + iconTheme + gOsSep + "colorwhite" + ".png"),   self.tr("White"),   qRgb(255,255,255))
        # TODO: Add Other... so the user can select custom colors

    # Slots ------------------------------------------------------------------

    @Slot(str)
    def comboBoxLanguageCurrentIndexChanged(self, lang):
        """
        TOWRITE

        :param `lang`: TOWRITE
        :type `lang`: QString
        """
        self.dialog_general_language = lang.lower()  #TODO# What to do with this?
        print('self.dialog_general_language = %s' % self.dialog_general_language)

    @Slot(str)
    def comboBoxIconThemeCurrentIndexChanged(self, theme):
        """
        TOWRITE

        :param `theme`: TOWRITE
        :type `theme`: QString
        """
        self.dialog_general_icon_theme = theme  #TODO# What to do with this?
        print('self.dialog_general_icon_theme = %s' % self.dialog_general_icon_theme)

    @Slot(int)
    def comboBoxIconSizeCurrentIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        comboBox = self.sender()  # QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
        if comboBox:

            ok = 0
            self.dialog_general_icon_size = int(comboBox.itemData(index))  # .toUInt(& ok)
            if not ok:
                self.dialog_general_icon_size = 16

        else:
            self.dialog_general_icon_size = 16

    @Slot(int)
    def checkBoxGeneralMdiBGUseLogoStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.preview_general_mdi_bg_use_logo = checked
        self.mainWin.mdiArea.useBackgroundLogo(checked)

    @Slot()
    def chooseGeneralMdiBackgroundLogo(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender());
        if button:

            try: # Qt5 # if QT_VERSION >= 0x050000
                picsLoc = QStandardPaths.writableLocation(QStandardPaths.PicturesLocation)
            except NameError: # Qt4
                picsLoc = QDesktopServices.storageLocation(QDesktopServices.PicturesLocation)
            selectedImage = '' # QString
            selectedImage = QFileDialog.getOpenFileName(self, self.tr("Open File"),
                                    picsLoc, self.tr("Images (*.bmp *.png *.jpg)"))

            if selectedImage:
                self.accept_general_mdi_bg_logo = selectedImage

            # Update immediately so it can be previewed
            self.mainWin.mdiArea.setBackgroundLogo(self.accept_general_mdi_bg_logo)

    @Slot(int)
    def checkBoxGeneralMdiBGUseTextureStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.preview_general_mdi_bg_use_texture = checked
        self.mainWin.mdiArea.useBackgroundTexture(checked)

    @Slot()
    def chooseGeneralMdiBackgroundTexture(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender());
        if button:

            try: # Qt5 # if QT_VERSION >= 0x050000
                picsLoc = QStandardPaths.writableLocation(QStandardPaths.PicturesLocation)
            except NameError: # Qt4
                picsLoc = QDesktopServices.storageLocation(QDesktopServices.PicturesLocation)
            selectedImage = '' # QString
            selectedImage = QFileDialog.getOpenFileName(self, self.tr("Open File"),
                                    picsLoc, self.tr("Images (*.bmp *.png *.jpg)"))
            # print('selectedImage', selectedImage)
            if selectedImage: # .isNull():
                self.accept_general_mdi_bg_texture = selectedImage[0]

            # Update immediately so it can be previewed.
            self.mainWin.mdiArea.setBackgroundTexture(self.accept_general_mdi_bg_texture)

    @Slot(int)
    def checkBoxGeneralMdiBGUseColorStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.preview_general_mdi_bg_use_color = checked
        self.mainWin.mdiArea.useBackgroundColor(checked)

    @Slot()
    def chooseGeneralMdiBackgroundColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender());
        if button:
            colorDialog = QColorDialog(QColor(self.accept_general_mdi_bg_color), self)
            colorDialog.currentColorChanged.connect(self.currentGeneralMdiBackgroundColorChanged) # (const QColor&)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_general_mdi_bg_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_general_mdi_bg_color))
                button.setIcon(QIcon(pix))
                self.mainWin.mdiArea.setBackgroundColor(QColor(self.accept_general_mdi_bg_color))

            else:
                self.mainWin.mdiArea.setBackgroundColor(QColor(self.dialog_general_mdi_bg_color))

    @Slot(QColor)
    def currentGeneralMdiBackgroundColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.preview_general_mdi_bg_color = color.rgb()
        self.mainWin.mdiArea.setBackgroundColor(QColor(self.preview_general_mdi_bg_color))

    @Slot(int)
    def checkBoxTipOfTheDayStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_general_tip_of_the_day = checked

    @Slot(int)
    def checkBoxUseOpenGLStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_display_use_opengl = checked

    @Slot(int)
    def checkBoxRenderHintAAStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_display_renderhint_aa = checked

    @Slot(int)
    def checkBoxRenderHintTextAAStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_display_renderhint_text_aa = checked

    @Slot(int)
    def checkBoxRenderHintSmoothPixStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_display_renderhint_smooth_pix = checked

    @Slot(int)
    def checkBoxRenderHintHighAAStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_display_renderhint_high_aa = checked

    @Slot(int)
    def checkBoxRenderHintNonCosmeticStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_display_renderhint_noncosmetic = checked

    @Slot(int)
    def checkBoxShowScrollBarsStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.preview_display_show_scrollbars = checked
        self.mainWin.updateAllViewScrollBars(self.preview_display_show_scrollbars)

    @Slot(int)
    def comboBoxScrollBarWidgetCurrentIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        self.dialog_display_scrollbar_widget_num = index

    @Slot(float)
    def spinBoxZoomScaleInValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_display_zoomscale_in = value


    @Slot(float)
    def spinBoxZoomScaleOutValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_display_zoomscale_out = value

    @Slot(int)
    def checkBoxDisableBGStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_printing_disable_bg = checked

    @Slot()
    def chooseDisplayCrossHairColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender());
        if button:
            colorDialog = QColorDialog(QColor(self.accept_display_crosshair_color), self)
            colorDialog.currentColorChanged.connect(self.currentDisplayCrossHairColorChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_display_crosshair_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_display_crosshair_color))
                button.setIcon(QIcon(pix))
                self.mainWin.updateAllViewCrossHairColors(self.accept_display_crosshair_color)

            else:
                self.mainWin.updateAllViewCrossHairColors(self.dialog_display_crosshair_color)

    @Slot(QColor)
    def currentDisplayCrossHairColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        preview_display_crosshair_color = color.rgb()
        self.mainWin.updateAllViewCrossHairColors(preview_display_crosshair_color)

    @Slot()
    def chooseDisplayBackgroundColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender())
        if button:
            colorDialog = QColorDialog(QColor(self.accept_display_bg_color), self)
            colorDialog.currentColorChanged.connect(self.currentDisplayBackgroundColorChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_display_bg_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_display_bg_color))
                button.setIcon(QIcon(pix))
                self.mainWin.updateAllViewBackgroundColors(self.accept_display_bg_color)

            else:
                self.mainWin.updateAllViewBackgroundColors(self.dialog_display_bg_color)

    @Slot(QColor)
    def currentDisplayBackgroundColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.preview_display_bg_color = color.rgb()
        self.mainWin.updateAllViewBackgroundColors(self.preview_display_bg_color)

    @Slot()
    def chooseDisplaySelectBoxLeftColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender())
        if button:
            colorDialog = QColorDialog(QColor(self.accept_display_selectbox_left_color), self)
            colorDialog.currentColorChanged.connect(self.currentDisplaySelectBoxLeftColorChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_display_selectbox_left_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_display_selectbox_left_color))
                button.setIcon(QIcon(pix))
                self.mainWin.updateAllViewSelectBoxColors(self.accept_display_selectbox_left_color,
                                                          self.accept_display_selectbox_left_fill,
                                                          self.accept_display_selectbox_right_color,
                                                          self.accept_display_selectbox_right_fill,
                                                          self.preview_display_selectbox_alpha)

            else:
                self.mainWin.updateAllViewSelectBoxColors(self.dialog_display_selectbox_left_color,
                                                          self.dialog_display_selectbox_left_fill,
                                                          self.dialog_display_selectbox_right_color,
                                                          self.dialog_display_selectbox_right_fill,
                                                          self.preview_display_selectbox_alpha)

    @Slot(QColor)
    def currentDisplaySelectBoxLeftColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.preview_display_selectbox_left_color = color.rgb()
        self.mainWin.updateAllViewSelectBoxColors(self.preview_display_selectbox_left_color,
                                                  self.preview_display_selectbox_left_fill,
                                                  self.preview_display_selectbox_right_color,
                                                  self.preview_display_selectbox_right_fill,
                                                  self.preview_display_selectbox_alpha)

    @Slot()
    def chooseDisplaySelectBoxLeftFill(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender())
        if button:

            colorDialog = QColorDialog(QColor(self.accept_display_selectbox_left_fill), self)
            colorDialog.currentColorChanged.connect(self.currentDisplaySelectBoxLeftFillChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_display_selectbox_left_fill = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_display_selectbox_left_fill))
                button.setIcon(QIcon(pix))
                self.mainWin.updateAllViewSelectBoxColors(self.accept_display_selectbox_left_color,
                                                          self.accept_display_selectbox_left_fill,
                                                          self.accept_display_selectbox_right_color,
                                                          self.accept_display_selectbox_right_fill,
                                                          self.preview_display_selectbox_alpha)

            else:
                self.mainWin.updateAllViewSelectBoxColors(self.dialog_display_selectbox_left_color,
                                                          self.dialog_display_selectbox_left_fill,
                                                          self.dialog_display_selectbox_right_color,
                                                          self.dialog_display_selectbox_right_fill,
                                                          self.preview_display_selectbox_alpha)

    @Slot(QColor)
    def currentDisplaySelectBoxLeftFillChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.preview_display_selectbox_left_fill = color.rgb()
        self.mainWin.updateAllViewSelectBoxColors(self.preview_display_selectbox_left_color,
                                                  self.preview_display_selectbox_left_fill,
                                                  self.preview_display_selectbox_right_color,
                                                  self.preview_display_selectbox_right_fill,
                                                  self.preview_display_selectbox_alpha)

    @Slot()
    def chooseDisplaySelectBoxRightColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender())
        if button:
            colorDialog = QColorDialog(QColor(self.accept_display_selectbox_right_color), self)
            colorDialog.currentColorChanged.connect(self.currentDisplaySelectBoxRightColorChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_display_selectbox_right_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_display_selectbox_right_color))
                button.setIcon(QIcon(pix))
                self.mainWin.updateAllViewSelectBoxColors(self.accept_display_selectbox_left_color,
                                                          self.accept_display_selectbox_left_fill,
                                                          self.accept_display_selectbox_right_color,
                                                          self.accept_display_selectbox_right_fill,
                                                          self.preview_display_selectbox_alpha)

            else:
                self.mainWin.updateAllViewSelectBoxColors(self.dialog_display_selectbox_left_color,
                                                          self.dialog_display_selectbox_left_fill,
                                                          self.dialog_display_selectbox_right_color,
                                                          self.dialog_display_selectbox_right_fill,
                                                          self.preview_display_selectbox_alpha)

    @Slot(QColor)
    def currentDisplaySelectBoxRightColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.preview_display_selectbox_right_color = color.rgb()
        self.mainWin.updateAllViewSelectBoxColors(self.preview_display_selectbox_left_color,
                                                  self.preview_display_selectbox_left_fill,
                                                  self.preview_display_selectbox_right_color,
                                                  self.preview_display_selectbox_right_fill,
                                                  self.preview_display_selectbox_alpha)

    @Slot()
    def chooseDisplaySelectBoxRightFill(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender())
        if button:
            colorDialog = QColorDialog(QColor(self.accept_display_selectbox_right_fill), self)
            colorDialog.currentColorChanged.connect(self.currentDisplaySelectBoxRightFillChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_display_selectbox_right_fill = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_display_selectbox_right_fill))
                button.setIcon(QIcon(pix))
                self.mainWin.updateAllViewSelectBoxColors(self.accept_display_selectbox_left_color,
                                                          self.accept_display_selectbox_left_fill,
                                                          self.accept_display_selectbox_right_color,
                                                          self.accept_display_selectbox_right_fill,
                                                          self.preview_display_selectbox_alpha)

            else:
                self.mainWin.updateAllViewSelectBoxColors(self.dialog_display_selectbox_left_color,
                                                          self.dialog_display_selectbox_left_fill,
                                                          self.dialog_display_selectbox_right_color,
                                                          self.dialog_display_selectbox_right_fill,
                                                          self.preview_display_selectbox_alpha)

    @Slot(QColor)
    def currentDisplaySelectBoxRightFillChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.preview_display_selectbox_right_fill = color.rgb()
        self.mainWin.updateAllViewSelectBoxColors(self.preview_display_selectbox_left_color,
                                                  self.preview_display_selectbox_left_fill,
                                                  self.preview_display_selectbox_right_color,
                                                  self.preview_display_selectbox_right_fill,
                                                  self.preview_display_selectbox_alpha)

    @Slot(int)
    def spinBoxDisplaySelectBoxAlphaValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: int
        """
        self.preview_display_selectbox_alpha = value
        self.mainWin.updateAllViewSelectBoxColors(self.accept_display_selectbox_left_color,
                                                  self.accept_display_selectbox_left_fill,
                                                  self.accept_display_selectbox_right_color,
                                                  self.accept_display_selectbox_right_fill,
                                                  self.preview_display_selectbox_alpha)

    @Slot()
    def choosePromptTextColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender());
        if button:

            colorDialog = QColorDialog(QColor(self.accept_prompt_text_color), self)
            colorDialog.currentColorChanged.connect(self.currentPromptTextColorChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_prompt_text_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_prompt_text_color))
                button.setIcon(QIcon(pix))
                self.mainWin.prompt.setPromptTextColor(QColor(self.accept_prompt_text_color))

            else:
                self.mainWin.prompt.setPromptTextColor(QColor(self.dialog_prompt_text_color))

    @Slot(QColor)
    def currentPromptTextColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.preview_prompt_text_color = color.rgb()
        self.mainWin.prompt.setPromptTextColor(QColor(self.preview_prompt_text_color))

    @Slot()
    def choosePromptBackgroundColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender())
        if button:
            colorDialog = QColorDialog(QColor(self.accept_prompt_bg_color), self)
            colorDialog.currentColorChanged.connect(self.currentPromptBackgroundColorChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:
                self.accept_prompt_bg_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_prompt_bg_color))
                button.setIcon(QIcon(pix))
                self.mainWin.prompt.setPromptBackgroundColor(QColor(self.accept_prompt_bg_color))

            else:
                self.mainWin.prompt.setPromptBackgroundColor(QColor(self.dialog_prompt_bg_color))

    @Slot(QColor)
    def currentPromptBackgroundColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        self.preview_prompt_bg_color = color.rgb()
        self.mainWin.prompt.setPromptBackgroundColor(QColor(self.preview_prompt_bg_color))

    @Slot(str)
    def comboBoxPromptFontFamilyCurrentIndexChanged(self, family):
        """
        TOWRITE

        :param `family`: TOWRITE
        :type `family`: QString
        """
        self.preview_prompt_font_family = family
        self.mainWin.prompt.setPromptFontFamily(self.preview_prompt_font_family)

    @Slot(str)
    def comboBoxPromptFontStyleCurrentIndexChanged(self, style):
        """
        TOWRITE

        :param `style`: TOWRITE
        :type `style`: QString
        """
        self.preview_prompt_font_style = style
        self.mainWin.prompt.setPromptFontStyle(self.preview_prompt_font_style)

    @Slot(int)
    def spinBoxPromptFontSizeValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: int
        """
        self.preview_prompt_font_size = value
        self.mainWin.prompt.setPromptFontSize(self.preview_prompt_font_size)

    @Slot(int)
    def checkBoxPromptSaveHistoryStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_prompt_save_history = checked

    @Slot(int)
    def checkBoxPromptSaveHistoryAsHtmlStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_prompt_save_history_as_html = checked

    @Slot(int)
    def checkBoxCustomFilterStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        checkBox = self.sender() # QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender())
        if checkBox:

            format = checkBox.text() # QString
            qDebug("CustomFilter: %s %d" %(format, checked))
            if checked:
                self.dialog_opensave_custom_filter.append(" *." + format.lower())
            else:
                self.dialog_opensave_custom_filter.remove("*." + format, Qt.CaseInsensitive)
            # self.dialog_opensave_custom_filter = checked # TODO

    @Slot()
    def buttonCustomFilterSelectAllClicked(self):
        """TOWRITE"""
        #TODO# emit buttonCustomFilterSelectAll(True)
        self.dialog_opensave_custom_filter = "supported"

    @Slot()
    def buttonCustomFilterClearAllClicked(self):
        """TOWRITE"""
        #TODO# emit buttonCustomFilterClearAll(False)
        self.dialog_opensave_custom_filter.clear()

    @Slot(int)
    def spinBoxRecentMaxFilesValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: int
        """
        self.dialog_opensave_recent_max_files = value

    @Slot(int)
    def spinBoxTrimDstNumJumpsValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: int
        """
        self.dialog_opensave_trim_dst_num_jumps = value

    @Slot(int)
    def checkBoxGridShowOnLoadStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_grid_show_on_load = checked

    @Slot(int)
    def checkBoxGridShowOriginStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_grid_show_origin = checked

    @Slot(int)
    def checkBoxGridColorMatchCrossHairStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_grid_color_match_crosshair = checked
        if self.dialog_grid_color_match_crosshair:
            self.mainWin.updateAllViewGridColors(self.accept_display_crosshair_color)
        else:
            self.mainWin.updateAllViewGridColors(self.accept_grid_color)

        senderObj = self.sender() # QObject*
        if senderObj:

            parent = senderObj.parent() # QObject*
            if parent:

                #QLabel* labelGridColor = parent.findChild<QLabel*>("labelGridColor")
                labelGridColor = parent.findChild(QLabel, "labelGridColor")
                if labelGridColor:
                    labelGridColor.setEnabled(not self.dialog_grid_color_match_crosshair)
                #QPushButton* buttonGridColor = parent.findChild<QPushButton*>("buttonGridColor")
                buttonGridColor = parent.findChild(QPushButton, "buttonGridColor")
                if buttonGridColor:
                    buttonGridColor.setEnabled(not self.dialog_grid_color_match_crosshair)

    @Slot()
    def chooseGridColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender())
        if button:

            colorDialog = QColorDialog(QColor(self.accept_grid_color), self)
            colorDialog.currentColorChanged.connect(self.currentGridColorChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:

                self.accept_grid_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_grid_color))
                button.setIcon(QIcon(pix))
                self.mainWin.updateAllViewGridColors(self.accept_grid_color)

            else:

                self.mainWin.updateAllViewGridColors(self.dialog_grid_color)

    @Slot(QColor)
    def currentGridColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        preview_grid_color = color.rgb()
        self.mainWin.updateAllViewGridColors(preview_grid_color)

    @Slot(int)
    def checkBoxGridLoadFromFileStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_grid_load_from_file = checked

        senderObj = self.sender() # QObject*
        if senderObj:

            parent = senderObj.parent() # QObject*
            if parent:

                labelGridType = parent.findChild(QLabel, "labelGridType")
                if labelGridType: labelGridType.setEnabled(not self.dialog_grid_load_from_file)
                comboBoxGridType = parent.findChild(QComboBox, "comboBoxGridType")
                if comboBoxGridType: comboBoxGridType.setEnabled(not self.dialog_grid_load_from_file)
                checkBoxGridCenterOnOrigin = parent.findChild(QCheckBox, "checkBoxGridCenterOnOrigin")
                if checkBoxGridCenterOnOrigin: checkBoxGridCenterOnOrigin.setEnabled(not self.dialog_grid_load_from_file)
                labelGridCenterX = parent.findChild(QLabel, "labelGridCenterX")
                if labelGridCenterX:
                    labelGridCenterX.setEnabled(not self.dialog_grid_load_from_file and not self.dialog_grid_center_on_origin)
                spinBoxGridCenterX = parent.findChild(QDoubleSpinBox, "spinBoxGridCenterX")
                if spinBoxGridCenterX:
                    spinBoxGridCenterX.setEnabled(not self.dialog_grid_load_from_file and not self.dialog_grid_center_on_origin)
                labelGridCenterY = parent.findChild(QLabel, "labelGridCenterY")
                if labelGridCenterY:
                    labelGridCenterY.setEnabled(not self.dialog_grid_load_from_file and not self.dialog_grid_center_on_origin)
                spinBoxGridCenterY = parent.findChild(QDoubleSpinBox, "spinBoxGridCenterY")
                if spinBoxGridCenterY:
                    spinBoxGridCenterY.setEnabled(not self.dialog_grid_load_from_file and not self.dialog_grid_center_on_origin)
                labelGridSizeX = parent.findChild(QLabel, "labelGridSizeX")
                if labelGridSizeX: labelGridSizeX.setEnabled(not self.dialog_grid_load_from_file)
                spinBoxGridSizeX = parent.findChild(QDoubleSpinBox, "spinBoxGridSizeX")
                if spinBoxGridSizeX: spinBoxGridSizeX.setEnabled(not self.dialog_grid_load_from_file)
                labelGridSizeY = parent.findChild(QLabel, "labelGridSizeY")
                if labelGridSizeY: labelGridSizeY.setEnabled(not self.dialog_grid_load_from_file)
                spinBoxGridSizeY = parent.findChild(QDoubleSpinBox, "spinBoxGridSizeY")
                if spinBoxGridSizeY: spinBoxGridSizeY.setEnabled(not self.dialog_grid_load_from_file)
                labelGridSpacingX = parent.findChild(QLabel, "labelGridSpacingX")
                if labelGridSpacingX: labelGridSpacingX.setEnabled(not self.dialog_grid_load_from_file)
                spinBoxGridSpacingX = parent.findChild(QDoubleSpinBox, "spinBoxGridSpacingX")
                if spinBoxGridSpacingX: spinBoxGridSpacingX.setEnabled(not self.dialog_grid_load_from_file)
                labelGridSpacingY = parent.findChild(QLabel, "labelGridSpacingY")
                if labelGridSpacingY: labelGridSpacingY.setEnabled(not self.dialog_grid_load_from_file)
                spinBoxGridSpacingY = parent.findChild(QDoubleSpinBox, "spinBoxGridSpacingY")
                if spinBoxGridSpacingY: spinBoxGridSpacingY.setEnabled(not self.dialog_grid_load_from_file)
                labelGridSizeRadius = parent.findChild(QLabel, "labelGridSizeRadius")
                if labelGridSizeRadius: labelGridSizeRadius.setEnabled(not self.dialog_grid_load_from_file)
                spinBoxGridSizeRadius = parent.findChild(QDoubleSpinBox, "spinBoxGridSizeRadius")
                if spinBoxGridSizeRadius: spinBoxGridSizeRadius.setEnabled(not self.dialog_grid_load_from_file)
                labelGridSpacingRadius = parent.findChild(QLabel, "labelGridSpacingRadius")
                if labelGridSpacingRadius: labelGridSpacingRadius.setEnabled(not self.dialog_grid_load_from_file)
                spinBoxGridSpacingRadius = parent.findChild(QDoubleSpinBox, "spinBoxGridSpacingRadius")
                if spinBoxGridSpacingRadius: spinBoxGridSpacingRadius.setEnabled(not self.dialog_grid_load_from_file)
                labelGridSpacingAngle = parent.findChild(QLabel, "labelGridSpacingAngle")
                if labelGridSpacingAngle: labelGridSpacingAngle.setEnabled(not self.dialog_grid_load_from_file)
                spinBoxGridSpacingAngle = parent.findChild(QDoubleSpinBox, "spinBoxGridSpacingAngle")
                if spinBoxGridSpacingAngle: spinBoxGridSpacingAngle.setEnabled(not self.dialog_grid_load_from_file)

    @Slot(str)
    def comboBoxGridTypeCurrentIndexChanged(self, type):
        """
        TOWRITE

        :param `type`: TOWRITE
        :type `type`: QString
        """
        self.dialog_grid_type = type

        senderObj = self.sender() # QObject*
        if senderObj:

            parent = senderObj.parent() # QObject*
            if parent:

                visibility = False
                if type == "Circular":
                    visibility = True

                labelGridSizeX = parent.findChild(QLabel, "labelGridSizeX")
                if labelGridSizeX: labelGridSizeX.setVisible(not visibility)
                spinBoxGridSizeX = parent.findChild(QDoubleSpinBox, "spinBoxGridSizeX")
                if spinBoxGridSizeX: spinBoxGridSizeX.setVisible(not visibility)
                labelGridSizeY = parent.findChild(QLabel, "labelGridSizeY")
                if labelGridSizeY: labelGridSizeY.setVisible(not visibility)
                spinBoxGridSizeY = parent.findChild(QDoubleSpinBox, "spinBoxGridSizeY")
                if spinBoxGridSizeY: spinBoxGridSizeY.setVisible(not visibility)
                labelGridSpacingX = parent.findChild(QLabel, "labelGridSpacingX")
                if labelGridSpacingX: labelGridSpacingX.setVisible(not visibility)
                spinBoxGridSpacingX = parent.findChild(QDoubleSpinBox, "spinBoxGridSpacingX")
                if spinBoxGridSpacingX: spinBoxGridSpacingX.setVisible(not visibility)
                labelGridSpacingY = parent.findChild(QLabel, "labelGridSpacingY")
                if labelGridSpacingY: labelGridSpacingY.setVisible(not visibility)
                spinBoxGridSpacingY = parent.findChild(QDoubleSpinBox, "spinBoxGridSpacingY")
                if spinBoxGridSpacingY: spinBoxGridSpacingY.setVisible(not visibility)
                labelGridSizeRadius = parent.findChild(QLabel, "labelGridSizeRadius")
                if labelGridSizeRadius: labelGridSizeRadius.setVisible(visibility)
                spinBoxGridSizeRadius = parent.findChild(QDoubleSpinBox, "spinBoxGridSizeRadius")
                if spinBoxGridSizeRadius: spinBoxGridSizeRadius.setVisible(visibility)
                labelGridSpacingRadius = parent.findChild(QLabel, "labelGridSpacingRadius")
                if labelGridSpacingRadius: labelGridSpacingRadius.setVisible(visibility)
                spinBoxGridSpacingRadius = parent.findChild(QDoubleSpinBox, "spinBoxGridSpacingRadius")
                if spinBoxGridSpacingRadius: spinBoxGridSpacingRadius.setVisible(visibility)
                labelGridSpacingAngle = parent.findChild(QLabel, "labelGridSpacingAngle")
                if labelGridSpacingAngle: labelGridSpacingAngle.setVisible(visibility)
                spinBoxGridSpacingAngle = parent.findChild(QDoubleSpinBox, "spinBoxGridSpacingAngle")
                if spinBoxGridSpacingAngle: spinBoxGridSpacingAngle.setVisible(visibility)

    @Slot(int)
    def checkBoxGridCenterOnOriginStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_grid_center_on_origin = checked

        senderObj = self.sender() # QObject*
        if senderObj:

            parent = senderObj.parent() # QObject*
            if parent:

                labelGridCenterX = parent.findChild(QLabel, "labelGridCenterX")
                if labelGridCenterX:
                    labelGridCenterX.setEnabled(not self.dialog_grid_center_on_origin)
                spinBoxGridCenterX = parent.findChild(QDoubleSpinBox, "spinBoxGridCenterX")
                if spinBoxGridCenterX:
                    spinBoxGridCenterX.setEnabled(not self.dialog_grid_center_on_origin)
                labelGridCenterY = parent.findChild(QLabel, "labelGridCenterY")
                if labelGridCenterY:
                    labelGridCenterY.setEnabled(not self.dialog_grid_center_on_origin)
                spinBoxGridCenterY = parent.findChild(QDoubleSpinBox, "spinBoxGridCenterY")
                if spinBoxGridCenterY:
                    spinBoxGridCenterY.setEnabled(not self.dialog_grid_center_on_origin)

    @Slot(float)
    def spinBoxGridCenterXValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_center_x = value

    @Slot(float)
    def spinBoxGridCenterYValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_center_y = value

    @Slot(float)
    def spinBoxGridSizeXValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_size_x = value

    @Slot(float)
    def spinBoxGridSizeYValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_size_y = value

    @Slot(float)
    def spinBoxGridSpacingXValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_spacing_x = value

    @Slot(float)
    def spinBoxGridSpacingYValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_spacing_y = value

    @Slot(float)
    def spinBoxGridSizeRadiusValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_size_radius = value

    @Slot(float)
    def spinBoxGridSpacingRadiusValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_spacing_radius = value

    @Slot(float)
    def spinBoxGridSpacingAngleValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_grid_spacing_angle = value

    @Slot(int)
    def checkBoxRulerShowOnLoadStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_ruler_show_on_load = checked

    @Slot(int)
    def comboBoxRulerMetricCurrentIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        comboBox = self.sender() # QComboBox* comboBox = qobject_cast<QComboBox*>(sender())
        if comboBox:

            ok = 0 # bool
            self.dialog_ruler_metric = comboBox.itemData(index).toBool()

        else:
            self.dialog_ruler_metric = True

    @Slot()
    def chooseRulerColor(self):
        """TOWRITE"""
        button = self.sender() # QPushButton* button = qobject_cast<QPushButton*>(sender())
        if button:

            colorDialog = QColorDialog(QColor(self.accept_ruler_color), self)
            colorDialog.currentColorChanged.connect(self.currentRulerColorChanged)
            colorDialog.exec_()

            if colorDialog.result() == QDialog.Accepted:

                self.accept_ruler_color = colorDialog.selectedColor().rgb()
                pix = QPixmap(16, 16)
                pix.fill(QColor(self.accept_ruler_color))
                button.setIcon(QIcon(pix))
                self.mainWin.updateAllViewRulerColors(self.accept_ruler_color)

            else:

                self.mainWin.updateAllViewRulerColors(self.dialog_ruler_color)

    @Slot(QColor)
    def currentRulerColorChanged(self, color):
        """
        TOWRITE

        :param `color`: TOWRITE
        :type `color`: `QColor`_
        """
        preview_ruler_color = color.rgb()
        self.mainWin.updateAllViewRulerColors(preview_ruler_color)

    @Slot(float)
    def spinBoxRulerPixelSizeValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: double
        """
        self.dialog_ruler_pixel_size = value

    @Slot(int)
    def checkBoxQSnapEndPointStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_endpoint = checked

    @Slot(int)
    def checkBoxQSnapMidPointStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_midpoint = checked

    @Slot(int)
    def checkBoxQSnapCenterStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_center = checked

    @Slot(int)
    def checkBoxQSnapNodeStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_node = checked

    @Slot(int)
    def checkBoxQSnapQuadrantStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_quadrant = checked

    @Slot(int)
    def checkBoxQSnapIntersectionStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_intersection = checked

    @Slot(int)
    def checkBoxQSnapExtensionStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_extension = checked

    @Slot(int)
    def checkBoxQSnapInsertionStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_insertion = checked

    @Slot(int)
    def checkBoxQSnapPerpendicularStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_perpendicular = checked

    @Slot(int)
    def checkBoxQSnapTangentStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_tangent = checked

    @Slot(int)
    def checkBoxQSnapNearestStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_nearest = checked

    @Slot(int)
    def checkBoxQSnapApparentStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_apparent = checked

    @Slot(int)
    def checkBoxQSnapParallelStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_qsnap_parallel = checked

    @Slot()
    def buttonQSnapSelectAllClicked(self):
        """TOWRITE"""
        pass #TODO/REMOVEME/TEMP
        #TODO# emit buttonQSnapSelectAll(True)

    @Slot()
    def buttonQSnapClearAllClicked(self):
        """TOWRITE"""
        pass #TODO/REMOVEME/TEMP
        #TODO# emit buttonQSnapClearAll(False)

    ##
    ## TODO: Figure out how to abstract the slot in a way that it can be used for comboBoxes in general
    ##       Currently comboBoxQSnapLocatorColorCurrentIndexChanged(int index)
    ##                 comboBoxSelectionCoolGripColorCurrentIndexChanged(int index)
    ##                 comboBoxSelectionHotGripColorCurrentIndexChanged(int index)
    ##       are all similar except the self.dialog_ variable being worked on and the QVariant.
    ##

    @Slot(int)
    def comboBoxQSnapLocatorColorCurrentIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        # TODO: Alert user if color matched the display bg color
        comboBox = self.sender() # QComboBox* comboBox = qobject_cast<QComboBox*>(sender())
        defaultColor = qRgb(255, 255, 0) # Yellow # QRgb
        if comboBox:

            ok = 0 # bool
            self.dialog_qsnap_locator_color = comboBox.itemData(index) # .toUInt(&ok)
            if not ok:
                self.dialog_qsnap_locator_color = defaultColor

        else:
            self.dialog_qsnap_locator_color = defaultColor

    @Slot(int)
    def sliderQSnapLocatorSizeValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: int
        """
        self.dialog_qsnap_locator_size = value

    @Slot(int)
    def sliderQSnapApertureSizeValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: int
        """
        self.dialog_qsnap_aperture_size = value

    @Slot(int)
    def checkBoxLwtShowLwtStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.preview_lwt_show_lwt = checked
        if self.preview_lwt_show_lwt:
            self.mainWin.statusbar.statusBarLwtButton.enableLwt()
        else:
            self.mainWin.statusbar.statusBarLwtButton.disableLwt()

        senderObj = self.sender() # QObject*
        if senderObj:

            parent = senderObj.parent() # QObject*
            if parent:

                checkBoxRealRender = parent.findChild(QCheckBox, "checkBoxRealRender")
                if checkBoxRealRender:
                    checkBoxRealRender.setEnabled(self.preview_lwt_show_lwt)

    @Slot(int)
    def checkBoxLwtRealRenderStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.preview_lwt_real_render = checked
        if self.preview_lwt_real_render:
            self.mainWin.statusbar.statusBarLwtButton.enableReal()
        else:
            self.mainWin.statusbar.statusBarLwtButton.disableReal()

    @Slot(int)
    def checkBoxSelectionModePickFirstStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_selection_mode_pickfirst = checked

    @Slot(int)
    def checkBoxSelectionModePickAddStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_selection_mode_pickadd = checked

    @Slot(int)
    def checkBoxSelectionModePickDragStateChanged(self, checked):
        """
        TOWRITE

        :param `checked`: TOWRITE
        :type `checked`: int
        """
        self.dialog_selection_mode_pickdrag = checked

    @Slot(int)
    def sliderSelectionGripSizeValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: int
        """
        self.dialog_selection_grip_size = value

    @Slot(int)
    def sliderSelectionPickBoxSizeValueChanged(self, value):
        """
        TOWRITE

        :param `value`: TOWRITE
        :type `value`: int
        """
        self.dialog_selection_pickbox_size = value

    @Slot(int)
    def comboBoxSelectionCoolGripColorCurrentIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        # TODO: Alert user if color matched the display bg color
        comboBox = self.sender() # QComboBox* comboBox = qobject_cast<QComboBox*>(sender())
        defaultColor = qRgb(0, 0, 255) # Blue # QRgb
        if comboBox:

            ok = 0 # bool
            self.dialog_selection_coolgrip_color = comboBox.itemData(index) #.toUInt(&ok)
            if not ok:
                self.dialog_selection_coolgrip_color = defaultColor

        else:
            self.dialog_selection_coolgrip_color = defaultColor

    @Slot(int)
    def comboBoxSelectionHotGripColorCurrentIndexChanged(self, index):
        """
        TOWRITE

        :param `index`: TOWRITE
        :type `index`: int
        """
        # TODO: Alert user if color matched the display bg color
        comboBox = self.sender() # QComboBox* comboBox = qobject_cast<QComboBox*>(sender())
        defaultColor = qRgb(255, 0, 0) # Red # QRgb
        if comboBox:

            ok = 0 # bool
            self.dialog_selection_hotgrip_color = comboBox.itemData(index) #.toUInt(&ok)
            if not ok:
                self.dialog_selection_hotgrip_color = defaultColor

        else:
            self.dialog_selection_hotgrip_color = defaultColor

    @Slot()
    def acceptChanges(self):
        """TOWRITE"""

        self.dialog_general_mdi_bg_use_logo = self.preview_general_mdi_bg_use_logo
        self.dialog_general_mdi_bg_use_texture = self.preview_general_mdi_bg_use_texture
        self.dialog_general_mdi_bg_use_color = self.preview_general_mdi_bg_use_color
        self.dialog_general_mdi_bg_logo = self.accept_general_mdi_bg_logo
        self.dialog_general_mdi_bg_texture = self.accept_general_mdi_bg_texture
        self.dialog_general_mdi_bg_color = self.accept_general_mdi_bg_color
        self.dialog_display_show_scrollbars = self.preview_display_show_scrollbars
        self.dialog_display_crosshair_color = self.accept_display_crosshair_color
        self.dialog_display_bg_color = self.accept_display_bg_color
        self.dialog_display_selectbox_left_color = self.accept_display_selectbox_left_color
        self.dialog_display_selectbox_left_fill = self.accept_display_selectbox_left_fill
        self.dialog_display_selectbox_right_color = self.accept_display_selectbox_right_color
        self.dialog_display_selectbox_right_fill = self.accept_display_selectbox_right_fill
        self.dialog_display_selectbox_alpha = self.preview_display_selectbox_alpha
        self.dialog_prompt_text_color = self.accept_prompt_text_color
        self.dialog_prompt_bg_color = self.accept_prompt_bg_color
        self.dialog_prompt_font_family = self.preview_prompt_font_family
        #TODO/PORT# self.dialog_prompt_font_style = self.preview_prompt_font_style
        self.dialog_prompt_font_size = self.preview_prompt_font_size
        if self.dialog_grid_color_match_crosshair:
            self.dialog_grid_color = self.accept_display_crosshair_color
        else:
            self.dialog_grid_color = self.accept_grid_color
        self.dialog_ruler_color = self.accept_ruler_color
        self.dialog_lwt_show_lwt = self.preview_lwt_show_lwt
        self.dialog_lwt_real_render = self.preview_lwt_real_render

        self.mainWin.setSettingsGeneralLanguage(self.dialog_general_language)
        self.mainWin.setSettingsGeneralIconTheme(self.dialog_general_icon_theme)
        self.mainWin.setSettingsGeneralIconSize(self.dialog_general_icon_size)
        self.mainWin.setSettingsGeneralMdiBGUseLogo(self.dialog_general_mdi_bg_use_logo)
        self.mainWin.setSettingsGeneralMdiBGUseTexture(self.dialog_general_mdi_bg_use_texture)
        self.mainWin.setSettingsGeneralMdiBGUseColor(self.dialog_general_mdi_bg_use_color)
        self.mainWin.setSettingsGeneralMdiBGLogo(self.dialog_general_mdi_bg_logo)
        self.mainWin.setSettingsGeneralMdiBGTexture(self.dialog_general_mdi_bg_texture)
        self.mainWin.setSettingsGeneralMdiBGColor(self.dialog_general_mdi_bg_color)
        self.mainWin.setSettingsGeneralTipOfTheDay(self.dialog_general_tip_of_the_day)
        # TODO: self.mainWin.setSettingsGeneralSystemHelpBrowser(self.dialog_general_system_help_browser)
        #TODO/PORT# self.mainWin.setSettingsDisplayUseOpenGL(self.dialog_display_use_opengl)
        #TODO/PORT# self.mainWin.setSettingsDisplayRenderHintAA(self.dialog_display_renderhint_aa)
        #TODO/PORT# self.mainWin.setSettingsDisplayRenderHintTextAA(self.dialog_display_renderhint_text_aa)
        #TODO/PORT# self.mainWin.setSettingsDisplayRenderHintSmoothPix(self.dialog_display_renderhint_smooth_pix)
        #TODO/PORT# self.mainWin.setSettingsDisplayRenderHintHighAA(self.dialog_display_renderhint_high_aa)
        #TODO/PORT# self.mainWin.setSettingsDisplayRenderHintNonCosmetic(self.dialog_display_renderhint_noncosmetic)
        self.mainWin.setSettingsDisplayShowScrollBars(self.dialog_display_show_scrollbars)
        self.mainWin.setSettingsDisplayScrollBarWidgetNum(self.dialog_display_scrollbar_widget_num)
        self.mainWin.setSettingsDisplayCrossHairColor(self.dialog_display_crosshair_color)
        self.mainWin.setSettingsDisplayBGColor(self.dialog_display_bg_color)
        self.mainWin.setSettingsDisplaySelectBoxLeftColor(self.dialog_display_selectbox_left_color)
        self.mainWin.setSettingsDisplaySelectBoxLeftFill(self.dialog_display_selectbox_left_fill)
        self.mainWin.setSettingsDisplaySelectBoxRightColor(self.dialog_display_selectbox_right_color)
        self.mainWin.setSettingsDisplaySelectBoxRightFill(self.dialog_display_selectbox_right_fill)
        self.mainWin.setSettingsDisplaySelectBoxAlpha(self.dialog_display_selectbox_alpha)
        self.mainWin.setSettingsDisplayZoomScaleIn(self.dialog_display_zoomscale_in)
        self.mainWin.setSettingsDisplayZoomScaleOut(self.dialog_display_zoomscale_out)
        # TODO: self.mainWin.setSettingsDisplayCrossHairPercent(self.dialog_display_crosshair_percent)
        # TODO: self.mainWin.setSettingsDisplayUnits(self.dialog_display_units)
        self.mainWin.setSettingsPromptTextColor(self.dialog_prompt_text_color)
        self.mainWin.setSettingsPromptBGColor(self.dialog_prompt_bg_color)
        self.mainWin.setSettingsPromptFontFamily(self.dialog_prompt_font_family)
        # TODO/PORT# self.mainWin.setSettingsPromptFontStyle(self.dialog_prompt_font_style)
        self.mainWin.setSettingsPromptFontSize(self.dialog_prompt_font_size)
        self.mainWin.setSettingsPromptSaveHistory(self.dialog_prompt_save_history)
        self.mainWin.setSettingsPromptSaveHistoryAsHtml(self.dialog_prompt_save_history_as_html)
        # TODO: self.mainWin.setSettingsPromptSaveHistoryFilename(self.dialog_prompt_save_history_filename)
        self.mainWin.setSettingsCustomFilter(self.dialog_opensave_custom_filter)
        # TODO: self.mainWin.setSettingsOpenFormat(self.dialog_opensave_open_format)
        # TODO: self.mainWin.setSettingsOpenThumbnail(self.dialog_opensave_open_thumbnail)
        # TODO: self.mainWin.setSettingsSaveFormat(self.dialog_opensave_save_format)
        # TODO: self.mainWin.setSettingsSaveThumbnail(self.dialog_opensave_save_thumbnail)
        self.mainWin.setSettingsRecentMaxFiles(self.dialog_opensave_recent_max_files)
        self.mainWin.setSettingsOpenSaveTrimDstNumJumps(self.dialog_opensave_trim_dst_num_jumps)
        # TODO: self.mainWin.setSettingsPrintingDefaultDevice(self.dialog_printing_default_device)
        # TODO: self.mainWin.setSettingsPrintingUseLastDevice(self.dialog_printing_use_last_device)
        self.mainWin.setSettingsPrintingDisableBG(self.dialog_printing_disable_bg)
        self.mainWin.setSettingsGridShowOnLoad(self.dialog_grid_show_on_load)
        self.mainWin.setSettingsGridShowOrigin(self.dialog_grid_show_origin)
        self.mainWin.setSettingsGridColorMatchCrossHair(self.dialog_grid_color_match_crosshair)
        self.mainWin.setSettingsGridColor(self.dialog_grid_color)
        # TODO: self.mainWin.setSettingsGridLoadFromFile(self.dialog_grid_load_from_file)
        self.mainWin.setSettingsGridType(self.dialog_grid_type)
        self.mainWin.setSettingsGridCenterOnOrigin(self.dialog_grid_center_on_origin)
        self.mainWin.setSettingsGridCenterX(self.dialog_grid_center_x)
        self.mainWin.setSettingsGridCenterY(self.dialog_grid_center_y)
        self.mainWin.setSettingsGridSizeX(self.dialog_grid_size_x)
        self.mainWin.setSettingsGridSizeY(self.dialog_grid_size_y)
        self.mainWin.setSettingsGridSpacingX(self.dialog_grid_spacing_x)
        self.mainWin.setSettingsGridSpacingY(self.dialog_grid_spacing_y)
        self.mainWin.setSettingsGridSizeRadius(self.dialog_grid_size_radius)
        self.mainWin.setSettingsGridSpacingRadius(self.dialog_grid_spacing_radius)
        self.mainWin.setSettingsGridSpacingAngle(self.dialog_grid_spacing_angle)
        self.mainWin.setSettingsRulerShowOnLoad(self.dialog_ruler_show_on_load)
        self.mainWin.setSettingsRulerMetric(self.dialog_ruler_metric)
        self.mainWin.setSettingsRulerColor(self.dialog_ruler_color)
        self.mainWin.setSettingsRulerPixelSize(self.dialog_ruler_pixel_size)
        # TODO: self.mainWin.setSettingsQSnapEnabled(self.dialog_qsnap_enabled)
        self.mainWin.setSettingsQSnapLocatorColor(self.dialog_qsnap_locator_color)
        self.mainWin.setSettingsQSnapLocatorSize(self.dialog_qsnap_locator_size)
        self.mainWin.setSettingsQSnapApertureSize(self.dialog_qsnap_aperture_size)
        self.mainWin.setSettingsQSnapEndPoint(self.dialog_qsnap_endpoint)
        self.mainWin.setSettingsQSnapMidPoint(self.dialog_qsnap_midpoint)
        self.mainWin.setSettingsQSnapCenter(self.dialog_qsnap_center)
        self.mainWin.setSettingsQSnapNode(self.dialog_qsnap_node)
        self.mainWin.setSettingsQSnapQuadrant(self.dialog_qsnap_quadrant)
        self.mainWin.setSettingsQSnapIntersection(self.dialog_qsnap_intersection)
        self.mainWin.setSettingsQSnapExtension(self.dialog_qsnap_extension)
        self.mainWin.setSettingsQSnapInsertion(self.dialog_qsnap_insertion)
        self.mainWin.setSettingsQSnapPerpendicular(self.dialog_qsnap_perpendicular)
        self.mainWin.setSettingsQSnapTangent(self.dialog_qsnap_tangent)
        self.mainWin.setSettingsQSnapNearest(self.dialog_qsnap_nearest)
        self.mainWin.setSettingsQSnapApparent(self.dialog_qsnap_apparent)
        self.mainWin.setSettingsQSnapParallel(self.dialog_qsnap_parallel)
        self.mainWin.setSettingsLwtShowLwt(self.dialog_lwt_show_lwt)
        self.mainWin.setSettingsLwtRealRender(self.dialog_lwt_real_render)
        self.mainWin.setSettingsSelectionModePickFirst(self.dialog_selection_mode_pickfirst)
        self.mainWin.setSettingsSelectionModePickAdd(self.dialog_selection_mode_pickadd)
        self.mainWin.setSettingsSelectionModePickDrag(self.dialog_selection_mode_pickdrag)
        self.mainWin.setSettingsSelectionCoolGripColor(self.dialog_selection_coolgrip_color)
        self.mainWin.setSettingsSelectionHotGripColor(self.dialog_selection_hotgrip_color)
        self.mainWin.setSettingsSelectionGripSize(self.dialog_selection_grip_size)
        self.mainWin.setSettingsSelectionPickBoxSize(self.dialog_selection_pickbox_size)

        # Make sure the user sees the changes applied immediately
        self.mainWin.mdiArea.useBackgroundLogo(self.dialog_general_mdi_bg_use_logo)
        self.mainWin.mdiArea.useBackgroundTexture(self.dialog_general_mdi_bg_use_texture)
        self.mainWin.mdiArea.useBackgroundColor(self.dialog_general_mdi_bg_use_color)
        self.mainWin.mdiArea.setBackgroundLogo(self.dialog_general_mdi_bg_logo)
        self.mainWin.mdiArea.setBackgroundTexture(self.dialog_general_mdi_bg_texture)
        self.mainWin.mdiArea.setBackgroundColor(self.dialog_general_mdi_bg_color)
        self.mainWin.iconResize(self.dialog_general_icon_size)
        self.mainWin.updateAllViewScrollBars(self.dialog_display_show_scrollbars)
        self.mainWin.updateAllViewCrossHairColors(self.dialog_display_crosshair_color)
        self.mainWin.updateAllViewBackgroundColors(self.dialog_display_bg_color)
        self.mainWin.updateAllViewSelectBoxColors(self.dialog_display_selectbox_left_color,
                                                  self.dialog_display_selectbox_left_fill,
                                                  self.dialog_display_selectbox_right_color,
                                                  self.dialog_display_selectbox_right_fill,
                                                  self.dialog_display_selectbox_alpha)
        #TODO/PORT# self.mainWin.prompt.setPromptTextColor(QColor(self.dialog_prompt_text_color))
        #TODO/PORT# self.mainWin.prompt.setPromptBackgroundColor(QColor(self.dialog_prompt_bg_color))
        #TODO/PORT# self.mainWin.prompt.setPromptFontFamily(self.dialog_prompt_font_family)
        #TODO/PORT# self.mainWin.prompt.setPromptFontStyle(self.dialog_prompt_font_style)
        #TODO/PORT# self.mainWin.prompt.setPromptFontSize(self.dialog_prompt_font_size)
        self.mainWin.updateAllViewGridColors(self.dialog_grid_color)
        self.mainWin.updateAllViewRulerColors(self.dialog_ruler_color)
        if self.dialog_lwt_show_lwt:
            self.mainWin.statusbar.statusBarLwtButton.enableLwt()
        else:
            self.mainWin.statusbar.statusBarLwtButton.disableLwt()
        if self.dialog_lwt_real_render:
            self.mainWin.statusbar.statusBarLwtButton.enableReal()
        else:
            self.mainWin.statusbar.statusBarLwtButton.disableReal()
        self.mainWin.updatePickAddMode(self.dialog_selection_mode_pickadd)

        self.mainWin.writeSettings()
        self.accept()

    @Slot()
    def rejectChanges(self):
        """ TOWRITE """

        # TODO: inform the user if they have changed settings

        # Update the view since the user must accept the preview
        self.mainWin.mdiArea.useBackgroundLogo(self.dialog_general_mdi_bg_use_logo)
        self.mainWin.mdiArea.useBackgroundTexture(self.dialog_general_mdi_bg_use_texture)
        self.mainWin.mdiArea.useBackgroundColor(self.dialog_general_mdi_bg_use_color)
        self.mainWin.mdiArea.setBackgroundLogo(self.dialog_general_mdi_bg_logo)
        self.mainWin.mdiArea.setBackgroundTexture(self.dialog_general_mdi_bg_texture)
        self.mainWin.mdiArea.setBackgroundColor(self.dialog_general_mdi_bg_color)
        self.mainWin.updateAllViewScrollBars(self.dialog_display_show_scrollbars)
        self.mainWin.updateAllViewCrossHairColors(self.dialog_display_crosshair_color)
        self.mainWin.updateAllViewBackgroundColors(self.dialog_display_bg_color)
        self.mainWin.updateAllViewSelectBoxColors(self.dialog_display_selectbox_left_color,
                                                  self.dialog_display_selectbox_left_fill,
                                                  self.dialog_display_selectbox_right_color,
                                                  self.dialog_display_selectbox_right_fill,
                                                  self.dialog_display_selectbox_alpha)
        #TODO/PORT# self.mainWin.prompt.setPromptTextColor(QColor(self.dialog_prompt_text_color))
        #TODO/PORT# self.mainWin.prompt.setPromptBackgroundColor(QColor(self.dialog_prompt_bg_color))
        #TODO/PORT# self.mainWin.prompt.setPromptFontFamily(self.dialog_prompt_font_family)
        #TODO/PORT# self.mainWin.prompt.setPromptFontStyle(self.dialog_prompt_font_style)
        #TODO/PORT# self.mainWin.prompt.setPromptFontSize(self.dialog_prompt_font_size)
        self.mainWin.updateAllViewGridColors(self.dialog_grid_color)
        self.mainWin.updateAllViewRulerColors(self.dialog_ruler_color)
        if self.dialog_lwt_show_lwt:
            self.mainWin.statusbar.statusBarLwtButton.enableLwt()
        else:
            self.mainWin.statusbar.statusBarLwtButton.disableLwt()
        if self.dialog_lwt_real_render:
            self.mainWin.statusbar.statusBarLwtButton.enableReal()
        else:
            self.mainWin.statusbar.statusBarLwtButton.disableReal()

        self.reject()

    # Signals ----------------------------------------------------------------

    buttonCustomFilterSelectAll = Signal(bool)
    buttonCustomFilterClearAll = Signal(bool)
    buttonQSnapSelectAll = Signal(bool)
    buttonQSnapClearAll = Signal(bool)


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
