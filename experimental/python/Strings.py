#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
MenuItem/ToolbarItem ShortHelp/LongHelp/Icon for Embroidermodder.
"""

from collections import OrderedDict

gHelpStringsDict = OrderedDict([
# MenuItem(with placed keyboard &)(will become shortHelp also), tuple(KeyboardShortcut, Bound Event/Action MethodName, Icon, LongHelp)


#---HelpMenu---
('&Help',               ('F1',              'OnHelp', 'help.png', 'Displays help.')),
('&Changelog',          ('',                'OnChangelog', 'changelog.png', 'Describes new features in this product.')),
('&Tip Of The Day',     ('',                'OnTipOfTheDay', 'tipoftheday.png', 'Displays a dialog with useful tips.')),
('&About',              ('',                'OnAbout', 'About', 'Displays information about this product.')),

# #---FileMenu---
# NewShortcut =                       'Ctrl+N'
# OpenShortcut =                      'Ctrl+O'
# SaveShortcut =                      'Ctrl+S'
# SaveAsShortcut =                    'Ctrl+Shift+S'
# PrintShortcut =                     'Ctrl+P'
# DetailsShortcut =                   'Ctrl+D'
# ExitShortcut =                      'Ctrl+Q'
# #---EditMenu---
# UndoShortcut =                      'Ctrl+Z'
# RedoShortcut =                      'Ctrl+Y'
# CutShortcut =                       'Ctrl+X'
# CopyShortcut =                      'Ctrl+C'
# PasteShortcut =                     'Ctrl+V'
# #---ViewMenu---
# ToggleFullscreenShortcut =          'F11'
# MaximizeShortcut =                  ''
# MinimizeShortcut =                  ''
# ToggleStatusBarShortcut =           ''
# ZoomRealtimeShortcut =              ''
# ZoomPreviousShortcut =              ''
# ZoomWindowShortcut =                ''
# ZoomDynamicShortcut =               ''
# ZoomScaleShortcut =                 ''
# ZoomCenterShortcut =                ''
# ZoomInShortcut =                    ''
# ZoomOutShortcut =                   ''
# ZoomSelectedShortcut =              ''
# ZoomAllShortcut =                   ''
# ZoomExtentsShortcut =               ''
# PanRealtimeShortcut =               ''
# PanPointShortcut =                  ''
# PanLeftShortcut =                   ''
# PanRightShortcut =                  ''
# PanUpShortcut =                     ''
# PanDownShortcut =                   ''
# ViewDayShortcut =                   ''
# ViewNightShortcut =                 ''
# #---ToolsMenu---
# DistanceShortcut =                  ''
# LocatePointShortcut =               ''
# #---DrawMenu---
# CircleShortcut =                    ''
# DolphinShortcut =                   ''
# EllipseShortcut =                   ''
# HeartShortcut =                     ''
# LineShortcut =                      ''
# PathShortcut =                      ''
# PointShortcut =                     ''
# PolygonShortcut =                   ''
# PolylineShortcut =                  ''
# RectangleShortcut =                 ''
# SingleLineTextShortcut =            ''
# SnowflakeShortcut =                 ''
# StarShortcut =                      ''
# #---SandboxMenu---
# RGBShortcut =                       ''
# SandboxShortcut =                   ''
# #---ModifyMenu---
# DeleteShortcut =                    ''
# MoveShortcut =                      ''
# RotateShortcut =                    ''
# ScaleShortcut =                     ''
# #---DimensionMenu---
# QuickLeaderShortcut =               ''
# #---SettingsMenu---
# SettingsShortcut =                  ''
# #---WindowMenu---
# WindowCloseShortcut =               'Ctrl+F4'
# WindowCloseAllShortcut =            ''
# WindowCascadeShortcut =             ''
# WindowTileShortcut =                ''
# WindowNextShortcut =                'Ctrl+Tab'
# WindowPreviousShortcut =            'Ctrl+Shift+Tab'
# #---HelpMenu---
# HelpShortcut =                      'F1'
# ChangelogShortcut =                 ''
# TipOfTheDayShortcut =               ''
# AboutEmbroidermodderShortcut =      ''


])


# kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on;
