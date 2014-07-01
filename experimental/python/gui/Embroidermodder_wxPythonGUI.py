#!/usr/bin/env python
# -*- coding: utf-8 -*-

__doc__ = """
===============================================
|module_summary| Embroidermodder_wxPythonGUI.py
===============================================

Embroidermodder v2.0 wxPython GUI

"""

#-Imports.----------------------------------------------------------------------
#--Python Imports.
import os
import sys
import webbrowser

#--wxPython Imports.
# import wxversion
# wxversion.select('2.8-msw-unicode')
# wxversion.select('2.9.4-msw')
# wxversion.select('2.9.5-msw')
# wxversion.select('3.0-msw')
# wxversion.select('2.9.5-msw-phoenix')
# wxversion.select('3.0.1-msw-phoenix')
import wx
# Use pyshell/crust for interactive debugging.
import wx.py as py
# Use pure python agw aui because it gets bugfixes/is updated.
import wx.lib.agw.aui as aui

#--Local Imports.
from KeyboardShortcuts import *


#-Globals-----------------------------------------------------------------------
# Application Directories.
try:
    gFileDir = os.path.dirname(os.path.abspath(__file__))
except:
    gFileDir = os.path.dirname(os.path.abspath(sys.argv[0]))

## print(gAppDir)
## print(os.path.basename(gAppDir))
if os.path.basename(os.path.dirname(gFileDir)) == 'python':
    # From experimental/python/gui dir
    gAppDir = gFileDir + os.sep + '..' + os.sep + '..' + os.sep + '..' + os.sep + 'embroidermodder2'
elif os.path.basename(gFileDir) == 'embroidermodder2':
    # From embroidermodder2 dir
    gAppDir = gFileDir

gImgDir = gAppDir + os.sep + 'images'
gIconDir = gAppDir + os.sep + 'icons' + os.sep + 'default'

gSpiralsImgPath = gImgDir + os.sep + 'texture-spirals.png'
gLogoSpiralsImgPath = gImgDir + os.sep + 'logo-spirals.png'

# Is wxPython Project Phoenix(PY2/PY3)?
PHOENIX = True if 'phoenix' in wx.version() else False

# Define a translation function.
_ = wx.GetTranslation

# Python Version Info Strings
wxPythonVersion = wx.version()
major, minor, micro, release = sys.version_info[0:-1]
pythonVersion = u'%d.%d.%d-%s'%(major, minor, micro, release)
pyVersionInfos = u'%s %s\n%s %s' %(_(u'Python'), pythonVersion, _(u'wxPython'), wxPythonVersion)

DIR_WX = dir(wx)
for ID in ('ID_ICONIZE_FRAME', 'ID_MAXIMIZE_FRAME'):
    if not ID in DIR_WX: # Older wx version hasn't incorporated these IDs yet.
        wx.ID_ICONIZE_FRAME = wx.NewId()
        wx.ID_MAXIMIZE_FRAME = wx.NewId()
        break

# IDs
ID_DETAILS = wx.NewId()
ID_CHANGELOG = wx.NewId()
ID_TIPOFTHEDAY = wx.NewId()
ID_WINDOW_LAYOUT_CASCADE = wx.NewId()
ID_WINDOW_LAYOUT_TILE = wx.NewId()
ID_WINDOW_NEXT = wx.NewId()
ID_WINDOW_PREVIOUS = wx.NewId()
ID_ONLINE_WEBSITE = wx.NewId()
ID_PYVERSION_INFOS = wx.NewId()
ID_FULLSCREEN = wx.NewId()

ID_TOOLS_DISTANCE = wx.NewId()
ID_TOOLS_LOCATEPOINT = wx.NewId()
ID_DRAW_CIRCLE = wx.NewId()
ID_DRAW_DOLPHIN = wx.NewId()
ID_DRAW_ELLIPSE = wx.NewId()
ID_DRAW_HEART = wx.NewId()
ID_DRAW_LINE = wx.NewId()
ID_DRAW_PATH = wx.NewId()
ID_DRAW_POINT = wx.NewId()
ID_DRAW_POLYGON = wx.NewId()
ID_DRAW_POLYLINE = wx.NewId()
ID_DRAW_RECTANGLE = wx.NewId()
ID_DRAW_SINGLELINETEXT = wx.NewId()
ID_DRAW_SNOWFLAKE = wx.NewId()
ID_DRAW_STAR = wx.NewId()
ID_SANDBOX_RGB = wx.NewId()
ID_SANDBOX_SANDBOX = wx.NewId()
ID_MODIFY_DELETE = wx.NewId()
ID_MODIFY_MOVE = wx.NewId()
ID_MODIFY_ROTATE = wx.NewId()
ID_MODIFY_SCALE = wx.NewId()
ID_DIMENSION_QUICKLEADER = wx.NewId()
ID_VIEW_ZOOM_REALTIME = wx.NewId()
ID_VIEW_ZOOM_PREVIOUS = wx.NewId()
ID_VIEW_ZOOM_WINDOW = wx.NewId()
ID_VIEW_ZOOM_DYNAMIC = wx.NewId()
ID_VIEW_ZOOM_SCALE = wx.NewId()
ID_VIEW_ZOOM_CENTER = wx.NewId()
ID_VIEW_ZOOM_IN = wx.NewId()
ID_VIEW_ZOOM_OUT = wx.NewId()
ID_VIEW_ZOOM_SELECTED = wx.NewId()
ID_VIEW_ZOOM_ALL = wx.NewId()
ID_VIEW_ZOOM_EXTENTS = wx.NewId()
ID_VIEW_PAN_REALTIME = wx.NewId()
ID_VIEW_PAN_POINT = wx.NewId()
ID_VIEW_PAN_LEFT = wx.NewId()
ID_VIEW_PAN_RIGHT = wx.NewId()
ID_VIEW_PAN_UP = wx.NewId()
ID_VIEW_PAN_DOWN = wx.NewId()
ID_VIEW_DAY = wx.NewId()
ID_VIEW_NIGHT = wx.NewId()

# Wildcards for Open/Save Dialogs.
WILDCARD_SUPPORTED = str('All Supported Files (*.100;*.10o;*.art;*.bmc;*.bro;*.cnd;*.col;*.csd;*.csv;*.dem;*.dsb;*.dst;*.dsz;*.dxf;*.edr;*.emd;*.exp;*.exy;*.eys;*.fxy;*.gnc;*.gt;*.hus;*.inb;*.inf;*.jef;*.ksm;*.max;*.mit;*.new;*.ofm;*.pcd;*.pcm;*.pcs;*.pec;*.pel;*.pem;*.pes;*.phb;*.phc;*.plt;*.rgb;*.sew;*.sst;*.stx;*.svg;*.t09;*.tap;*.thr;*.u00;*.u01;*.vip;*.vp3;*.xxx;*.zsk)'
                                            '|*.100;*.10o;*.art;*.bmc;*.bro;*.cnd;*.col;*.csd;*.csv;*.dem;*.dsb;*.dst;*.dsz;*.dxf;*.edr;*.emd;*.exp;*.exy;*.eys;*.fxy;*.gnc;*.gt;*.hus;*.inb;*.inf;*.jef;*.ksm;*.max;*.mit;*.new;*.ofm;*.pcd;*.pcm;*.pcs;*.pec;*.pel;*.pem;*.pes;*.phb;*.phc;*.plt;*.rgb;*.sew;*.sst;*.stx;*.svg;*.t09;*.tap;*.thr;*.u00;*.u01;*.vip;*.vp3;*.xxx;*.zsk')
WILDCARD_ALL = str('All Files (*.*)|*.*|'
    '100 (*.100)|*.100|'
    '10o (*.10o)|*.10o|'
    'ART (*.art)|*.art|'
    'BMC (*.bmc)|*.bmc|'
    'BRO (*.bro)|*.bro|'
    'CND (*.cnd)|*.cnd|'
    'COL (*.col)|*.col|'
    'CSD (*.csd)|*.csd|'
    'CSV (*.csv)|*.csv|'
    'DAT (*.dat)|*.dat|'
    'DEM (*.dem)|*.dem|'
    'DSB (*.dsb)|*.dsb|'
    'DST (*.dst)|*.dst|'
    'DSZ (*.dsz)|*.dsz|'
    'DXF (*.dxf)|*.dxf|'
    'EDR (*.edr)|*.edr|'
    'EMD (*.emd)|*.emd|'
    'EXP (*.exp)|*.exp|'
    'EXY (*.exy)|*.exy|'
    'EYS (*.eys)|*.eys|'
    'FXY (*.fxy)|*.fxy|'
    'GNC (*.gnc)|*.gnc|'
    'GT (*.gt)|*.gt|'
    'HUS (*.hus)|*.hus|'
    'INB (*.inb)|*.inb|'
    'INF (*.inf)|*.inf|'
    'JEF (*.jef)|*.jef|'
    'KSM (*.ksm)|*.ksm|'
    'MAX (*.max)|*.max|'
    'MIT (*.mit)|*.mit|'
    'NEW (*.new)|*.new|'
    'OFM (*.ofm)|*.ofm|'
    'PCD (*.pcd)|*.pcd|'
    'PCM (*.pcm)|*.pcm|'
    'PCQ (*.pcq)|*.pcq|'
    'PCS (*.pcs)|*.pcs|'
    'PEC (*.pec)|*.pec|'
    'PEL (*.pel)|*.pel|'
    'PEM (*.pem)|*.pem|'
    'PES (*.pes)|*.pes|'
    'PHB (*.phb)|*.phb|'
    'PHC (*.phc)|*.phc|'
    'PLT (*.plt)|*.plt|'
    'RGB (*.rgb)|*.rgb|'
    'SEW (*.sew)|*.sew|'
    'SHV (*.shv)|*.shv|'
    'SST (*.sst)|*.sst|'
    'STX (*.stx)|*.stx|'
    'SVG (*.svg)|*.svg|'
    'T09 (*.t09)|*.t09|'
    'TAP (*.tap)|*.tap|'
    'THR (*.thr)|*.thr|'
    'TXT (*.txt)|*.txt|'
    'U00 (*.u00)|*.u00|'
    'U01 (*.u01)|*.u01|'
    'VIP (*.vip)|*.vip|'
    'VP3 (*.vp3)|*.vp3|'
    'XXX (*.xxx)|*.xxx|'
    'ZSK (*.zsk)|*.zsk')

# Define Colors.
EMBROIDERBLUE1 = wx.Colour(12, 106, 176) # '#0C6AB0'
EMBROIDERBLUE2 = wx.Colour(85, 196, 230) # '#55C4E6'

# Embedded Python Shell.
PYSHELL = 'crust'

class PyShellPanel(wx.Panel):
    """
    An Interactive Embedded Python Shell Class.
    """
    def __init__(self, parent, id=wx.ID_ANY,
                 pos=wx.DefaultPosition, size=wx.DefaultSize,
                 style=wx.BORDER_SUNKEN, name='panel'):
        """Default class constructor."""
        wx.Panel.__init__(self, parent, id, pos, size, style, name)

        self.CreatePyShell()

        vbSizer = wx.BoxSizer(wx.VERTICAL)
        try:
            vbSizer.Add(self.pythoncrust, 1, wx.EXPAND | wx.ALL, 5)
        except Exception:
            vbSizer.Add(self.pythonshell, 1, wx.EXPAND | wx.ALL, 5)
        self.SetSizer(vbSizer)

    def CreatePyShell(self):
        """Create and return the Python Shell."""
        if PYSHELL == 'crust': # Use pycrust
            self.pythoncrust = py.crust.Crust(self, id=wx.ID_ANY,
                    pos=wx.DefaultPosition, size=wx.DefaultSize,
                    style=4194304,
                    intro='Welcome To PyCrust %s - The Flakiest Python Shell' % (py.version.VERSION),
                    locals=None, InterpClass=None,
                    startupScript=None, execStartupScript=True)
            self.pythonshell = self.pythoncrust.shell
        else: # Use pyshell
            self.pythonshell = py.shell.Shell(self, id=wx.ID_ANY,
                    pos=wx.DefaultPosition, size=wx.DefaultSize,
                    style=4194304,
                    intro='Welcome To PyCrust %s - The Flakiest Python Shell' % (py.version.VERSION),
                    locals=None, InterpClass=None,
                    startupScript=None, execStartupScript=True)

        global gPyShell
        gPyShell = gMainWin.gPyShell = self.pythonshell

        self.pythonshell.SetHelpText(_(u'PyShell is an embedded interactive Python shell.' + '\n' +
                                     _(u'Useful for live debugging also.')))
        try:
            return self.pythoncrust
        except Exception as exc:
            return self.pythonshell


class EmbroidermodderPanel(wx.Panel):
    """"""
    def __init__(self, parent, id=wx.ID_ANY,
                 pos=wx.DefaultPosition, size=wx.DefaultSize,
                 style=wx.BORDER_SUNKEN, name='panel'):
        """Default class constructor."""
        wx.Panel.__init__(self, parent, id, pos, size, style, name)

        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_ERASE_BACKGROUND, self.OnEraseBackground)
        self.Bind(wx.EVT_PAINT, self.OnPaint)

        self.backgroundBitmap = wx.Bitmap(gSpiralsImgPath, wx.BITMAP_TYPE_PNG)
        self.backgroundLogo = wx.Bitmap(gLogoSpiralsImgPath, wx.BITMAP_TYPE_PNG)

        self.backgroundLogoW, self.backgroundLogoH  = self.backgroundLogo.GetSize()

        self.displaySizeBackgroundBmp = self.MakeDisplaySizeBackgroundBitmap(self.backgroundBitmap)

        try:
            self.SetBackgroundStyle(wx.BG_STYLE_PAINT)
        except AttributeError: # wx28
            self.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)
        ## self.SetDoubleBuffered(True)

        self.SetHelpText(_(u'Welcome to Embroidermodder!') +

                  '\n\n' + _(u'This is the startup window.') +
                  '\n' + _(u'You can choose to start a new design file,') +
                  '\n' + _(u'Open an existing design, or browse the ') +
                  '\n' + _(u'Embroidermodder Help files to get a start.') +

                  '\n\n' + _(u'Now Stitchify!')
                  )

        self.Bind(wx.EVT_LEFT_DCLICK, parent.OnOpen)


    def MakeDisplaySizeBackgroundBitmap(self, bitmap=None):
        """
        Create a Display-Size Bitmap for tiling on :class:`EmbroidermodderPanel`.

        :param `bitmap`: a `wx.Bitmap`.
        """
        width, height = wx.GetDisplaySize()

        if PHOENIX:
            bmp = wx.Bitmap(width, height)
        else: # Classic wxPython
            bmp = wx.EmptyBitmap(width, height)
        dc = wx.MemoryDC(bmp)

        bgBmp = bitmap or self.backgroundBitmap
        bmpW = bgBmp.GetWidth()
        bmpH = bgBmp.GetHeight()
        localDrawBitmap = dc.DrawBitmap
        [[localDrawBitmap(bgBmp, x, y, True)
            for y in range(0, height, bmpH)]
                for x in range(0, width, bmpW)]

        return dc.GetAsBitmap(wx.Rect(0, 0, width, height))

    def OnSize(self, event):
        """
        Handles the ``wx.EVT_SIZE`` event for :class:`EmbroidermodderPanel`.

        :param `event`: A `wx.SizeEvent`_ to be processed.
        """
        event.Skip()
        self.Refresh()

    def OnEraseBackground(self, event):
        """
        Handles the ``wx.EVT_ERASE_BACKGROUND`` event for :class:`EmbroidermodderPanel`.

        :param `event`: A `wx.EraseEvent`_ to be processed.
        """
        event.Skip() # essentially pass; Reduce Flicker because we are using a BufferedPaintDC

    def OnPaint(self, event):
        """
        Handles the ``wx.EVT_PAINT`` event for :class:`EmbroidermodderPanel`.

        :param `event`: A `wx.PaintEvent`_ to be processed.
        """
        event.Skip()
        cSizeX, cSizeY = self.GetClientSize()
        pdc = wx.BufferedPaintDC(self)
        pdc.Clear()
        pdc.DrawBitmap(self.displaySizeBackgroundBmp, 0, 0)
        if self.backgroundLogoW > cSizeX:
            # Proportional Scaling an Image.
            #
            # newWidth/oldWidth = newHeight/oldHeight
            #
            # Plug in the values that you know and solve
            # for the new dimension that you don't know.
            # Like this:
            #
            # newWidth/oldWidth = newHeight/oldHeight
            # oldHeight * newWidth/oldWidth = newHeight
            # newHeight = 300
            #
            # 400/800 = newHeight/600
            # 600 * 400/800 = newHeight
            # newHeight = 300

            newHeight = self.backgroundLogoH * cSizeX//self.backgroundLogoW
            scaledBmp = self.backgroundLogo.ConvertToImage().Scale(cSizeX, newHeight, wx.IMAGE_QUALITY_HIGH).ConvertToBitmap()
            pdc.DrawBitmap(scaledBmp, 0, cSizeY // 2 - scaledBmp.GetHeight() // 2)
        else:
            pdc.DrawBitmap(self.backgroundLogo, cSizeX // 2 - self.backgroundLogoW // 2, cSizeY // 2 - self.backgroundLogoH // 2)


class MainAuiManager(aui.AuiManager):
    """Advanced User Interface Manager for Embroidermodder."""
    def __init__(self, managed_window=None, agwFlags=
                 aui.AUI_MGR_ALLOW_FLOATING
                 # | aui.AUI_MGR_ALLOW_ACTIVE_PANE
                 # | aui.AUI_MGR_TRANSPARENT_DRAG
                 | aui.AUI_MGR_TRANSPARENT_HINT
                 | aui.AUI_MGR_VENETIAN_BLINDS_HINT
                 # | aui.AUI_MGR_RECTANGLE_HINT
                 | aui.AUI_MGR_HINT_FADE
                 # | aui.AUI_MGR_NO_VENETIAN_BLINDS_FADE
                 # | aui.AUI_MGR_LIVE_RESIZE
                 | aui.AUI_MGR_ANIMATE_FRAMES
                 | aui.AUI_MGR_PREVIEW_MINIMIZED_PANES
                 # | aui.AUI_MGR_AERO_DOCKING_GUIDES
                 | aui.AUI_MGR_WHIDBEY_DOCKING_GUIDES
                 | aui.AUI_MGR_SMOOTH_DOCKING
                 | aui.AUI_MGR_USE_NATIVE_MINIFRAMES
                 # | aui.AUI_MGR_AUTONB_NO_CAPTION
                 | 0):
        """Default class constructor."""
        aui.AuiManager.__init__(self, managed_window, agwFlags)

        # ... Tell AuiManager to manage this frame
        self.SetManagedWindow(managed_window)

        self.SetAutoNotebookStyle(
            agwStyle=
            aui.AUI_NB_DRAW_DND_TAB
            # | aui.AUI_NB_TOP
            # | aui.AUI_NB_LEFT
            # | aui.AUI_NB_RIGHT
            | aui.AUI_NB_BOTTOM
            | aui.AUI_NB_TAB_SPLIT
            | aui.AUI_NB_TAB_MOVE
            # | aui.AUI_NB_TAB_EXTERNAL_MOVE
            # | aui.AUI_NB_TAB_FIXED_WIDTH
            | aui.AUI_NB_SCROLL_BUTTONS
            | aui.AUI_NB_WINDOWLIST_BUTTON
            # | aui.AUI_NB_CLOSE_BUTTON
            | aui.AUI_NB_CLOSE_ON_ACTIVE_TAB
            # | aui.AUI_NB_CLOSE_ON_ALL_TABS
            | aui.AUI_NB_MIDDLE_CLICK_CLOSE
            | aui.AUI_NB_SUB_NOTEBOOK
            # | aui.AUI_NB_HIDE_ON_SINGLE_TAB
            | aui.AUI_NB_SMART_TABS
            # | aui.AUI_NB_USE_IMAGES_DROPDOWN
            # | aui.AUI_NB_CLOSE_ON_TAB_LEFT
            | aui.AUI_NB_TAB_FLOAT
            | 0)
        arts = [aui.AuiDefaultTabArt, aui.AuiSimpleTabArt, aui.VC71TabArt, aui.FF2TabArt, aui.VC8TabArt, aui.ChromeTabArt]
        self.SetAutoNotebookTabArt(arts[5]()) # Embroidermodder theme is blue, so we will do ChromeTabs
        self.SetAnimationStep(40.0) # 30.0 is default


class EmbroidermodderMainWindow(wx.Frame):
    """Main Frame Window for Embroidermodder application."""
    def __init__(self, parent, id=wx.ID_ANY, title=wx.EmptyString,
                 pos=wx.DefaultPosition, size=wx.DefaultSize,
                 style=wx.DEFAULT_FRAME_STYLE, name='frame'):
        """Default class constructor."""
        wx.Frame.__init__(self, parent, id, title, pos, size, style, name)

        global gMainWin
        gMainWin = self

        self.gImgDir = gImgDir

        #--- AuiManager
        self._mgr = MainAuiManager(self)

        ## self.SetDoubleBuffered(True)
        self.SetBackgroundColour(EMBROIDERBLUE1)

        self.gStatusBar = CustomStatusBar(self)
        self.SetStatusBar(self.gStatusBar)
        # Initial SetStatusText
        self.gStatusBar.SetStatusText('Welcome to Embroidermodder v2.0 build/rev #-----', 0)

        self.gMenuBar = self.CreateMenuBar()
        self.SetMenuBar(self.gMenuBar)

        self.BindEvents()

        self.embroidermodderPanel = EmbroidermodderPanel(self)
        self.pyshellPanel = PyShellPanel(self)
        self.pyshellPanel.Hide()

        vbSizer = wx.BoxSizer(wx.VERTICAL)
        vbSizer.Add(self.embroidermodderPanel, 1, wx.EXPAND | wx.ALL, 0)
        vbSizer.Add(self.pyshellPanel, 1, wx.EXPAND | wx.ALL, 0)
        self.SetSizer(vbSizer)


    def BindEvents(self):
        """Bind Events."""
        self.Bind(wx.EVT_CLOSE, self.OnDestroy)

        self.Bind(wx.EVT_MENU, self.OnDestroy, id=wx.ID_EXIT)
        self.Bind(wx.EVT_MENU, self.OnOpen, id=wx.ID_OPEN)
        self.Bind(wx.EVT_MENU, self.OnSaveAs, id=wx.ID_SAVEAS)

        self.Bind(wx.EVT_MENU, self.DoIconize, id=wx.ID_ICONIZE_FRAME)
        self.Bind(wx.EVT_MENU, self.DoMaximize, id=wx.ID_MAXIMIZE_FRAME)
        self.Bind(wx.EVT_MENU, self.OnToggleFullScreenMode, id=ID_FULLSCREEN)

        self.Bind(wx.EVT_MENU, self.OnHelp, id=wx.ID_HELP)
        self.Bind(wx.EVT_MENU, self.OnAbout, id=wx.ID_ABOUT)

    def CreateMenuBar(self):
        """Create the MenuBar for the :class:`EmbroidermodderMainWindow`."""
        self.gMenuBar = wx.MenuBar()

        self.gMenu_File = self.CreateMenu_File()
        self.gMenu_Edit = self.CreateMenu_Edit()
        self.gMenu_View = self.CreateMenu_View()
        self.gMenu_Tools = self.CreateMenu_Tools()
        self.gMenu_Draw = self.CreateMenu_Draw()
        self.gMenu_Sandbox = self.CreateMenu_Sandbox()
        self.gMenu_Modify = self.CreateMenu_Modify()
        self.gMenu_Dimension = self.CreateMenu_Dimension()
        self.gMenu_Settings = self.CreateMenu_Settings()
        self.gMenu_Window = self.CreateMenu_Window()
        self.gMenu_Help = self.CreateMenu_Help()

        self.gMenuBar.Append(self.gMenu_File, '&File')
        self.gMenuBar.Append(self.gMenu_Edit, '&Edit')
        self.gMenuBar.Append(self.gMenu_View, '&View')
        self.gMenuBar.Append(self.gMenu_Tools, '&Tools')
        self.gMenuBar.Append(self.gMenu_Draw, '&Draw')
        self.gMenuBar.Append(self.gMenu_Sandbox, '&Sandbox')
        self.gMenuBar.Append(self.gMenu_Modify, '&Modify')
        self.gMenuBar.Append(self.gMenu_Dimension, '&Dimension')
        self.gMenuBar.Append(self.gMenu_Settings, '&Settings')
        self.gMenuBar.Append(self.gMenu_Window, '&Window')
        self.gMenuBar.Append(self.gMenu_Help, '&Help')

        return self.gMenuBar

    def CreateMenu_File(self):
        """
        Create the `File` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu() # File Menu isn't Tear-off able... why not # style=wx.MENU_TEAROFF

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, wx.ID_NEW, u'%s\t%s' %(_(u'&New'), NewShortcut), _(u'Create a new file.'))
        bmp = wx.Image(gIconDir + os.sep + 'new.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_OPEN, u'%s\t%s' %(_(u'&Open'), OpenShortcut), _(u'Open an existing file.'))
        bmp = wx.Image(gIconDir + os.sep + 'open.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, wx.ID_SAVE, u'%s\t%s' %(_(u'&Save'), SaveShortcut), _(u'Save the design to disk.'))
        bmp = wx.Image(gIconDir + os.sep + 'save.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_SAVEAS, u'%s\t%s' %(_(u'Save &As...'), SaveAsShortcut), _(u'Save the design under a new name.'))
        bmp = wx.Image(gIconDir + os.sep + 'saveas.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, wx.ID_PRINT, u'%s\t%s' %(_(u'&Print'), PrintShortcut), _(u'Print the design.'))
        bmp = wx.Image(gIconDir + os.sep + 'print.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, wx.ID_CLOSE, u'%s\t%s' %(_(u'&Close'), WindowCloseShortcut), _(u'Close the active window.'))
        bmp = wx.Image(gIconDir + os.sep + 'windowclose.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_DETAILS, u'%s\t%s' %(_(u'&Details'), DetailsShortcut), _(u'Details of the current design.'))
        bmp = wx.Image(gIconDir + os.sep + 'histogram.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, wx.ID_EXIT, u'%s\t%s' %(_(u'&Exit'), ExitShortcut), _(u'Exit the application.'))
        bmp = wx.Image(gIconDir + os.sep + 'exit.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Edit(self):
        """
        Create the `Edit` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, wx.ID_UNDO, u'%s\t%s' %(_(u'&Undo'), UndoShortcut), _(u'Reverses the most recent action.'))
        bmp = wx.Image(gIconDir + os.sep + 'undo.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_REDO, u'%s\t%s' %(_(u'&Redo'), RedoShortcut), _(u'Reverses the effects of the previous undo action.'))
        bmp = wx.Image(gIconDir + os.sep + 'redo.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, wx.ID_CUT, u'%s\t%s' %(_(u'Cu&t'), CutShortcut), _(u'Cut the current selection\'s contents to the clipboard.'))
        bmp = wx.Image(gIconDir + os.sep + 'cut.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_COPY, u'%s\t%s' %(_(u'&Copy'), CopyShortcut), _(u'Copy the current selection\'s contents to the clipboard.'))
        bmp = wx.Image(gIconDir + os.sep + 'copy.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_PASTE, u'%s\t%s' %(_(u'&Paste'), PasteShortcut), _(u'Paste the clipboard\'s contents into the current selection.'))
        bmp = wx.Image(gIconDir + os.sep + 'paste.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_View(self):
        """
        Create the `View` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        menu.AppendSubMenu(self.CreateMenu_Zoom(), _(u'Zoom'))
        menu.AppendSubMenu(self.CreateMenu_Pan(), _(u'Pan'))

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_VIEW_DAY, u'&%s\t%s' %(_(u'Day'), ViewDayShortcut), _(u'Updates the current view using day vision settings.'))
        bmp = wx.Image(gIconDir + os.sep + 'day.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_NIGHT, u'&%s\t%s' %(_(u'Night'), ViewNightShortcut), _(u'Updates the current view using night vision settings.'))
        bmp = wx.Image(gIconDir + os.sep + 'night.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_FULLSCREEN, u'&%s\t%s' %(_(u'Fullscreen'), ToggleFullscreenShortcut), _(u'Toggle Fullscreen Mode.'))
        # bmp = wx.Image(gIconDir + os.sep + 'windowmaximize.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        # mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_ICONIZE_FRAME, _(u'Iconize Frame'), _(u'Configure settings specific to this project.'))
        bmp = wx.Image(gIconDir + os.sep + 'windowminimize.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_MAXIMIZE_FRAME, _(u'Maximize Frame'), _(u'Configure settings specific to this project.'))
        bmp = wx.Image(gIconDir + os.sep + 'windowmaximize.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Zoom(self):
        """
        Create the `Zoom` menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_REALTIME, u'%s\t%s' %(_(u'Zoom Realtime'), ZoomRealtimeShortcut), _(u'Zooms to increase or decrease the apparent size of objects in the current viewport.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomrealtime.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_PREVIOUS, u'%s\t%s' %(_(u'Zoom Previous'), ZoomPreviousShortcut), _(u'Zooms to display the previous view.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomprevious.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_WINDOW, u'%s\t%s' %(_(u'Zoom Window'), ZoomWindowShortcut), _(u'Zooms to display an area specified by a rectangular window.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomwindow.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_DYNAMIC, u'%s\t%s' %(_(u'Zoom Dynamic'), ZoomDynamicShortcut), _(u'Zooms to display the generated portion of the drawing.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomdynamic.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_SCALE, u'%s\t%s' %(_(u'Zoom Scale'), ZoomScaleShortcut), _(u'Zooms the display using a specified scale factor.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomscale.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_CENTER, u'%s\t%s' %(_(u'Zoom Center'), ZoomCenterShortcut), _(u'Zooms to display a view specified by a center point and magnification or height.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomcenter.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_IN, u'%s\t%s' %(_(u'Zoom In'), ZoomInShortcut), _(u'Zooms to display the selected objects.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomin.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_OUT, u'%s\t%s' %(_(u'Zoom Out'), ZoomOutShortcut), _(u'Zooms to display the drawing extents or the grid limits.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomout.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_SELECTED, u'%s\t%s' %(_(u'Zoom Selected'), ZoomSelectedShortcut), _(u'Zooms to display the drawing extents.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomselected.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_ALL, u'%s\t%s' %(_(u'Zoom All'), ZoomAllShortcut), _(u'Zooms to display the drawing extents or the grid limits.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomall.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_ZOOM_EXTENTS, u'%s\t%s' %(_(u'Zoom Extents'), ZoomExtentsShortcut), _(u'Zooms to display the drawing extents.'))
        bmp = wx.Image(gIconDir + os.sep + 'zoomextents.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Pan(self):
        """
        Create the `Pan` menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, ID_VIEW_PAN_REALTIME, u'%s\t%s' %(_(u'Pan Realtime'), PanRealtimeShortcut), _(u'Moves the view in the current viewport.'))
        bmp = wx.Image(gIconDir + os.sep + 'panrealtime.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_PAN_POINT, u'%s\t%s' %(_(u'Pan Point'), PanPointShortcut), _(u'Moves the view by the specified distance.'))
        bmp = wx.Image(gIconDir + os.sep + 'panpoint.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_VIEW_PAN_LEFT, u'%s\t%s' %(_(u'Pan Left'), PanLeftShortcut), _(u'Moves the view to the left.'))
        bmp = wx.Image(gIconDir + os.sep + 'panleft.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_PAN_RIGHT, u'%s\t%s' %(_(u'Pan Right'), PanRightShortcut), _(u'Moves the view to the right.'))
        bmp = wx.Image(gIconDir + os.sep + 'panright.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_PAN_UP, u'%s\t%s' %(_(u'Pan Up'), PanUpShortcut), _(u'Moves the view up.'))
        bmp = wx.Image(gIconDir + os.sep + 'panup.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_VIEW_PAN_DOWN, u'%s\t%s' %(_(u'Pan Down'), PanDownShortcut), _(u'Moves the view down.'))
        bmp = wx.Image(gIconDir + os.sep + 'pandown.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Tools(self):
        """
        Create the `Tools` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, ID_TOOLS_DISTANCE, u'%s\t%s' %(_(u'&Distance'), DistanceShortcut), _(u'Measures the distance and angle between two points: DIST'))
        bmp = wx.Image(gIconDir + os.sep + 'distance.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_TOOLS_LOCATEPOINT, u'%s\t%s' %(_(u'&Locate Point'), LocatePointShortcut), _(u'Displays the coordinate values of a location: ID'))
        bmp = wx.Image(gIconDir + os.sep + 'locatepoint.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Modify(self):
        """
        Create the `Modify` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, ID_MODIFY_DELETE, u'%s\t%s' %(_(u'&Delete'), DeleteShortcut), _(u'Removes objects from a drawing: DELETE'))
        bmp = wx.Image(gIconDir + os.sep + 'erase.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_MODIFY_MOVE, u'%s\t%s' %(_(u'&Move'), MoveShortcut), _(u'Displaces objects a specified distance in a specified direction: MOVE'))
        bmp = wx.Image(gIconDir + os.sep + 'move.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_MODIFY_ROTATE, u'%s\t%s' %(_(u'&Rotate'), RotateShortcut), _(u'Rotates objects around a base point: ROTATE'))
        bmp = wx.Image(gIconDir + os.sep + 'rotate.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_MODIFY_SCALE, u'%s\t%s' %(_(u'&Scale'), ScaleShortcut), _(u'TODO?'))
        bmp = wx.Image(gIconDir + os.sep + 'scale.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Dimension(self):
        """
        Create the `Dimension` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, ID_DIMENSION_QUICKLEADER, u'%s\t%s' %(_(u'&QuickLeader'), QuickLeaderShortcut), _(u'Creates a leader and annotation: QUICKLEADER'))
        bmp = wx.Image(gIconDir + os.sep + 'quickleader.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Sandbox(self):
        """
        Create the `Sandbox` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, ID_SANDBOX_RGB, u'%s\t%s' %(_(u'&RGB'), RGBShortcut), _(u'Updates the current view colors: RGB'))
        bmp = wx.Image(gIconDir + os.sep + 'rgb.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_SANDBOX_SANDBOX, u'%s\t%s' %(_(u'&Sandbox'), SandboxShortcut), _(u'A sandbox to play in: SANDBOX'))
        bmp = wx.Image(gIconDir + os.sep + 'sandbox.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Draw(self):
        """
        Create the `Draw` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, ID_DRAW_CIRCLE, u'%s\t%s' %(_(u'&Circle'), CircleShortcut), _(u'Creates a circle: CIRCLE'))
        bmp = wx.Image(gIconDir + os.sep + 'circle.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_DOLPHIN, u'%s\t%s' %(_(u'&Dolphin'), DolphinShortcut), _(u'Creates a dolphin: DOLPHIN'))
        bmp = wx.Image(gIconDir + os.sep + 'dolphin.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_ELLIPSE, u'%s\t%s' %(_(u'&Ellipse'), EllipseShortcut), _(u'Creates a ellipse: ELLIPSE'))
        bmp = wx.Image(gIconDir + os.sep + 'ellipse.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_HEART, u'%s\t%s' %(_(u'&Heart'), HeartShortcut), _(u'Creates a heart: HEART'))
        bmp = wx.Image(gIconDir + os.sep + 'heart.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_LINE, u'%s\t%s' %(_(u'&Line'), LineShortcut), _(u'Creates straight line segments: LINE'))
        bmp = wx.Image(gIconDir + os.sep + 'line.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_PATH, u'%s\t%s' %(_(u'&Path'), PathShortcut), _(u'Creates a 2D path: PATH'))
        bmp = wx.Image(gIconDir + os.sep + 'path.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_POINT, u'%s\t%s' %(_(u'&Point'), PointShortcut), _(u'Creates multiple points: POINT'))
        bmp = wx.Image(gIconDir + os.sep + 'point.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_POLYGON, u'%s\t%s' %(_(u'&Polygon'), PolygonShortcut), _(u'Creates a rectangular polygon: POLYGON'))
        bmp = wx.Image(gIconDir + os.sep + 'polygon.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_POLYLINE, u'%s\t%s' %(_(u'&Polyline'), PolylineShortcut), _(u'Creates a 2D polyline: PLINE'))
        bmp = wx.Image(gIconDir + os.sep + 'polyline.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_RECTANGLE, u'%s\t%s' %(_(u'&Rectangle'), RectangleShortcut), _(u'Creates a rectangular polyline: RECTANGLE'))
        bmp = wx.Image(gIconDir + os.sep + 'rectangle.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_SINGLELINETEXT, u'%s\t%s' %(_(u'&Single Line Text'), SingleLineTextShortcut), _(u'Creates single-line text objects: TEXT'))
        bmp = wx.Image(gIconDir + os.sep + 'singlelinetext.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_SNOWFLAKE, u'%s\t%s' %(_(u'&Snowflake'), SnowflakeShortcut), _(u'Creates a snowflake: SNOWFLAKE'))
        bmp = wx.Image(gIconDir + os.sep + 'snowflake.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_DRAW_STAR, u'%s\t%s' %(_(u'&Star'), StarShortcut), _(u'Creates a star: STAR'))
        bmp = wx.Image(gIconDir + os.sep + 'star.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Settings(self):
        """
        Create the `Settings` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, wx.ID_HELP, u'%s\t%s' %(_(u'&Settings'), SettingsShortcut), _(u'Configure settings specific to this project.'))
        bmp = wx.Image(gIconDir + os.sep + 'settingsdialog.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def CreateMenu_Window(self):
        """
        Create the `Window` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu() # Window Menu isn't Tear-off able... why not # style=wx.MENU_TEAROFF

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, wx.ID_CLOSE, u'%s\t%s' %(_(u'&Close'), WindowCloseShortcut), _(u'Close the active window.'))
        bmp = wx.Image(gIconDir + os.sep + 'windowclose.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_CLOSE_ALL, u'%s\t%s' %(_(u'Close &All'), WindowCloseAllShortcut), _(u'Close all the windows.'))
        bmp = wx.Image(gIconDir + os.sep + 'windowcloseall.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_WINDOW_LAYOUT_CASCADE, u'%s\t%s' %(_(u'&Cascade'), WindowCascadeShortcut), _(u'Cascade the windows layout.'))
        bmp = wx.Image(gIconDir + os.sep + 'windowcascade.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_WINDOW_LAYOUT_TILE, u'%s\t%s' %(_(u'&Tile'), WindowTileShortcut), _(u'Tile the windows layout.'))
        bmp = wx.Image(gIconDir + os.sep + 'windowtile.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_WINDOW_NEXT, u'%s\t%s' %(_(u'&Next'), WindowNextShortcut), _(u'Move the focus to the next window.'))
        bmp = wx.Image(gIconDir + os.sep + 'windownext.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_WINDOW_PREVIOUS, u'%s\t%s' %(_(u'&Previous'), WindowPreviousShortcut), _(u'Move the focus to the previous window.'))
        bmp = wx.Image(gIconDir + os.sep + 'windowprevious.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        return menu

    def CreateMenu_Help(self):
        """
        Create the `Help` top-level menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, wx.ID_HELP, u'%s\t%s' %(_(u'&Help'), HelpShortcut), _(u'Displays help.'))
        bmp = wx.Image(gIconDir + os.sep + 'help.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, ID_CHANGELOG, u'%s\t%s' %(_(u'&Changelog'), ChangelogShortcut), _(u'Describes new features in this product.'))
        bmp = wx.Image(gIconDir + os.sep + 'changelog.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        mi = wx.MenuItem(menu, ID_TIPOFTHEDAY, u'%s\t%s' %(_(u'&Tip Of The Day'), TipOfTheDayShortcut), _(u'Displays a dialog with useful tips.'))
        bmp = wx.Image(gIconDir + os.sep + 'tipoftheday.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        mi = wx.MenuItem(menu, wx.ID_ABOUT, u'%s\t%s' %(_(u'&About Embroidermodder'), AboutEmbroidermodderShortcut), _(u'Displays information about this product.'))
        bmp = wx.Image(gIconDir + os.sep + 'about.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        menu.AppendSeparator()

        menu.AppendSubMenu(self.CreateMenu_OnlineWeblinks(), _(u'Online Weblinks'))

        return menu

    def CreateMenu_OnlineWeblinks(self):
        """
        Create the `Online Weblinks` menu.

        :return: a instance of `wx.Menu`.
        """
        menu = wx.Menu(style=wx.MENU_TEAROFF)

        if PHOENIX: menu.AppendItem = menu.Append

        mi = wx.MenuItem(menu, ID_ONLINE_WEBSITE, _(u'Visit Embroidermodder Online'), _(u'Opens the Embroidermodder website up in your default webbrowser.'))
        bmp = wx.Image(gIconDir + os.sep + 'world.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        mi.SetBitmap(bmp)
        menu.AppendItem(mi)

        return menu

    def OnOpen(self, event=None):
        """
        Show the Open `wx.FileDialog`.

        :param `event`: an `wx.MenuEvent`_ to be processed.
        """
        print('CWD: %s' % os.getcwd())

        # Create the dialog. In this case the current directory is forced as the starting
        # directory for the dialog, and no default file name is forced. This can easilly
        # be changed in your program. This is an 'open' dialog, and allows multitple
        # file selections as well.
        #
        # Finally, if the directory is changed in the process of getting files, this
        # dialog is set up to change the current working directory to the path chosen.
        dlg = wx.FileDialog(
            self, message=_(u'Choose a file...'),
            defaultDir=os.getcwd(),
            defaultFile='',
            wildcard=WILDCARD_ALL,
            style=wx.FD_OPEN | wx.FD_MULTIPLE |
                  wx.FD_CHANGE_DIR | wx.FD_FILE_MUST_EXIST |
                  wx.FD_PREVIEW
            )

        # Show the dialog and retrieve the user response. If it is the OK response,
        # process the data.
        if dlg.ShowModal() == wx.ID_OK:
            # This returns a Python list of files that were selected.
            paths = dlg.GetPaths()

            print('You selected %d files:' % len(paths))

            for path in paths:
                print('    %s' % path)

        # Compare this with the debug above; did we change working dirs?
        print('CWD: %s' % os.getcwd())

        # Destroy the dialog. Don't do this until you are done with it!
        # BAD things can happen otherwise!
        dlg.Destroy()

    def OnSaveAs(self, event=None):
        """
        Show the Save `wx.FileDialog`.

        :param `event`: an `wx.MenuEvent`_ to be processed.
        """
        print('CWD: %s' % os.getcwd())

        # Create the dialog. In this case the current directory is forced as the starting
        # directory for the dialog, and no default file name is forced. This can easilly
        # be changed in your program. This is an 'save' dialog.
        #
        # Unlike the 'open dialog' example found elsewhere, this example does NOT
        # force the current working directory to change if the user chooses a different
        # directory than the one initially set.

        dlg = wx.FileDialog(
            self, message=_(u'Save file as ...'), defaultDir=os.getcwd(),
            defaultFile='', wildcard=WILDCARD_ALL, style=wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT
            )

        # This sets the default filter that the user will initially see. Otherwise,
        # the first filter in the list will be used by default.
        dlg.SetFilterIndex(1)

        # Show the dialog and retrieve the user response. If it is the OK response,
        # process the data.
        if dlg.ShowModal() == wx.ID_OK:
            path = dlg.GetPath()
            print('You selected "%s"' % path)

            # Normally, at this point you would save your data using the file and path
            # data that the user provided to you, but since we didn't actually start
            # with any data to work with, that would be difficult.
            #
            # The code to do so would be similar to this, assuming 'data' contains
            # the data you want to save:
            #
            # fp = file(path, 'w') # Create file anew
            # fp.write(data)
            # fp.close()
            #
            # You might want to add some error checking :-)

        # Note that the current working dir didn't change. This is good since
        # that's the way we set it up.
        print('CWD: %s' % os.getcwd())

        # Destroy the dialog. Don't do this until you are done with it!
        # BAD things can happen otherwise!
        dlg.Destroy()

    def OnIconize(self, event):
        """
        The :class:`EmbroidermodderMainWindow` has recieved an `wx.IconizeEvent`.

        :param `event`: an `wx.IconizeEvent`_ to be processed.
        """
        event.Skip()

    def DoIconize(self, event):
        """
        Minimize the :class:`EmbroidermodderMainWindow`.
        This will send an `wx.IconizeEvent` to :class:`EmbroidermodderMainWindow`.

        :param `event`: an `wx.MenuEvent`_ to be processed.
        """
        self.Iconize() #

    def OnMaximize(self, event):
        """
        The :class:`EmbroidermodderMainWindow` has recieved an `wx.MaximizeEvent`.

        :param `event`: an `wx.MaximizeEvent`_ to be processed.
        """
        event.Skip()

    def DoMaximize(self, event):
        """
        Maximize the :class:`EmbroidermodderMainWindow`.
        This will send an `wx.MaximizeEvent` to :class:`EmbroidermodderMainWindow`.

        :param `event`: an `wx.MenuEvent`_ to be processed.
        """
        self.Maximize()

    def OnToggleFullScreenMode(self, event=None, mode=wx.FULLSCREEN_NOBORDER):
        """
        Toggle Fullscreen mode On/Off for the application.

        :param `event`: A `wx.MenuEvent`_ to be processed.
        :param `mode`: one of the wx.FULLSCREEN_* Constants.
            wx.FULLSCREEN_NOMENUBAR
            wx.FULLSCREEN_NOTOOLBAR
            wx.FULLSCREEN_NOSTATUSBAR
            wx.FULLSCREEN_NOBORDER
            wx.FULLSCREEN_NOCAPTION
            wx.FULLSCREEN_ALL
        """
        self.ShowFullScreen(not self.IsFullScreen(), mode)

    def OnHelp(self, event=None):
        """
        Open the Embroidermodder Help files up in the default Web Browser.

        :param `event`: an `wx.MenuEvent`_ to be processed.
        """
        webbrowser.open_new_tab('file:///' + gAppDir + os.sep + 'help' + os.sep + 'doc-index.html')

    def OnContextHelp(self, event=None):
        """
        Send the application into context sensitive help mode.

        :param `event`: an `wx.MenuEvent`_ to be processed.
        """
        wx.ContextHelp(window=self, doNow=True)

    def OnAbout(self, event=None):
        """
        Show the Embroidermodder About Dialog.

        :param `event`: an `wx.MenuEvent`_ to be processed.
        """
        dlg = AboutDialog(self)
        dlg.ShowModal()

    def OnDestroy(self, event=None):
        """
        Close and Destroy the :class:`EmbroidermodderMainWindow`.

        :param `event`: an `wx.MenuEvent`_ to be processed.
        """
        self.Destroy()


class CustomStatusBar(wx.StatusBar):
    """
    Custom StatusBar GUI Element with fields that can inject custom widgets into.
    """
    def __init__(self, parent, id=wx.ID_ANY, style=0, name='statusbar'):
        """Default class constructor."""
        if wx.VERSION > (2, 8, 12, 1, ''):
            style = (wx.STB_DEFAULT_STYLE
                     | wx.STB_SIZEGRIP
                     # | wx.STB_ELLIPSIZE_END
                     # | wx.STB_ELLIPSIZE_MIDDLE
                     # | wx.STB_ELLIPSIZE_START
                     | wx.STB_SHOW_TIPS)
        else:
            style = (wx.DEFAULT_STATUSBAR_STYLE
                     | wx.ST_SIZEGRIP)
        wx.StatusBar.__init__(self, parent, id, style, name)

        self.SetHelpText(_(u'A status bar is a narrow window that can') +
                  '\n' + _(u'be placed along the bottom of a frame to') +
                  '\n' + _(u'give small amounts of status information.'))

        self.StatBar_Py = 1
        self.StatBar_Help = 2

        self.SetFieldsCount(3)
        self.SetStatusWidths([-1, 32, 32])

        self.pyVersionInfosStaticBitmap = PythonVersionInfosStaticBitmap(self)
        self.contextHelpStaticBitmap = ContextHelpBouyStaticBitmap(self)

        self.sizeChanged = False

        #Bind Events
        self.Bind(wx.EVT_SIZE, self.OnSize)
        self.Bind(wx.EVT_IDLE, self.OnIdle)

    def OnSize(self, event):
        """
        Handles the ``wx.EVT_SIZE`` event for :class:`CustomStatusBar`.

        :param `event`: A `wx.SizeEvent`_ to be processed.
        """
        self.Reposition()  # for normal size events

        # Set a flag so the idle time handler will also do the repositioning.
        # It is done this way to get around a buglet where GetFieldRect is not
        # accurate during the EVT_SIZE resulting from a frame maximize.
        self.sizeChanged = True

        self.contextHelpStaticBitmap.SendSizeEvent()

    def OnIdle(self, event):
        """
        Handles the ``wx.EVT_IDLE`` event for :class:`CustomStatusBar`.

        :param `event`: A `wx.IdleEvent`_ to be processed.
        """
        if self.sizeChanged:
            self.Reposition()

    def Reposition(self):
        """Adjust the custom statusbar elements when resized."""
        if PHOENIX:
            x, y = self.GetSize()
            self.pyVersionInfosStaticBitmap.SetPosition((x - 96, 0 + 4))
            self.contextHelpStaticBitmap.SetPosition((x - 48, 0 + 4))
        else:
            rect = self.GetFieldRect(self.StatBar_Py)
            self.pyVersionInfosStaticBitmap.SetPosition((rect.x + 4, rect.y))
            rect = self.GetFieldRect(self.StatBar_Help)
            self.contextHelpStaticBitmap.SetPosition((rect.x + 4, rect.y))

        self.sizeChanged = False


class PythonVersionInfosStaticBitmap(wx.StaticBitmap):
    """
    Context Help Static Bitmap Element that sends the
    application into Context Help Sensitive Mode when clicked on.
    """
    def __init__(self, parent, id=ID_PYVERSION_INFOS,
                 bitmap=wx.NullBitmap,
                 pos=wx.DefaultPosition, size=(16, 16),
                 style=0,
                 name='staticbitmap'):
        """Default class constructor."""
        wx.StaticBitmap.__init__(self, parent, id, bitmap, pos, size, style, name)

        self.SetBackgroundColour(parent.GetBackgroundColour())

        self.SetToolTip(wx.ToolTip(pyVersionInfos))

        bmp = wx.Image(gIconDir + os.sep + 'pyscript.png', wx.BITMAP_TYPE_PNG).Scale(16, 16).ConvertToBitmap()
        self.SetBitmap(bmp)

        self.SetHelpText(pyVersionInfos)

        self.Bind(wx.EVT_LEFT_UP, self.TogglePyShell)

    def TogglePyShell(self, event=None):
        """
        Toggle the visibility of the :class:`PyShellPanel`.

        :param `event`: A `wx.MouseEvent`_ to be processed.
        """
        if gMainWin.pyshellPanel.IsShown():
            gMainWin.pyshellPanel.Hide()
        else:
            gMainWin.pyshellPanel.Show()
        gMainWin.SendSizeEvent()


class ContextHelpBouyStaticBitmap(wx.StaticBitmap):
    """
    Context Help Static Bitmap Element that sends the
    application into Context Help Sensitive Mode when clicked on.
    """
    def __init__(self, parent, id=wx.ID_CONTEXT_HELP,
                 bitmap=wx.NullBitmap,
                 pos=wx.DefaultPosition, size=(16, 16),
                 style=0,
                 name='staticbitmap'):
        """Default class constructor."""
        wx.StaticBitmap.__init__(self, parent, id, bitmap, pos, size, style, name)

        self.SetBackgroundColour(parent.GetBackgroundColour())

        self.SetToolTip(wx.ToolTip(_(u"What's This?")))

        self.SetBitmap(wx.Bitmap(gIconDir + os.sep + 'lifesaver_help16.png', wx.BITMAP_TYPE_PNG))

        self.SetHelpText(_(u'I\'m a context help button.' + '\n' +
                           u'Click on me and then on another widget to get' + '\n' +
                           u'a more detailed message of what it does!'))

        self.Bind(wx.EVT_LEFT_UP, gMainWin.OnContextHelp)


class AboutDialog(wx.Dialog):
    """
    About Dialog for Embroidermodder.
    """
    def __init__(self, parent, id=wx.ID_ANY, title=_(u'About Embroidermodder'),
                 pos=wx.DefaultPosition, size=wx.DefaultSize,
                 style=wx.DEFAULT_DIALOG_STYLE
                     | wx.DIALOG_EX_CONTEXTHELP
                     | wx.RESIZE_BORDER,
                 name='dialog'):
        """Default class constructor."""
        wx.Dialog.__init__(self, parent, id, title, pos, size, style, name)

        self.Bind(wx.EVT_INIT_DIALOG, self.OnInitDialog)
        self.Bind(wx.EVT_CLOSE, self.OnClose)

        aboutText = """\
Embroidermodder

http://embroidermodder.sourceforge.net

Available Platforms: GNU/Linux, Windows, Mac OSX, Raspberry Pi

Embroidery formats by Josh Varga.
User Interface by Jonathan Greig.

Free under the zlib/libpng license."""

        bmp = wx.Bitmap(gImgDir + os.sep + 'logo-small.png', type=wx.BITMAP_TYPE_PNG)
        embBmp = wx.StaticBitmap(self, -1, bmp)

        ohYeahButton = wx.Button(self, -1, 'Oh, Yeah!')
        ohYeahButton.Bind(wx.EVT_BUTTON, self.OnClose)

        # aboutTextWidget = wx.TextCtrl(self, -1, value=aboutText, style=wx.TE_READONLY | wx.TE_MULTILINE)
        aboutTextWidget = wx.StaticText(self, -1, label=aboutText)

        vbSizer = wx.BoxSizer(wx.VERTICAL)
        vbSizer.Add(embBmp, 0, wx.ALIGN_CENTER | wx.ALL, 10)
        vbSizer.Add(aboutTextWidget, 1, wx.EXPAND | wx.LEFT | wx.RIGHT, 10)
        vbSizer.Add(ohYeahButton, 0, wx.ALIGN_CENTER | wx.ALL, 10)
        self.SetSizerAndFit(vbSizer)

        self.SetMinSize(self.GetSize())


    def OnInitDialog(self, event):
        """
        Handles the ``wx.EVT_INIT_DIALOG`` event for :class:`AboutDialog`.

        :param `event`: A `wx.InitDialogEvent`_ to be processed.
        """
        event.Skip()

    def OnClose(self, event):
        """
        Handles the ``wx.EVT_CLOSE`` event for :class:`AboutDialog`.

        :param `event`: A `wx.CloseEvent`_ to be processed.
        """
        event.Skip()
        self.Destroy()


class EmbroidermodderApp(wx.App):
    """
    The Embroidermodder Application Class.
    """
    def OnInit(self):
        """Initialize."""
        self.SetClassName('EmbroidermodderApp')
        self.SetAppName('EmbroidermodderApp')

        # ContextHelp - Set an application-wide help provider.
        provider = wx.SimpleHelpProvider()
        if PHOENIX:
            wx.HelpProvider.Set(provider)
        else:
            wx.HelpProvider_Set(provider)
        # self.Bind(wx.EVT_HELP, self.OnHelp) # Context Help

        gMainWin = EmbroidermodderMainWindow(None, size=(600, 400))
        gMainWin.SetTitle('Embroidermodder - wxPython GUI')
        if PHOENIX:
            icon = wx.Icon(wx.Bitmap(gIconDir + os.sep + 'app.png', wx.BITMAP_TYPE_PNG))
        else:
            icon = wx.IconFromBitmap(wx.Bitmap(gIconDir + os.sep + 'app.png', wx.BITMAP_TYPE_PNG))
        gMainWin.SetIcon(icon)

        self.SetTopWindow(gMainWin)

        gMainWin.Show()

        # We don't want the logo to look completely lousy when the
        # frame width is real small so use 300 as the min size width.
        gMainWin.SetMinSize((300, 16 + 16 + 141)) # est height statusbar + est height menubar + logo height

        return True


if __name__ == '__main__':
    gApp = EmbroidermodderApp(redirect=False,
                              filename=None,
                              useBestVisual=False,
                              clearSigInt=True)

    gApp.MainLoop()


#-modelines
def modeline(indentSize=4, tabSize=4, useTabs=False, trimWhitespace=True,
             longLine=0, bom=False):
    return locals()

# kate: bom off; indent-mode python; indent-width 4; replace-trailing-space-save on;
