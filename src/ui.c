/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * UI data
 *
 * Editor Data
 * TODO: Use proper icons for tool buttons.
 */

#include "core.h" 

WidgetData zoom_gb_data[] = {
    {
        .key = "labelZoomInScale",
        .label = "Zoom In Scale",
        .id = DISPLAY_ZOOMSCALE_IN
    },
    {
        .key = "labelZoomOutScale",
        .label = "Zoom Out Scale",
        .id = DISPLAY_ZOOMSCALE_OUT
    },
    {
        .key = END_SYMBOL,
        .label = END_SYMBOL,
        .id = -1
    }
};

WidgetData grid_gb_data[] = {
    {
        .key = "labelGridCenterX",
        .label = "Grid Center X",
        .id = GRID_CENTER_X
    },
    {
        .key = "labelGridCenterY",
        .label = "Grid Center Y",
        .id = GRID_CENTER_Y
    },
    {
        .key = "labelGridSizeX",
        .label = "Grid Size X",
        .id = GRID_SIZE_X
    },
    {
        .key = "labelGridSizeY",
        .label = "Grid Size Y",
        .id = GRID_SIZE_Y
    },
    {
        .key = "labelGridSpacingX",
        .label = "Grid Spacing X",
        .id = GRID_SPACING_X
    },
    {
        .key = "labelGridSpacingY",
        .label = "Grid Spacing Y",
        .id = GRID_SPACING_Y
    },
    {
        .key = "labelGridSizeRadius",
        .label = "Grid Size Radius",
        .id = GRID_SIZE_RADIUS
    },
    {
        .key = "labelGridSpacingRadius",
        .label = "Grid Spacing Radius",
        .id = GRID_SPACING_RADIUS
    },
    {
        .key = "labelGridSpacingAngle",
        .label = "Grid Spacing Angle",
        .id = GRID_SPACING_ANGLE
    },
    {
        .key = END_SYMBOL,
        .label = END_SYMBOL,
        .id = -1
    }
};

EmbStringTable button_list = {
    "SNAP",
    "GRID",
    "RULER",
    "ORTHO",
    "POLAR",
    "QSNAP",
    "QTRACK",
    "LWT",
    END_SYMBOL
};

char *ui_labels[] = {
    "labelTextColor", "Text Color",
    "labelBackgroundColor", "Background Color",

    "labelTotalStitches", "Total Stitches:",
    "labelRealStitches", "Real Stitches:",
    "labelJumpStitches", "Jump Stitches:",
    "labelTrimStitches", "Trim Stitches:",
    "labelTotalColors", "Total Colors:",
    "labelColorChanges", "Color Changes:",
    "labelRectLeft", "Left:",
    "labelRectTop", "Top:",
    "labelRectRight", "Right:",
    "labelRectBottom", "Bottom:",
    "labelRectWidth", "Width:",
    "labelRectHeight", "Height:",

    "labelCrosshairColor", "Crosshair Color",
    "labelBackgroundColor", "Background Color",
    "labelSelectionColorCrossing", "Selection Box Color (Crossing)",
    "labelSelectionFillCrossing", "Selection Box Fill (Crossing)",
    "labelSelectionColorWindow", "Selection Box Color (Window)",
    "labelSelectionFillWindow", "Selection Box Fill (Window)",
    "labelSelectionFillAlpha", "Selection Box Fill Alpha",

    "labelRulerPixelSize", "Ruler Pixel Size",
    END_SYMBOL
};

char *patternDetailsBox[] = {
    "labelTotalStitches", "fieldTotalStitches",
    "labelRealStitches", "fieldRealStitches",
    "labelJumpStitches", "fieldJumpStitches",
    "labelTrimStitches", "fieldTrimStitches",
    "labelTotalColors", "fieldTotalColors",
    "labelColorChanges", "fieldColorChanges",
    "labelRectLeft", "fieldRectLeft",
    "labelRectTop", "fieldRectTop",
    "labelRectRight", "fieldRectRight",
    "labelRectBottom", "fieldRectBottom",
    "labelRectWidth", "fieldRectWidth",
    "labelRectHeight", "fieldRectHeight",
    END_SYMBOL
};

char *selectionBoxLayout[] = {
    "labelCrosshairColor", "buttonCrosshairColor",
    "labelBackgroundColor", "buttonBackgroundColor",
    "labelSelectionColorCrossing", "buttonSelectionColorCrossing",
    "labelSelectionFillCrossing", "buttonSelectionFillCrossing",
    "labelSelectionColorWindow", "buttonSelectionColorWindow",
    "labelSelectionFillWindow", "buttonSelectionFillWindow",
    "labelSelectionFillAlpha", "spinboxSelectionFillAlpha",
    END_SYMBOL
};

char *promptColorLayout[] = {
    "labelTextColor", "buttonTextColor",
    "labelBackgroundColor", "buttonBackgroundColor",
    END_SYMBOL
};

char *rulerLayout[] = {
    "labelRulerPixelSize", "spinboxRulerPixelSize",
    END_SYMBOL
};

/* The data section is a 4 x n table of line editor data in the order:
 *     (data_type, icon, label, signal_name).
 */
GroupBoxData group_box_list[] = {
    {
        .id = GB_GENERAL,
        .key = "General",
        .label = "General",
        .data = {
            "combobox", "blank", "Layer", "GeneralLayer",
            "combobox", "blank", "Color", "GeneralColor",
            "combobox", "blank", "Line Type", "GeneralLineType",
            "combobox", "blank", "Line Weight", "GeneralLineWeight",
            END_SYMBOL
        },
        .object = 0
    },
    {
        .id = GB_GEOM_ARC,
        .key = "GeometryArc",
        .label = "Geometry",
        .data = {
            "double", "blank", "Center X", "ArcCenterX",
            "double", "blank", "Center Y", "ArcCenterY",
            "double", "blank", "Radius", "ArcRadius",
            "double", "blank", "Start Angle", "ArcStartAngle",
            "double", "blank", "End Angle", "ArcEndAngle",
            "double", "blank", "Start X", "",
            "double", "blank", "Start Y", "",
            "double", "blank", "End X", "",
            "double", "blank", "End Y", "",
            "double", "blank", "Area", "",
            "double", "blank", "Arc Length", "",
            "double", "blank", "Chord", "",
            "double", "blank", "Included Angle", "",
            END_SYMBOL
        },
        .object =  OBJ_ARC
    },
    {
        .id = GB_MISC_ARC,
        .key = "MiscArc",
        .label = "Misc",
        .data = {
            "combobox", "blank", "Clockwise", "ArcClockwise",
            END_SYMBOL
        },
        .object = OBJ_ARC
    },
    {
        .id = GB_GEOM_BLOCK,
        .key = "GeometryBlock",
        .label = "Geometry",
        .data = {
            "double", "blank", "Position X", "BlockPositionX",
            "double", "blank", "Position Y", "BlockPositionY",
            END_SYMBOL
        },
        .object = OBJ_BLOCK
    },
    {
        .id = GB_GEOM_CIRCLE,
        .key = "GeometryCircle",
        .label = "Geometry",
        .data = {
             "double", "blank", "Center X", "CircleCenterX",
             "double", "blank", "Center Y", "CircleCenterY",
             "double", "blank", "Radius", "CircleRadius",
             "double", "blank", "Diameter", "CircleDiameter",
             "double", "blank", "Area", "CircleArea",
             "double", "blank", "Circumference", "CircleCircumference",
             END_SYMBOL
        },
        .object = OBJ_CIRCLE
    },
    {
        .id = GB_GEOM_DIMALIGNED,
        .key = "GeometryDimAligned",
        .label = "Geometry",
        .data = {
            END_SYMBOL
         },
        .object = -1
    },
    {
        .id = GB_GEOM_DIMANGULAR,
        .key = "GeometryDimAngular",
        .label = "Geometry",
        .data = {
            END_SYMBOL
         },
        .object = -1
    },
    {
        .id = GB_GEOM_DIMARCLENGTH,
        .key = "GeometryDimArcLength",
        .label = "Geometry",
        .data = {
            END_SYMBOL
         },
        .object = -1
    },
    {
        .id = GB_GEOM_DIMDIAMETER,
        .key = "GeometryDiameter",
        .label = "Geometry",
        .data = {
            END_SYMBOL
         },
        .object = -1
    },
    {
        .id = GB_GEOM_DIMLEADER,
        .key = "GeometryDimLeader",
        .label = "Geometry",
        .data = {
            END_SYMBOL
         },
        .object = -1
    },
    {
        .id = GB_GEOM_DIMLINEAR,
        .key = "GeometryDimLinear",
        .label = "Geometry",
        .data = {
            END_SYMBOL
         },
        .object = -1
    },
    {
        .id = GB_GEOM_DIMORDINATE,
        .key = "GeometryDimOrdinate",
        .label = "Geometry",
        .data = {
            END_SYMBOL
         },
        .object = -1
    },
    {
        .id = GB_GEOM_DIMRADIUS,
        .key = "GeometryDimRadius",
        .label = "Geometry",
        .data = {
            END_SYMBOL
         },
        .object = -1
    },
    {
        .id = GB_GEOM_ELLIPSE,
        .key = "GeometryEllipse",
        .label = "Geometry",
        .data = {
            "double", "blank", "Center X", "EllipseCenterX",
            "double", "blank", "Center Y", "EllipseCenterY",
            "double", "blank", "Major Radius", "EllipseRadiusMajor",
            "double", "blank", "Minor Radius", "EllipseRadiusMinor",
            "double", "blank", "Major Diameter", "EllipseDiameterMajor",
            "double", "blank", "Minor Diameter", "EllipseDiameterMinor",
            END_SYMBOL
        },
        .object = OBJ_ELLIPSE
    },
    {
        .id = GB_GEOM_IMAGE,
        .key = "GeometryImage",
        .label = "Geometry",
        .data = {
            "double", "blank", "Position X", "ImagePositionX",
            "double", "blank", "Position Y", "ImagePositionY",
            "double", "blank", "Width", "ImageWidth",
            "double", "blank", "Height", "ImageHeight",
            END_SYMBOL
         },
        .object = OBJ_IMAGE
    },
    {
        .id = GB_MISC_IMAGE,
        .key = "MiscImage",
        .label = "Misc",
        .data = {
            "double", "blank", "Name", "",
            "double", "blank", "Path", "",
            END_SYMBOL
        },
        .object = OBJ_IMAGE
    },
    {
        .id = GB_GEOM_INFINITE_LINE,
        .key = "GeometryInfiniteLine",
        .label = "Geometry",
        .data = {
            "double", "blank", "Start X", "InfiniteLineX1",
            "double", "blank", "Start Y", "InfiniteLineY1",
            "double", "blank", "2nd X", "InfiniteLineX2",
            "double", "blank", "2nd Y", "InfiniteLineY2",
            "double", "blank", "Vector X", "InfiniteLineVectorX",
            "double", "blank", "Vector Y", "InfiniteLineVectorY",
            END_SYMBOL
        },
        .object = OBJ_INFINITELINE
    },
    {
        .id = GB_GEOM_LINE,
        .key = "GeometryLine",
        .label = "Geometry",
        .data = {
            "double", "blank", "Start X", "LineStartX",
            "double", "blank", "Start Y", "LineStartY",
            "double", "blank", "End X", "LineEndX",
            "double", "blank", "End Y", "LineEndY",
            "double", "blank", "Delta X", "",
            "double", "blank", "Delta Y", "",
            "double", "blank", "Angle", "",
            "double", "blank", "Length", "",
            END_SYMBOL
         },
        .object = OBJ_LINE
    },
    {
        .id = GB_GEOM_PATH,
        .key = "GeometryPath",
        .label = "Geometry",
        .data = {
             "combobox", "blank", "Vertex #", "PathVertexNumber",
             "double", "blank", "Vertex X", "PathVertexX",
             "double", "blank", "Vertex Y", "PathVertexY",
             "double", "blank", "Area", "",
             "double", "blank", "Length", "",
             END_SYMBOL
         },
        .object = OBJ_PATH
    },
    {
        .id = GB_MISC_PATH,
        .key = "MiscPath",
        .label = "Misc",
        .data = {
            "combobox", "blank", "Closed", "PathClosed",
            END_SYMBOL
         },
        .object = OBJ_PATH
    },
    {
        .id = GB_GEOM_POINT,
        .key = "GeometryPoint",
        .label = "Geometry",
        .data = {
            "double", "blank", "Position X", "PointX",
            "double", "blank", "Position Y", "PointY",
            END_SYMBOL
        },
        .object = OBJ_POINT
    },
    {
        .id = GB_GEOM_POLYGON,
        .key = "GeometryPolygon",
        .label = "Geometry",
        .data = {
            "double", "blank", "Center X", "PolygonCenterX",
            "double", "blank", "Center Y", "PolygonCenterY",
            "double", "blank", "Vertex Radius", "PolygonVertexRadius",
            "double", "blank", "Side Radius", "PolygonSideRadius",
            "double", "blank", "Vertex Diameter", "PolygonVertexDiameter",
            "double", "blank", "Side Diameter", "PolygonSideDiameter",
            "double", "blank", "Interior Angle", "",
            END_SYMBOL
        },
        .object = OBJ_POLYGON
    },
    {
        .id = GB_GEOM_POLYLINE,
        .key = "GeometryPolyline",
        .label = "Geometry",
        .data = {
            "combobox", "blank", "Vertex #", "PolylineVertexNum",
            "double", "blank", "Vertex X", "PolylineVertexX",
            "double", "blank", "Vertex Y", "PolylineVertexX",
            "double", "blank", "Area", "",
            "double", "blank", "Length", "",
            END_SYMBOL
        },
        .object = OBJ_POLYLINE
    },
    {
        .id = GB_MISC_POLYLINE,
        .key = "MiscPolyline",
        .label = "Geometry",
        .data = {
            "combobox", "blank", "Closed", "PolylineClosed",
            END_SYMBOL
        },
        .object = OBJ_POLYLINE
    },
    {
        .id = GB_GEOM_RAY,
        .key = "GeometryRay",
        .label = "Geometry",
        .data = {
            "double", "blank", "Start X", "RayStartX",
            "double", "blank", "Start Y", "RayStartY",
            "double", "blank", "2nd X", "RayEndX",
            "double", "blank", "2nd Y", "RayEndY",
            "double", "blank", "Vector X", "",
            "double", "blank", "Vector Y", "",
            END_SYMBOL
        },
        .object = OBJ_RAY
    },
    {
        .id = GB_GEOM_RECT,
        .key = "GeometryRectangle",
        .label = "Geometry",
        .data = {
            "double", "blank", "Corner 1 X", "RectangleCorner1X",
            "double", "blank", "Corner 1 Y", "RectangleCorner1Y",
            "double", "blank", "Corner 2 X", "RectangleCorner2X",
            "double", "blank", "Corner 2 Y", "RectangleCorner2Y",
            "double", "blank", "Corner 3 X", "RectangleCorner3X",
            "double", "blank", "Corner 3 Y", "RectangleCorner3Y",
            "double", "blank", "Corner 4 X", "RectangleCorner4X",
            "double", "blank", "Corner 4 Y", "RectangleCorner4Y",
            "double", "blank", "Width", "RectangleWidth",
            "double", "blank", "Height", "RectangleHeight",
            "double", "blank", "Area", "",
            END_SYMBOL
        },
        .object = OBJ_RECTANGLE
    },
    {
        .id = GB_GEOM_TEXT_MULTI,
        .key = "GeometryTextMulti",
        .label = "Geometry",
        .data = {
             "double", "blank", "Position X", "TextPositionX",
             "double", "blank", "Position Y", "TextPositionX",
             END_SYMBOL
         },
        .object = OBJ_TEXTMULTI
    },
    {
        .id = GB_TEXT_TEXT_SINGLE,
        .key = "TextTextSingle",
        .label = "Text",
        .data = {
            "string", "blank", "Contents", "TextContents",
            "fontcombobox", "blank", "Font", "TextFont",
            "combobox", "blank", "Justify", "TextJustify",
            "double", "blank", "Height", "TextHeight",
            "double", "blank", "Rotation", "TextRotation",
            END_SYMBOL
        },
        .object = OBJ_TEXTSINGLE
    },
    {
        .id = GB_GEOM_TEXT_SINGLE,
        .key = "GeometryTextSingle",
        .label = "Geometry",
        .data = {
            "double", "blank", "Position X", "TextSingleX",
            "double", "blank", "Position Y", "TextSingleY",
            END_SYMBOL
        },
        .object = OBJ_TEXTSINGLE
    },
    {
        .id = GB_MISC_TEXT_SINGLE,
        .key = "MiscTextSingle",
        .label = "Misc",
        .data = {
            "combobox", "blank", "Backward", "TextSingleBackward",
            "combobox", "blank", "Upside Down", "TextSingleUpsideDown",
            END_SYMBOL
        },
        .object = OBJ_POLYLINE
    },
    {
        .id = -1,
        .key = END_SYMBOL,
        .label = END_SYMBOL,
        .data = {
            END_SYMBOL
        },
        .object = -1
    }
};

/* Matches the order of the settings constants table in constants.h.
 *
 * A 6 x n table in the order:
 *     (key, data_type, label, icon, default_value, attributes)
 *
 * The row number is recorded as a comment so the constants table is easier
 * to keep in line.
 */
char *settings_data[] = {
    /* 0 */
    "Language", "string", "Language", "blank", "default", "",
    /* 1 */
    "IconTheme", "string", "Icon theme", "blank", "default", "",
    /* 2 */
    "IconSize", "int", "Icon size", "blank", "16", "",
    /* 3 */
    "MdiBGUseLogo", "bool", "Use Logo", "blank", "1", "",
    /* 4 */
    "MdiBGUseTexture", "bool", "Use Texture", "blank", "1", "",
    /* 5 */
    "MdiBGUseColor", "bool", "Use Color", "blank", "1", "",
    /* 6 */
    "MdiBGLogo", "string", "MDI area background logo", "blank",
    "logo_spirals", "",
    /* 7 */
    "MdiBGTexture", "string", "MDI area background texture",
    "blank", "texture_spirals", "",
    /* 8 */
    "MdiBGColor", "int", "MDI area background color",
    "blank", "12632256", "",
    /* 9 */
    "TipOfTheDay", "bool", "Show Tip of the Day on startup",
    "blank", "1", "",
    /* 10 */
    "CurrentTip", "int", "Current tip", "blank", "0", "",
    /* 11 */
    "SystemHelpBrowser", "bool", "Help Browser", "blank", "1", "",
    /* 12 */
    "CheckForUpdates", "bool", "Check for updates", "blank", "0", "",
    /* 13 */
    "Display_UseOpenGL", "bool", "Use OpenGL", "0", "blank", "",
    /* 14 */
    "Display_RenderHintAntiAlias", "bool", "Renderhint anti-alias",
    "blank", "0", "",
    /* 15 */
    "Display_RenderHintTextAntiAlias", "bool", "Renderhint text anti-alias",
    "blank", "0", "",
    /* 16 */
    "Display_RenderHintSmoothPixmap", "bool", "Renderhint smooth pixmap",
    "blank", "0", "",
    /* 17 */
    "Display_RenderHintHighQualityAntiAlias", "bool",
    "Renderhint high quality anti-alias", "blank", "0", "",
    /* 18 */
    "Display_RenderHintNonCosmetic", "bool", "Renderhint high non-cosmetic",
    "blank", "0", "",
    /* 19 */
    "Display_ShowScrollBars", "bool", "Show Scrollbars", "blank", "1", "",
    /* 20 */
    "Display_ScrollBarWidgetNum", "int", "Scrollbar widget number",
    "blank", "0", "",
    /* 21 */
    "Display_CrossHairColor", "int", "Cross-hair color", "blank", "0", "",
    /* 22 */
    "Display_BackgroundColor", "int", "Background color",
    "blank", "15461355", "",
    /* 23 */
    "Display_SelectBoxLeftColor", "int", "Selection Box Color (Crossing)",
    "blank", "32768", "",
    /* 24 */
    "Display_SelectBoxLeftFill", "int", "Selection Box Fill (Crossing)",
    "blank", "65280", "",
    /* 25 */
    "Display_SelectBoxRightColor", "int", "Selection Box Color (Window)",
    "blank", "128", "",
    /* 26 */
    "Display_SelectBoxRightFill", "int", "Selection Box Fill (Window)",
    "blank", "255", "",
    /* 27 */
    "Display_SelectBoxAlpha", "int", "Selection Box Transparency", "blank",
    "32", "",
    /* 28 */
    "Display_ZoomScaleIn", "real", "Zoom In Scale", "blank", "2.0",
    "0.01,1.01,10.00",
    /* 29 */
    "Display_ZoomScaleOut", "real", "Zoom Out Scale", "blank", "0.5",
    "0.01,0.01,0.99",
    /* 30 */
    "Display_CrossHairPercent", "int", "Crosshair percent", "blank", "5", "",
    /* 31 */
    "Units", "string", "Display_Units", "blank", "mm", "",
    /* 32 */
    "Prompt_TextColor", "int", "Text color", "blank", "0", "",
    /* 33 */
    "Prompt_BackgroundColor", "int", "Background color", "blank",
    "16777215", "",
    /* 34 */
    "Prompt_FontFamily", "string", "Font family", "blank", "Monospace", "",
    /* 35 */
    "Prompt_FontStyle", "string", "Font style", "blank", "normal", "",
    /* 36 */
    "Prompt_FontSize", "int", "Font size", "blank", "12", "",
    /* 37 */
    "Prompt_SaveHistory", "bool", "Save History", "blank", "1", "",
    /* 38 */
    "Prompt_SaveHistoryAsHtml", "bool", "Save As HTML", "blank", "0", "",
    /* 39 */
    "Prompt_SaveHistoryFilename", "string", "Save history filename",
    "blank", "prompt.log", "",
    /* 40 */
    "OpenSave_CustomFilter", "string", "Custom filter", "blank", "supported", "",
    /* 41 */
    "OpenSave_OpenFormat", "string", "Open format", "blank", "*.*", "",
    /* 42 */
    "OpenSave_OpenThumbnail", "bool", "Open thumbnail", "blank", "0", "",
    /* 43 */
    "OpenSave_SaveFormat", "string", "Save format", "blank", "*.*", "",
    /* 44 */
    "OpenSave_SaveThumbnail", "bool", "Save thumbnail", "blank", "0", "",
    /* 45 */
    "OpenSave_RecentMax", "int", "Maximum recent files", "blank", "10", "",
    /* 46: TODO should be a ; seperated table */
    "OpenSave_RecentFiles", "string", "Recent files", "blank", "", "",
    /* 47 */
    "OpenSave_RecentDirectory", "string", "Recent directory", "blank", "", "",
    /* 48 */
    "OpenSave_TrimDstNumJumps", "int", "Number of jumps to become trim DST",
    "blank", "5", "",
    /* 49 */
    "Printing_DefaultDevice", "string", "Default device", "blank", "", "",
    /* 50 */
    "Printing_UseLastDevice", "bool", "Use last device", "blank", "0", "",
    /* 51 */
    "Printing_DisableBG", "bool", "Disable Background", "blank", "1", "",
    /* 52 */
    "Grid_ShowOnLoad", "bool", "Initially show grid when loading a file",
    "blank", "1", "",
    /* 53 */
    "Grid_ShowOrigin", "bool", "Show the origin when the grid is enabled",
    "blank", "1", "",
    /* 54 */
    "Grid_ColorMatchCrossHair", "bool", "Grid color matches cross-hair color",
    "blank", "1", "",
    /* 55 */
    "Grid_Color", "int", "Color", "blank", "0", "",
    /* 56 */
    "Grid_LoadFromFile", "bool", "Load from file", "blank", "1", "",
    /* 57 */
    "Grid_Type", "string", "Type", "blank", "Rectangular", "",
    /* 58 */
    "Grid_CenterOnOrigin", "bool", "Center on origin", "blank", "1", "",
    /* 59 */
    "Grid_CenterX", "real", "Grid Center X", "blank", "0.0",
    "1.000,-1000.000,1000.000",
    /* 60 */
    "Grid_CenterY", "real", "Grid Center Y", "blank", "0.0",
    "1.000,-1000.000,1000.000",
    /* 61 */
    "Grid_SizeX", "real", "Grid Size X", "blank", "100.0",
    "1.000,1.000,1000.000",
    /* 62 */
    "Grid_SizeY", "real", "Grid Size Y", "blank", "100.0",
    "1.000,1.000,1000.000",
    /* 63 */
    "Grid_SpacingX", "real", "X spacing", "blank", "25.0",
    "1.000,0.001,1000.000",
    /* 64 */
    "Grid_SpacingY", "real", "Y spacing", "blank", "25.0",
    "1.000,0.001,1000.000",
    /* 65 */
    "Grid_SizeRadius", "real", "Size radius", "blank", "50.0",
    "1.000,1.000,1000.000",
    /* 66 */
    "Grid_SpacingRadius", "real", "Spacing radius", "blank", "25.0",
    "1.000,0.001,1000.000",
    /* 67 */
    "Grid_SpacingAngle", "real", "Spacing angle", "blank", "45.0",
    "1.000,0.001,1000.000",
    /* 68 */
    "Ruler_ShowOnLoad", "bool", "Initially show ruler when loading a file",
    "blank", "1", "",
    /* 69 */
    "Ruler_Metric", "bool", "Metric", "blank", "1", "",
    /* 70 */
    "Ruler_Color", "int", "Color", "blank", "13816370", "",
    /* 71 */
    "Ruler_PixelSize", "int", "Pixel size", "blank", "20", "1,20,100",
    /* 72 */
    "QuickSnap_Enabled", "bool", "Enabled", "blank", "1", "",
    /* 73 */
    "QuickSnap_LocatorColor", "int", "Locator color", "blank", "16776960", "",
    /* 74 */
    "QuickSnap_LocatorSize", "int", "Locator size", "blank", "4", "",
    /* 75 */
    "QuickSnap_ApertureSize", "int", "Aperture size", "blank", "10", "",
    /* 76 */
    "QuickSnap_EndPoint", "bool", "End point", "blank", "1", "",
    /* 77 */
    "QuickSnap_MidPoint", "bool", "Mid point", "blank", "1", "",
    /* 78 */
    "QuickSnap_Center", "bool", "Center", "blank", "1", "",
    /* 79 */
    "QuickSnap_Node", "bool", "Node", "blank", "1", "",
    /* 80 */
    "QuickSnap_Quadrant", "bool", "Quadrant", "blank", "1", "",
    /* 81 */
    "QuickSnap_Intersection", "bool", "Intersection", "blank", "1", "",
    /* 82 */
    "QuickSnap_Extension", "bool", "Extension", "blank", "1", "",
    /* 83 */
    "QuickSnap_Insertion", "bool", "Insertion", "blank", "0", "",
    /* 84 */
    "QuickSnap_Perpendicular", "bool", "Perpendicular", "blank", "1", "",
    /* 85 */
    "QuickSnap_Tangent", "bool", "Tangent", "blank", "1", "",
    /* 86 */
    "QuickSnap_Nearest", "bool", "Nearest", "blank", "0", "",
    /* 87 */
    "QuickSnap_Apparent", "bool", "Apparent Intersection", "blank", "0", "",
    /* 88 */
    "QuickSnap_Parallel", "bool", "Parallel", "blank", "0", "",
    /* 89 */
    "LineWeight_ShowLineWeight", "bool", "Show line weight", "blank", "0", "",
    /* 90 */
    "LineWeight_RealRender", "bool", "Real render", "blank", "1", "",
    /* 91 */
    "LineWeight_DefaultLineWeight", "real", "Default line weight",
    "blank", "0", "",
    /* 92 */
    "Selection_PickFirst", "bool", "Allow Preselection (PickFirst)", "blank",
    "1", "",
    /* 93 */
    "Selection_PickAdd", "bool", "Add to Selection (PickAdd)", "blank", "1", "",
    /* 94 */
    "Selection_PickDrag", "bool", "Drag to Select (PickDrag)", "blank", "0", "",
    /* 95 */
    "Selection_CoolGripColor", "int", "Cool grip color", "blank", "255", "",
    /* 96 */
    "Selection_HotGripColor", "int", "Hot grip color", "blank", "16711680", "",
    /* 97 */
    "Selection_GripSize", "int", "Grip size", "blank", "4", "",
    /* 98 */
    "Selection_PickBoxSize", "int", "Pick box size", "blank", "4", "",
    /* 99 */
    "Text_Font", "string", "Font", "blank", "Arial", "",
    /* 100 */
    "Text_Size", "real", "Size", "blank", "12", "",
    /* 101 */
    "Text_Angle", "real", "Angle", "blank", "0", "",
    /* 102 */
    "Text_StyleBold", "bool", "Bold", "blank", "0", "",
    /* 103 */
    "Text_StyleItalic", "bool", "Italic", "blank", "0", "",
    /* 104*/
    "Text_StyleUnderline", "bool", "Underline", "blank", "0", "",
    /* 105 */
    "Text_StyleOverline", "bool", "Overline", "blank", "0", "",
    /* 106 */
    "Text_StyleStrikeOut", "bool", "Strikeout", "blank", "0", "",
    END_SYMBOL
};

char *grid_enabled_group[] = {
    "labelGridType", "comboBoxGridType",
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
    END_SYMBOL
};

char *rectangular_grid_visible_group[] = {
    "labelGridSizeX", "spinBoxGridSizeX",
    "labelGridSizeY", "spinBoxGridSizeY",
    "labelGridSpacingX", "spinBoxGridSpacingX",
    "labelGridSpacingY", "spinBoxGridSpacingY",
    END_SYMBOL
};

char *circular_grid_visible_group[] = {
    "labelGridSizeRadius", "spinBoxGridSizeRadius",
    "labelGridSpacingRadius", "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle", "spinBoxGridSpacingAngle",
    END_SYMBOL
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
    END_SYMBOL
};

EmbStringTable zoom_layout = {
    "labelZoomInScale", "spinboxZoomInScale",
    "labelZoomOutScale", "spinboxZoomOutScale",
    END_SYMBOL
};

