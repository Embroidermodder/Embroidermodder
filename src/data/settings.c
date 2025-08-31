/* Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 */

#include "data.h"

const SettingsData settings_table[N_SETTINGS] = {
    [ST_LANGUAGE] = {
        .section = "General",
        .key = "Language",
        .default_value = "default",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_ICON_THEME] = {
        .section = "General",
        .key = "IconTheme",
        .default_value = "default",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_ICON_SIZE] = {
        .section = "General",
        .key = "IconSize",
        .default_value = "16",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_MDI_BG_USE_LOGO] = {
        .section = "General",
        .key = "MdiBGUseLogo",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_MDI_BG_USE_TEXTURE] = {
        .section = "General",
        .key = "MdiBGUseTexture",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_MDI_BG_USE_COLOR] = {
        .section = "General",
        .key = "MdiBGUseColor",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_MDI_BG_LOGO] = {
        .section = "General",
        .key = "MdiBGLogo",
        .default_value = "images/logo-spirals.png",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_MDI_BG_TEXTURE] = {
        .section = "General",
        .key = "MdiBGTexture",
        .default_value = "images/texture-spirals.png",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_MDI_BG_COLOR] = {
        .section = "General",
        .key = "MdiBGColor",
        .default_value = "12632256",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_TIP_OF_THE_DAY] = {
        .section = "General",
        .key = "TipOfTheDay",
        .default_value = "true",
        .type = 'b',
        .description = "Show Tip of the Day on startup",
        .enabled = true,
        .icon = ""
    },
    [ST_CURRENT_TIP] = {
        .section = "General",
        .key = "CurrentTip",
        .default_value = "0",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_SYSTEM_HELP_BROWSER] = {
        .section = "General",
        .key = "SystemHelpBrowser",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_CHECK_FOR_UPDATES] = {
        .section = "General",
        .key = "CheckForUpdates",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_USE_OPENGL] = {
        .section = "Display",
        .key = "UseOpenGL",
        .default_value = "false",
        .type = 'b',
        .description = "Use OpenGL",
        .enabled = false,
        .icon = ""
    },
    [ST_RENDERHINT_AA] = {
        .section = "Display",
        .key = "RenderHintAntiAlias",
        .default_value = "false",
        .type = 'b',
        .description = "Antialias",
        .enabled = false,
        .icon = ""
    },
    [ST_RENDERHINT_TEXT_AA] = {
        .section = "Display",
        .key = "RenderHintTextAntiAlias",
        .default_value = "false",
        .type = 'b',
        .description = "Antialias Text",
        .enabled = false,
        .icon = ""
    },
    [ST_RENDERHINT_SMOOTHPIX] = {
        .section = "Display",
        .key = "RenderHintSmoothPixmap",
        .default_value = "false",
        .type = 'b',
        .description = "Smooth Pixmap",
        .enabled = false,
        .icon = ""
    },
    [ST_RENDERHINT_HIGH_AA] = {
        .section = "Display",
        .key = "RenderHintHighQualityAntiAlias",
        .default_value = "false",
        .type = 'b',
        .description = "High Quality Antialiasing (OpenGL)",
        .enabled = false,
        .icon = ""
    },
    [ST_RENDERHINT_NONCOSMETIC] = {
        .section = "Display",
        .key = "RenderHintNonCosmetic",
        .default_value = "false",
        .type = 'b',
        .description = "Non Cosmetic",
        .enabled = false,
        .icon = ""
    },
    [ST_SHOW_SCROLLBARS] = {
        .section = "Display",
        .key = "ShowScrollBars",
        .default_value = "true",
        .type = 'b',
        .enabled = false,
        .icon = ""
    },
    [ST_SCROLLBAR_WIDGET_NUM] = {
        .section = "Display",
        .key = "ScrollBarWidgetNum",
        .default_value = "0",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_CROSSHAIR_COLOR] = {
        .section = "Display",
        .key = "CrossHairColor",
        .default_value = "0",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_BG_COLOR] = {
        .section = "Display",
        .key = "BackgroundColor",
        .default_value = "15461355",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTBOX_LEFT_COLOR] = {
        .section = "Display",
        .key = "SelectBoxLeftColor",
        .default_value = "32768",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTBOX_LEFT_FILL] = {
        .section = "Display",
        .key = "SelectBoxLeftFill",
        .default_value = "65280",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTBOX_RIGHT_COLOR] = {
        .section = "Display",
        .key = "SelectBoxRightColor",
        .default_value = "128",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTBOX_RIGHT_FILL] = {
        .section = "Display",
        .key = "SelectBoxRightFill",
        .default_value = "255",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTBOX_ALPHA] = {
        .section = "Display",
        .key = "SelectBoxAlpha",
        .default_value = "32",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_ZOOMSCALE_IN] = {
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
    [ST_ZOOMSCALE_OUT] = {
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
    [ST_CROSSHAIR_PERCENT] = {
        .section = "Display",
        .key = "CrossHairPercent",
        .default_value = "5",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_UNITS] = {
        .section = "Display",
        .key = "Units",
        .default_value = "mm",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_PROMPT_TEXT_COLOR] = {
        .section = "Prompt",
        .key = "TextColor",
        .default_value = "0",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_PROMPT_BG_COLOR] = {
        .section = "Prompt",
        .key = "BackgroundColor",
        .default_value = "16777215",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_PROMPT_FONT_FAMILY] = {
        .section = "Prompt",
        .key = "FontFamily",
        .default_value = "Monospace",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_PROMPT_FONT_STYLE] = {
        .section = "Prompt",
        .key = "FontStyle",
        .default_value = "normal",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_PROMPT_FONT_SIZE] = {
        .section = "Prompt",
        .key = "FontSize",
        .default_value = "12",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_PROMPT_SAVE_HISTORY] = {
        .section = "Prompt",
        .key = "SaveHistory",
        .default_value = "true",
        .type = 'b',
        .description = "Save History",
        .enabled = true,
        .icon = ""
    },
    [ST_PROMPT_SAVE_AS_HTML] = {
        .section = "Prompt",
        .key = "SaveHistoryAsHtml",
        .default_value = "false",
        .type = 'b',
        .description = "Save As HTML",
        .enabled = true,
        .icon = ""
    },
    /* FIXME: this file sits in settingsDir rather than appDir */
    [ST_PROMPT_SAVE_FNAME] = {
        .section = "Prompt",
        .key = "SaveHistoryFilename",
        .default_value = "prompt.log",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_OPENSAVE_CUSTOM_FILTER] = {
        .section = "OpenSave",
        .key = "CustomFilter",
        .default_value = "supported",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_OPEN_FORMAT] = {
        .section = "OpenSave",
        .key = "OpenFormat",
        .default_value = "*.*",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_OPEN_THUMBNAIL] = {
        .section = "OpenSave",
        .key = "OpenThumbnail",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_SAVE_FORMAT] = {
        .section = "OpenSave",
        .key = "SaveFormat",
        .default_value = "*.*",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_SAVE_THUMBNAIL] = {
        .section = "OpenSave",
        .key = "SaveThumbnail",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_RECENT_MAX_FILES] = {
        .section = "OpenSave",
        .key = "RecentMax",
        .default_value = "10",
        .type = 'i',
        .enabled = true

    },
    [ST_RECENT_FILES] = {
        .section = "OpenSave",
        .key = "RecentFiles",
        .default_value = "",
        .type = 'l',
        .enabled = true,
        .icon = ""
    },
    [ST_RECENT_DIRECTORY] = {
        .section = "OpenSave",
        .key = "RecentDirectory",
        .default_value = "samples",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_TRIM_DST_NUM_JUMPS] = {
        .section = "OpenSave",
        .key = "TrimDstNumJumps",
        .default_value = "5",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_PRINTING_DEFAULT_DEVICE] = {
        .section = "Printing",
        .key = "DefaultDevice",
        .default_value = "",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_PRINTING_USE_LAST_DEVICE] = {
        .section = "Printing",
        .key = "UseLastDevice",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_PRINTING_DISABLE_BG] = {
        .section = "Printing",
        .key = "DisableBG",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_GRID_SHOW_ON_LOAD] = {
        .section = "Grid",
        .key = "ShowOnLoad",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_GRID_SHOW_ORIGIN] = {
        .section = "Grid",
        .key = "ShowOrigin",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    /* FIXME: override the grid color if this is true on load. */
    [ST_GRID_COLOR_MATCH_CROSSHAIR] = {
        .section = "Grid",
        .key = "ColorMatchCrossHair",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_GRID_COLOR] = {
        .section = "Grid",
        .key = "Color",
        .default_value = "0",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_GRID_LOAD_FROM_FILE] = {
        .section = "Grid",
        .key = "LoadFromFile",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_GRID_TYPE] = {
        .section = "Grid",
        .key = "Type",
        .default_value = "Rectangular",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_GRID_CENTER_ON_ORIGIN] = {
        .section = "Grid",
        .key = "CenterOnOrigin",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_GRID_CENTER_X] = {
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
    [ST_GRID_CENTER_Y] = {
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
    [ST_GRID_SIZE_X] = {
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
    [ST_GRID_SIZE_Y] = {
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
    [ST_GRID_SPACING_X] = {
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
    [ST_GRID_SPACING_Y] = {
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
    [ST_GRID_SIZE_RADIUS] = {
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
    [ST_GRID_SPACING_RADIUS] = {
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
    [ST_GRID_SPACING_ANGLE] = {
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
    [ST_RULER_SHOW_ON_LOAD] = {
        .section = "Ruler",
        .key = "ShowOnLoad",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_RULER_METRIC] = {
        .section = "Ruler",
        .key = "Metric",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_RULER_COLOR] = {
        .section = "Ruler",
        .key = "Color",
        .default_value = "13816370",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_RULER_PIXEL_SIZE] = {
        .section = "Ruler",
        .key = "PixelSize",
        .default_value = "20",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_QSNAP_ENABLED] = {
        .section = "QuickSnap",
        .key = "Enabled",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_QSNAP_LOCATOR_COLOR] = {
        .section = "QuickSnap",
        .key = "LocatorColor",
        .default_value = "16776960",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_QSNAP_LOCATOR_SIZE] = {
        .section = "QuickSnap",
        .key = "LocatorSize",
        .default_value = "4",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_QSNAP_APERTURE_SIZE] = {
        .section = "QuickSnap",
        .key = "ApertureSize",
        .default_value = "10",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_QSNAP_ENDPOINT] = {
        .section = "QuickSnap",
        .key = "EndPoint",
        .default_value = "true",
        .type = 'b',
        .description = "Endpoint",
        .enabled = true,
        .icon = "locator-snaptoendpoint"
    },
    [ST_QSNAP_MIDPOINT] = {
        .section = "QuickSnap",
        .key = "MidPoint",
        .default_value = "true",
        .type = 'b',
        .description = "Midpoint",
        .enabled = true,
        .icon = "locator-snaptomidpoint"
    },
    [ST_QSNAP_CENTER] = {
        .section = "QuickSnap",
        .key = "Center",
        .default_value = "true",
        .type = 'b',
        .description = "Center",
        .enabled = true,
        .icon = "locator-snaptocenter"
    },
    [ST_QSNAP_NODE] = {
        .section = "QuickSnap",
        .key = "Node",
        .default_value = "true",
        .type = 'b',
        .description = "Node",
        .enabled = true,
        .icon = "locator-snaptonode"
    },
    [ST_QSNAP_QUADRANT] = {
        .section = "QuickSnap",
        .key = "Quadrant",
        .default_value = "true",
        .type = 'b',
        .description = "Quadrant",
        .enabled = true,
        .icon = "locator-snaptoquadrant"
    },
    [ST_QSNAP_INTERSECTION] = {
        .section = "QuickSnap",
        .key = "Intersection",
        .default_value = "true",
        .type = 'b',
        .description = "Intersection",
        .enabled = true,
        .icon = "locator-snaptointersection"
    },
    [ST_QSNAP_EXTENSION] = {
        .section = "QuickSnap",
        .key = "Extension",
        .default_value = "true",
        .type = 'b',
        .description = "Extension",
        .enabled = true,
        .icon = "locator-snaptoextension"
    },
    [ST_QSNAP_INSERTION] = {
        .section = "QuickSnap",
        .key = "Insertion",
        .default_value = "false",
        .type = 'b',
        .description = "Insertion",
        .enabled = true,
        .icon = "locator-snaptoinsertion"
    },
    [ST_QSNAP_PERPENDICULAR] = {
        .section = "QuickSnap",
        .key = "Perpendicular",
        .default_value = "true",
        .type = 'b',
        .description = "Perpendicular",
        .enabled = true,
        .icon = "locator-snaptoperpendicular"
    },
    [ST_QSNAP_TANGENT] = {
        .section = "QuickSnap",
        .key = "Tangent",
        .default_value = "true",
        .type = 'b',
        .description = "Tangent",
        .enabled = true,
        .icon = "locator-snaptotangent"
    },
    [ST_QSNAP_NEAREST] = {
        .section = "QuickSnap",
        .key = "Nearest",
        .default_value = "false",
        .type = 'b',
        .description = "Nearest",
        .enabled = true,
        .icon = "locator-snaptonearest"
    },
    [ST_QSNAP_APPARENT] = {
        .section = "QuickSnap",
        .key = "Apparent",
        .default_value = "false",
        .type = 'b',
        .description = "Apparent Intersection",
        .enabled = true,
        .icon = "locator-snaptoapparent"
    },
    [ST_QSNAP_PARALLEL] = {
        .section = "QuickSnap",
        .key = "Parallel",
        .default_value = "false",
        .type = 'b',
        .description = "Parallel",
        .enabled = true,
        .icon = "locator-snaptoparallel"
    },
    [ST_LWT_SHOW_LWT] = {
        .section = "LineWeight",
        .key = "ShowLineWeight",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_LWT_REAL_RENDER] = {
        .section = "LineWeight",
        .key = "RealRender",
        .default_value = "true",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_LWT_DEFAULT_LWT] = {
        .section = "LineWeight",
        .key = "DefaultLineWeight",
        .default_value = "0",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTION_MODE_PICKFIRST] = {
        .section = "Selection",
        .key = "PickFirst",
        .default_value = "true",
        .type = 'b',
        .description = "Allow Preselection (PickFirst)",
        /* TODO: Remove this line when Post-selection is available */
        .enabled = false,
        .icon = ""
    },
    [ST_SELECTION_MODE_PICKADD] = {
        .section = "Selection",
        .key = "PickAdd",
        .default_value = "true",
        .type = 'b',
        .description = "Add to Selection (PickAdd)",
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTION_MODE_PICKDRAG] = {
        .section = "Selection",
        .key = "PickDrag",
        .default_value = "false",
        .type = 'b',
        .description = "Drag to Select (PickDrag)",
        /* TODO: Remove this line when available. */
        .enabled = false,
        .icon = ""
    },
    [ST_SELECTION_COOLGRIP_COLOR] = {
        .section = "Selection",
        .key = "CoolGripColor",
        .default_value = "255",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTION_HOTGRIP_COLOR] = {
        .section = "Selection",
        .key = "HotGripColor",
        .default_value = "0xFF0000",
        .type = 'c',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTION_GRIP_SIZE] = {
        .section = "Selection",
        .key = "GripSize",
        .default_value = "4",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_SELECTION_PICKBOX_SIZE] = {
        .section = "Selection",
        .key = "PickBoxSize",
        .default_value = "4",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_TEXT_FONT] = {
        .section = "Text",
        .key = "Font",
        .default_value = "Arial",
        .type = 's',
        .enabled = true,
        .icon = ""
    },
    [ST_TEXT_SIZE] = {
        .section = "Text",
        .key = "Size",
        .default_value = "12",
        .type = 'i',
        .enabled = true,
        .icon = ""
    },
    [ST_TEXT_ANGLE] = {
        .section = "Text",
        .key = "Angle",
        .default_value = "0.0",
        .type = 'r',
        .enabled = true,
        .icon = ""
    },
    [ST_TEXT_BOLD] = {
        .section = "Text",
        .key = "StyleBold",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_TEXT_ITALIC] = {
        .section = "Text",
        .key = "StyleItalic",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_TEXT_UNDERLINE] = {
        .section = "Text",
        .key = "StyleUnderline",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_TEXT_OVERLINE] = {
        .section = "Text",
        .key = "StyleOverline",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    },
    [ST_TEXT_STRIKEOUT] = {
        .section = "Text",
        .key = "StyleStrikeOut",
        .default_value = "false",
        .type = 'b',
        .enabled = true,
        .icon = ""
    }
};

