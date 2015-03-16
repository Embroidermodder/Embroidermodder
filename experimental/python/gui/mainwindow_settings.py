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

#--PySide/PyQt Imports.

#include "mainwindow.h"
#include "settings-dialog.h"

#include <QDebug>
#include <QSettings>

try:
    from PySide.QtCore import qDebug, QPoint, QSettings, QSize
    from PySide.QtGui import qRgb
except ImportError:
    raise
#    from PyQt4.QtCore import qDebug, QPoint, QSettings, QSize
#    from PyQt4.QtGui import qRgb

#--Local Imports.
#TODO# from settings_dialog import Settings_Dialog



def DoReadSettings(self): # void MainWindow::
    """"""
    qDebug("Reading Settings...")
    theSettingsIniPath = self.gAppDir + os.sep + "settings-py.ini"

    self.gSettings = gSettings = QSettings(theSettingsIniPath, QSettings.IniFormat)
    pos = gSettings.value("Window/Position", QPoint(0, 0))#.toPoint()
    size = gSettings.value("Window/Size", QSize(800, 600))#.toSize()

    self.layoutState = gSettings.value("LayoutState")#.toByteArray()
    if not self.layoutState: # None
        qDebug("LayoutState NOT restored! Setting Default Layout...")
        # someToolBar.setVisible(True)
    else:
        self.restoreState(self.layoutState)

    # General
    self.settings_general_language               = gSettings.value("Language",                            "default")#.toString()
    self.settings_general_icon_theme             = gSettings.value("IconTheme",                           "default")#.toString()
    self.settings_general_icon_size              = gSettings.value("IconSize",                                   16)#.toInt()
    self.settings_general_mdi_bg_use_logo        = gSettings.value("MdiBGUseLogo",                             True)#.toBool()
    self.settings_general_mdi_bg_use_texture     = gSettings.value("MdiBGUseTexture",                          True)#.toBool()
    self.settings_general_mdi_bg_use_color       = gSettings.value("MdiBGUseColor",                            True)#.toBool()
    self.settings_general_mdi_bg_logo            = gSettings.value("MdiBGLogo",           "images/logo-spirals.png")#.toString()
    self.settings_general_mdi_bg_texture         = gSettings.value("MdiBGTexture",     "images/texture-spirals.png")#.toString()
    self.settings_general_mdi_bg_color           = gSettings.value("MdiBGColor",                  qRgb(192,192,192))#.toInt()
    self.settings_general_tip_of_the_day         = gSettings.value("TipOfTheDay",                                 1)#.toBool()
    self.settings_general_current_tip            = gSettings.value("CurrentTip",                                  0)#.toInt()
    # Display
    self.settings_display_use_opengl             = gSettings.value("Display/UseOpenGL",                       False)#.toBool()
    self.settings_display_renderhint_aa          = gSettings.value("Display/RenderHintAntiAlias",             False)#.toBool()
    self.settings_display_renderhint_text_aa     = gSettings.value("Display/RenderHintTextAntiAlias",         False)#.toBool()
    self.settings_display_renderhint_smooth_pix  = gSettings.value("Display/RenderHintSmoothPixmap",          False)#.toBool()
    self.settings_display_renderhint_high_aa     = gSettings.value("Display/RenderHintHighQualityAntiAlias",  False)#.toBool()
    self.settings_display_renderhint_noncosmetic = gSettings.value("Display/RenderHintNonCosmetic",           False)#.toBool()
    self.settings_display_show_scrollbars        = gSettings.value("Display/ShowScrollBars",                   True)#.toBool()
    self.settings_display_scrollbar_widget_num   = gSettings.value("Display/ScrollBarWidgetNum",                  0)#.toInt()
    self.settings_display_crosshair_color        = gSettings.value("Display/CrossHairColor",      qRgb(  0,  0,  0))#.toInt()
    self.settings_display_bg_color               = gSettings.value("Display/BackgroundColor",     qRgb(235,235,235))#.toInt()
    self.settings_display_selectbox_left_color   = gSettings.value("Display/SelectBoxLeftColor",  qRgb(  0,128,  0))#.toInt()
    self.settings_display_selectbox_left_fill    = gSettings.value("Display/SelectBoxLeftFill",   qRgb(  0,255,  0))#.toInt()
    self.settings_display_selectbox_right_color  = gSettings.value("Display/SelectBoxRightColor", qRgb(  0,  0,128))#.toInt()
    self.settings_display_selectbox_right_fill   = gSettings.value("Display/SelectBoxRightFill",  qRgb(  0,  0,255))#.toInt()
    self.settings_display_selectbox_alpha        = gSettings.value("Display/SelectBoxAlpha",                     32)#.toInt()
    self.settings_display_zoomscale_in           = gSettings.value("Display/ZoomScaleIn",                       2.0)#.toFloat()
    self.settings_display_zoomscale_out          = gSettings.value("Display/ZoomScaleOut",                      0.5)#.toFloat()
    self.settings_display_crosshair_percent      = gSettings.value("Display/CrossHairPercent",                    5)#.toInt()
    self.settings_display_units                  = gSettings.value("Display/Units",                            "mm")#.toString()
    # Prompt
    self.settings_prompt_text_color              = gSettings.value("Prompt/TextColor",            qRgb(  0,  0,  0))#.toInt()
    self.settings_prompt_bg_color                = gSettings.value("Prompt/BackgroundColor",      qRgb(255,255,255))#.toInt()
    self.settings_prompt_font_family             = gSettings.value("Prompt/FontFamily",                 "Monospace")#.toString()
    self.settings_prompt_font_style              = gSettings.value("Prompt/FontStyle",                     "normal")#.toString()
    self.settings_prompt_font_size               = gSettings.value("Prompt/FontSize",                            12)#.toInt()
    self.settings_prompt_save_history            = gSettings.value("Prompt/SaveHistory",                       True)#.toBool()
    self.settings_prompt_save_history_as_html    = gSettings.value("Prompt/SaveHistoryAsHtml",                False)#.toBool()
    self.settings_prompt_save_history_filename   = gSettings.value("Prompt/SaveHistoryFilename",       "prompt.log")#.toString()
    # OpenSave
    self.settings_opensave_custom_filter         = gSettings.value("OpenSave/CustomFilter",             "supported")#.toString()
    self.settings_opensave_open_format           = gSettings.value("OpenSave/OpenFormat",                     "*.*")#.toString()
    self.settings_opensave_open_thumbnail        = gSettings.value("OpenSave/OpenThumbnail",                  False)#.toBool()
    self.settings_opensave_save_format           = gSettings.value("OpenSave/SaveFormat",                     "*.*")#.toString()
    self.settings_opensave_save_thumbnail        = gSettings.value("OpenSave/SaveThumbnail",                  False)#.toBool()
    # Recent
    self.settings_opensave_recent_max_files      = gSettings.value("OpenSave/RecentMax",                         10)#.toInt()
    self.settings_opensave_recent_list_of_files  = gSettings.value("OpenSave/RecentFiles")                          #.toStringList()
    self.settings_opensave_recent_directory      = gSettings.value("OpenSave/RecentDirectory")                      #.toString()
    # Trimming
    self.settings_opensave_trim_dst_num_jumps    = gSettings.value("OpenSave/TrimDstNumJumps",                    5)#.toInt()
    # Printing
    self.settings_printing_default_device        = gSettings.value("Printing/DefaultDevice",                     "")#.toString()
    self.settings_printing_use_last_device       = gSettings.value("Printing/UseLastDevice",                  False)#.toBool()
    self.settings_printing_disable_bg            = gSettings.value("Printing/DisableBG",                       True)#.toBool()
    # Grid
    self.settings_grid_show_on_load              = gSettings.value("Grid/ShowOnLoad",                          True)#.toBool()
    self.settings_grid_show_origin               = gSettings.value("Grid/ShowOrigin",                          True)#.toBool()
    self.settings_grid_color_match_crosshair     = gSettings.value("Grid/ColorMatchCrossHair",                 True)#.toBool()
    self.settings_grid_color                     = gSettings.value("Grid/Color",                  qRgb(  0,  0,  0))#.toInt()
    self.settings_grid_load_from_file            = gSettings.value("Grid/LoadFromFile",                        True)#.toBool()
    self.settings_grid_type                      = gSettings.value("Grid/Type",                       "Rectangular")#.toString()
    self.settings_grid_center_on_origin          = gSettings.value("Grid/CenterOnOrigin",                      True)#.toBool()
    self.settings_grid_center_x                  = gSettings.value("Grid/CenterX",                              0.0)#.toFloat()
    self.settings_grid_center_y                  = gSettings.value("Grid/CenterY",                              0.0)#.toFloat()
    self.settings_grid_size_x                    = gSettings.value("Grid/SizeX",                              100.0)#.toFloat()
    self.settings_grid_size_y                    = gSettings.value("Grid/SizeY",                              100.0)#.toFloat()
    self.settings_grid_spacing_x                 = gSettings.value("Grid/SpacingX",                            25.0)#.toFloat()
    self.settings_grid_spacing_y                 = gSettings.value("Grid/SpacingY",                            25.0)#.toFloat()
    self.settings_grid_size_radius               = gSettings.value("Grid/SizeRadius",                          50.0)#.toFloat()
    self.settings_grid_spacing_radius            = gSettings.value("Grid/SpacingRadius",                       25.0)#.toFloat()
    self.settings_grid_spacing_angle             = gSettings.value("Grid/SpacingAngle",                        45.0)#.toFloat()
    # Ruler
    self.settings_ruler_show_on_load             = gSettings.value("Ruler/ShowOnLoad",                         True)#.toBool()
    self.settings_ruler_metric                   = gSettings.value("Ruler/Metric",                             True)#.toBool()
    self.settings_ruler_color                    = gSettings.value("Ruler/Color",                 qRgb(210,210, 50))#.toInt()
    self.settings_ruler_pixel_size               = gSettings.value("Ruler/PixelSize",                            20)#.toInt()
    # Quick Snap
    self.settings_qsnap_enabled                  = gSettings.value("QuickSnap/Enabled",                        True)#.toBool()
    self.settings_qsnap_locator_color            = gSettings.value("QuickSnap/LocatorColor",      qRgb(255,255,  0))#.toInt()
    self.settings_qsnap_locator_size             = gSettings.value("QuickSnap/LocatorSize",                       4)#.toInt()
    self.settings_qsnap_aperture_size            = gSettings.value("QuickSnap/ApertureSize",                     10)#.toInt()
    self.settings_qsnap_endpoint                 = gSettings.value("QuickSnap/EndPoint",                       True)#.toBool()
    self.settings_qsnap_midpoint                 = gSettings.value("QuickSnap/MidPoint",                       True)#.toBool()
    self.settings_qsnap_center                   = gSettings.value("QuickSnap/Center",                         True)#.toBool()
    self.settings_qsnap_node                     = gSettings.value("QuickSnap/Node",                           True)#.toBool()
    self.settings_qsnap_quadrant                 = gSettings.value("QuickSnap/Quadrant",                       True)#.toBool()
    self.settings_qsnap_intersection             = gSettings.value("QuickSnap/Intersection",                   True)#.toBool()
    self.settings_qsnap_extension                = gSettings.value("QuickSnap/Extension",                      True)#.toBool()
    self.settings_qsnap_insertion                = gSettings.value("QuickSnap/Insertion",                     False)#.toBool()
    self.settings_qsnap_perpendicular            = gSettings.value("QuickSnap/Perpendicular",                  True)#.toBool()
    self.settings_qsnap_tangent                  = gSettings.value("QuickSnap/Tangent",                        True)#.toBool()
    self.settings_qsnap_nearest                  = gSettings.value("QuickSnap/Nearest",                       False)#.toBool()
    self.settings_qsnap_apparent                 = gSettings.value("QuickSnap/Apparent",                      False)#.toBool()
    self.settings_qsnap_parallel                 = gSettings.value("QuickSnap/Parallel",                      False)#.toBool()
    # LineWeight
    self.settings_lwt_show_lwt                   = gSettings.value("LineWeight/ShowLineWeight",               False)#.toBool()
    self.settings_lwt_real_render                = gSettings.value("LineWeight/RealRender",                    True)#.toBool()
    self.settings_lwt_default_lwt                = gSettings.value("LineWeight/DefaultLineWeight",                0)#.toReal()
    # Selection
    self.settings_selection_mode_pickfirst       = gSettings.value("Selection/PickFirst",                      True)#.toBool()
    self.settings_selection_mode_pickadd         = gSettings.value("Selection/PickAdd",                        True)#.toBool()
    self.settings_selection_mode_pickdrag        = gSettings.value("Selection/PickDrag",                      False)#.toBool()
    self.settings_selection_coolgrip_color       = gSettings.value("Selection/CoolGripColor",     qRgb(  0,  0,255))#.toInt()
    self.settings_selection_hotgrip_color        = gSettings.value("Selection/HotGripColor",      qRgb(255,  0,  0))#.toInt()
    self.settings_selection_grip_size            = gSettings.value("Selection/GripSize",                          4)#.toInt()
    self.settings_selection_pickbox_size         = gSettings.value("Selection/PickBoxSize",                       4)#.toInt()
    # Text
    self.settings_text_font                      = gSettings.value("Text/Font",                             "Arial")#.toString()
    self.settings_text_size                      = gSettings.value("Text/Size",                                  12)#.toReal()
    self.settings_text_angle                     = gSettings.value("Text/Angle",                                  0)#.toReal()
    self.settings_text_style_bold                = gSettings.value("Text/StyleBold",                          False)#.toBool()
    self.settings_text_style_italic              = gSettings.value("Text/StyleItalic",                        False)#.toBool()
    self.settings_text_style_underline           = gSettings.value("Text/StyleUnderline",                     False)#.toBool()
    self.settings_text_style_strikeout           = gSettings.value("Text/StyleStrikeOut",                     False)#.toBool()
    self.settings_text_style_overline            = gSettings.value("Text/StyleOverline",                      False)#.toBool()

    self.move(pos.x(), pos.y())
    self.resize(size.width(), size.height())


def DoWriteSettings(self): # void MainWindow::

    qDebug("Writing Settings...")
    ## settings = QSettings("settings-py.ini", QSettings.IniFormat)
    gSettings = self.gSettings

    gSettings.setValue("Window/Position", self.pos())
    gSettings.setValue("Window/Size", self.size())

    # General
    gSettings.setValue("LayoutState",                               self.layoutState)
    gSettings.setValue("Language",                                  self.settings_general_language)
    gSettings.setValue("IconTheme",                                 self.settings_general_icon_theme)
    gSettings.setValue("IconSize",                               str(self.settings_general_icon_size))
    gSettings.setValue("MdiBGUseLogo",                              self.settings_general_mdi_bg_use_logo)
    gSettings.setValue("MdiBGUseTexture",                           self.settings_general_mdi_bg_use_texture)
    gSettings.setValue("MdiBGUseColor",                             self.settings_general_mdi_bg_use_color)
    gSettings.setValue("MdiBGLogo",                                 self.settings_general_mdi_bg_logo)
    gSettings.setValue("MdiBGTexture",                              self.settings_general_mdi_bg_texture)
    gSettings.setValue("MdiBGColor",                            str(self.settings_general_mdi_bg_color))
    gSettings.setValue("TipOfTheDay",                               self.settings_general_tip_of_the_day)
    gSettings.setValue("CurrentTip",                            str(int(self.settings_general_current_tip) + 1))
    # Display
    gSettings.setValue("Display/UseOpenGL",                         self.settings_display_use_opengl)
    gSettings.setValue("Display/RenderHintAntiAlias",               self.settings_display_renderhint_aa)
    gSettings.setValue("Display/RenderHintTextAntiAlias",           self.settings_display_renderhint_text_aa)
    gSettings.setValue("Display/RenderHintSmoothPixmap",            self.settings_display_renderhint_smooth_pix)
    gSettings.setValue("Display/RenderHintHighQualityAntiAlias",    self.settings_display_renderhint_high_aa)
    gSettings.setValue("Display/RenderHintNonCosmetic",             self.settings_display_renderhint_noncosmetic)
    gSettings.setValue("Display/ShowScrollBars",                    self.settings_display_show_scrollbars)
    gSettings.setValue("Display/ScrollBarWidgetNum",            str(self.settings_display_scrollbar_widget_num))
    gSettings.setValue("Display/CrossHairColor",                str(self.settings_display_crosshair_color))
    gSettings.setValue("Display/BackgroundColor",               str(self.settings_display_bg_color))
    gSettings.setValue("Display/SelectBoxLeftColor",            str(self.settings_display_selectbox_left_color))
    gSettings.setValue("Display/SelectBoxLeftFill",             str(self.settings_display_selectbox_left_fill))
    gSettings.setValue("Display/SelectBoxRightColor",           str(self.settings_display_selectbox_right_color))
    gSettings.setValue("Display/SelectBoxRightFill",            str(self.settings_display_selectbox_right_fill))
    gSettings.setValue("Display/SelectBoxAlpha",                str(self.settings_display_selectbox_alpha))
    gSettings.setValue("Display/ZoomScaleIn",                   str(self.settings_display_zoomscale_in))
    gSettings.setValue("Display/ZoomScaleOut",                  str(self.settings_display_zoomscale_out))
    gSettings.setValue("Display/CrossHairPercent",              str(self.settings_display_crosshair_percent))
    gSettings.setValue("Display/Units",                             self.settings_display_units)
    # Prompt
    gSettings.setValue("Prompt/TextColor",                      str(self.settings_prompt_text_color))
    gSettings.setValue("Prompt/BackgroundColor",                str(self.settings_prompt_bg_color))
    gSettings.setValue("Prompt/FontFamily",                         self.settings_prompt_font_family)
    gSettings.setValue("Prompt/FontStyle",                          self.settings_prompt_font_style)
    gSettings.setValue("Prompt/FontSize",                       str(self.settings_prompt_font_size))
    gSettings.setValue("Prompt/SaveHistory",                        self.settings_prompt_save_history)
    gSettings.setValue("Prompt/SaveHistoryAsHtml",                  self.settings_prompt_save_history_as_html)
    gSettings.setValue("Prompt/SaveHistoryFilename",                self.settings_prompt_save_history_filename)
    # OpenSave
    gSettings.setValue("OpenSave/CustomFilter",                     self.settings_opensave_custom_filter)
    gSettings.setValue("OpenSave/OpenFormat",                       self.settings_opensave_open_format)
    gSettings.setValue("OpenSave/OpenThumbnail",                    self.settings_opensave_open_thumbnail)
    gSettings.setValue("OpenSave/SaveFormat",                       self.settings_opensave_save_format)
    gSettings.setValue("OpenSave/SaveThumbnail",                    self.settings_opensave_save_thumbnail)
    # Recent
    gSettings.setValue("OpenSave/RecentMax",                    str(self.settings_opensave_recent_max_files))
    gSettings.setValue("OpenSave/RecentFiles",                      self.settings_opensave_recent_list_of_files)
    gSettings.setValue("OpenSave/RecentDirectory",                  self.settings_opensave_recent_directory)
    # Trimming
    gSettings.setValue("OpenSave/TrimDstNumJumps",              str(self.settings_opensave_trim_dst_num_jumps))
    # Printing
    gSettings.setValue("Printing/DefaultDevice",                    self.settings_printing_default_device)
    gSettings.setValue("Printing/UseLastDevice",                    self.settings_printing_use_last_device)
    gSettings.setValue("Printing/DisableBG",                        self.settings_printing_disable_bg)
    # Grid
    gSettings.setValue("Grid/ShowOnLoad",                           self.settings_grid_show_on_load)
    gSettings.setValue("Grid/ShowOrigin",                           self.settings_grid_show_origin)
    gSettings.setValue("Grid/ColorMatchCrossHair",                  self.settings_grid_color_match_crosshair)
    gSettings.setValue("Grid/Color",                            str(self.settings_grid_color))
    gSettings.setValue("Grid/LoadFromFile",                         self.settings_grid_load_from_file)
    gSettings.setValue("Grid/Type",                                 self.settings_grid_type)
    gSettings.setValue("Grid/CenterOnOrigin",                       self.settings_grid_center_on_origin)
    gSettings.setValue("Grid/CenterX",                          str(self.settings_grid_center_x))
    gSettings.setValue("Grid/CenterY",                          str(self.settings_grid_center_y))
    gSettings.setValue("Grid/SizeX",                            str(self.settings_grid_size_x))
    gSettings.setValue("Grid/SizeY",                            str(self.settings_grid_size_y))
    gSettings.setValue("Grid/SpacingX",                         str(self.settings_grid_spacing_x))
    gSettings.setValue("Grid/SpacingY",                         str(self.settings_grid_spacing_y))
    gSettings.setValue("Grid/SizeRadius",                       str(self.settings_grid_size_radius))
    gSettings.setValue("Grid/SpacingRadius",                    str(self.settings_grid_spacing_radius))
    gSettings.setValue("Grid/SpacingAngle",                     str(self.settings_grid_spacing_angle))
    # Ruler
    gSettings.setValue("Ruler/ShowOnLoad",                          self.settings_ruler_show_on_load)
    gSettings.setValue("Ruler/Metric",                              self.settings_ruler_metric)
    gSettings.setValue("Ruler/Color",                           str(self.settings_ruler_color))
    gSettings.setValue("Ruler/PixelSize",                       str(self.settings_ruler_pixel_size))
    # Quick Snap
    gSettings.setValue("QuickSnap/Enabled",                         self.settings_qsnap_enabled)
    gSettings.setValue("QuickSnap/LocatorColor",                str(self.settings_qsnap_locator_color))
    gSettings.setValue("QuickSnap/LocatorSize",                 str(self.settings_qsnap_locator_size))
    gSettings.setValue("QuickSnap/ApertureSize",                str(self.settings_qsnap_aperture_size))
    gSettings.setValue("QuickSnap/EndPoint",                        self.settings_qsnap_endpoint)
    gSettings.setValue("QuickSnap/MidPoint",                        self.settings_qsnap_midpoint)
    gSettings.setValue("QuickSnap/Center",                          self.settings_qsnap_center)
    gSettings.setValue("QuickSnap/Node",                            self.settings_qsnap_node)
    gSettings.setValue("QuickSnap/Quadrant",                        self.settings_qsnap_quadrant)
    gSettings.setValue("QuickSnap/Intersection",                    self.settings_qsnap_intersection)
    gSettings.setValue("QuickSnap/Extension",                       self.settings_qsnap_extension)
    gSettings.setValue("QuickSnap/Insertion",                       self.settings_qsnap_insertion)
    gSettings.setValue("QuickSnap/Perpendicular",                   self.settings_qsnap_perpendicular)
    gSettings.setValue("QuickSnap/Tangent",                         self.settings_qsnap_tangent)
    gSettings.setValue("QuickSnap/Nearest",                         self.settings_qsnap_nearest)
    gSettings.setValue("QuickSnap/Apparent",                        self.settings_qsnap_apparent)
    gSettings.setValue("QuickSnap/Parallel",                        self.settings_qsnap_parallel)
    # LineWeight
    gSettings.setValue("LineWeight/ShowLineWeight",                 self.settings_lwt_show_lwt)
    gSettings.setValue("LineWeight/RealRender",                     self.settings_lwt_real_render)
    gSettings.setValue("LineWeight/DefaultLineWeight",          str(self.settings_lwt_default_lwt))
    # Selection
    gSettings.setValue("Selection/PickFirst",                       self.settings_selection_mode_pickfirst)
    gSettings.setValue("Selection/PickAdd",                         self.settings_selection_mode_pickadd)
    gSettings.setValue("Selection/PickDrag",                        self.settings_selection_mode_pickdrag)
    gSettings.setValue("Selection/CoolGripColor",               str(self.settings_selection_coolgrip_color))
    gSettings.setValue("Selection/HotGripColor",                str(self.settings_selection_hotgrip_color))
    gSettings.setValue("Selection/GripSize",                    str(self.settings_selection_grip_size))
    gSettings.setValue("Selection/PickBoxSize",                 str(self.settings_selection_pickbox_size))
    # Text
    gSettings.setValue("Text/Font",                                 self.settings_text_font)
    gSettings.setValue("Text/Size",                             str(self.settings_text_size))
    gSettings.setValue("Text/Angle",                            str(self.settings_text_angle))
    gSettings.setValue("Text/StyleBold",                            self.settings_text_style_bold)
    gSettings.setValue("Text/StyleItalic",                          self.settings_text_style_italic)
    gSettings.setValue("Text/StyleUnderline",                       self.settings_text_style_underline)
    gSettings.setValue("Text/StyleStrikeOut",                       self.settings_text_style_strikeout)
    gSettings.setValue("Text/StyleOverline",                        self.settings_text_style_overline)


def settingsPrompt(self):
    """
    Show the Settings Dialog and open on the "Prompt" tab page.
    """
    settingsDialog(self, "Prompt")


def settingsDialog(self, showTab): # void MainWindow::
    """
    Show the Settings Dialog.

    :param `showTab`:
    :type `showTab`: QString
    """
    dialog = Settings_Dialog(self, showTab, self)
    dialog.exec_()


# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
