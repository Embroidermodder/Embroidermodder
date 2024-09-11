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
        .value = "default",
        .type = SCRIPT_STRING
    },
    {
        /* 1 */
        .id = GENERAL_ICON_THEME,
        .label = "Icon theme",
        .key = "IconTheme",
        .value = "default",
        .type = SCRIPT_STRING
    },
    {
        /* 2 */
        .id = GENERAL_ICON_SIZE,
        .label = "Icon size",
        .key = "IconSize",
        .value = "16",
        .type = SCRIPT_INT
    },
    {
        /* 3 */
        .id = GENERAL_MDI_BG_USE_LOGO,
        .label = "Use MDI area background logo",
        .key = "MdiBGUseLogo",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 4 */
        .id = GENERAL_MDI_BG_USE_TEXTURE,
        .label = "Use MDI area background texture",
        .key = "MdiBGUseTexture",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 5 */
        .id = GENERAL_MDI_BG_USE_COLOR,
        .label = "Use MDI area background color",
        .key = "MdiBGUseColor",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 6 */
        .id = GENERAL_MDI_BG_LOGO,
        .label = "MDI area background logo",
        .key = "MdiBGLogo",
        .value = "logo_spirals",
        .type = SCRIPT_STRING
    },
    {
        /* 7 */
        .id = GENERAL_MDI_BG_TEXTURE,
        .label = "MDI area background texture",
        .key = "MdiBGTexture",
        .value = "texture_spirals",
        .type = SCRIPT_STRING
    },
    {
        /* 8 */
        .id = GENERAL_MDI_BG_COLOR,
        .label = "MDI area background color",
        .key = "MdiBGColor",
        .value = "12632256",
        .type = SCRIPT_INT
    },
    {
        /* 9 */
        .id = GENERAL_TIP_OF_THE_DAY,
        .label = "Tip of the day",
        .key = "TipOfTheDay",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 10 */
        .id = GENERAL_CURRENT_TIP,
        .label = "Current tip",
        .key = "CurrentTip",
        .value = "0",
        .type = SCRIPT_INT
    },
    {
        /* 11 */
        .id = GENERAL_SYSTEM_HELP_BROWSER,
        .label = "Help Browser",
        .key = "SystemHelpBrowser",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 12 */
        .id = GENERAL_CHECK_FOR_UPDATES,
        .label = "Check for updates",
        .key = "CheckForUpdates",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 13 */
        .id = DISPLAY_USE_OPENGL,
        .label = "Use OpenGL",
        .key = "Display_UseOpenGL",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 14 */
        .id = DISPLAY_RENDERHINT_AA,
        .label = "Renderhint anti-alias",
        .key = "Display_RenderHintAntiAlias",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 15 */
        .id = DISPLAY_RENDERHINT_TEXT_AA,
        .label = "Renderhint text anti-alias",
        .key = "Display_RenderHintTextAntiAlias",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 16 */
        .id = DISPLAY_RENDERHINT_SMOOTH_PIX,
        .label = "Renderhint smooth pixmap",
        .key = "Display_RenderHintSmoothPixmap",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 17 */
        .id = DISPLAY_RENDERHINT_HIGH_AA,
        .label = "Renderhint high quality anti-alias",
        .key = "Display_RenderHintHighQualityAntiAlias",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 18 */
        .id = DISPLAY_RENDERHINT_NONCOSMETIC,
        .label = "Renderhint high non-cosmetic",
        .key = "Display_RenderHintNonCosmetic",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 19 */
        .id = DISPLAY_SHOW_SCROLLBARS,
        .label = "Show scroll bars",
        .key = "Display_ShowScrollBars",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 20 */
        .id = DISPLAY_SCROLLBAR_WIDGET_NUM,
        .label = "Scrollbar widget number",
        .key = "Display_ScrollBarWidgetNum",
        .value = "0",
        .type = SCRIPT_INT
    },
    {
        /* 21 */
        .id = DISPLAY_CROSSHAIR_COLOR,
        .label = "Cross-hair color",
        .key = "Display_CrossHairColor",
        .value = "0",
        .type = SCRIPT_INT
    },
    {
        /* 22 */
        .id = DISPLAY_BG_COLOR,
        .label = "Background color",
        .key = "Display_BackgroundColor",
        .value = "15461355",
        .type = SCRIPT_INT
    },
    {
        /* 23 */
        .id = DISPLAY_SELECTBOX_LEFT_COLOR,
        .label = "Selectbox left color",
        .key = "Display_SelectBoxLeftColor",
        .value = "32768",
        .type = SCRIPT_INT
    },
    {
        /* 24 */
        .id = DISPLAY_SELECTBOX_LEFT_FILL,
        .label = "Selectbox left fill",
        .key = "Display_SelectBoxLeftFill",
        .value = "65280",
        .type = SCRIPT_INT
    },
    {
        /* 25 */
        .id = DISPLAY_SELECTBOX_RIGHT_COLOR,
        .label = "Selectbox right color",
        .key = "Display_SelectBoxRightColor",
        .value = "128",
        .type = SCRIPT_INT
    },
    {
        /* 26 */
        .id = DISPLAY_SELECTBOX_RIGHT_FILL,
        .label = "Selectbox right fill",
        .key = "Display_SelectBoxRightFill",
        .value = "255",
        .type = SCRIPT_INT
    },
    {
        /* 27 */
        .id = DISPLAY_SELECTBOX_ALPHA,
        .label = "Selectbox alpha",
        .key = "Display_SelectBoxAlpha",
        .value = "32",
        .type = SCRIPT_INT
    },
    {
        /* 28 */
        .id = DISPLAY_ZOOMSCALE_IN,
        .label = "Zoom scale in",
        .key = "Display_ZoomScaleIn",
        .value = "2.0",
        .type = SCRIPT_REAL
    },
    {
        /* 29 */
        .id = DISPLAY_ZOOMSCALE_OUT,
        .label = "Zoom scale out",
        .key = "Display_ZoomScaleOut",
        .value = "0.5",
        .type = SCRIPT_REAL
    },
    {
        /* 30 */
        .id = DISPLAY_CROSSHAIR_PERCENT,
        .label = "Crosshair percent",
        .key = "Display_CrossHairPercent",
        .value = "5",
        .type = SCRIPT_INT
    },
    {
        /* 31 */
        .id = DISPLAY_UNITS,
        .label = "Units",
        .key = "Display_Units",
        .value = "mm",
        .type = SCRIPT_STRING
    },
    {
        /* 32 */
        .id = PROMPT_TEXT_COLOR,
        .label = "Text color",
        .key = "Prompt_TextColor",
        .value = "0",
        .type = SCRIPT_INT
    },
    {
        /* 33 */
        .id = PROMPT_BG_COLOR,
        .label = "Background color",
        .key = "Prompt_BackgroundColor",
        .value = "16777215",
        .type = SCRIPT_INT
    },
    {
        /* 34 */
        .id = PROMPT_FONT_FAMILY,
        .label = "Font family",
        .key = "Prompt_FontFamily",
        .value = "Monospace",
        .type = SCRIPT_STRING
    },
    {
        /* 35 */
        .id = PROMPT_FONT_STYLE,
        .label = "Font style",
        .key = "Prompt_FontStyle",
        .value = "normal",
        .type = SCRIPT_STRING
    },
    {
        /* 36 */
        .id = PROMPT_FONT_SIZE,
        .label = "Font size",
        .key = "Prompt_FontSize",
        .value = "12",
        .type = SCRIPT_INT
    },
    {
        /* 37 */
        .id = PROMPT_SAVE_HISTORY,
        .label = "Save history",
        .key = "Prompt_SaveHistory",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 38 */
        .id = PROMPT_SAVE_HISTORY_AS_HTML,
        .label = "Save history as HTML",
        .key = "Prompt_SaveHistoryAsHtml",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 39 */
        .id = PROMPT_SAVE_HISTORY_FILENAME,
        .label = "Save history filename",
        .key = "Prompt_SaveHistoryFilename",
        .value = "prompt.log",
        .type = SCRIPT_STRING
    },
    {
        /* 40 */
        .id = OPENSAVE_CUSTOM_FILTER,
        .label = "Custom filter",
        .key = "OpenSave_CustomFilter",
        .value = "supported",
        .type = SCRIPT_STRING
    },
    {
        /* 41 */
        .id = OPENSAVE_OPEN_FORMAT,
        .label = "Open format",
        .key = "OpenSave_OpenFormat",
        .value = "*.*",
        .type = SCRIPT_STRING
    },
    {
        /* 42 */
        .id = OPENSAVE_OPEN_THUMBNAIL,
        .label = "Open thumbnail",
        .key = "OpenSave_OpenThumbnail",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 43 */
        .id = OPENSAVE_SAVE_FORMAT,
        .label = "Save format",
        .key = "OpenSave_SaveFormat",
        .value = "*.*",
        .type = SCRIPT_STRING
    },
    {
        /* 44 */
        .id = OPENSAVE_SAVE_THUMBNAIL,
        .label = "Save thumbnail",
        .key = "OpenSave_SaveThumbnail",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 45 */
        .id = OPENSAVE_RECENT_MAX_FILES,
        .label = "Maximum recent files",
        .key = "OpenSave_RecentMax",
        .value = "10",
        .type = SCRIPT_INT
    },
    {
        /* 46 */
        .id = OPENSAVE_RECENT_LIST_OF_FILES,
        .label = "Recent files",
        .key = "OpenSave_RecentFiles",
        .value = "",
        .type = SCRIPT_TABLE
    },
    {
        /* 47 */
        .id = OPENSAVE_RECENT_DIRECTORY,
        .label = "Recent directory",
        .key = "OpenSave_RecentDirectory",
        .value = "",
        .type = SCRIPT_STRING
    },
    {
        /* 48 */
        .id = OPENSAVE_TRIM_DST_NUM_JUMPS,
        .label = "Number of jumps to become trim DST",
        .key = "OpenSave_TrimDstNumJumps",
        .value = "5",
        .type = SCRIPT_INT
    },
    {
        /* 49 */
        .id = PRINTING_DEFAULT_DEVICE,
        .label = "Default device",
        .key = "Printing_DefaultDevice",
        .value = "",
        .type = SCRIPT_STRING
    },
    {
        /* 50 */
        .id = PRINTING_USE_LAST_DEVICE,
        .label = "Use last device",
        .key = "Printing_UseLastDevice",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 51 */
        .id = PRINTING_DISABLE_BG,
        .label = "Disable background",
        .key = "Printing_DisableBG",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 52 */
        .id = GRID_SHOW_ON_LOAD,
        .label = "Show on load",
        .key = "Grid_ShowOnLoad",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 53 */
        .id = GRID_SHOW_ORIGIN,
        .label = "Show origin",
        .key = "Grid_ShowOrigin",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 54 */
        .id = GRID_COLOR_MATCH_CROSSHAIR,
        .label = "Grid color matches cross-hair color",
        .key = "Grid_ColorMatchCrossHair",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 55 */
        .id = GRID_COLOR,
        .label = "Color",
        .key = "Grid_Color",
        .value = "0",
        .type = SCRIPT_INT
    },
    {
        /* 56 */
        .id = GRID_LOAD_FROM_FILE,
        .label = "Load from file",
        .key = "Grid_LoadFromFile",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 57 */
        .id = GRID_TYPE,
        .label = "Type",
        .key = "Grid_Type",
        .value = "Rectangular",
        .type = SCRIPT_STRING
    },
    {
        /* 58 */
        .id = GRID_CENTER_ON_ORIGIN,
        .label = "Center on origin",
        .key = "Grid_CenterOnOrigin",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 59 */
        .id = GRID_CENTER_X,
        .label = "Center x",
        .key = "Grid_CenterX",
        .value = "0.0",
        .type = SCRIPT_REAL
    },
    {
        /* 60 */
        .id = GRID_CENTER_Y,
        .label = "Center y",
        .key = "Grid_CenterY",
        .value = "0.0",
        .type = SCRIPT_REAL
    },
    {
        /* 61 */
        .id = GRID_SIZE_X,
        .label = "X size",
        .key = "Grid_SizeX",
        .value = "100.0",
        .type = SCRIPT_REAL
    },
    {
        /* 62 */
        .id = GRID_SIZE_Y,
        .label = "Y size",
        .key = "Grid_SizeY",
        .value = "100.0",
        .type = SCRIPT_REAL
    },
    {
        /* 63 */
        .id = GRID_SPACING_X,
        .label = "X spacing",
        .key = "Grid_SpacingX",
        .value = "25.0",
        .type = SCRIPT_REAL
    },
    {
        /* 64 */
        .id = GRID_SPACING_Y,
        .label = "Y spacing",
        .key = "Grid_SpacingY",
        .value = "25.0",
        .type = SCRIPT_REAL
    },
    {
        /* 65 */
        .id = GRID_SIZE_RADIUS,
        .label = "Size radius",
        .key = "Grid_SizeRadius",
        .value = "50.0",
        .type = SCRIPT_REAL
    },
    {
        /* 66 */
        .id = GRID_SPACING_RADIUS,
        .label = "Spacing radius",
        .key = "Grid_SpacingRadius",
        .value = "25.0",
        .type = SCRIPT_REAL
    },
    {
        /* 67 */
        .id = GRID_SPACING_ANGLE,
        .label = "Spacing angle",
        .key = "Grid_SpacingAngle",
        .value = "45.0",
        .type = SCRIPT_REAL
    },
    {
        /* 68 */
        .id = GRID_SHOW_ON_LOAD,
        .label = "Show on load",
        .key = "Ruler_ShowOnLoad",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 69 */
        .id = RULER_METRIC,
        .label = "Metric",
        .key = "Ruler_Metric",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 70 */
        .id = RULER_COLOR,
        .label = "Color",
        .key = "Ruler_Color",
        .value = "13816370",
        .type = SCRIPT_INT
    },
    {
        /* 71 */
        .id = RULER_PIXEL_SIZE,
        .label = "Pixel size",
        .key = "Ruler_PixelSize",
        .value = "20",
        .type = SCRIPT_INT
    },
    {
        /* 72 */
        .id = QSNAP_ENABLED,
        .label = "Enabled",
        .key = "QuickSnap_Enabled",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 73 */
        .id = QSNAP_LOCATOR_COLOR,
        .label = "Locator color",
        .key = "QuickSnap_LocatorColor",
        .value = "16776960",
        .type = SCRIPT_INT
    },
    {
        /* 74 */
        .id = QSNAP_LOCATOR_SIZE,
        .label = "Locator size",
        .key = "QuickSnap_LocatorSize",
        .value = "4",
        .type = SCRIPT_INT
    },
    {
        /* 75 */
        .id = QSNAP_APERTURE_SIZE,
        .label = "Aperture size",
        .key = "QuickSnap_ApertureSize",
        .value = "10",
        .type = SCRIPT_INT
    },
    {
        /* 76 */
        .id = QSNAP_ENDPOINT,
        .label = "End point",
        .key = "QuickSnap_EndPoint",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 77 */
        .id = QSNAP_MIDPOINT,
        .label = "Mid point",
        .key = "QuickSnap_MidPoint",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 78 */
        .id = QSNAP_CENTER,
        .label = "Center",
        .key = "QuickSnap_Center",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 79 */
        .id = QSNAP_NODE,
        .label = "Node",
        .key = "QuickSnap_Node",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 80 */
        .id = QSNAP_QUADRANT,
        .label = "Quadrant",
        .key = "QuickSnap_Quadrant",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 81 */
        .id = QSNAP_INTERSECTION,
        .label = "Intersection",
        .key = "QuickSnap_Intersection",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 82 */
        .id = QSNAP_EXTENSION,
        .label = "Extension",
        .key = "QuickSnap_Extension",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 83 */
        .id = QSNAP_INSERTION,
        .label = "Insertion",
        .key = "QuickSnap_Insertion",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 84 */
        .id = QSNAP_PERPENDICULAR,
        .label = "Perpendicular",
        .key = "QuickSnap_Perpendicular",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 85 */
        .id = QSNAP_TANGENT,
        .label = "Tangent",
        .key = "QuickSnap_Tangent",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 86 */
        .id = QSNAP_NEAREST,
        .label = "Nearest",
        .key = "QuickSnap_Nearest",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 87 */
        .id = QSNAP_APPARENT,
        .label = "Apparent",
        .key = "QuickSnap_Apparent",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 88 */
        .id = QSNAP_PARALLEL,
        .label = "Parallel",
        .key = "QuickSnap_Parallel",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 89 */
        .id = LWT_SHOW_LWT,
        .label = "Show line weight",
        .key = "LineWeight_ShowLineWeight",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 90 */
        .id = LWT_REAL_RENDER,
        .label = "Real render",
        .key = "LineWeight_RealRender",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 91 */
        .id = LWT_DEFAULT_LWT,
        .label = "Default line weight",
        .key = "LineWeight_DefaultLineWeight",
        .value = "0",
        .type = SCRIPT_REAL
    },
    {
        /* 92 */
        .id = SELECTION_MODE_PICKFIRST,
        .label = "Pick first",
        .key = "Selection_PickFirst",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 93 */
        .id = SELECTION_MODE_PICKADD,
        .label = "Pick add",
        .key = "Selection_PickAdd",
        .value = "1",
        .type = SCRIPT_BOOL
    },
    {
        /* 94 */
        .id = SELECTION_MODE_PICKDRAG,
        .label = "Pick drag",
        .key = "Selection_PickDrag",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 95 */
        .id = SELECTION_COOLGRIP_COLOR,
        .label = "Cool grip color",
        .key = "Selection_CoolGripColor",
        .value = "255",
        .type = SCRIPT_INT
    },
    {
        /* 96 */
        .id = SELECTION_HOTGRIP_COLOR,
        .label = "Hot grip color",
        .key = "Selection_HotGripColor",
        .value = "16711680",
        .type = SCRIPT_INT
    },
    {
        /* 97 */
        .id = SELECTION_GRIP_SIZE,
        .label = "Grip size",
        .key = "Selection_GripSize",
        .value = "4",
        .type = SCRIPT_INT
    },
    {
        /* 98 */
        .id = SELECTION_PICKBOX_SIZE,
        .label = "Pick box size",
        .key = "Selection_PickBoxSize",
        .value = "4",
        .type = SCRIPT_INT
    },
    {
        /* 99 */
        .id = TEXT_FONT,
        .label = "Font",
        .key = "Text_Font",
        .value = "Arial",
        .type = SCRIPT_STRING
    },
    {
        /* 100 */
        .id = TEXT_SIZE,
        .label = "Size",
        .key = "Text_Size",
        .value = "12",
        .type = SCRIPT_REAL
    },
    {
        /* 101 */
        .id = TEXT_ANGLE,
        .label = "Angle",
        .key = "Text_Angle",
        .value = "0",
        .type = SCRIPT_REAL
    },
    {
        /* 102 */
        .id = TEXT_STYLE_BOLD,
        .label = "Bold",
        .key = "Text_StyleBold",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 103 */
        .id = TEXT_STYLE_ITALIC,
        .label = "Italic",
        .key = "Text_StyleItalic",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 104*/
        .id = TEXT_STYLE_UNDERLINE,
        .label = "Underline",
        .key = "Text_StyleUnderline",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 105 */
        .id = TEXT_STYLE_OVERLINE,
        .label = "Overline",
        .key = "Text_StyleOverline",
        .value = "0",
        .type = SCRIPT_BOOL
    },
    {
        /* 106 */
        .id = TEXT_STYLE_STRIKEOUT,
        .label = "Strikeout",
        .key = "Text_StyleStrikeOut",
        .value = "0",
        .type = SCRIPT_BOOL
    }
};

int render_hints[] = {
    DISPLAY_USE_OPENGL,
    DISPLAY_RENDERHINT_AA,
    DISPLAY_RENDERHINT_TEXT_AA,
    DISPLAY_RENDERHINT_SMOOTH_PIX,
    DISPLAY_RENDERHINT_HIGH_AA,
    DISPLAY_RENDERHINT_NONCOSMETIC,
    TERMINATOR_SYMBOL
};

char *extensions[] = {
    "100",
    "10o",
    "ART",
    "BMC",
    "BRO",
    "CND",
    "COL",
    "CSD",
    "CSV",
    "DAT",
    "DEM",
    "DSB",
    "DST",
    "DSZ",
    "DXF",
    "EDR",
    "EMD",
    "EXP",
    "EXY",
    "EYS",
    "FXY",
    "GNC",
    "GT",
    "HUS",
    "INB",
    "JEF",
    "KSM",
    "PCD",
    "PCM",
    "PCQ",
    "PCS",
    "PEC",
    "PEL",
    "PEM",
    "PES",
    "PHB",
    "PHC",
    "RGB",
    "SEW",
    "SHV",
    "SST",
    "STX",
    "SVG",
    "T09",
    "TAP",
    "THR",
    "TXT",
    "U00",
    "U01",
    "VIP",
    "VP3",
    "XXX",
    "ZSK",
    "END"
};

char *grid_load_from_file_group[] = {
    "labelGridType",
    "comboBoxGridType",
    "checkBoxGridCenterOnOrigin",
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

char *defined_origin_group[] = {
    "labelGridCenterX",
    "spinBoxGridCenterX",
    "labelGridCenterY",
    "spinBoxGridCenterY",
    "labelGridSizeX",
    "spinBoxGridSizeX",
    "labelGridSizeY",
    "spinBoxGridSizeY",
    "END"
};

char *rectangular_grid_group[] = {
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

char *circular_grid_group[] = {
    "labelGridSizeRadius",
    "spinBoxGridSizeRadius",
    "labelGridSpacingRadius",
    "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle",
    "spinBoxGridSpacingAngle",
    "END"
};

/* FIXME:. */
char *center_on_origin_group[] = {
    "END"
};

