#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
=======================================
|module_summary| mainwindow_settings.py
=======================================

TOWRITE
"""

#-Imports.----------------------------------------------------------------------

#--Python Imports.
import os
import sys
from collections import deque

try:
    if sys.version_info[0] == 2:
        import cPickle as pickle  #TODO# what's this for?
        import ConfigParser as configparser  # Saving of ini settings
    elif sys.version_info[0] == 3:
        import pickle
        import configparser
except ImportError as exc:
    raise exc
gSettings = configparser.ConfigParser()

#--PySide/PyQt Imports.

#include "mainwindow.h"
#include "settings-dialog.h"

#include <QDebug>
#include <QSettings>

if PYSIDE:
    from PySide.QtCore import qDebug, QPoint, QSettings, QSize, Slot, QByteArray
    from PySide.QtGui import qRgb
elif PYQT4:
    import sip
    sip.setapi('QString', 2)
    sip.setapi('QVariant', 2)
    from PyQt4.QtCore import qDebug, QPoint, QSettings, QSize, QByteArray
    from PyQt4.QtCore import pyqtSlot as Slot
    from PyQt4.QtGui import qRgb

#--Local Imports.
from settings_dialog import Settings_Dialog



def readSettings(self): # void MainWindow::
    """"""
    qDebug("Reading Settings...")
    theSettingsIniPath = self.gAppDir + os.sep + "settings-py.ini"

    # self.gSettings = gSettings = QSettings(theSettingsIniPath, QSettings.IniFormat)
    # pos = gSettings.value("Window/Position", QPoint(0, 0))#.toPoint()
    # size = gSettings.value("Window/Size", QSize(800, 600))#.toSize()

    # self.layoutState = gSettings.value("LayoutState")#.toByteArray()
    # from array import array
    # self.layoutState = array("B", gSettings.get("General", "LayoutState")) #.toByteArray()
    # if not self.layoutState: # None
    #     qDebug("LayoutState NOT restored! Setting Default Layout...")
    #     # someToolBar.setVisible(True)
    # else:
    #     self.restoreState(self.layoutState)

    ## # PySide has this bad habit *sometimes* of all settings being strings on reloading.
    ## # This is a pain in the ass to not have a clear way to retrieve values as they where
    ## # upon saving.
    ## # Maybe should just switch to pythons built in configparser module for ini.
    ## # At least it has config.getboolean(section, option)
    ## isBoolStr = ('true', 'True')
    ## # General
    ## self.settings_general_language               =       gSettings.value("Language",                            "default")               #.toString()
    ## self.settings_general_icon_theme             =       gSettings.value("IconTheme",                           "default")               #.toString()
    ## self.settings_general_icon_size              =   int(gSettings.value("IconSize",                                   16))              #.toInt()
    ## self.settings_general_mdi_bg_use_logo        =       gSettings.value("MdiBGUseLogo",                             True) in isBoolStr  #.toBool()
    ## self.settings_general_mdi_bg_use_texture     =       gSettings.value("MdiBGUseTexture",                          True) in isBoolStr  #.toBool()
    ## self.settings_general_mdi_bg_use_color       =       gSettings.value("MdiBGUseColor",                            True) in isBoolStr  #.toBool()
    ## self.settings_general_mdi_bg_logo            =       gSettings.value("MdiBGLogo",           "images/logo-spirals.png")               #.toString()
    ## self.settings_general_mdi_bg_texture         =       gSettings.value("MdiBGTexture",     "images/texture-spirals.png")               #.toString()
    ## self.settings_general_mdi_bg_color           =   int(gSettings.value("MdiBGColor",                  qRgb(192,192,192)))              #.toInt()
    ## self.settings_general_tip_of_the_day         =       gSettings.value("TipOfTheDay",                                 1) in isBoolStr  #.toBool()
    ## self.settings_general_current_tip            =   int(gSettings.value("CurrentTip",                                  0))              #.toInt()
    ## # Display
    ## self.settings_display_use_opengl             =       gSettings.value("Display/UseOpenGL",                       False) in isBoolStr  #.toBool()
    ## self.settings_display_renderhint_aa          =       gSettings.value("Display/RenderHintAntiAlias",             False) in isBoolStr  #.toBool()
    ## self.settings_display_renderhint_text_aa     =       gSettings.value("Display/RenderHintTextAntiAlias",         False) in isBoolStr  #.toBool()
    ## self.settings_display_renderhint_smooth_pix  =       gSettings.value("Display/RenderHintSmoothPixmap",          False) in isBoolStr  #.toBool()
    ## self.settings_display_renderhint_high_aa     =       gSettings.value("Display/RenderHintHighQualityAntiAlias",  False) in isBoolStr  #.toBool()
    ## self.settings_display_renderhint_noncosmetic =       gSettings.value("Display/RenderHintNonCosmetic",           False) in isBoolStr  #.toBool()
    ## self.settings_display_show_scrollbars        =       gSettings.value("Display/ShowScrollBars",                   True) in isBoolStr  #.toBool()
    ## self.settings_display_scrollbar_widget_num   =   int(gSettings.value("Display/ScrollBarWidgetNum",                  0))              #.toInt()
    ## self.settings_display_crosshair_color        =   int(gSettings.value("Display/CrossHairColor",      qRgb(  0,  0,  0)))              #.toInt()
    ## self.settings_display_bg_color               =   int(gSettings.value("Display/BackgroundColor",     qRgb(235,235,235)))              #.toInt()
    ## self.settings_display_selectbox_left_color   =   int(gSettings.value("Display/SelectBoxLeftColor",  qRgb(  0,128,  0)))              #.toInt()
    ## self.settings_display_selectbox_left_fill    =   int(gSettings.value("Display/SelectBoxLeftFill",   qRgb(  0,255,  0)))              #.toInt()
    ## self.settings_display_selectbox_right_color  =   int(gSettings.value("Display/SelectBoxRightColor", qRgb(  0,  0,128)))              #.toInt()
    ## self.settings_display_selectbox_right_fill   =   int(gSettings.value("Display/SelectBoxRightFill",  qRgb(  0,  0,255)))              #.toInt()
    ## self.settings_display_selectbox_alpha        =   int(gSettings.value("Display/SelectBoxAlpha",                     32))              #.toInt()
    ## self.settings_display_zoomscale_in           = float(gSettings.value("Display/ZoomScaleIn",                       2.0))              #.toFloat()
    ## self.settings_display_zoomscale_out          = float(gSettings.value("Display/ZoomScaleOut",                      0.5))              #.toFloat()
    ## self.settings_display_crosshair_percent      =   int(gSettings.value("Display/CrossHairPercent",                    5))              #.toInt()
    ## self.settings_display_units                  =       gSettings.value("Display/Units",                            "mm")               #.toString()
    ## # Prompt
    ## self.settings_prompt_text_color              =   int(gSettings.value("Prompt/TextColor",            qRgb(  0,  0,  0)))              #.toInt()
    ## self.settings_prompt_bg_color                =   int(gSettings.value("Prompt/BackgroundColor",      qRgb(255,255,255)))              #.toInt()
    ## self.settings_prompt_font_family             =       gSettings.value("Prompt/FontFamily",                 "Monospace")               #.toString()
    ## self.settings_prompt_font_style              =       gSettings.value("Prompt/FontStyle",                     "normal")               #.toString()
    ## self.settings_prompt_font_size               =   int(gSettings.value("Prompt/FontSize",                            12))              #.toInt()
    ## self.settings_prompt_save_history            =       gSettings.value("Prompt/SaveHistory",                       True) in isBoolStr  #.toBool()
    ## self.settings_prompt_save_history_as_html    =       gSettings.value("Prompt/SaveHistoryAsHtml",                False) in isBoolStr  #.toBool()
    ## self.settings_prompt_save_history_filename   =       gSettings.value("Prompt/SaveHistoryFilename",       "prompt.log")               #.toString()
    ## # OpenSave
    ## self.settings_opensave_custom_filter         =       gSettings.value("OpenSave/CustomFilter",             "supported")               #.toString()
    ## self.settings_opensave_open_format           =       gSettings.value("OpenSave/OpenFormat",                     "*.*")               #.toString()
    ## self.settings_opensave_open_thumbnail        =       gSettings.value("OpenSave/OpenThumbnail",                  False) in isBoolStr  #.toBool()
    ## self.settings_opensave_save_format           =       gSettings.value("OpenSave/SaveFormat",                     "*.*")               #.toString()
    ## self.settings_opensave_save_thumbnail        =       gSettings.value("OpenSave/SaveThumbnail",                  False) in isBoolStr  #.toBool()
    ## # Recent
    ## self.settings_opensave_recent_max_files      =   int(gSettings.value("OpenSave/RecentMax",                         10))              #.toInt()
    ## self.settings_opensave_recent_list_of_files  =       gSettings.value("OpenSave/RecentFiles")                                         #.toStringList()
    ## self.settings_opensave_recent_directory      =       gSettings.value("OpenSave/RecentDirectory")                                     #.toString()
    ## # Trimming
    ## self.settings_opensave_trim_dst_num_jumps    =   int(gSettings.value("OpenSave/TrimDstNumJumps",                    5))              #.toInt()
    ## # Printing
    ## self.settings_printing_default_device        =       gSettings.value("Printing/DefaultDevice",                     "")               #.toString()
    ## self.settings_printing_use_last_device       =       gSettings.value("Printing/UseLastDevice",                  False) in isBoolStr  #.toBool()
    ## self.settings_printing_disable_bg            =       gSettings.value("Printing/DisableBG",                       True) in isBoolStr  #.toBool()
    ## # Grid
    ## self.settings_grid_show_on_load              =       gSettings.value("Grid/ShowOnLoad",                          True) in isBoolStr  #.toBool()
    ## self.settings_grid_show_origin               =       gSettings.value("Grid/ShowOrigin",                          True) in isBoolStr  #.toBool()
    ## self.settings_grid_color_match_crosshair     =       gSettings.value("Grid/ColorMatchCrossHair",                 True) in isBoolStr  #.toBool()
    ## self.settings_grid_color                     =   int(gSettings.value("Grid/Color",                  qRgb(  0,  0,  0)))              #.toInt()
    ## self.settings_grid_load_from_file            =       gSettings.value("Grid/LoadFromFile",                        True) in isBoolStr  #.toBool()
    ## self.settings_grid_type                      =       gSettings.value("Grid/Type",                       "Rectangular")               #.toString()
    ## self.settings_grid_center_on_origin          =       gSettings.value("Grid/CenterOnOrigin",                      True) in isBoolStr  #.toBool()
    ## self.settings_grid_center_x                  = float(gSettings.value("Grid/CenterX",                              0.0))              #.toFloat()
    ## self.settings_grid_center_y                  = float(gSettings.value("Grid/CenterY",                              0.0))              #.toFloat()
    ## self.settings_grid_size_x                    = float(gSettings.value("Grid/SizeX",                              100.0))              #.toFloat()
    ## self.settings_grid_size_y                    = float(gSettings.value("Grid/SizeY",                              100.0))              #.toFloat()
    ## self.settings_grid_spacing_x                 = float(gSettings.value("Grid/SpacingX",                            25.0))              #.toFloat()
    ## self.settings_grid_spacing_y                 = float(gSettings.value("Grid/SpacingY",                            25.0))              #.toFloat()
    ## self.settings_grid_size_radius               = float(gSettings.value("Grid/SizeRadius",                          50.0))              #.toFloat()
    ## self.settings_grid_spacing_radius            = float(gSettings.value("Grid/SpacingRadius",                       25.0))              #.toFloat()
    ## self.settings_grid_spacing_angle             = float(gSettings.value("Grid/SpacingAngle",                        45.0))              #.toFloat()
    ## # Ruler
    ## self.settings_ruler_show_on_load             =       gSettings.value("Ruler/ShowOnLoad",                         True) in isBoolStr  #.toBool()
    ## self.settings_ruler_metric                   =       gSettings.value("Ruler/Metric",                             True) in isBoolStr  #.toBool()
    ## self.settings_ruler_color                    =   int(gSettings.value("Ruler/Color",                 qRgb(210,210, 50)))              #.toInt()
    ## self.settings_ruler_pixel_size               =   int(gSettings.value("Ruler/PixelSize",                            20))              #.toInt()
    ## # Quick Snap
    ## self.settings_qsnap_enabled                  =       gSettings.value("QuickSnap/Enabled",                        True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_locator_color            =   int(gSettings.value("QuickSnap/LocatorColor",      qRgb(255,255,  0)))              #.toInt()
    ## self.settings_qsnap_locator_size             =   int(gSettings.value("QuickSnap/LocatorSize",                       4))              #.toInt()
    ## self.settings_qsnap_aperture_size            =   int(gSettings.value("QuickSnap/ApertureSize",                     10))              #.toInt()
    ## self.settings_qsnap_endpoint                 =       gSettings.value("QuickSnap/EndPoint",                       True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_midpoint                 =       gSettings.value("QuickSnap/MidPoint",                       True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_center                   =       gSettings.value("QuickSnap/Center",                         True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_node                     =       gSettings.value("QuickSnap/Node",                           True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_quadrant                 =       gSettings.value("QuickSnap/Quadrant",                       True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_intersection             =       gSettings.value("QuickSnap/Intersection",                   True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_extension                =       gSettings.value("QuickSnap/Extension",                      True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_insertion                =       gSettings.value("QuickSnap/Insertion",                     False) in isBoolStr  #.toBool()
    ## self.settings_qsnap_perpendicular            =       gSettings.value("QuickSnap/Perpendicular",                  True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_tangent                  =       gSettings.value("QuickSnap/Tangent",                        True) in isBoolStr  #.toBool()
    ## self.settings_qsnap_nearest                  =       gSettings.value("QuickSnap/Nearest",                       False) in isBoolStr  #.toBool()
    ## self.settings_qsnap_apparent                 =       gSettings.value("QuickSnap/Apparent",                      False) in isBoolStr  #.toBool()
    ## self.settings_qsnap_parallel                 =       gSettings.value("QuickSnap/Parallel",                      False) in isBoolStr  #.toBool()
    ## # LineWeight
    ## self.settings_lwt_show_lwt                   =       gSettings.value("LineWeight/ShowLineWeight",               False) in isBoolStr  #.toBool()
    ## self.settings_lwt_real_render                =       gSettings.value("LineWeight/RealRender",                    True) in isBoolStr  #.toBool()
    ## self.settings_lwt_default_lwt                = float(gSettings.value("LineWeight/DefaultLineWeight",                0))              #.toReal()
    ## # Selection
    ## self.settings_selection_mode_pickfirst       =       gSettings.value("Selection/PickFirst",                      True) in isBoolStr  #.toBool()
    ## self.settings_selection_mode_pickadd         =       gSettings.value("Selection/PickAdd",                        True) in isBoolStr  #.toBool()
    ## self.settings_selection_mode_pickdrag        =       gSettings.value("Selection/PickDrag",                      False) in isBoolStr  #.toBool()
    ## self.settings_selection_coolgrip_color       =   int(gSettings.value("Selection/CoolGripColor",     qRgb(  0,  0,255)))              #.toInt()
    ## self.settings_selection_hotgrip_color        =   int(gSettings.value("Selection/HotGripColor",      qRgb(255,  0,  0)))              #.toInt()
    ## self.settings_selection_grip_size            =   int(gSettings.value("Selection/GripSize",                          4))              #.toInt()
    ## self.settings_selection_pickbox_size         =   int(gSettings.value("Selection/PickBoxSize",                       4))              #.toInt()
    ## # Text
    ## self.settings_text_font                      =       gSettings.value("Text/Font",                             "Arial")               #.toString()
    ## self.settings_text_size                      = float(gSettings.value("Text/Size",                                  12))              #.toReal()
    ## self.settings_text_angle                     = float(gSettings.value("Text/Angle",                                  0))              #.toReal()
    ## self.settings_text_style_bold                =       gSettings.value("Text/StyleBold",                          False) in isBoolStr  #.toBool()
    ## self.settings_text_style_italic              =       gSettings.value("Text/StyleItalic",                        False) in isBoolStr  #.toBool()
    ## self.settings_text_style_underline           =       gSettings.value("Text/StyleUnderline",                     False) in isBoolStr  #.toBool()
    ## self.settings_text_style_strikeout           =       gSettings.value("Text/StyleStrikeOut",                     False) in isBoolStr  #.toBool()
    ## self.settings_text_style_overline            =       gSettings.value("Text/StyleOverline",                      False) in isBoolStr  #.toBool()

    ############

    ## # General
    ## self.settings_general_language               = gSettings.value("Language",                            "default")#.toString()
    ## self.settings_general_icon_theme             = gSettings.value("IconTheme",                           "default")#.toString()
    ## self.settings_general_icon_size              = gSettings.value("IconSize",                                   16)#.toInt()
    ## self.settings_general_mdi_bg_use_logo        = gSettings.value("MdiBGUseLogo",                             True)#.toBool()
    ## self.settings_general_mdi_bg_use_texture     = gSettings.value("MdiBGUseTexture",                          True)#.toBool()
    ## self.settings_general_mdi_bg_use_color       = gSettings.value("MdiBGUseColor",                            True)#.toBool()
    ## self.settings_general_mdi_bg_logo            = gSettings.value("MdiBGLogo",           "images/logo-spirals.png")#.toString()
    ## self.settings_general_mdi_bg_texture         = gSettings.value("MdiBGTexture",     "images/texture-spirals.png")#.toString()
    ## self.settings_general_mdi_bg_color           = gSettings.value("MdiBGColor",                  qRgb(192,192,192))#.toInt()
    ## self.settings_general_tip_of_the_day         = gSettings.value("TipOfTheDay",                                 1)#.toBool()
    ## self.settings_general_current_tip            = gSettings.value("CurrentTip",                                  0)#.toInt()
    ## # Display
    ## self.settings_display_use_opengl             = gSettings.value("Display/UseOpenGL",                       False)#.toBool()
    ## self.settings_display_renderhint_aa          = gSettings.value("Display/RenderHintAntiAlias",             False)#.toBool()
    ## self.settings_display_renderhint_text_aa     = gSettings.value("Display/RenderHintTextAntiAlias",         False)#.toBool()
    ## self.settings_display_renderhint_smooth_pix  = gSettings.value("Display/RenderHintSmoothPixmap",          False)#.toBool()
    ## self.settings_display_renderhint_high_aa     = gSettings.value("Display/RenderHintHighQualityAntiAlias",  False)#.toBool()
    ## self.settings_display_renderhint_noncosmetic = gSettings.value("Display/RenderHintNonCosmetic",           False)#.toBool()
    ## self.settings_display_show_scrollbars        = gSettings.value("Display/ShowScrollBars",                   True)#.toBool()
    ## self.settings_display_scrollbar_widget_num   = gSettings.value("Display/ScrollBarWidgetNum",                  0)#.toInt()
    ## self.settings_display_crosshair_color        = gSettings.value("Display/CrossHairColor",      qRgb(  0,  0,  0))#.toInt()
    ## self.settings_display_bg_color               = gSettings.value("Display/BackgroundColor",     qRgb(235,235,235))#.toInt()
    ## self.settings_display_selectbox_left_color   = gSettings.value("Display/SelectBoxLeftColor",  qRgb(  0,128,  0))#.toInt()
    ## self.settings_display_selectbox_left_fill    = gSettings.value("Display/SelectBoxLeftFill",   qRgb(  0,255,  0))#.toInt()
    ## self.settings_display_selectbox_right_color  = gSettings.value("Display/SelectBoxRightColor", qRgb(  0,  0,128))#.toInt()
    ## self.settings_display_selectbox_right_fill   = gSettings.value("Display/SelectBoxRightFill",  qRgb(  0,  0,255))#.toInt()
    ## self.settings_display_selectbox_alpha        = gSettings.value("Display/SelectBoxAlpha",                     32)#.toInt()
    ## self.settings_display_zoomscale_in           = gSettings.value("Display/ZoomScaleIn",                       2.0)#.toFloat()
    ## self.settings_display_zoomscale_out          = gSettings.value("Display/ZoomScaleOut",                      0.5)#.toFloat()
    ## self.settings_display_crosshair_percent      = gSettings.value("Display/CrossHairPercent",                    5)#.toInt()
    ## self.settings_display_units                  = gSettings.value("Display/Units",                            "mm")#.toString()
    ## # Prompt
    ## self.settings_prompt_text_color              = gSettings.value("Prompt/TextColor",            qRgb(  0,  0,  0))#.toInt()
    ## self.settings_prompt_bg_color                = gSettings.value("Prompt/BackgroundColor",      qRgb(255,255,255))#.toInt()
    ## self.settings_prompt_font_family             = gSettings.value("Prompt/FontFamily",                 "Monospace")#.toString()
    ## self.settings_prompt_font_style              = gSettings.value("Prompt/FontStyle",                     "normal")#.toString()
    ## self.settings_prompt_font_size               = gSettings.value("Prompt/FontSize",                            12)#.toInt()
    ## self.settings_prompt_save_history            = gSettings.value("Prompt/SaveHistory",                       True)#.toBool()
    ## self.settings_prompt_save_history_as_html    = gSettings.value("Prompt/SaveHistoryAsHtml",                False)#.toBool()
    ## self.settings_prompt_save_history_filename   = gSettings.value("Prompt/SaveHistoryFilename",       "prompt.log")#.toString()
    ## # OpenSave
    ## self.settings_opensave_custom_filter         = gSettings.value("OpenSave/CustomFilter",             "supported")#.toString()
    ## self.settings_opensave_open_format           = gSettings.value("OpenSave/OpenFormat",                     "*.*")#.toString()
    ## self.settings_opensave_open_thumbnail        = gSettings.value("OpenSave/OpenThumbnail",                  False)#.toBool()
    ## self.settings_opensave_save_format           = gSettings.value("OpenSave/SaveFormat",                     "*.*")#.toString()
    ## self.settings_opensave_save_thumbnail        = gSettings.value("OpenSave/SaveThumbnail",                  False)#.toBool()
    ## # Recent
    ## self.settings_opensave_recent_max_files      = gSettings.value("OpenSave/RecentMax",                         10)#.toInt()
    ## self.settings_opensave_recent_list_of_files  = gSettings.value("OpenSave/RecentFiles")                          #.toStringList()
    ## self.settings_opensave_recent_directory      = gSettings.value("OpenSave/RecentDirectory")                      #.toString()
    ## # Trimming
    ## self.settings_opensave_trim_dst_num_jumps    = gSettings.value("OpenSave/TrimDstNumJumps",                    5)#.toInt()
    ## # Printing
    ## self.settings_printing_default_device        = gSettings.value("Printing/DefaultDevice",                     "")#.toString()
    ## self.settings_printing_use_last_device       = gSettings.value("Printing/UseLastDevice",                  False)#.toBool()
    ## self.settings_printing_disable_bg            = gSettings.value("Printing/DisableBG",                       True)#.toBool()
    ## # Grid
    ## self.settings_grid_show_on_load              = gSettings.value("Grid/ShowOnLoad",                          True)#.toBool()
    ## self.settings_grid_show_origin               = gSettings.value("Grid/ShowOrigin",                          True)#.toBool()
    ## self.settings_grid_color_match_crosshair     = gSettings.value("Grid/ColorMatchCrossHair",                 True)#.toBool()
    ## self.settings_grid_color                     = gSettings.value("Grid/Color",                  qRgb(  0,  0,  0))#.toInt()
    ## self.settings_grid_load_from_file            = gSettings.value("Grid/LoadFromFile",                        True)#.toBool()
    ## self.settings_grid_type                      = gSettings.value("Grid/Type",                       "Rectangular")#.toString()
    ## self.settings_grid_center_on_origin          = gSettings.value("Grid/CenterOnOrigin",                      True)#.toBool()
    ## self.settings_grid_center_x                  = gSettings.value("Grid/CenterX",                              0.0)#.toFloat()
    ## self.settings_grid_center_y                  = gSettings.value("Grid/CenterY",                              0.0)#.toFloat()
    ## self.settings_grid_size_x                    = gSettings.value("Grid/SizeX",                              100.0)#.toFloat()
    ## self.settings_grid_size_y                    = gSettings.value("Grid/SizeY",                              100.0)#.toFloat()
    ## self.settings_grid_spacing_x                 = gSettings.value("Grid/SpacingX",                            25.0)#.toFloat()
    ## self.settings_grid_spacing_y                 = gSettings.value("Grid/SpacingY",                            25.0)#.toFloat()
    ## self.settings_grid_size_radius               = gSettings.value("Grid/SizeRadius",                          50.0)#.toFloat()
    ## self.settings_grid_spacing_radius            = gSettings.value("Grid/SpacingRadius",                       25.0)#.toFloat()
    ## self.settings_grid_spacing_angle             = gSettings.value("Grid/SpacingAngle",                        45.0)#.toFloat()
    ## # Ruler
    ## self.settings_ruler_show_on_load             = gSettings.value("Ruler/ShowOnLoad",                         True)#.toBool()
    ## self.settings_ruler_metric                   = gSettings.value("Ruler/Metric",                             True)#.toBool()
    ## self.settings_ruler_color                    = gSettings.value("Ruler/Color",                 qRgb(210,210, 50))#.toInt()
    ## self.settings_ruler_pixel_size               = gSettings.value("Ruler/PixelSize",                            20)#.toInt()
    ## # Quick Snap
    ## self.settings_qsnap_enabled                  = gSettings.value("QuickSnap/Enabled",                        True)#.toBool()
    ## self.settings_qsnap_locator_color            = gSettings.value("QuickSnap/LocatorColor",      qRgb(255,255,  0))#.toInt()
    ## self.settings_qsnap_locator_size             = gSettings.value("QuickSnap/LocatorSize",                       4)#.toInt()
    ## self.settings_qsnap_aperture_size            = gSettings.value("QuickSnap/ApertureSize",                     10)#.toInt()
    ## self.settings_qsnap_endpoint                 = gSettings.value("QuickSnap/EndPoint",                       True)#.toBool()
    ## self.settings_qsnap_midpoint                 = gSettings.value("QuickSnap/MidPoint",                       True)#.toBool()
    ## self.settings_qsnap_center                   = gSettings.value("QuickSnap/Center",                         True)#.toBool()
    ## self.settings_qsnap_node                     = gSettings.value("QuickSnap/Node",                           True)#.toBool()
    ## self.settings_qsnap_quadrant                 = gSettings.value("QuickSnap/Quadrant",                       True)#.toBool()
    ## self.settings_qsnap_intersection             = gSettings.value("QuickSnap/Intersection",                   True)#.toBool()
    ## self.settings_qsnap_extension                = gSettings.value("QuickSnap/Extension",                      True)#.toBool()
    ## self.settings_qsnap_insertion                = gSettings.value("QuickSnap/Insertion",                     False)#.toBool()
    ## self.settings_qsnap_perpendicular            = gSettings.value("QuickSnap/Perpendicular",                  True)#.toBool()
    ## self.settings_qsnap_tangent                  = gSettings.value("QuickSnap/Tangent",                        True)#.toBool()
    ## self.settings_qsnap_nearest                  = gSettings.value("QuickSnap/Nearest",                       False)#.toBool()
    ## self.settings_qsnap_apparent                 = gSettings.value("QuickSnap/Apparent",                      False)#.toBool()
    ## self.settings_qsnap_parallel                 = gSettings.value("QuickSnap/Parallel",                      False)#.toBool()
    ## # LineWeight
    ## self.settings_lwt_show_lwt                   = gSettings.value("LineWeight/ShowLineWeight",               False)#.toBool()
    ## self.settings_lwt_real_render                = gSettings.value("LineWeight/RealRender",                    True)#.toBool()
    ## self.settings_lwt_default_lwt                = gSettings.value("LineWeight/DefaultLineWeight",                0)#.toReal()
    ## # Selection
    ## self.settings_selection_mode_pickfirst       = gSettings.value("Selection/PickFirst",                      True)#.toBool()
    ## self.settings_selection_mode_pickadd         = gSettings.value("Selection/PickAdd",                        True)#.toBool()
    ## self.settings_selection_mode_pickdrag        = gSettings.value("Selection/PickDrag",                      False)#.toBool()
    ## self.settings_selection_coolgrip_color       = gSettings.value("Selection/CoolGripColor",     qRgb(  0,  0,255))#.toInt()
    ## self.settings_selection_hotgrip_color        = gSettings.value("Selection/HotGripColor",      qRgb(255,  0,  0))#.toInt()
    ## self.settings_selection_grip_size            = gSettings.value("Selection/GripSize",                          4)#.toInt()
    ## self.settings_selection_pickbox_size         = gSettings.value("Selection/PickBoxSize",                       4)#.toInt()
    ## # Text
    ## self.settings_text_font                      = gSettings.value("Text/Font",                             "Arial")#.toString()
    ## self.settings_text_size                      = gSettings.value("Text/Size",                                  12)#.toReal()
    ## self.settings_text_angle                     = gSettings.value("Text/Angle",                                  0)#.toReal()
    ## self.settings_text_style_bold                = gSettings.value("Text/StyleBold",                          False)#.toBool()
    ## self.settings_text_style_italic              = gSettings.value("Text/StyleItalic",                        False)#.toBool()
    ## self.settings_text_style_underline           = gSettings.value("Text/StyleUnderline",                     False)#.toBool()
    ## self.settings_text_style_strikeout           = gSettings.value("Text/StyleStrikeOut",                     False)#.toBool()
    ## self.settings_text_style_overline            = gSettings.value("Text/StyleOverline",                      False)#.toBool()

    ########################


    if not os.path.exists(self.gAppDir + os.sep + 'settings-py.ini'):  # Default
        qDebug('gSettings-Default')
        self.layoutState = QByteArray()
        pos = QPoint(0, 0)      #.toPoint()
        size = QSize(800, 600)  #.toSize()

        # General
        self.settings_general_language               =                    "default"  #.toString()
        self.settings_general_icon_theme             =                    "default"  #.toString()
        self.settings_general_icon_size              =                           16  #.toInt()
        self.settings_general_mdi_bg_use_logo        =                         True  #.toBool()
        self.settings_general_mdi_bg_use_texture     =                         True  #.toBool()
        self.settings_general_mdi_bg_use_color       =                         True  #.toBool()
        self.settings_general_mdi_bg_logo            =    "images/logo-spirals.png"  #.toString()
        self.settings_general_mdi_bg_texture         = "images/texture-spirals.png"  #.toString()
        self.settings_general_mdi_bg_color           =            qRgb(192,192,192)  #.toInt()
        self.settings_general_tip_of_the_day         =                            1  #.toBool()
        self.settings_general_current_tip            =                            0  #.toInt()
        # Display
        self.settings_display_use_opengl             =             False  #.toBool()
        self.settings_display_renderhint_aa          =             False  #.toBool()
        self.settings_display_renderhint_text_aa     =             False  #.toBool()
        self.settings_display_renderhint_smooth_pix  =             False  #.toBool()
        self.settings_display_renderhint_high_aa     =             False  #.toBool()
        self.settings_display_renderhint_noncosmetic =             False  #.toBool()
        self.settings_display_show_scrollbars        =              True  #.toBool()
        self.settings_display_scrollbar_widget_num   =                 0  #.toInt()
        self.settings_display_crosshair_color        = qRgb(  0,  0,  0)  #.toInt()
        self.settings_display_bg_color               = qRgb(235,235,235)  #.toInt()
        self.settings_display_selectbox_left_color   = qRgb(  0,128,  0)  #.toInt()
        self.settings_display_selectbox_left_fill    = qRgb(  0,255,  0)  #.toInt()
        self.settings_display_selectbox_right_color  = qRgb(  0,  0,128)  #.toInt()
        self.settings_display_selectbox_right_fill   = qRgb(  0,  0,255)  #.toInt()
        self.settings_display_selectbox_alpha        =                32  #.toInt()
        self.settings_display_zoomscale_in           =               2.0  #.toFloat()
        self.settings_display_zoomscale_out          =               0.5  #.toFloat()
        self.settings_display_crosshair_percent      =                 5  #.toInt()
        self.settings_display_units                  =              "mm"  #.toString()
        # Prompt
        self.settings_prompt_text_color              = qRgb(  0,  0,  0)  #.toInt()
        self.settings_prompt_bg_color                = qRgb(255,255,255)  #.toInt()
        self.settings_prompt_font_family             =       "Monospace"  #.toString()
        self.settings_prompt_font_style              =          "normal"  #.toString()
        self.settings_prompt_font_size               =                12  #.toInt()
        self.settings_prompt_save_history            =              True  #.toBool()
        self.settings_prompt_save_history_as_html    =             False  #.toBool()
        self.settings_prompt_save_history_filename   =      "prompt.log"  #.toString()
        # OpenSave
        self.settings_opensave_custom_filter         =       "supported"  #.toString()
        self.settings_opensave_open_format           =             "*.*"  #.toString()
        self.settings_opensave_open_thumbnail        =             False  #.toBool()
        self.settings_opensave_save_format           =             "*.*"  #.toString()
        self.settings_opensave_save_thumbnail        =             False  #.toBool()
        # Recent
        self.settings_opensave_recent_max_files      =  10                #.toInt()
        self.settings_opensave_recent_list_of_files  =  deque(maxlen=10)  #.toStringList()
        self.settings_opensave_recent_directory      =  ""                #.toString()
        # Trimming
        self.settings_opensave_trim_dst_num_jumps    =   5                #.toInt()
        # Printing
        self.settings_printing_default_device        =    ""              #.toString()
        self.settings_printing_use_last_device       = False              #.toBool()
        self.settings_printing_disable_bg            =  True              #.toBool()
        # Grid
        self.settings_grid_show_on_load              =              True  #.toBool()
        self.settings_grid_show_origin               =              True  #.toBool()
        self.settings_grid_color_match_crosshair     =              True  #.toBool()
        self.settings_grid_color                     = qRgb(  0,  0,  0)  #.toInt()
        self.settings_grid_load_from_file            =              True  #.toBool()
        self.settings_grid_type                      =     "Rectangular"  #.toString()
        self.settings_grid_center_on_origin          =              True  #.toBool()
        self.settings_grid_center_x                  =               0.0  #.toFloat()
        self.settings_grid_center_y                  =               0.0  #.toFloat()
        self.settings_grid_size_x                    =             100.0  #.toFloat()
        self.settings_grid_size_y                    =             100.0  #.toFloat()
        self.settings_grid_spacing_x                 =              25.0  #.toFloat()
        self.settings_grid_spacing_y                 =              25.0  #.toFloat()
        self.settings_grid_size_radius               =              50.0  #.toFloat()
        self.settings_grid_spacing_radius            =              25.0  #.toFloat()
        self.settings_grid_spacing_angle             =              45.0  #.toFloat()
        # Ruler
        self.settings_ruler_show_on_load             =              True  #.toBool()
        self.settings_ruler_metric                   =              True  #.toBool()
        self.settings_ruler_color                    = qRgb(210,210, 50)  #.toInt()
        self.settings_ruler_pixel_size               =                20  #.toInt()
        # Quick Snap
        self.settings_qsnap_enabled                  =              True  #.toBool()
        self.settings_qsnap_locator_color            = qRgb(255,255,  0)  #.toInt()
        self.settings_qsnap_locator_size             =                 4  #.toInt()
        self.settings_qsnap_aperture_size            =                10  #.toInt()
        self.settings_qsnap_endpoint                 =              True  #.toBool()
        self.settings_qsnap_midpoint                 =              True  #.toBool()
        self.settings_qsnap_center                   =              True  #.toBool()
        self.settings_qsnap_node                     =              True  #.toBool()
        self.settings_qsnap_quadrant                 =              True  #.toBool()
        self.settings_qsnap_intersection             =              True  #.toBool()
        self.settings_qsnap_extension                =              True  #.toBool()
        self.settings_qsnap_insertion                =             False  #.toBool()
        self.settings_qsnap_perpendicular            =              True  #.toBool()
        self.settings_qsnap_tangent                  =              True  #.toBool()
        self.settings_qsnap_nearest                  =             False  #.toBool()
        self.settings_qsnap_apparent                 =             False  #.toBool()
        self.settings_qsnap_parallel                 =             False  #.toBool()
        # LineWeight
        self.settings_lwt_show_lwt                   = False              #.toBool()
        self.settings_lwt_real_render                =  True              #.toBool()
        self.settings_lwt_default_lwt                =     0              #.toReal()
        # Selection
        self.settings_selection_mode_pickfirst       =              True  #.toBool()
        self.settings_selection_mode_pickadd         =              True  #.toBool()
        self.settings_selection_mode_pickdrag        =             False  #.toBool()
        self.settings_selection_coolgrip_color       = qRgb(  0,  0,255)  #.toInt()
        self.settings_selection_hotgrip_color        = qRgb(255,  0,  0)  #.toInt()
        self.settings_selection_grip_size            =                 4  #.toInt()
        self.settings_selection_pickbox_size         =                 4  #.toInt()
        # Text
        self.settings_text_font                      = "Arial"  #.toString()
        self.settings_text_size                      =      12  #.toReal()
        self.settings_text_angle                     =       0  #.toReal()
        self.settings_text_style_bold                =   False  #.toBool()
        self.settings_text_style_italic              =   False  #.toBool()
        self.settings_text_style_underline           =   False  #.toBool()
        self.settings_text_style_strikeout           =   False  #.toBool()
        self.settings_text_style_overline            =   False  #.toBool()

    else:
        gSettings.read(self.gAppDir + os.sep + 'settings-py.ini')
        qDebug('gSettings-Saved')

        # from array import array
        # self.layoutState = array("B", gSettings.get("General", "LayoutState")) #.toByteArray()
        self.layoutState = QByteArray(gSettings.get("General", "LayoutState")) #.toByteArray()
        if not self.layoutState: # None
            qDebug("LayoutState NOT restored! Setting Default Layout...")
            # someToolBar.setVisible(True)
        else:
            self.restoreState(self.layoutState)

        try:
            pos = QPoint(gSettings.getint("Window", "PositionX"), gSettings.getint("Window", "PositionY"))              #.toPoint()
            size = QSize(gSettings.getint("Window", "SizeX"), gSettings.getint("Window", "SizeY"))                      #.toSize()
        except (configparser.NoOptionError, ValueError):
            pointStr = gSettings.get("Window", "Position")
            pos = QPoint(int(pointStr[pointStr.find('(') + 1:pointStr.find(' ')]), int(pointStr[pointStr.find(' ') + 1:-1]))
            sizeStr = gSettings.get("Window", "Position")
            size = QSize(int(pointStr[sizeStr.find('(') + 1:sizeStr.find(' ')]), int(pointStr[sizeStr.find(' ') + 1:-1]))

        # General
        self.settings_general_language               = gSettings.get("General", "Language")                       #.toString()
        self.settings_general_icon_theme             = gSettings.get("General", "IconTheme")                      #.toString()
        self.settings_general_icon_size              = gSettings.getint("General", "IconSize")                    #.toInt()
        self.settings_general_mdi_bg_use_logo        = gSettings.getboolean("General", "MdiBGUseLogo")            #.toBool()
        self.settings_general_mdi_bg_use_texture     = gSettings.getboolean("General", "MdiBGUseTexture")         #.toBool()
        self.settings_general_mdi_bg_use_color       = gSettings.getboolean("General", "MdiBGUseColor")           #.toBool()
        self.settings_general_mdi_bg_logo            = gSettings.get("General", "MdiBGLogo")                      #.toString()
        self.settings_general_mdi_bg_texture         = gSettings.get("General", "MdiBGTexture")                   #.toString()
        self.settings_general_mdi_bg_color           = gSettings.getint("General", "MdiBGColor")                  #.toInt()
        self.settings_general_tip_of_the_day         = gSettings.getboolean("General", "TipOfTheDay")             #.toBool()
        self.settings_general_current_tip            = gSettings.getint("General", "CurrentTip")                  #.toInt()
        # Display
        self.settings_display_use_opengl             = gSettings.getboolean("Display", "UseOpenGL")               #.toBool()
        self.settings_display_renderhint_aa          = gSettings.getboolean("Display", "RenderHintAntiAlias")     #.toBool()
        self.settings_display_renderhint_text_aa     = gSettings.getboolean("Display", "RenderHintTextAntiAlias") #.toBool()
        self.settings_display_renderhint_smooth_pix  = gSettings.getboolean("Display", "RenderHintSmoothPixmap")  #.toBool()
        self.settings_display_renderhint_high_aa     = gSettings.getboolean("Display", "RenderHintHighQualityAntiAlias")#.toBool()
        self.settings_display_renderhint_noncosmetic = gSettings.getboolean("Display", "RenderHintNonCosmetic")   #.toBool()
        self.settings_display_show_scrollbars        = gSettings.getboolean("Display", "ShowScrollBars")          #.toBool()
        self.settings_display_scrollbar_widget_num   = gSettings.getint("Display", "ScrollBarWidgetNum")          #.toInt()
        self.settings_display_crosshair_color        = gSettings.getint("Display", "CrossHairColor")              #.toInt()
        self.settings_display_bg_color               = gSettings.getint("Display", "BackgroundColor")             #.toInt()
        self.settings_display_selectbox_left_color   = gSettings.getint("Display", "SelectBoxLeftColor")          #.toInt()
        self.settings_display_selectbox_left_fill    = gSettings.getint("Display", "SelectBoxLeftFill")           #.toInt()
        self.settings_display_selectbox_right_color  = gSettings.getint("Display", "SelectBoxRightColor")         #.toInt()
        self.settings_display_selectbox_right_fill   = gSettings.getint("Display", "SelectBoxRightFill")          #.toInt()
        self.settings_display_selectbox_alpha        = gSettings.getint("Display", "SelectBoxAlpha")              #.toInt()
        self.settings_display_zoomscale_in           = gSettings.getfloat("Display", "ZoomScaleIn")               #.toFloat()
        self.settings_display_zoomscale_out          = gSettings.getfloat("Display", "ZoomScaleOut")              #.toFloat()
        self.settings_display_crosshair_percent      = gSettings.getint("Display", "CrossHairPercent")            #.toInt()
        self.settings_display_units                  = gSettings.get("Display", "Units")                          #.toString()
        # Prompt
        self.settings_prompt_text_color              = gSettings.getint("Prompt", "TextColor")                    #.toInt()
        self.settings_prompt_bg_color                = gSettings.getint("Prompt", "BackgroundColor")              #.toInt()
        self.settings_prompt_font_family             = gSettings.get("Prompt", "FontFamily")                      #.toString()
        self.settings_prompt_font_style              = gSettings.get("Prompt", "FontStyle")                       #.toString()
        self.settings_prompt_font_size               = gSettings.getint("Prompt", "FontSize")                     #.toInt()
        self.settings_prompt_save_history            = gSettings.getboolean("Prompt", "SaveHistory")              #.toBool()
        self.settings_prompt_save_history_as_html    = gSettings.getboolean("Prompt", "SaveHistoryAsHtml")        #.toBool()
        self.settings_prompt_save_history_filename   = gSettings.get("Prompt", "SaveHistoryFilename")             #.toString()
        # OpenSave
        self.settings_opensave_custom_filter         = gSettings.get("OpenSave", "CustomFilter")                  #.toString()
        self.settings_opensave_open_format           = gSettings.get("OpenSave", "OpenFormat")                    #.toString()
        self.settings_opensave_open_thumbnail        = gSettings.getboolean("OpenSave", "OpenThumbnail")          #.toBool()
        self.settings_opensave_save_format           = gSettings.get("OpenSave", "SaveFormat")                    #.toString()
        self.settings_opensave_save_thumbnail        = gSettings.getboolean("OpenSave", "SaveThumbnail")          #.toBool()
        # Recent
        self.settings_opensave_recent_max_files      = gSettings.getint("OpenSave", "RecentMax")                  #.toInt()
        self.settings_opensave_recent_list_of_files  = eval(gSettings.get("OpenSave", "RecentFiles"))             #.toStringList()
        self.settings_opensave_recent_directory      = gSettings.get("OpenSave", "RecentDirectory")               #.toString()
        # Trimming
        self.settings_opensave_trim_dst_num_jumps    = gSettings.getint("OpenSave", "TrimDstNumJumps")            #.toInt()
        # Printing
        self.settings_printing_default_device        = gSettings.get("Printing", "DefaultDevice")                 #.toString()
        self.settings_printing_use_last_device       = gSettings.getboolean("Printing", "UseLastDevice")          #.toBool()
        self.settings_printing_disable_bg            = gSettings.getboolean("Printing", "DisableBG")              #.toBool()
        # Grid
        self.settings_grid_show_on_load              = gSettings.getboolean("Grid", "ShowOnLoad")                 #.toBool()
        self.settings_grid_show_origin               = gSettings.getboolean("Grid", "ShowOrigin")                 #.toBool()
        self.settings_grid_color_match_crosshair     = gSettings.getboolean("Grid", "ColorMatchCrossHair")        #.toBool()
        self.settings_grid_color                     = gSettings.getint("Grid", "Color")                          #.toInt()
        self.settings_grid_load_from_file            = gSettings.getboolean("Grid", "LoadFromFile")               #.toBool()
        self.settings_grid_type                      = gSettings.get("Grid", "Type")                              #.toString()
        self.settings_grid_center_on_origin          = gSettings.getboolean("Grid", "CenterOnOrigin")             #.toBool()
        self.settings_grid_center_x                  = gSettings.getfloat("Grid", "CenterX")                      #.toFloat()
        self.settings_grid_center_y                  = gSettings.getfloat("Grid", "CenterY")                      #.toFloat()
        self.settings_grid_size_x                    = gSettings.getfloat("Grid", "SizeX")                        #.toFloat()
        self.settings_grid_size_y                    = gSettings.getfloat("Grid", "SizeY")                        #.toFloat()
        self.settings_grid_spacing_x                 = gSettings.getfloat("Grid", "SpacingX")                     #.toFloat()
        self.settings_grid_spacing_y                 = gSettings.getfloat("Grid", "SpacingY")                     #.toFloat()
        self.settings_grid_size_radius               = gSettings.getfloat("Grid", "SizeRadius")                   #.toFloat()
        self.settings_grid_spacing_radius            = gSettings.getfloat("Grid", "SpacingRadius")                #.toFloat()
        self.settings_grid_spacing_angle             = gSettings.getfloat("Grid", "SpacingAngle")                 #.toFloat()
        # Ruler
        self.settings_ruler_show_on_load             = gSettings.getboolean("Ruler", "ShowOnLoad")                #.toBool()
        self.settings_ruler_metric                   = gSettings.getboolean("Ruler", "Metric")                    #.toBool()
        self.settings_ruler_color                    = gSettings.getint("Ruler", "Color")                         #.toInt()
        self.settings_ruler_pixel_size               = gSettings.getint("Ruler", "PixelSize")                     #.toInt()
        # Quick Snap
        self.settings_qsnap_enabled                  = gSettings.getboolean("QuickSnap", "Enabled")               #.toBool()
        self.settings_qsnap_locator_color            = gSettings.getint("QuickSnap", "LocatorColor")              #.toInt()
        self.settings_qsnap_locator_size             = gSettings.getint("QuickSnap", "LocatorSize")               #.toInt()
        self.settings_qsnap_aperture_size            = gSettings.getint("QuickSnap", "ApertureSize")              #.toInt()
        self.settings_qsnap_endpoint                 = gSettings.getboolean("QuickSnap", "EndPoint")              #.toBool()
        self.settings_qsnap_midpoint                 = gSettings.getboolean("QuickSnap", "MidPoint")              #.toBool()
        self.settings_qsnap_center                   = gSettings.getboolean("QuickSnap", "Center")                #.toBool()
        self.settings_qsnap_node                     = gSettings.getboolean("QuickSnap", "Node")                  #.toBool()
        self.settings_qsnap_quadrant                 = gSettings.getboolean("QuickSnap", "Quadrant")              #.toBool()
        self.settings_qsnap_intersection             = gSettings.getboolean("QuickSnap", "Intersection")          #.toBool()
        self.settings_qsnap_extension                = gSettings.getboolean("QuickSnap", "Extension")             #.toBool()
        self.settings_qsnap_insertion                = gSettings.getboolean("QuickSnap", "Insertion")             #.toBool()
        self.settings_qsnap_perpendicular            = gSettings.getboolean("QuickSnap", "Perpendicular")         #.toBool()
        self.settings_qsnap_tangent                  = gSettings.getboolean("QuickSnap", "Tangent")               #.toBool()
        self.settings_qsnap_nearest                  = gSettings.getboolean("QuickSnap", "Nearest")               #.toBool()
        self.settings_qsnap_apparent                 = gSettings.getboolean("QuickSnap", "Apparent")              #.toBool()
        self.settings_qsnap_parallel                 = gSettings.getboolean("QuickSnap", "Parallel")              #.toBool()
        # LineWeight
        self.settings_lwt_show_lwt                   = gSettings.getboolean("LineWeight", "ShowLineWeight")       #.toBool()
        self.settings_lwt_real_render                = gSettings.getboolean("LineWeight", "RealRender")           #.toBool()
        self.settings_lwt_default_lwt                = gSettings.getfloat("LineWeight", "DefaultLineWeight")      #.toReal()
        # Selection
        self.settings_selection_mode_pickfirst       = gSettings.getboolean("Selection", "PickFirst")             #.toBool()
        self.settings_selection_mode_pickadd         = gSettings.getboolean("Selection", "PickAdd")               #.toBool()
        self.settings_selection_mode_pickdrag        = gSettings.getboolean("Selection", "PickDrag")              #.toBool()
        self.settings_selection_coolgrip_color       = gSettings.getint("Selection", "CoolGripColor")             #.toInt()
        self.settings_selection_hotgrip_color        = gSettings.getint("Selection", "HotGripColor")              #.toInt()
        self.settings_selection_grip_size            = gSettings.getint("Selection", "GripSize")                  #.toInt()
        self.settings_selection_pickbox_size         = gSettings.getint("Selection", "PickBoxSize")               #.toInt()
        # Text
        self.settings_text_font                      = gSettings.get("Text", "Font")                              #.toString()
        self.settings_text_size                      = gSettings.getfloat("Text", "Size")                         #.toReal()
        self.settings_text_angle                     = gSettings.getfloat("Text", "Angle")                        #.toReal()
        self.settings_text_style_bold                = gSettings.getboolean("Text", "StyleBold")                  #.toBool()
        self.settings_text_style_italic              = gSettings.getboolean("Text", "StyleItalic")                #.toBool()
        self.settings_text_style_underline           = gSettings.getboolean("Text", "StyleUnderline")             #.toBool()
        self.settings_text_style_strikeout           = gSettings.getboolean("Text", "StyleStrikeOut")             #.toBool()
        self.settings_text_style_overline            = gSettings.getboolean("Text", "StyleOverline")              #.toBool()


    self.move(pos.x(), pos.y())
    self.resize(size.width(), size.height())


def writeSettings(self):
    """ TOWRITE """ # void MainWindow::

    qDebug("Writing Settings...")
    ## settings = QSettings("settings-py.ini", QSettings.IniFormat)
    # gSettings = self.gSettings
    #
    # gSettings.setValue("Window/Position", self.pos())
    # gSettings.setValue("Window/Size", self.size())
    #
    # # General
    # gSettings.setValue("LayoutState",                               self.layoutState)
    # gSettings.setValue("Language",                                  self.settings_general_language)
    # gSettings.setValue("IconTheme",                                 self.settings_general_icon_theme)
    # gSettings.setValue("IconSize",                              int(self.settings_general_icon_size))
    # gSettings.setValue("MdiBGUseLogo",                              self.settings_general_mdi_bg_use_logo)
    # gSettings.setValue("MdiBGUseTexture",                           self.settings_general_mdi_bg_use_texture)
    # gSettings.setValue("MdiBGUseColor",                             self.settings_general_mdi_bg_use_color)
    # gSettings.setValue("MdiBGLogo",                                 self.settings_general_mdi_bg_logo)
    # gSettings.setValue("MdiBGTexture",                              self.settings_general_mdi_bg_texture)
    # gSettings.setValue("MdiBGColor",                            str(self.settings_general_mdi_bg_color))
    # gSettings.setValue("TipOfTheDay",                               self.settings_general_tip_of_the_day)
    # gSettings.setValue("CurrentTip",                            str(int(self.settings_general_current_tip) + 1))
    # # Display
    # gSettings.setValue("Display/UseOpenGL",                         self.settings_display_use_opengl)
    # gSettings.setValue("Display/RenderHintAntiAlias",               self.settings_display_renderhint_aa)
    # gSettings.setValue("Display/RenderHintTextAntiAlias",           self.settings_display_renderhint_text_aa)
    # gSettings.setValue("Display/RenderHintSmoothPixmap",            self.settings_display_renderhint_smooth_pix)
    # gSettings.setValue("Display/RenderHintHighQualityAntiAlias",    self.settings_display_renderhint_high_aa)
    # gSettings.setValue("Display/RenderHintNonCosmetic",             self.settings_display_renderhint_noncosmetic)
    # gSettings.setValue("Display/ShowScrollBars",                    self.settings_display_show_scrollbars)
    # gSettings.setValue("Display/ScrollBarWidgetNum",            str(self.settings_display_scrollbar_widget_num))
    # gSettings.setValue("Display/CrossHairColor",                str(self.settings_display_crosshair_color))
    # gSettings.setValue("Display/BackgroundColor",               str(self.settings_display_bg_color))
    # gSettings.setValue("Display/SelectBoxLeftColor",            str(self.settings_display_selectbox_left_color))
    # gSettings.setValue("Display/SelectBoxLeftFill",             str(self.settings_display_selectbox_left_fill))
    # gSettings.setValue("Display/SelectBoxRightColor",           str(self.settings_display_selectbox_right_color))
    # gSettings.setValue("Display/SelectBoxRightFill",            str(self.settings_display_selectbox_right_fill))
    # gSettings.setValue("Display/SelectBoxAlpha",                str(self.settings_display_selectbox_alpha))
    # gSettings.setValue("Display/ZoomScaleIn",                   str(self.settings_display_zoomscale_in))
    # gSettings.setValue("Display/ZoomScaleOut",                  str(self.settings_display_zoomscale_out))
    # gSettings.setValue("Display/CrossHairPercent",              str(self.settings_display_crosshair_percent))
    # gSettings.setValue("Display/Units",                             self.settings_display_units)
    # # Prompt
    # gSettings.setValue("Prompt/TextColor",                      str(self.settings_prompt_text_color))
    # gSettings.setValue("Prompt/BackgroundColor",                str(self.settings_prompt_bg_color))
    # gSettings.setValue("Prompt/FontFamily",                         self.settings_prompt_font_family)
    # gSettings.setValue("Prompt/FontStyle",                          self.settings_prompt_font_style)
    # gSettings.setValue("Prompt/FontSize",                       str(self.settings_prompt_font_size))
    # gSettings.setValue("Prompt/SaveHistory",                        self.settings_prompt_save_history)
    # gSettings.setValue("Prompt/SaveHistoryAsHtml",                  self.settings_prompt_save_history_as_html)
    # gSettings.setValue("Prompt/SaveHistoryFilename",                self.settings_prompt_save_history_filename)
    # # OpenSave
    # gSettings.setValue("OpenSave/CustomFilter",                     self.settings_opensave_custom_filter)
    # gSettings.setValue("OpenSave/OpenFormat",                       self.settings_opensave_open_format)
    # gSettings.setValue("OpenSave/OpenThumbnail",                    self.settings_opensave_open_thumbnail)
    # gSettings.setValue("OpenSave/SaveFormat",                       self.settings_opensave_save_format)
    # gSettings.setValue("OpenSave/SaveThumbnail",                    self.settings_opensave_save_thumbnail)
    # # Recent
    # gSettings.setValue("OpenSave/RecentMax",                    str(self.settings_opensave_recent_max_files))
    # gSettings.setValue("OpenSave/RecentFiles",                      self.settings_opensave_recent_list_of_files)
    # gSettings.setValue("OpenSave/RecentDirectory",                  self.settings_opensave_recent_directory)
    # # Trimming
    # gSettings.setValue("OpenSave/TrimDstNumJumps",              str(self.settings_opensave_trim_dst_num_jumps))
    # # Printing
    # gSettings.setValue("Printing/DefaultDevice",                    self.settings_printing_default_device)
    # gSettings.setValue("Printing/UseLastDevice",                    self.settings_printing_use_last_device)
    # gSettings.setValue("Printing/DisableBG",                        self.settings_printing_disable_bg)
    # # Grid
    # gSettings.setValue("Grid/ShowOnLoad",                           self.settings_grid_show_on_load)
    # gSettings.setValue("Grid/ShowOrigin",                           self.settings_grid_show_origin)
    # gSettings.setValue("Grid/ColorMatchCrossHair",                  self.settings_grid_color_match_crosshair)
    # gSettings.setValue("Grid/Color",                            str(self.settings_grid_color))
    # gSettings.setValue("Grid/LoadFromFile",                         self.settings_grid_load_from_file)
    # gSettings.setValue("Grid/Type",                                 self.settings_grid_type)
    # gSettings.setValue("Grid/CenterOnOrigin",                       self.settings_grid_center_on_origin)
    # gSettings.setValue("Grid/CenterX",                          str(self.settings_grid_center_x))
    # gSettings.setValue("Grid/CenterY",                          str(self.settings_grid_center_y))
    # gSettings.setValue("Grid/SizeX",                            str(self.settings_grid_size_x))
    # gSettings.setValue("Grid/SizeY",                            str(self.settings_grid_size_y))
    # gSettings.setValue("Grid/SpacingX",                         str(self.settings_grid_spacing_x))
    # gSettings.setValue("Grid/SpacingY",                         str(self.settings_grid_spacing_y))
    # gSettings.setValue("Grid/SizeRadius",                       str(self.settings_grid_size_radius))
    # gSettings.setValue("Grid/SpacingRadius",                    str(self.settings_grid_spacing_radius))
    # gSettings.setValue("Grid/SpacingAngle",                     str(self.settings_grid_spacing_angle))
    # # Ruler
    # gSettings.setValue("Ruler/ShowOnLoad",                          self.settings_ruler_show_on_load)
    # gSettings.setValue("Ruler/Metric",                              self.settings_ruler_metric)
    # gSettings.setValue("Ruler/Color",                           str(self.settings_ruler_color))
    # gSettings.setValue("Ruler/PixelSize",                       str(self.settings_ruler_pixel_size))
    # # Quick Snap
    # gSettings.setValue("QuickSnap/Enabled",                         self.settings_qsnap_enabled)
    # gSettings.setValue("QuickSnap/LocatorColor",                str(self.settings_qsnap_locator_color))
    # gSettings.setValue("QuickSnap/LocatorSize",                 str(self.settings_qsnap_locator_size))
    # gSettings.setValue("QuickSnap/ApertureSize",                str(self.settings_qsnap_aperture_size))
    # gSettings.setValue("QuickSnap/EndPoint",                        self.settings_qsnap_endpoint)
    # gSettings.setValue("QuickSnap/MidPoint",                        self.settings_qsnap_midpoint)
    # gSettings.setValue("QuickSnap/Center",                          self.settings_qsnap_center)
    # gSettings.setValue("QuickSnap/Node",                            self.settings_qsnap_node)
    # gSettings.setValue("QuickSnap/Quadrant",                        self.settings_qsnap_quadrant)
    # gSettings.setValue("QuickSnap/Intersection",                    self.settings_qsnap_intersection)
    # gSettings.setValue("QuickSnap/Extension",                       self.settings_qsnap_extension)
    # gSettings.setValue("QuickSnap/Insertion",                       self.settings_qsnap_insertion)
    # gSettings.setValue("QuickSnap/Perpendicular",                   self.settings_qsnap_perpendicular)
    # gSettings.setValue("QuickSnap/Tangent",                         self.settings_qsnap_tangent)
    # gSettings.setValue("QuickSnap/Nearest",                         self.settings_qsnap_nearest)
    # gSettings.setValue("QuickSnap/Apparent",                        self.settings_qsnap_apparent)
    # gSettings.setValue("QuickSnap/Parallel",                        self.settings_qsnap_parallel)
    # # LineWeight
    # gSettings.setValue("LineWeight/ShowLineWeight",                 self.settings_lwt_show_lwt)
    # gSettings.setValue("LineWeight/RealRender",                     self.settings_lwt_real_render)
    # gSettings.setValue("LineWeight/DefaultLineWeight",          str(self.settings_lwt_default_lwt))
    # # Selection
    # gSettings.setValue("Selection/PickFirst",                       self.settings_selection_mode_pickfirst)
    # gSettings.setValue("Selection/PickAdd",                         self.settings_selection_mode_pickadd)
    # gSettings.setValue("Selection/PickDrag",                        self.settings_selection_mode_pickdrag)
    # gSettings.setValue("Selection/CoolGripColor",               str(self.settings_selection_coolgrip_color))
    # gSettings.setValue("Selection/HotGripColor",                str(self.settings_selection_hotgrip_color))
    # gSettings.setValue("Selection/GripSize",                    str(self.settings_selection_grip_size))
    # gSettings.setValue("Selection/PickBoxSize",                 str(self.settings_selection_pickbox_size))
    # # Text
    # gSettings.setValue("Text/Font",                                 self.settings_text_font)
    # gSettings.setValue("Text/Size",                             str(self.settings_text_size))
    # gSettings.setValue("Text/Angle",                            str(self.settings_text_angle))
    # gSettings.setValue("Text/StyleBold",                            self.settings_text_style_bold)
    # gSettings.setValue("Text/StyleItalic",                          self.settings_text_style_italic)
    # gSettings.setValue("Text/StyleUnderline",                       self.settings_text_style_underline)
    # gSettings.setValue("Text/StyleStrikeOut",                       self.settings_text_style_strikeout)
    # gSettings.setValue("Text/StyleOverline",                        self.settings_text_style_overline)


    if not gSettings.has_section("Window"):
        gSettings.add_section("Window")
    gSettings.set("Window", "PositionX", self.pos().x())
    gSettings.set("Window", "PositionY", self.pos().y())
    gSettings.set("Window", "SizeX", self.size().width())
    gSettings.set("Window", "SizeY", self.size().height())

    # General
    if not gSettings.has_section("General"):
        gSettings.add_section("General")
    gSettings.set("General", "LayoutState",                     str(self.layoutState))
    gSettings.set("General", "Language",                        str(self.settings_general_language))
    gSettings.set("General", "IconTheme",                       str(self.settings_general_icon_theme))
    gSettings.set("General", "IconSize",                        str(self.settings_general_icon_size))
    gSettings.set("General", "MdiBGUseLogo",                    str(self.settings_general_mdi_bg_use_logo))
    gSettings.set("General", "MdiBGUseTexture",                 str(self.settings_general_mdi_bg_use_texture))
    gSettings.set("General", "MdiBGUseColor",                   str(self.settings_general_mdi_bg_use_color))
    gSettings.set("General", "MdiBGLogo",                       str(self.settings_general_mdi_bg_logo))
    gSettings.set("General", "MdiBGTexture",                    str(self.settings_general_mdi_bg_texture))
    gSettings.set("General", "MdiBGColor",                      str(self.settings_general_mdi_bg_color))
    gSettings.set("General", "TipOfTheDay",                     str(self.settings_general_tip_of_the_day))
    gSettings.set("General", "CurrentTip",                  str(int(self.settings_general_current_tip) + 1))
    # Display
    if not gSettings.has_section("Display"):
        gSettings.add_section("Display")
    gSettings.set("Display", "UseOpenGL",                       str(self.settings_display_use_opengl))
    gSettings.set("Display", "RenderHintAntiAlias",             str(self.settings_display_renderhint_aa))
    gSettings.set("Display", "RenderHintTextAntiAlias",         str(self.settings_display_renderhint_text_aa))
    gSettings.set("Display", "RenderHintSmoothPixmap",          str(self.settings_display_renderhint_smooth_pix))
    gSettings.set("Display", "RenderHintHighQualityAntiAlias",  str(self.settings_display_renderhint_high_aa))
    gSettings.set("Display", "RenderHintNonCosmetic",           str(self.settings_display_renderhint_noncosmetic))
    gSettings.set("Display", "ShowScrollBars",                  str(self.settings_display_show_scrollbars))
    gSettings.set("Display", "ScrollBarWidgetNum",              str(self.settings_display_scrollbar_widget_num))
    gSettings.set("Display", "CrossHairColor",                  str(self.settings_display_crosshair_color))
    gSettings.set("Display", "BackgroundColor",                 str(self.settings_display_bg_color))
    gSettings.set("Display", "SelectBoxLeftColor",              str(self.settings_display_selectbox_left_color))
    gSettings.set("Display", "SelectBoxLeftFill",               str(self.settings_display_selectbox_left_fill))
    gSettings.set("Display", "SelectBoxRightColor",             str(self.settings_display_selectbox_right_color))
    gSettings.set("Display", "SelectBoxRightFill",              str(self.settings_display_selectbox_right_fill))
    gSettings.set("Display", "SelectBoxAlpha",                  str(self.settings_display_selectbox_alpha))
    gSettings.set("Display", "ZoomScaleIn",                     str(self.settings_display_zoomscale_in))
    gSettings.set("Display", "ZoomScaleOut",                    str(self.settings_display_zoomscale_out))
    gSettings.set("Display", "CrossHairPercent",                str(self.settings_display_crosshair_percent))
    gSettings.set("Display", "Units",                           str(self.settings_display_units))
    # Prompt
    if not gSettings.has_section("Prompt"):
        gSettings.add_section("Prompt")
    gSettings.set("Prompt", "TextColor",                        str(self.settings_prompt_text_color))
    gSettings.set("Prompt", "BackgroundColor",                  str(self.settings_prompt_bg_color))
    gSettings.set("Prompt", "FontFamily",                       str(self.settings_prompt_font_family))
    gSettings.set("Prompt", "FontStyle",                        str(self.settings_prompt_font_style))
    gSettings.set("Prompt", "FontSize",                         str(self.settings_prompt_font_size))
    gSettings.set("Prompt", "SaveHistory",                      str(self.settings_prompt_save_history))
    gSettings.set("Prompt", "SaveHistoryAsHtml",                str(self.settings_prompt_save_history_as_html))
    gSettings.set("Prompt", "SaveHistoryFilename",              str(self.settings_prompt_save_history_filename))
    # OpenSave
    if not gSettings.has_section("OpenSave"):
        gSettings.add_section("OpenSave")
    gSettings.set("OpenSave", "CustomFilter",                   str(self.settings_opensave_custom_filter))
    gSettings.set("OpenSave", "OpenFormat",                     str(self.settings_opensave_open_format))
    gSettings.set("OpenSave", "OpenThumbnail",                  str(self.settings_opensave_open_thumbnail))
    gSettings.set("OpenSave", "SaveFormat",                     str(self.settings_opensave_save_format))
    gSettings.set("OpenSave", "SaveThumbnail",                  str(self.settings_opensave_save_thumbnail))
    # Recent
    gSettings.set("OpenSave", "RecentMax",                      str(self.settings_opensave_recent_max_files))
    gSettings.set("OpenSave", "RecentFiles",                    str(self.settings_opensave_recent_list_of_files))
    gSettings.set("OpenSave", "RecentDirectory",                str(self.settings_opensave_recent_directory))
    # Trimming
    gSettings.set("OpenSave", "TrimDstNumJumps",                str(self.settings_opensave_trim_dst_num_jumps))
    # Printing
    if not gSettings.has_section("Printing"):
        gSettings.add_section("Printing")
    gSettings.set("Printing", "DefaultDevice",                  str(self.settings_printing_default_device))
    gSettings.set("Printing", "UseLastDevice",                  str(self.settings_printing_use_last_device))
    gSettings.set("Printing", "DisableBG",                      str(self.settings_printing_disable_bg))
    # Grid
    if not gSettings.has_section("Grid"):
        gSettings.add_section("Grid")
    gSettings.set("Grid", "ShowOnLoad",                         str(self.settings_grid_show_on_load))
    gSettings.set("Grid", "ShowOrigin",                         str(self.settings_grid_show_origin))
    gSettings.set("Grid", "ColorMatchCrossHair",                str(self.settings_grid_color_match_crosshair))
    gSettings.set("Grid", "Color",                              str(self.settings_grid_color))
    gSettings.set("Grid", "LoadFromFile",                       str(self.settings_grid_load_from_file))
    gSettings.set("Grid", "Type",                               str(self.settings_grid_type))
    gSettings.set("Grid", "CenterOnOrigin",                     str(self.settings_grid_center_on_origin))
    gSettings.set("Grid", "CenterX",                           str(self.settings_grid_center_x))
    gSettings.set("Grid", "CenterY",                           str(self.settings_grid_center_y))
    gSettings.set("Grid", "SizeX",                             str(self.settings_grid_size_x))
    gSettings.set("Grid", "SizeY",                             str(self.settings_grid_size_y))
    gSettings.set("Grid", "SpacingX",                          str(self.settings_grid_spacing_x))
    gSettings.set("Grid", "SpacingY",                          str(self.settings_grid_spacing_y))
    gSettings.set("Grid", "SizeRadius",                        str(self.settings_grid_size_radius))
    gSettings.set("Grid", "SpacingRadius",                     str(self.settings_grid_spacing_radius))
    gSettings.set("Grid", "SpacingAngle",                      str(self.settings_grid_spacing_angle))
    # Ruler
    if not gSettings.has_section("Ruler"):
        gSettings.add_section("Ruler")
    gSettings.set("Ruler", "ShowOnLoad",                       str(self.settings_ruler_show_on_load))
    gSettings.set("Ruler", "Metric",                           str(self.settings_ruler_metric))
    gSettings.set("Ruler", "Color",                            str(self.settings_ruler_color))
    gSettings.set("Ruler", "PixelSize",                        str(self.settings_ruler_pixel_size))
    # Quick Snap
    if not gSettings.has_section("QuickSnap"):
        gSettings.add_section("QuickSnap")
    gSettings.set("QuickSnap", "Enabled",                      str(self.settings_qsnap_enabled))
    gSettings.set("QuickSnap", "LocatorColor",                 str(self.settings_qsnap_locator_color))
    gSettings.set("QuickSnap", "LocatorSize",                  str(self.settings_qsnap_locator_size))
    gSettings.set("QuickSnap", "ApertureSize",                 str(self.settings_qsnap_aperture_size))
    gSettings.set("QuickSnap", "EndPoint",                     str(self.settings_qsnap_endpoint))
    gSettings.set("QuickSnap", "MidPoint",                     str(self.settings_qsnap_midpoint))
    gSettings.set("QuickSnap", "Center",                       str(self.settings_qsnap_center))
    gSettings.set("QuickSnap", "Node",                         str(self.settings_qsnap_node))
    gSettings.set("QuickSnap", "Quadrant",                     str(self.settings_qsnap_quadrant))
    gSettings.set("QuickSnap", "Intersection",                 str(self.settings_qsnap_intersection))
    gSettings.set("QuickSnap", "Extension",                    str(self.settings_qsnap_extension))
    gSettings.set("QuickSnap", "Insertion",                    str(self.settings_qsnap_insertion))
    gSettings.set("QuickSnap", "Perpendicular",                str(self.settings_qsnap_perpendicular))
    gSettings.set("QuickSnap", "Tangent",                      str(self.settings_qsnap_tangent))
    gSettings.set("QuickSnap", "Nearest",                      str(self.settings_qsnap_nearest))
    gSettings.set("QuickSnap", "Apparent",                     str(self.settings_qsnap_apparent))
    gSettings.set("QuickSnap", "Parallel",                     str(self.settings_qsnap_parallel))
    # LineWeight
    if not gSettings.has_section("LineWeight"):
        gSettings.add_section("LineWeight")
    gSettings.set("LineWeight", "ShowLineWeight",              str(self.settings_lwt_show_lwt))
    gSettings.set("LineWeight", "RealRender",                  str(self.settings_lwt_real_render))
    gSettings.set("LineWeight", "DefaultLineWeight",           str(self.settings_lwt_default_lwt))
    # Selection
    if not gSettings.has_section("Selection"):
        gSettings.add_section("Selection")
    gSettings.set("Selection", "PickFirst",                    str(self.settings_selection_mode_pickfirst))
    gSettings.set("Selection", "PickAdd",                      str(self.settings_selection_mode_pickadd))
    gSettings.set("Selection", "PickDrag",                     str(self.settings_selection_mode_pickdrag))
    gSettings.set("Selection", "CoolGripColor",                str(self.settings_selection_coolgrip_color))
    gSettings.set("Selection", "HotGripColor",                 str(self.settings_selection_hotgrip_color))
    gSettings.set("Selection", "GripSize",                     str(self.settings_selection_grip_size))
    gSettings.set("Selection", "PickBoxSize",                  str(self.settings_selection_pickbox_size))
    # Text
    if not gSettings.has_section("Text"):
        gSettings.add_section("Text")
    gSettings.set("Text", "Font",                              str(self.settings_text_font))
    gSettings.set("Text", "Size",                              str(self.settings_text_size))
    gSettings.set("Text", "Angle",                             str(self.settings_text_angle))
    gSettings.set("Text", "StyleBold",                         str(self.settings_text_style_bold))
    gSettings.set("Text", "StyleItalic",                       str(self.settings_text_style_italic))
    gSettings.set("Text", "StyleUnderline",                    str(self.settings_text_style_underline))
    gSettings.set("Text", "StyleStrikeOut",                    str(self.settings_text_style_strikeout))
    gSettings.set("Text", "StyleOverline",                     str(self.settings_text_style_overline))

    # Write the UserConfigurableSettings.ini file
    cfgfile = open(self.gAppDir + os.sep + 'settings-py.ini', 'w')
    gSettings.write(cfgfile)
    cfgfile.close()

    qDebug("Done writing settings.")


@Slot()
def settingsPrompt(self):
    """
    Show the Settings Dialog and open on the "Prompt" tab page.
    """
    self.settingsDialog("Prompt")


@Slot(str)
def settingsDialog(self, showTab="General"): # void MainWindow::
    """
    Show the Settings Dialog.

    :param `showTab`:
    :type `showTab`: QString
    """
    dialog = Settings_Dialog(self, showTab, self)
    dialog.exec_()

