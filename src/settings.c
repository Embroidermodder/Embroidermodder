/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Settings: data for each setting, including defaults.
 */

#include <stdbool.h>

#include "core.h"

int preview_to_dialog[] = {
    ST_MDI_BG_USE_LOGO,
    ST_MDI_BG_USE_TEXTURE,
    ST_MDI_BG_USE_COLOR,
    ST_SHOW_SCROLLBARS,
    ST_SELECTBOX_ALPHA,
    ST_PROMPT_FONT_FAMILY,
    ST_PROMPT_FONT_STYLE,
    ST_PROMPT_FONT_SIZE,
    ST_LWT_SHOW_LWT,
    ST_LWT_REAL_RENDER,
    -1
};

int accept_to_dialog[] = {
    ST_MDI_BG_LOGO,
    ST_MDI_BG_TEXTURE,
    ST_MDI_BG_COLOR,
    ST_CROSSHAIR_COLOR,
    ST_BG_COLOR,
    ST_SELECTBOX_LEFT_COLOR,
    ST_SELECTBOX_LEFT_FILL,
    ST_SELECTBOX_RIGHT_COLOR,
    ST_SELECTBOX_RIGHT_FILL,
    ST_PROMPT_TEXT_COLOR,
    ST_PROMPT_BG_COLOR,
    ST_RULER_COLOR,
    ST_GRID_COLOR,
    -1
};

const char *settings_tabs[] = {
    "General",
    "Files/Path",
    "Display",
    "Prompt",
    "Open/Save",
    "Printing",
    "Snap",
    "Grid/Ruler",
    "Ortho/Polar",
    "QuickSnap",
    "QuickTrack",
    "LineWeight",
    "Selection",
    "^END"
};

/*
 * NOTE: The order of this table matters, so it has to be an array.
 *       It is indexed against the ST_* macro constants.
 */
SettingsData settings_table[N_SETTINGS] = {
    {
        .section = "General",
        .key = "Language",
        .default_value = "default",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "IconTheme",
        .default_value = "default",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "IconSize",
        .default_value = "16",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "MdiBGUseLogo",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "MdiBGUseTexture",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "MdiBGUseColor",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "MdiBGLogo",
        .default_value = "images/logo-spirals.png",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "MdiBGTexture",
        .default_value = "images/texture-spirals.png",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "MdiBGColor",
        .default_value = "12632256",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "TipOfTheDay",
        .default_value = "true",
        .type = 'b',
        .description = "Show Tip of the Day on startup",
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "CurrentTip",
        .default_value = "0",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "SystemHelpBrowser",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "General",
        .key = "CheckForUpdates",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "UseOpenGL",
        .default_value = "false",
        .type = 'b',
        .description = "Use OpenGL",
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "RenderHintAntiAlias",
        .default_value = "false",
        .type = 'b',
        .description = "Antialias",
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "RenderHintTextAntiAlias",
        .default_value = "false",
        .type = 'b',
        .description = "Antialias Text",
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "RenderHintSmoothPixmap",
        .default_value = "false",
        .type = 'b',
        .description = "Smooth Pixmap",
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "RenderHintHighQualityAntiAlias",
        .default_value = "false",
        .type = 'b',
        .description = "High Quality Antialiasing (OpenGL)",
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "RenderHintNonCosmetic",
        .default_value = "false",
        .type = 'b',
        .description = "Non Cosmetic",
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "ShowScrollBars",
        .default_value = "true",
        .type = 'b',
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "ScrollBarWidgetNum",
        .default_value = "0",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "CrossHairColor",
        .default_value = "0",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "BackgroundColor",
        .default_value = "15461355",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "SelectBoxLeftColor",
        .default_value = "32768",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "SelectBoxLeftFill",
        .default_value = "65280",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "SelectBoxRightColor",
        .default_value = "128",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "SelectBoxRightFill",
        .default_value = "255",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "SelectBoxAlpha",
        .default_value = "32",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "ZoomScaleIn",
        .default_value = "2.0",
        .type = 'r',
        .description = "Zoom In Scale",
        .enabled = true,
        .icon = "",
        .lower = 1.01,
        .upper = 10.0,
        .single_step = 0.01
    },
    {
        .section = "Display",
        .key = "ZoomScaleOut",
        .default_value = "0.5",
        .type = 'r',
        .description = "Zoom Out Scale",
        .enabled = true,
        .icon = "",
        .lower = 0.01,
        .upper = 0.99,
        .single_step = 0.01
    },
    {
        .section = "Display",
        .key = "CrossHairPercent",
        .default_value = "5",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Display",
        .key = "Units",
        .default_value = "mm",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Prompt",
        .key = "TextColor",
        .default_value = "0",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Prompt",
        .key = "BackgroundColor",
        .default_value = "16777215",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Prompt",
        .key = "FontFamily",
        .default_value = "Monospace",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Prompt",
        .key = "FontStyle",
        .default_value = "normal",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Prompt",
        .key = "FontSize",
        .default_value = "12",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Prompt",
        .key = "SaveHistory",
        .default_value = "true",
        .type = 'b',
        .description = "Save History",
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Prompt",
        .key = "SaveHistoryAsHtml",
        .default_value = "false",
        .type = 'b',
        .description = "Save As HTML",
        .enabled = true,
        .icon = ""
    },
    // FIXME: this file sits in settingsDir rather than appDir
    {
        .section = "Prompt",
        .key = "SaveHistoryFilename",
        .default_value = "promptlog",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "OpenSave",
        .key = "CustomFilter",
        .default_value = "supported",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "OpenSave",
        .key = "OpenFormat",
        .default_value = "*.*",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "OpenSave",
        .key = "OpenThumbnail",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "OpenSave",
        .key = "SaveFormat",
        .default_value = "*.*",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "OpenSave",
        .key = "SaveThumbnail",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "OpenSave",
        .key = "RecentMax",
        .default_value = "10",
        .type = 'i',
        .enabled = true

    },
    {
        .section = "OpenSave",
        .key = "RecentFiles",
        .default_value = "",
        .type = 'l',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "OpenSave",
        .key = "RecentDirectory",
        .default_value = "samples",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "OpenSave",
        .key = "TrimDstNumJumps",
        .default_value = "5",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Printing",
        .key = "DefaultDevice",
        .default_value = "",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Printing",
        .key = "UseLastDevice",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Printing",
        .key = "DisableBG",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Grid",
        .key = "ShowOnLoad",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Grid",
        .key = "ShowOrigin",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    // FIXME: override the grid color if this is true on load.
    {
        .section = "Grid",
        .key = "ColorMatchCrossHair",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Grid",
        .key = "Color",
        .default_value = "0",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Grid",
        .key = "LoadFromFile",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Grid",
        .key = "Type",
        .default_value = "Rectangular",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Grid",
        .key = "CenterOnOrigin",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Grid",
        .key = "CenterX",
        .default_value = "0.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = -1000.0,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Grid",
        .key = "CenterY",
        .default_value = "0.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = -1000.0,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Grid",
        .key = "SizeX",
        .default_value = "100.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = 1.0,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Grid",
        .key = "SizeY",
        .default_value = "100.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = 1.0,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Grid",
        .key = "SpacingX",
        .default_value = "25.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = 0.001,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Grid",
        .key = "SpacingY",
        .default_value = "25.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = 0.001,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Grid",
        .key = "SizeRadius",
        .default_value = "50.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = 1.0,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Grid",
        .key = "SpacingRadius",
        .default_value = "25.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = 0.001,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Grid",
        .key = "SpacingAngle",
        .default_value = "45.0",
        .type = 'r',
        .enabled = true,
        .icon = "",
        .lower = 0.001,
        .upper = 1000.0,
        .single_step = 1.000
    },
    {
        .section = "Ruler",
        .key = "ShowOnLoad",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Ruler",
        .key = "Metric",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Ruler",
        .key = "Color",
        .default_value = "13816370",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Ruler",
        .key = "PixelSize",
        .default_value = "20",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "QuickSnap",
        .key = "Enabled",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "QuickSnap",
        .key = "LocatorColor",
        .default_value = "16776960",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "QuickSnap",
        .key = "LocatorSize",
        .default_value = "4",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "QuickSnap",
        .key = "ApertureSize",
        .default_value = "10",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "QuickSnap",
        .key = "EndPoint",
        .default_value = "true",
        .type = 'b',
        .description = "Endpoint",
        .enabled = true,
        .icon = "locator-snaptoendpoint"
    },
    {
        .section = "QuickSnap",
        .key = "MidPoint",
        .default_value = "true",
        .type = 'b',
        .description = "Midpoint",
        .enabled = true,
        .icon = "locator-snaptomidpoint"
    },
    {
        .section = "QuickSnap",
        .key = "Center",
        .default_value = "true",
        .type = 'b',
        .description = "Center",
        .enabled = true,
        .icon = "locator-snaptocenter"
    },
    {
        .section = "QuickSnap",
        .key = "Node",
        .default_value = "true",
        .type = 'b',
        .description = "Node",
        .enabled = true,
        .icon = "locator-snaptonode"
    },
    {
        .section = "QuickSnap",
        .key = "Quadrant",
        .default_value = "true",
        .type = 'b',
        .description = "Quadrant",
        .enabled = true,
        .icon = "locator-snaptoquadrant"
    },
    {
        .section = "QuickSnap",
        .key = "Intersection",
        .default_value = "true",
        .type = 'b',
        .description = "Intersection",
        .enabled = true,
        .icon = "locator-snaptointersection"
    },
    {
        .section = "QuickSnap",
        .key = "Extension",
        .default_value = "true",
        .type = 'b',
        .description = "Extension",
        .enabled = true,
        .icon = "locator-snaptoextension"
    },
    {
        .section = "QuickSnap",
        .key = "Insertion",
        .default_value = "false",
        .type = 'b',
        .description = "Insertion",
        .enabled = true,
        .icon = "locator-snaptoinsertion"
    },
    {
        .section = "QuickSnap",
        .key = "Perpendicular",
        .default_value = "true",
        .type = 'b',
        .description = "Perpendicular",
        .enabled = true,
        .icon = "locator-snaptoperpendicular"
    },
    {
        .section = "QuickSnap",
        .key = "Tangent",
        .default_value = "true",
        .type = 'b',
        .description = "Tangent",
        .enabled = true,
        .icon = "locator-snaptotangent"
    },
    {
        .section = "QuickSnap",
        .key = "Nearest",
        .default_value = "false",
        .type = 'b',
        .description = "Nearest",
        .enabled = true,
        .icon = "locator-snaptonearest"
    },
    {
        .section = "QuickSnap",
        .key = "Apparent",
        .default_value = "false",
        .type = 'b',
        .description = "Apparent Intersection",
        .enabled = true,
        .icon = "locator-snaptoapparent"
    },
    {
        .section = "QuickSnap",
        .key = "Parallel",
        .default_value = "false",
        .type = 'b',
        .description = "Parallel",
        .enabled = true,
        .icon = "locator-snaptoparallel"
    },
    {
        .section = "LineWeight",
        .key = "ShowLineWeight",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "LineWeight",
        .key = "RealRender",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "LineWeight",
        .key = "DefaultLineWeight",
        .default_value = "0",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    // TODO: Enable this when Post-selection is available
    {
        .section = "Selection",
        .key = "PickFirst",
        .default_value = "true",
        .type = 'b',
        .description = "Allow Preselection (PickFirst)",
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Selection",
        .key = "PickAdd",
        .default_value = "true",
        .type = 'b',
        .description = "Add to Selection (PickAdd)",
        .enabled = true,
        .icon = ""
    },
    // TODO: Enable this when pick dragging is available
    {
        .section = "Selection",
        .key = "PickDrag",
        .default_value = "false",
        .type = 'b',
        .description = "Drag to Select (PickDrag)",
        .enabled = false,
        .icon = ""
    },
    {
        .section = "Selection",
        .key = "CoolGripColor",
        .default_value = "255",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Selection",
        .key = "HotGripColor",
        .default_value = "0xFF0000",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Selection",
        .key = "GripSize",
        .default_value = "4",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Selection",
        .key = "PickBoxSize",
        .default_value = "4",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Text",
        .key = "Font",
        .default_value = "Arial",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Text",
        .key = "Size",
        .default_value = "12",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Text",
        .key = "Angle",
        .default_value = "0.0",
        .type = 'r',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Text",
        .key = "StyleBold",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Text",
        .key = "StyleItalic",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Text",
        .key = "StyleUnderline",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Text",
        .key = "StyleOverline",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    {
        .section = "Text",
        .key = "StyleStrikeOut",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    }
};

