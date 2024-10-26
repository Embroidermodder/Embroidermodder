/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * Settings Data
 */

#include "core.h"

/* Matches the order of the settings constants table in constants.h. */
SettingsData settings_data[] = {
    {
        /* 0 */
        .id = GENERAL_LANGUAGE,
        .label = "Language",
        .key = "Language",
        .icon = "",
        .value = "default",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 1 */
        .id = GENERAL_ICON_THEME,
        .label = "Icon theme",
        .key = "IconTheme",
        .icon = "",
        .value = "default",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 2 */
        .id = GENERAL_ICON_SIZE,
        .label = "Icon size",
        .key = "IconSize",
        .icon = "",
        .value = "16",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 3 */
        .id = GENERAL_MDI_BG_USE_LOGO,
        .label = "Use Logo",
        .key = "MdiBGUseLogo",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 4 */
        .id = GENERAL_MDI_BG_USE_TEXTURE,
        .label = "Use Texture",
        .key = "MdiBGUseTexture",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 5 */
        .id = GENERAL_MDI_BG_USE_COLOR,
        .label = "Use Color",
        .key = "MdiBGUseColor",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 6 */
        .id = GENERAL_MDI_BG_LOGO,
        .label = "MDI area background logo",
        .key = "MdiBGLogo",
        .icon = "",
        .value = "logo_spirals",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 7 */
        .id = GENERAL_MDI_BG_TEXTURE,
        .label = "MDI area background texture",
        .key = "MdiBGTexture",
        .icon = "",
        .value = "texture_spirals",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 8 */
        .id = GENERAL_MDI_BG_COLOR,
        .label = "MDI area background color",
        .key = "MdiBGColor",
        .icon = "",
        .value = "12632256",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 9 */
        .id = GENERAL_TIP_OF_THE_DAY,
        .label = "Show Tip of the Day on startup",
        .key = "TipOfTheDay",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 10 */
        .id = GENERAL_CURRENT_TIP,
        .label = "Current tip",
        .key = "CurrentTip",
        .icon = "",
        .value = "0",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 11 */
        .id = GENERAL_SYSTEM_HELP_BROWSER,
        .label = "Help Browser",
        .key = "SystemHelpBrowser",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 12 */
        .id = GENERAL_CHECK_FOR_UPDATES,
        .label = "Check for updates",
        .key = "CheckForUpdates",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 13 */
        .id = DISPLAY_USE_OPENGL,
        .label = "Use OpenGL",
        .key = "Display_UseOpenGL",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 14 */
        .id = DISPLAY_RENDERHINT_AA,
        .label = "Renderhint anti-alias",
        .key = "Display_RenderHintAntiAlias",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 15 */
        .id = DISPLAY_RENDERHINT_TEXT_AA,
        .label = "Renderhint text anti-alias",
        .key = "Display_RenderHintTextAntiAlias",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 16 */
        .id = DISPLAY_RENDERHINT_SMOOTH_PIX,
        .label = "Renderhint smooth pixmap",
        .key = "Display_RenderHintSmoothPixmap",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 17 */
        .id = DISPLAY_RENDERHINT_HIGH_AA,
        .label = "Renderhint high quality anti-alias",
        .key = "Display_RenderHintHighQualityAntiAlias",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 18 */
        .id = DISPLAY_RENDERHINT_NONCOSMETIC,
        .label = "Renderhint high non-cosmetic",
        .key = "Display_RenderHintNonCosmetic",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 19 */
        .id = DISPLAY_SHOW_SCROLLBARS,
        .label = "Show Scrollbars",
        .key = "Display_ShowScrollBars",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 20 */
        .id = DISPLAY_SCROLLBAR_WIDGET_NUM,
        .label = "Scrollbar widget number",
        .key = "Display_ScrollBarWidgetNum",
        .icon = "",
        .value = "0",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 21 */
        .id = DISPLAY_CROSSHAIR_COLOR,
        .label = "Cross-hair color",
        .key = "Display_CrossHairColor",
        .icon = "",
        .value = "0",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 22 */
        .id = DISPLAY_BG_COLOR,
        .label = "Background color",
        .key = "Display_BackgroundColor",
        .icon = "",
        .value = "15461355",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 23 */
        .id = DISPLAY_SELECTBOX_LEFT_COLOR,
        .label = "Selection Box Color (Crossing)",
        .key = "Display_SelectBoxLeftColor",
        .icon = "",
        .value = "32768",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 24 */
        .id = DISPLAY_SELECTBOX_LEFT_FILL,
        .label = "Selection Box Fill (Crossing)",
        .key = "Display_SelectBoxLeftFill",
        .icon = "",
        .value = "65280",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 25 */
        .id = DISPLAY_SELECTBOX_RIGHT_COLOR,
        .label = "Selection Box Color (Window)",
        .key = "Display_SelectBoxRightColor",
        .icon = "",
        .value = "128",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 26 */
        .id = DISPLAY_SELECTBOX_RIGHT_FILL,
        .label = "Selection Box Fill (Window)",
        .key = "Display_SelectBoxRightFill",
        .icon = "",
        .value = "255",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 27 */
        .id = DISPLAY_SELECTBOX_ALPHA,
        .label = "Selection Box Transparency",
        .key = "Display_SelectBoxAlpha",
        .icon = "",
        .value = "32",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 28 */
        .id = DISPLAY_ZOOMSCALE_IN,
        .label = "Zoom In Scale",
        .key = "Display_ZoomScaleIn",
        .icon = "",
        .value = "2.0",
        .type = SCRIPT_REAL,
        .editor_data = "0.01,1.01,10.00"
    },
    {
        /* 29 */
        .id = DISPLAY_ZOOMSCALE_OUT,
        .label = "Zoom Out Scale",
        .key = "Display_ZoomScaleOut",
        .icon = "",
        .value = "0.5",
        .type = SCRIPT_REAL,
        .editor_data = "0.01,0.01,0.99"
    },
    {
        /* 30 */
        .id = DISPLAY_CROSSHAIR_PERCENT,
        .label = "Crosshair percent",
        .key = "Display_CrossHairPercent",
        .icon = "",
        .value = "5",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 31 */
        .id = DISPLAY_UNITS,
        .label = "Units",
        .key = "Display_Units",
        .icon = "",
        .value = "mm",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 32 */
        .id = PROMPT_TEXT_COLOR,
        .label = "Text color",
        .key = "Prompt_TextColor",
        .icon = "",
        .value = "0",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 33 */
        .id = PROMPT_BG_COLOR,
        .label = "Background color",
        .key = "Prompt_BackgroundColor",
        .icon = "",
        .value = "16777215",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 34 */
        .id = PROMPT_FONT_FAMILY,
        .label = "Font family",
        .key = "Prompt_FontFamily",
        .icon = "",
        .value = "Monospace",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 35 */
        .id = PROMPT_FONT_STYLE,
        .label = "Font style",
        .key = "Prompt_FontStyle",
        .icon = "",
        .value = "normal",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 36 */
        .id = PROMPT_FONT_SIZE,
        .label = "Font size",
        .key = "Prompt_FontSize",
        .icon = "",
        .value = "12",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 37 */
        .id = PROMPT_SAVE_HISTORY,
        .label = "Save History",
        .key = "Prompt_SaveHistory",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 38 */
        .id = PROMPT_SAVE_HISTORY_AS_HTML,
        .label = "Save As HTML",
        .key = "Prompt_SaveHistoryAsHtml",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 39 */
        .id = PROMPT_SAVE_HISTORY_FILENAME,
        .label = "Save history filename",
        .key = "Prompt_SaveHistoryFilename",
        .icon = "",
        .value = "prompt.log",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 40 */
        .id = OPENSAVE_CUSTOM_FILTER,
        .label = "Custom filter",
        .key = "OpenSave_CustomFilter",
        .icon = "",
        .value = "supported",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 41 */
        .id = OPENSAVE_OPEN_FORMAT,
        .label = "Open format",
        .key = "OpenSave_OpenFormat",
        .icon = "",
        .value = "*.*",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 42 */
        .id = OPENSAVE_OPEN_THUMBNAIL,
        .label = "Open thumbnail",
        .key = "OpenSave_OpenThumbnail",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 43 */
        .id = OPENSAVE_SAVE_FORMAT,
        .label = "Save format",
        .key = "OpenSave_SaveFormat",
        .icon = "",
        .value = "*.*",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 44 */
        .id = OPENSAVE_SAVE_THUMBNAIL,
        .label = "Save thumbnail",
        .key = "OpenSave_SaveThumbnail",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 45 */
        .id = OPENSAVE_RECENT_MAX_FILES,
        .label = "Maximum recent files",
        .key = "OpenSave_RecentMax",
        .icon = "",
        .value = "10",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 46 */
        .id = OPENSAVE_RECENT_LIST_OF_FILES,
        .label = "Recent files",
        .key = "OpenSave_RecentFiles",
        .icon = "",
        .value = "",
        .type = SCRIPT_TABLE
    },
    {
        /* 47 */
        .id = OPENSAVE_RECENT_DIRECTORY,
        .label = "Recent directory",
        .key = "OpenSave_RecentDirectory",
        .icon = "",
        .value = "",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 48 */
        .id = OPENSAVE_TRIM_DST_NUM_JUMPS,
        .label = "Number of jumps to become trim DST",
        .key = "OpenSave_TrimDstNumJumps",
        .icon = "",
        .value = "5",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 49 */
        .id = PRINTING_DEFAULT_DEVICE,
        .label = "Default device",
        .key = "Printing_DefaultDevice",
        .icon = "",
        .value = "",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 50 */
        .id = PRINTING_USE_LAST_DEVICE,
        .label = "Use last device",
        .key = "Printing_UseLastDevice",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 51 */
        .id = PRINTING_DISABLE_BG,
        .label = "Disable Background",
        .key = "Printing_DisableBG",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 52 */
        .id = GRID_SHOW_ON_LOAD,
        .label = "Initially show grid when loading a file",
        .key = "Grid_ShowOnLoad",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 53 */
        .id = GRID_SHOW_ORIGIN,
        .label = "Show the origin when the grid is enabled",
        .key = "Grid_ShowOrigin",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 54 */
        .id = GRID_COLOR_MATCH_CROSSHAIR,
        .label = "Grid color matches cross-hair color",
        .key = "Grid_ColorMatchCrossHair",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 55 */
        .id = GRID_COLOR,
        .label = "Color",
        .key = "Grid_Color",
        .icon = "",
        .value = "0",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 56 */
        .id = GRID_LOAD_FROM_FILE,
        .label = "Load from file",
        .key = "Grid_LoadFromFile",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 57 */
        .id = GRID_TYPE,
        .label = "Type",
        .key = "Grid_Type",
        .icon = "",
        .value = "Rectangular",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 58 */
        .id = GRID_CENTER_ON_ORIGIN,
        .label = "Center on origin",
        .key = "Grid_CenterOnOrigin",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 59 */
        .id = GRID_CENTER_X,
        .label = "Grid Center X",
        .key = "Grid_CenterX",
        .icon = "",
        .value = "0.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,-1000.000,1000.000"
    },
    {
        /* 60 */
        .id = GRID_CENTER_Y,
        .label = "Grid Center Y",
        .key = "Grid_CenterY",
        .icon = "",
        .value = "0.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,-1000.000,1000.000"
    },
    {
        /* 61 */
        .id = GRID_SIZE_X,
        .label = "Grid Size X",
        .key = "Grid_SizeX",
        .icon = "",
        .value = "100.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,1.000,1000.000"
    },
    {
        /* 62 */
        .id = GRID_SIZE_Y,
        .label = "Grid Size Y",
        .key = "Grid_SizeY",
        .icon = "",
        .value = "100.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,1.000,1000.000"
    },
    {
        /* 63 */
        .id = GRID_SPACING_X,
        .label = "X spacing",
        .key = "Grid_SpacingX",
        .icon = "",
        .value = "25.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,0.001,1000.000"
    },
    {
        /* 64 */
        .id = GRID_SPACING_Y,
        .label = "Y spacing",
        .key = "Grid_SpacingY",
        .icon = "",
        .value = "25.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,0.001,1000.000"
    },
    {
        /* 65 */
        .id = GRID_SIZE_RADIUS,
        .label = "Size radius",
        .key = "Grid_SizeRadius",
        .icon = "",
        .value = "50.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,1.000,1000.000"
    },
    {
        /* 66 */
        .id = GRID_SPACING_RADIUS,
        .label = "Spacing radius",
        .key = "Grid_SpacingRadius",
        .icon = "",
        .value = "25.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,0.001,1000.000"
    },
    {
        /* 67 */
        .id = GRID_SPACING_ANGLE,
        .label = "Spacing angle",
        .key = "Grid_SpacingAngle",
        .icon = "",
        .value = "45.0",
        .type = SCRIPT_REAL,
        .editor_data = "1.000,0.001,1000.000"
    },
    {
        /* 68 */
        .id = GRID_SHOW_ON_LOAD,
        .label = "Initially show ruler when loading a file",
        .key = "Ruler_ShowOnLoad",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 69 */
        .id = RULER_METRIC,
        .label = "Metric",
        .key = "Ruler_Metric",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 70 */
        .id = RULER_COLOR,
        .label = "Color",
        .key = "Ruler_Color",
        .icon = "",
        .value = "13816370",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 71 */
        .id = RULER_PIXEL_SIZE,
        .label = "Pixel size",
        .key = "Ruler_PixelSize",
        .icon = "",
        .value = "20",
        .type = SCRIPT_INT,
        .editor_data = "1,20,100"
    },
    {
        /* 72 */
        .id = QSNAP_ENABLED,
        .label = "Enabled",
        .key = "QuickSnap_Enabled",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 73 */
        .id = QSNAP_LOCATOR_COLOR,
        .label = "Locator color",
        .key = "QuickSnap_LocatorColor",
        .icon = "",
        .value = "16776960",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 74 */
        .id = QSNAP_LOCATOR_SIZE,
        .label = "Locator size",
        .key = "QuickSnap_LocatorSize",
        .icon = "",
        .value = "4",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 75 */
        .id = QSNAP_APERTURE_SIZE,
        .label = "Aperture size",
        .key = "QuickSnap_ApertureSize",
        .icon = "",
        .value = "10",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 76 */
        .id = QSNAP_ENDPOINT,
        .label = "End point",
        .key = "QuickSnap_EndPoint",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 77 */
        .id = QSNAP_MIDPOINT,
        .label = "Mid point",
        .key = "QuickSnap_MidPoint",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 78 */
        .id = QSNAP_CENTER,
        .label = "Center",
        .key = "QuickSnap_Center",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 79 */
        .id = QSNAP_NODE,
        .label = "Node",
        .key = "QuickSnap_Node",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 80 */
        .id = QSNAP_QUADRANT,
        .label = "Quadrant",
        .key = "QuickSnap_Quadrant",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 81 */
        .id = QSNAP_INTERSECTION,
        .label = "Intersection",
        .key = "QuickSnap_Intersection",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 82 */
        .id = QSNAP_EXTENSION,
        .label = "Extension",
        .key = "QuickSnap_Extension",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 83 */
        .id = QSNAP_INSERTION,
        .label = "Insertion",
        .key = "QuickSnap_Insertion",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 84 */
        .id = QSNAP_PERPENDICULAR,
        .label = "Perpendicular",
        .key = "QuickSnap_Perpendicular",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 85 */
        .id = QSNAP_TANGENT,
        .label = "Tangent",
        .key = "QuickSnap_Tangent",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 86 */
        .id = QSNAP_NEAREST,
        .label = "Nearest",
        .key = "QuickSnap_Nearest",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 87 */
        .id = QSNAP_APPARENT,
        .label = "Apparent Intersection",
        .key = "QuickSnap_Apparent",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 88 */
        .id = QSNAP_PARALLEL,
        .label = "Parallel",
        .key = "QuickSnap_Parallel",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 89 */
        .id = LWT_SHOW_LWT,
        .label = "Show line weight",
        .key = "LineWeight_ShowLineWeight",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 90 */
        .id = LWT_REAL_RENDER,
        .label = "Real render",
        .key = "LineWeight_RealRender",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 91 */
        .id = LWT_DEFAULT_LWT,
        .label = "Default line weight",
        .key = "LineWeight_DefaultLineWeight",
        .icon = "",
        .value = "0",
        .type = SCRIPT_REAL,
        .editor_data = ""
    },
    {
        /* 92 */
        .id = SELECTION_MODE_PICKFIRST,
        .label = "Allow Preselection (PickFirst)",
        .key = "Selection_PickFirst",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 93 */
        .id = SELECTION_MODE_PICKADD,
        .label = "Add to Selection (PickAdd)",
        .key = "Selection_PickAdd",
        .icon = "",
        .value = "1",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 94 */
        .id = SELECTION_MODE_PICKDRAG,
        .label = "Drag to Select (PickDrag)",
        .key = "Selection_PickDrag",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 95 */
        .id = SELECTION_COOLGRIP_COLOR,
        .label = "Cool grip color",
        .key = "Selection_CoolGripColor",
        .icon = "",
        .value = "255",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 96 */
        .id = SELECTION_HOTGRIP_COLOR,
        .label = "Hot grip color",
        .key = "Selection_HotGripColor",
        .icon = "",
        .value = "16711680",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 97 */
        .id = SELECTION_GRIP_SIZE,
        .label = "Grip size",
        .key = "Selection_GripSize",
        .icon = "",
        .value = "4",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 98 */
        .id = SELECTION_PICKBOX_SIZE,
        .label = "Pick box size",
        .key = "Selection_PickBoxSize",
        .icon = "",
        .value = "4",
        .type = SCRIPT_INT,
        .editor_data = ""
    },
    {
        /* 99 */
        .id = TEXT_FONT,
        .label = "Font",
        .key = "Text_Font",
        .icon = "",
        .value = "Arial",
        .type = SCRIPT_STRING,
        .editor_data = ""
    },
    {
        /* 100 */
        .id = TEXT_SIZE,
        .label = "Size",
        .key = "Text_Size",
        .icon = "",
        .value = "12",
        .type = SCRIPT_REAL,
        .editor_data = ""
    },
    {
        /* 101 */
        .id = TEXT_ANGLE,
        .label = "Angle",
        .key = "Text_Angle",
        .icon = "",
        .value = "0",
        .type = SCRIPT_REAL,
        .editor_data = ""
    },
    {
        /* 102 */
        .id = TEXT_STYLE_BOLD,
        .label = "Bold",
        .key = "Text_StyleBold",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 103 */
        .id = TEXT_STYLE_ITALIC,
        .label = "Italic",
        .key = "Text_StyleItalic",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 104*/
        .id = TEXT_STYLE_UNDERLINE,
        .label = "Underline",
        .key = "Text_StyleUnderline",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 105 */
        .id = TEXT_STYLE_OVERLINE,
        .label = "Overline",
        .key = "Text_StyleOverline",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    },
    {
        /* 106 */
        .id = TEXT_STYLE_STRIKEOUT,
        .label = "Strikeout",
        .key = "Text_StyleStrikeOut",
        .icon = "",
        .value = "0",
        .type = SCRIPT_BOOL,
        .editor_data = ""
    }
};

EmbStringTable grid_enabled_group = {
    "labelGridType",
    "comboBoxGridType",
    "checkBoxGridCenterOnOrigin",
    "labelGridCenterX",
    "spinBoxGridCenterX",
    "labelGridCenterY",
    "spinBoxGridCenterY",
    "labelGridSizeX",
    "spinBoxGridSizeX",
    "labelGridSizeY",
    "spinBoxGridSizeY",
    "labelGridSpacingX",
    "spinBoxGridSpacingX",
    "labelGridSpacingY",
    "spinBoxGridSpacingY",
    "labelGridSizeRadius",
    "spinBoxGridSizeRadius",
    "labelGridSpacingRadius",
    "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle",
    "spinBoxGridSpacingAngle",
    "END"
};

EmbStringTable rectangular_grid_visible_group = {
    "labelGridSizeX",
    "spinBoxGridSizeX",
    "labelGridSizeY",
    "spinBoxGridSizeY",
    "labelGridSpacingX",
    "spinBoxGridSpacingX",
    "labelGridSpacingY",
    "spinBoxGridSpacingY",
    "END"
};

EmbStringTable circular_grid_visible_group = {
    "labelGridSizeRadius",
    "spinBoxGridSizeRadius",
    "labelGridSpacingRadius",
    "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle",
    "spinBoxGridSpacingAngle",
    "END"
};

EmbStringTable grid_layout = {
    "checkBoxGridLoadFromFile", "labelGridType",
    "", "comboBoxGridType",
    "checkBoxGridCenterOnOrigin",
    "labelGridCenterX", "spinBoxGridCenterX",
    "labelGridCenterY", "spinBoxGridCenterY",
    "labelGridSizeX", "spinBoxGridSizeX",
    "labelGridSizeY", "spinBoxGridSizeY",
    "labelGridSpacingX", "spinBoxGridSpacingX",
    "labelGridSpacingY", "spinBoxGridSpacingY",
    "labelGridSizeRadius", "spinBoxGridSizeRadius",
    "labelGridSpacingRadius", "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle", "spinBoxGridSpacingAngle",
    "END", "END"
};

