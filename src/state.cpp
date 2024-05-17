/*!
 * \file state.c
 *
 * Embroidermodder 2.
 *
 * Copyright 2011-2025 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 */

#include "core.h"

#include <string>

ScriptEnv *global;

char formatFilterOpen[MAX_LONG_STRING];
char formatFilterSave[MAX_LONG_STRING];
char open_files_path[MAX_STRING_LENGTH];
char prompt_color_[MAX_STRING_LENGTH];
char prompt_selection_bg_color_[MAX_STRING_LENGTH];
char prompt_bg_color_[MAX_STRING_LENGTH];
char prompt_selection_color_[MAX_STRING_LENGTH];

bool document_memory[MAX_OPEN_FILES];

bool shiftKeyPressedState = false;
bool cmdActive;
bool rapidFireEnabled = false;
bool isBlinking = false;
int numOfDocs = 0;
int docIndex = 0;

char settings_file[MAX_STRING_LENGTH] = "settings.ini";
bool key_state[N_KEY_SEQUENCES] = {
    false, false, false, false, false,
    false, false, false, false, false,
    false, false, false, false, false,
    false, false, false, false, false,
    false, false, false, false
};
Setting setting[N_SETTINGS];
char recent_files[MAX_FILES][MAX_STRING_LENGTH] = {
    END_SYMBOL
};

const char *index_name[] = {
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven"
};

const char *index_th_name[] = {
    "first",
    "second",
    "third",
    "fourth",
    "fifth",
    "sixth",
    "seventh"
};

const ScriptValue script_true = {
    .r = 0.0F,
    .i = 1,
    .b = true,
    .s = "",
    .label = "true",
    .type = SCRIPT_BOOL
};

const ScriptValue script_false = {
    .r = 0.0F,
    .i = 0,
    .b = false,
    .s = "",
    .label = "false",
    .type = SCRIPT_BOOL
};

char *_appName_ = "Embroidermodder";
char *_appVer_ = "v2.0 alpha";
int testing_mode = 0;

char defaultPrefix[200];
char prefix[200];
char curCmd[200];

/* ---------------------------- Menus ------------------------------ */

int menubar_full_list[] = {
    MENU_FILE,
    MENU_EDIT,
    MENU_VIEW,
    MENU_DRAW,
    MENU_TOOLS,
    MENU_MODIFY,
    MENU_DIMENSION,
    MENU_SANDBOX,
    MENU_WINDOW,
    MENU_HELP,
    TERMINATOR_SYMBOL
};

int menubar_no_docs[] = {
    MENU_FILE,
    MENU_EDIT,
    MENU_WINDOW,
    MENU_HELP,
    TERMINATOR_SYMBOL
};

/* ---------------------------- Toolbars ------------------------------ */

int top_toolbar[] = {
    TOOLBAR_BREAK,
    TOOLBAR_FILE,
    TOOLBAR_EDIT,
    TOOLBAR_HELP,
    TOOLBAR_ICON,
    TOOLBAR_VIEW,
    TOOLBAR_BREAK,
    TOOLBAR_LAYER,
    TOOLBAR_PROPERTIES,
    TOOLBAR_BREAK,
    TOOLBAR_TEXT,
    TERMINATOR_SYMBOL
};

int left_toolbar[] = {
    TOOLBAR_DRAW,
    TOOLBAR_MODIFY,
    TOOLBAR_BREAK,
    TERMINATOR_SYMBOL
};

int bottom_toolbar[] = {
    TOOLBAR_PROMPT,
    TERMINATOR_SYMBOL
};

int toolbar_horizontal[] = {
    TOOLBAR_VIEW,
    TOOLBAR_LAYER,
    TOOLBAR_PROPERTIES,
    TOOLBAR_TEXT,
    TOOLBAR_PROMPT,
    TERMINATOR_SYMBOL
};

int toolbars_when_docs[] = {
    TOOLBAR_VIEW,
    TOOLBAR_ICON,
    TOOLBAR_HELP,
    TOOLBAR_LAYER,
    TOOLBAR_TEXT,
    TOOLBAR_PROPERTIES,
    TOOLBAR_DRAW,
    TERMINATOR_SYMBOL
};

/* ---------------------------- Groups ------------------------------- */

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
    END_SYMBOL
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
    END_SYMBOL
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
    END_SYMBOL
};

char *circular_grid_group[] = {
    "labelGridSizeRadius",
    "spinBoxGridSizeRadius",
    "labelGridSpacingRadius",
    "spinBoxGridSpacingRadius",
    "labelGridSpacingAngle",
    "spinBoxGridSpacingAngle",
    END_SYMBOL
};

/* FIXME:. */
char *center_on_origin_group[] = {
    END_SYMBOL
};

/* ---------------------------- Settings ------------------------------- */

char *settings_tab_labels[] = {
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
    END_SYMBOL
};

int preview_to_dialog[] = {
    GENERAL_MDI_BG_USE_LOGO,
    GENERAL_MDI_BG_USE_TEXTURE,
    GENERAL_MDI_BG_USE_COLOR,
    DISPLAY_SHOW_SCROLLBARS,
    DISPLAY_SELECTBOX_ALPHA,
    PROMPT_FONT_FAMILY,
    PROMPT_FONT_STYLE,
    PROMPT_FONT_SIZE,
    LWT_SHOW_LWT,
    LWT_REAL_RENDER,
    TERMINATOR_SYMBOL
};

int accept_to_dialog[] = {
    GENERAL_MDI_BG_LOGO,
    GENERAL_MDI_BG_TEXTURE,
    GENERAL_MDI_BG_COLOR,
    DISPLAY_CROSSHAIR_COLOR,
    DISPLAY_BG_COLOR,
    DISPLAY_SELECTBOX_LEFT_COLOR,
    DISPLAY_SELECTBOX_LEFT_FILL,
    DISPLAY_SELECTBOX_RIGHT_COLOR,
    DISPLAY_SELECTBOX_RIGHT_FILL,
    PROMPT_TEXT_COLOR,
    PROMPT_BG_COLOR,
    RULER_COLOR,
    TERMINATOR_SYMBOL
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

/* ---------------------------- Objects ------------------------------- */

char *object_names[] = {
    "Base",
    "Arc",
    "Block",
    "Circle",
    "Aligned Dimension",
    "Angular Dimension",
    "Arc Length Dimension",
    "Diameter Dimension",
    "Leader Dimension",
    "Linear Dimension",
    "Ordinate Dimension",
    "Radius Dimension",
    "Ellipse",
    "Elliptical Arc",
    "Rubber",
    "Grid",
    "Hatch",
    "Image",
    "Infinite Line",
    "Line",
    "Path",
    "Point",
    "Polygon",
    "Polyline",
    "Ray",
    "Rectangle",
    "Slot",
    "Spline",
    "Multi Line Text",
    "Single Line Text",
    "Unknown",
    END_SYMBOL
};

/* ---------------------------- Misc ---------------------------------- */

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
    END_SYMBOL
};

StringMap aliasHash[MAX_ALIASES];
IntMap obj_index[MAX_OBJECTS];

EmbIdList *cutCopyObjectList;

bool blinkState;

int iconSize;

bool pickAdd;

int promptInputNum = 0;

int precisionAngle;
int precisionLength;

int n_aliases = 0;
int n_objects = 0;
int n_widgets = 0;
int n_actions = 0;

EmbString curText;

char promptHistoryData[MAX_LONG_STRING];

/* Used when checking if fields vary. */
EmbString fieldOldText;
EmbString fieldNewText;
EmbString fieldVariesText;
EmbString fieldYesText;
EmbString fieldNoText;
EmbString fieldOnText;
EmbString fieldOffText;

/*
 * UI data
 *
 * Editor Data
 * TODO: Use proper icons for tool buttons.
 */

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

char *button_list[] = {
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

char *grid_layout[] = {
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

char *zoom_layout[] = {
    "labelZoomInScale", "spinboxZoomInScale",
    "labelZoomOutScale", "spinboxZoomOutScale",
    END_SYMBOL
};

char lastCmd[MAX_STRING_LENGTH] = "help";

/* BUG: combine with other labels in libembroidery */
char *geometry_type_keys[] = {
    "ARC",
    "BLOCK",
    "CIRCLE",
    "DIMALIGNED",
    "DIMANGULAR",
    "DIMARCLENGTH",
    "DIMDIAMETER",
    "DIMLEADER",
    "DIMLINEAR",
    "DIMORDINATE",
    "DIMRADIUS",
    "ELLIPSE",
    "ELLIPSEARC",
    "HATCH",
    "IMAGE",
    "INFINITELINE",
    "LINE",
    "PATH",
    "POINT",
    "POLYGON",
    "POLYLINE",
    "RAY",
    "RECTANGLE",
    "SPLINE",
    "TEXTMULTI",
    "TEXTSINGLE",
    END_SYMBOL
};

/* Command Data: interface to command function pointers and necessary data
 * for the menu and toolbar presentation of the commands.
 *
 * Shortcuts should match:
 *     https://doc.qt.io/qt-6/qkeysequence.html#standard-shortcuts
 * Apple platforms may need an additional argument like ".apple_shortcut".
 */
CommandData command_data[MAX_COMMANDS] = {
    /* ACTION_NULL                    0 */
    {
        .command = "donothing",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = do_nothing_command
    },
    /* 1 */
    {
        .command = "donothing",
        .arguments = "",
        .icon = "x_circle",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = do_nothing_command
    },
    /* 2 */
    {
        .command = "new",
        .arguments = "",
        .icon = "document",
        .tooltip = "&New",
        .statustip = "Create a new file. Command: NEW.",
        .alias = "NEW",
        .shortcut = "Ctrl+N",
        .flags = CONTEXT_FREE | CLEAR_SELECTION,
        .action = new_command
    },
    /* 3 */
    {
        .command = "open",
        .arguments = "",
        .icon = "folder_open",
        .tooltip = "&Open",
        .statustip = "Open an existing file. Command: OPEN.",
        .alias = "OPEN",
        .shortcut = "Ctrl+O",
        .flags = REQUIRED_VIEW,
        .action = open_command
    },
    /* 4 */
    {
        .command = "save",
        .arguments = "",
        .icon = "arrow_down_on_square",
        .tooltip = "&Save",
        .statustip = "Save the design to disk. Command: SAVE.",
        .alias = "SAVE",
        .shortcut = "Ctrl+S",
        .flags = REQUIRED_VIEW,
        .action = save_command
    },
    /* 5 */
    {
        .command = "saveas",
        .arguments = "",
        .icon = "arrow_down_on_square_stack",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name. Command: SAVEAS.",
        .alias = "SAVEAS",
        .shortcut = "Ctrl+Shift+S",
        .flags = REQUIRED_VIEW,
        .action = save_as_command
    },
    /* 6 */
    {
        .command = "print",
        .arguments = "",
        .icon = "printer",
        .tooltip = "&Print",
        .statustip = "Print the design. Command: PRINT.",
        .alias = "PRINT",
        .shortcut = "Ctrl+P",
        .flags = REQUIRED_VIEW,
        .action = print_command
    },
    /* 7 */
    {
        .command = "designdetails",
        .arguments = "",
        .icon = "chart_bar",
        .tooltip = "&Details",
        .statustip = "Details of the current design. Command: DETAILS",
        .alias = "details",
        .shortcut = "Ctrl+D",
        .flags = REQUIRED_VIEW,
        .action = details_command
    },
    /* 8 */
    {
        .command = "exit",
        .arguments = "",
        .icon = "power",
        .tooltip = "E&xit",
        .statustip = "Exit the application:  EXIT",
        .alias = "quit",
        .shortcut = "Ctrl+Q",
        .flags = REQUIRED_VIEW,
        .action = exit_command
    },
    /* 9 */
    {
        .command = "cut",
        .arguments = "",
        .icon = "scissors",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard. Command: CUT.",
        .alias = "CUT",
        .shortcut = "Ctrl+X",
        .flags = REQUIRED_VIEW,
        .action = cut_command
    },
    /* 10 */
    {
        .command = "copy",
        .arguments = "",
        .icon = "document_duplicate",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard. Command: COPY.",
        .alias = "",
        .shortcut = "Ctrl+C",
        .flags = REQUIRED_VIEW,
        .action = copy_command
    },
    /* 11 */
    {
        .command = "copy-selected",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = copy_selected_command
    },
    /* 12 */
    {
        .command = "paste",
        .arguments = "",
        .icon = "clipboard_document",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection. Command: PASTE.",
        .alias = "PASTE",
        .shortcut = "Ctrl+V",
        .flags = REQUIRED_VIEW,
        .action = paste_command
    },
    /* 13 */
    {
        .command = "paste-selected",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = paste_selected_command
    },
    /* 14 */
    {
        .command = "undo",
        .arguments = "",
        .icon = "arrow_uturn_left",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action. Command: UNDO.",
        .alias = "U",
        .shortcut = "Ctrl+Z",
        .flags = REQUIRED_VIEW,
        .action = undo_command
    },
    /* 15 */
    {
        .command = "redo",
        .arguments = "",
        .icon = "arrow_uturn_right",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action. Command: REDO.",
        .alias = "REDO",
        .shortcut = "Ctrl+Shift+Z",
        .flags = REQUIRED_VIEW,
        .action = redo_command
    },
    /* 16 */
    {
        .command = "windowclose",
        .arguments = "",
        .icon = "x_mark",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window. Command: CLOSE.",
        .alias = "CLOSE",
        .shortcut = "Ctrl+W",
        .flags = REQUIRED_VIEW,
        .action = window_close_command
    },
    /* 17 */
    {
        .command = "windowcloseall",
        .arguments = "",
        .icon = "windowcloseall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows. Command: CLOSEALL",
        .alias = "CLOSEALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = window_close_all_command
    },
    /* 18 */
    {
        .command = "windowcascade",
        .arguments = "",
        .icon = "windowcascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows. Command: CASCADE.",
        .alias = "CASCADE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = window_cascade_command
    },
    /* 19 */
    {
        .command = "windowtile",
        .arguments = "",
        .icon = "windowtile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows:  TILE",
        .alias = "TILE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = window_tile_command
    },
    /* 20 */
    {
        .command = "windownext",
        .arguments = "",
        .icon = "windownext",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window:  NEXT",
        .alias = "NEXT",
        .shortcut = "Ctrl+Tab",
        .flags = REQUIRED_VIEW,
        .action = window_next_command
    },
    /* 21 */
    {
        .command = "windowprevious",
        .arguments = "",
        .icon = "windowprevious",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window:  PREVIOUS",
        .alias = "PREV,PREVIOUS",
        .shortcut = "Ctrl+Shift+Tab",
        .flags = REQUIRED_VIEW,
        .action = window_previous_command
    },
    /* 22 */
    {
        .command = "help",
        .arguments = "",
        .icon = "question_mark_circle",
        .tooltip = "&Help",
        .statustip = "Displays help. Command: HELP",
        .alias = "?,HELP",
        .shortcut = "F1",
        .flags = CONTEXT_FREE,
        .action = help_command
    },
    /* 23 */
    {
        .command = "changelog",
        .arguments = "",
        .icon = "queue_list",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product. Command: CHANGELOG.",
        .alias = "CHANGELOG",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = changelog_command
    },
    /* 24 */
    {
        .command = "tipoftheday",
        .arguments = "",
        .icon = "light_bulb",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips:  TIPS",
        .alias = "TIPS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = tip_of_the_day_command
    },
    /* 25 */
    {
        .command = "about",
        .arguments = "",
        .icon = "information_circle",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product. Command: ABOUT.",
        .alias = "ABOUT",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = about_command
    },
    /* 26 */
    {
        .command = "whatsthis",
        .arguments = "",
        .icon = "lifebuoy",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help! Command: WHATSTHIS.",
        .alias = "WHATSTHIS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = whats_this_command
    },
    /* 27 */
    {
        .command = "icon16",
        .arguments = "",
        .icon = "icon16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16:  ICON16",
        .alias = "ICON16",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = icon16_command
    },
    /* 28 */
    {
        .command = "icon24",
        .arguments = "",
        .icon = "icon24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = icon24_command
    },
    /* 29 */
    {
        .command = "icon32",
        .arguments = "",
        .icon = "icon32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32:  ICON32",
        .alias = "ICON32",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = icon32_command
    },
    /* 30 */
    {
        .command = "icon48",
        .arguments = "",
        .icon = "icon48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48:  ICON48",
        .alias = "ICON48",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = icon48_command
    },
    /* 31 */
    {
        .command = "icon64",
        .arguments = "",
        .icon = "icon64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64:  ICON64",
        .alias = "ICON64",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = icon64_command
    },
    /* 32 */
    {
        .command = "icon128",
        .arguments = "",
        .icon = "icon128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128:  ICON128",
        .alias = "ICON128",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = icon128_command
    },
    /* 33 */
    {
        .command = "settingsdialog",
        .arguments = "",
        .icon = "cog-8-tooth",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product. Command: SETTINGS.",
        .alias = "SETTINGS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = settings_command
    },
    /* 34 */
    {
        .command = "makelayercurrent",
        .arguments = "",
        .icon = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .alias = "MAKELAYERCURRENT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = make_layer_active_command
    },
    /* 35 */
    {
        .command = "layers",
        .arguments = "",
        .icon = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .alias = "LAYER",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = layers_command
    },
    /* 36 */
    {
        .command = "layerselector",
        .arguments = "",
        .icon = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .alias = "LAYERSELECTOR",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = layer_selector_command
    },
    /* 37 */
    {
        .command = "layerprevious",
        .arguments = "",
        .icon = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings:  LAYERP",
        .alias = "LAYERP",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = layer_previous_command
    },
    /* 38 */
    {
        .command = "colorselector",
        .arguments = "",
        .icon = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .alias = "COLORSELECTOR",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = color_selector_command
    },
    /* 39 */
    {
        .command = "linetypeselector",
        .arguments = "",
        .icon = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .alias = "LINETYPESELECTOR",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = line_type_selector_command
    },
    /* 40 */
    {
        .command = "lineweightselector",
        .arguments = "",
        .icon = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .alias = "LINEWEIGHTSELECTOR",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = line_weight_selector_command
    },
    /* 41 */
    {
        .command = "hidealllayers",
        .arguments = "",
        .icon = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .alias = "HIDEALLLAYERS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = hide_all_layers_command
    },
    /* 42 */
    {
        .command = "showalllayers",
        .arguments = "",
        .icon = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .alias = "SHOWALLLAYERS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = show_all_layers_command
    },
    /* 43 */
    {
        .command = "freezealllayers",
        .arguments = "",
        .icon = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .alias = "FREEZEALLLAYERS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = freeze_all_layers_command
    },
    /* 44 */
    {
        .command = "thawalllayers",
        .arguments = "",
        .icon = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .alias = "THAWALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = thaw_all_layers_command
    },
    /* 45 */
    {
        .command = "lockalllayers",
        .arguments = "",
        .icon = "lock_closed",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .alias = "LOCKALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = lock_all_layers_command
    },
    /* 46 */
    {
        .command = "unlockalllayers",
        .arguments = "",
        .icon = "lock_open",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .alias = "UNLOCKALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = unlock_all_layers_command
    },
    /* 47 */
    {
        .command = "textbold",
        .arguments = "",
        .icon = "bold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold. Command: BOLD.",
        .alias = "BOLD",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = text_bold_command
    },
    /* 48 */
    {
        .command = "textitalic",
        .arguments = "",
        .icon = "italic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic. Command: ITALIC.",
        .alias = "ITALIC",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = text_italic_command
    },
    /* 49 */
    {
        .command = "textunderline",
        .arguments = "",
        .icon = "underline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined. Command: UNDERLINE.",
        .alias = "UNDERLINE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = text_underline_command
    },
    /* 50 */
    {
        .command = "textstrikeout",
        .arguments = "",
        .icon = "strikethrough",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out. Command: STRIKEOUT.",
        .alias = "STRIKEOUT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = text_strikeout_command
    },
    /* 51 */
    {
        .command = "textoverline",
        .arguments = "",
        .icon = "overline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined. Command: OVERLINE.",
        .alias = "OVERLINE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = text_overline_command
    },
    /* 52 */
    {
        .command = "zoomrealtime",
        .arguments = "",
        .icon = "magnifying_glass",
        .tooltip = "Zoom &Real Time",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport. Command: ZOOMREALTIME",
        .alias = "ZOOMREALTIME",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_real_time_command
    },
    /* 53 */
    {
        .command = "zoomprevious",
        .arguments = "",
        .icon = "magnifying_glass",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view. Command: ZOOMPREVIOUS.",
        .alias = "ZOOMPREVIOUS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_previous_command
    },
    /* 54 */
    {
        .command = "zoomwindow",
        .arguments = "",
        .icon = "magnifying_glass",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window. Command: ZOOMIN",
        .alias = "ZOOMWINDOW",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_window_command
    },
    /* 55 */
    {
        .command = "zoomdynamic",
        .arguments = "",
        .icon = "magnifying_glass",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing. Command: ZOOMDYNAMIC.",
        .alias = "ZOOMDYNAMIC",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_dynamic_command
    },
    /* 56 */
    {
        .command = "zoomscale",
        .arguments = "",
        .icon = "magnifying_glass",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor. Command: ZOOMSCALE.",
        .alias = "ZOOMSCALE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_scale_command
    },
    /* 57 */
    {
        .command = "zoomcenter",
        .arguments = "",
        .icon = "magnifying_glass",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height. Command: ZOOMCENTER.",
        .alias = "ZOOMCENTER",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_center_command
    },
    /* 58 */
    {
        .command = "zoomin",
        .arguments = "",
        .icon = "magnifying_glass_plus",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects. Command: zoomin.",
        .alias = "zoomin",
        .shortcut = "Ctrl+Plus",
        .flags = REQUIRED_VIEW,
        .action = zoom_in_command
    },
    /* 59 */
    {
        .command = "zoomout",
        .arguments = "",
        .icon = "magnifying_glass_minus",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects. Command: ZOOMOUT",
        .alias = "ZOOMOUT",
        .shortcut = "Ctrl+Minus",
        .flags = REQUIRED_VIEW,
        .action = zoom_out_command
    },
    /* 60 */
    {
        .command = "zoomselected",
        .arguments = "",
        .icon = "magnifying_glass",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects. Command: ZOOMSELECTED.",
        .alias = "ZOOMSELECTED",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_selected_command
    },
    /* 61 */
    {
        .command = "zoomall",
        .arguments = "",
        .icon = "magnifying_glass",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits. Command: ZOOMALL.",
        .alias = "ZOOMALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_all_command
    },
    /* 62 */
    {
        .command = "zoomextents",
        .arguments = "",
        .icon = "zoom",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents. Command: ZOOMEXTENTS",
        .alias = "ZOOMEXTENTS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = zoom_extents_command
    },
    /* 63 */
    {
        .command = "panrealtime",
        .arguments = "",
        .icon = "panrealtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport. Command: PANREALTIME",
        .alias = "PANREALTIME",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = panrealtime_command
    },
    /* 64 */
    {
        .command = "panpoint",
        .arguments = "",
        .icon = "viewfinder_circle",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .alias = "PANPOINT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = panpoint_command
    },
    /* 65 */
    {
        .command = "panleft",
        .arguments = "",
        .icon = "chevron_left",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left:  PANLEFT",
        .alias = "PANLEFT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = panleft_command
    },
    /* 66 */
    {
        .command = "panright",
        .arguments = "",
        .icon = "chevron_right",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right:  PANRIGHT",
        .alias = "PANRIGHT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = panright_command
    },
    /* 67 */
    {
        .command = "panup",
        .arguments = "",
        .icon = "chevron_up",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up:  PANUP",
        .alias = "PANUP",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = panup_command
    },
    /* 68 */
    {
        .command = "pandown",
        .arguments = "",
        .icon = "chevron_down",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down:  PANDOWN",
        .alias = "PANDOWN",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = pandown_command
    },
    /* 69 */
    {
        .command = "day",
        .arguments = "",
        .icon = "sun",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings. Command: DAY",
        .alias = "DAY",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = day_command
    },
    /* 70 */
    {
        .command = "night",
        .arguments = "",
        .icon = "moon",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings. Command: NIGHT.",
        .alias = "NIGHT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = night_command
    },
    /* 71 */
    {
        .command = "alert",
        .arguments = "s",
        .icon = "warning",
        .tooltip = "&Alert",
        .statustip = "Creates a dialog to alert the user. Command: ALERT.",
        .alias = "ALERT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = alert_command
    },
    /* 72 */
    {
        .command = "get",
        .arguments = "",
        .icon = "get",
        .tooltip = "&Get",
        .statustip = "Print a value to one of the pre-defined global variables.",
        .alias = "GET",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = get_command
    },
    /* 73 */
    {
        .command = "set",
        .arguments = "",
        .icon = "set",
        .tooltip = "&Set",
        .statustip = "Assign a value to one of the pre-defined global variables.",
        .alias = "SET",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = set_command
    },
    /* 74: ACTION_CLEAR is covered by the flags, so the function pointer is
     * do_nothing_command.
     */
    {
        .command = "clear",
        .arguments = "",
        .icon = "clear",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = REQUIRED_VIEW | CLEAR_SELECTION,
        .action = do_nothing_command
    },
    /* 75 */
    {
        .command = "angle",
        .arguments = "rrr",
        .icon = "angle",
        .tooltip = "&Angle",
        .statustip = "Calculate the angle between two lines and display it. Command: ANGLE, CALCANGLE",
        .alias = "CALCANGLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW | CLEAR_SELECTION,
        .action = angle_command
    },
    /* 76 */
    {
        .command = "circle",
        .arguments = "rrr",
        .icon = "circle",
        .tooltip = "&Circle",
        .statustip = "Creates a circle. Command: CIRCLE.",
        .alias = "C",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = circle_command
    },
    /* 77 */
    {
        .command = "debug",
        .arguments = "s",
        .icon = "debug",
        .tooltip = "Debug",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "DEBUG",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = debug_command
    },
    /* ACTION_DIM_LEADER             78 */
    {
        .command = "donothing",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = dimleader_command
    },
    /* 79 */
    {
        .command = "disable",
        .arguments = "",
        .icon = "disable",
        .tooltip = "&Disable",
        .statustip = "Disable one of the pre-defined global boolean variables.",
        .alias = "DISABLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = disable_command
    },
    /* 80 */
    {
        .command = "distance",
        .arguments = "rrrr",
        .icon = "distance",
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points. Command: DIST",
        .alias = "DI,DIST",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = distance_command
    },
    /* 81 */
    {
        .command = "dolphin",
        .arguments = "",
        .icon = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Creates a dolphin. Command: DOLPHIN.",
        .alias = "DOLPHIN",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = dolphin_command
    },
    /* 82 */
    {
        .command = "ellipse",
        .arguments = "",
        .icon = "ellipse",
        .tooltip = "&Ellipse",
        .statustip = "Creates an ellipse. Command: ELLIPSE.",
        .alias = "EL",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = ellipse_command
    },
    /* 83 */
    {
        .command = "enable",
        .arguments = "",
        .icon = "enable",
        .tooltip = "&Enable",
        .statustip = "Enable a pre-defined global variables.",
        .alias = "ENABLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = enable_command
    },
    /* 84 */
    {
        .command = "erase",
        .arguments = "",
        .icon = "trash",
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing. Command: DELETE.",
        .alias = "e,erase,del,delete",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = erase_command
    },
    /* 85 */
    {
        .command = "error",
        .arguments = "",
        .icon = "error",
        .tooltip = "Error",
        .statustip = "Sends an error message to the user. Command: ERROR",
        .alias = "ERROR",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = error_command
    },
    /* 86 */
    {
        .command = "heart",
        .arguments = "",
        .icon = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart:  HEART",
        .alias = "HEART",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = heart_command
    },
    /* 87 */
    {
        .command = "line",
        .arguments = "",
        .icon = "line",
        .tooltip = "&Line",
        .statustip = "Creates straight line segments:  LINE",
        .alias = "L,LINE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = line_command
    },
    /* 88 */
    {
        .command = "locatepoint",
        .arguments = "",
        .icon = "locatepoint",
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location:  ID",
        .alias = "ID",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = located_point_command
    },
    /* 89 */
    {
        .command = "mirrorselected",
        .arguments = "",
        .icon = "mirror",
        .tooltip = "&Mirror Selected",
        .statustip = "Command: MIRRORSELECTED.",
        .alias = "MIRRORSELECTED",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = mirror_selected_command
    },
    /* 90 */
    {
        .command = "move",
        .arguments = "",
        .icon = "move",
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction:  MOVE",
        .alias = "M,MOVE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = move_command
    },
    /* 91 */
    {
        .command = "moveselected",
        .arguments = "",
        .icon = "moveselected",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = move_selected_command
    },
    /* 92 */
    {
        .command = "path",
        .arguments = "",
        .icon = "path",
        .tooltip = "&Path",
        .statustip = "Creates a 2D path:  PATH",
        .alias = "PA,PATH",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = path_command
    },
    /* 93 */
    {
        .command = "platform",
        .arguments = "",
        .icon = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use. Command: PLATFORM.",
        .alias = "PLATFORM",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = platform_command
    },
    /* 94 */
    {
        .command = "point",
        .arguments = "",
        .icon = "point",
        .tooltip = "&Point",
        .statustip = "Creates multiple points:  POINT",
        .alias = "PO,POINT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = point_command
    },
    /* 95 */
    {
        .command = "polygon",
        .arguments = "",
        .icon = "polygon",
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon. Command: POLYGON.",
        .alias = "POL,POLYGON",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = polygon_command
    },
    /* 96 */
    {
        .command = "polyline",
        .arguments = "",
        .icon = "polyline",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline:  PLINE",
        .alias = "PL,PLINE,POLYLINE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = polyline_command
    },
    /* 97 */
    {
        .command = "previewoff",
        .arguments = "",
        .icon = "previewoff",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = previewoff_command
    },
    /* 98 */
    {
        .command = "previewon",
        .arguments = "",
        .icon = "previewon",
        .tooltip = "&Preview On",
        .statustip = "Preview on.",
        .alias = "PREVIEWON",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = previewon_command
    },
    /* 99 */
    {
        .command = "quickleader",
        .arguments = "",
        .icon = "quickleader",
        .tooltip = "&QuickLeader",
        .statustip = "Creates a leader and annotation:  QUICKLEADER",
        .alias = "LE,LEADER,QLEADER",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = quickleader_command
    },
    /* 100 */
    {
        .command = "rectangle",
        .arguments = "",
        .icon = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline. Command: RECTANGLE.",
        .alias = "REC,RECT,RECTANG,RECTANGLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = rectangle_command
    },
    /* 101 */
    {
        .command = "rgb",
        .arguments = "",
        .icon = "rgb",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24:  ICON24",
        .alias = "ICON24",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = rgb_command
    },
    /* 102 */
    {
        .command = "rotate",
        .arguments = "",
        .icon = "arrow_path",
        .tooltip = "&Rotate",
        .statustip = "Rotates objects about a base point:  ROTATE",
        .alias = "RO,ROTATE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = rotate_command
    },
    /* 103 */
    {
        .command = "donothing",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = do_nothing_command
    },
    /* 104 */
    {
        .command = "scale",
        .arguments = "",
        .icon = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions:  SCALE",
        .alias = "SC,SCALE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = scale_command
    },
    /* 105 */
    {
        .command = "scaleselected",
        .arguments = "",
        .icon = "scaleselected",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing.",
        .alias = "SCALESELECTED",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = scale_selected_command
    },
    /* 106 */
    {
        .command = "selectall",
        .arguments = "",
        .icon = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects:  SELECTALL",
        .alias = "AI_SELALL,SELALL,SELECTALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = select_all_command
    },
    /* 107 */
    {
        .command = "singlelinetext",
        .arguments = "",
        .icon = "singlelinetext",
        .tooltip = "&Single Line Text",
        .statustip = "Creates single-line text objects:  TEXT",
        .alias = "DT,DTEXT,TEXT,SINGLELINETEXT",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = single_line_text_command
    },
    /* 108 */
    {
        .command = "snowflake",
        .arguments = "",
        .icon = "snowflake",
        .tooltip = "&Snowflake",
        .statustip = "Creates a snowflake:  SNOWFLAKE",
        .alias = "SNOWFLAKE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = snowflake_command
    },
    /* 109 */
    {
        .command = "star",
        .arguments = "",
        .icon = "star",
        .tooltip = "&Star",
        .statustip = "Creates a star:  STAR",
        .alias = "STAR",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = star_command
    },
    /* 110 */
    {
        .command = "syswindows",
        .arguments = "",
        .icon = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows. Command: SYSWINDOWS",
        .alias = "WINDOWS",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = syswindows_command
    },
    /* 111 */
    {
        .command = "todo",
        .arguments = "",
        .icon = "todo",
        .tooltip = "&About",
        .statustip = "Displays information about this product:  ABOUT",
        .alias = "TODO",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = todo_command
    },
    /* 112 */
    {
        .command = "vulcanize",
        .arguments = "",
        .icon = "vulcanize",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action:  UNDO",
        .alias = "VULCANIZE",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = vulcanize_command
    },
    /* 113 */
    {
        .command = "add",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = add_geometry_command
    },
    /* 114 */
    {
        .command = "delete",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = delete_command
    },
    /* ACTION_GRIP_EDIT             115 */
    {
        .command = "donothing",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = grip_edit_command
    },
    /* ACTION_NAV                   116 */
    {
        .command = "donothing",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = nav_command
    },
    /* ACTION_MIRROR                117 */
    {
        .command = "donothing",
        .arguments = "",
        .icon = "no-symbol",
        .tooltip = "This action is unfinished.",
        .statustip = "This action is unfinished.",
        .alias = "",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = mirror_command
    },
    /* 118 */
    {
        .command = "test",
        .arguments = "",
        .icon = "test",
        .tooltip = "&Test",
        .statustip = "Run a sequence of commands for QA. Command: TEST.",
        .alias = "TEST",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = test_command
    },
    /* 119 */
    {
        .command = "simulate",
        .arguments = "",
        .icon = "simulate",
        .tooltip = "Show Simulation Controls",
        .statustip = "Show simulation controls.",
        .alias = "SIMULATE,SIM",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = simulate_command
    },
    /* 120 */
    {
        .command = "play",
        .arguments = "",
        .icon = "play",
        .tooltip = "Play",
        .statustip = "Start/resume the simulation.",
        .alias = "PLAY",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = play_command
    },
    /* 121 */
    {
        .command = "stop",
        .arguments = "",
        .icon = "stop",
        .tooltip = "Stop simulation",
        .statustip = "Stop the active simulation.",
        .alias = "STOP",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = stop_command
    },
    /* 122 */
    {
        .command = "pause",
        .arguments = "",
        .icon = "pause",
        .tooltip = "Pause",
        .statustip = "Simulate the current pattern.",
        .alias = "PAUSE",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = pause_command
    },
    /* 123 */
    {
        .command = "fast-forward",
        .arguments = "",
        .icon = "simulate",
        .tooltip = "Fast-forward",
        .statustip = "Simulate the current pattern.",
        .alias = "FAST_FORWARD,FF",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = fast_forward_command
    },
    /* 124 */
    {
        .command = "rewind",
        .arguments = "",
        .icon = "simulate",
        .tooltip = "Rewind",
        .statustip = "Rewind the active simulation.",
        .alias = "REWIND,REW",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = simulate_command
    },
    /* 125 */
    {
        .command = "export-video",
        .arguments = "",
        .icon = "film",
        .tooltip = "Export simulation video",
        .statustip = "Export the simulation as a *.gif or *.mp4 file.",
        .alias = "export-video",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = export_command
    },
    /* 126 */
    {
        .command = "qr",
        .arguments = "",
        .icon = "qr_code",
        .tooltip = "Generate QR code",
        .statustip = "Generate a QR code.",
        .alias = "QR",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = qr_command
    },
    /* 127 */
    {
        .command = "lettering",
        .arguments = "",
        .icon = "lettering",
        .tooltip = "Generate stitched lettering",
        .statustip = "Generate a lettering as stitches for machines without fonts.",
        .alias = "LETTERING",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = lettering_command
    },
    /* 128 */
    {
        .command = "pattern",
        .arguments = "s",
        .icon = "pattern",
        .tooltip = "Paste stitch pattern...",
        .statustip = "Paste one of the included designs to the stitch layer.",
        .alias = "PATTERN",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = pattern_command
    },
    /* 129 */
    {
        .command = "design",
        .arguments = "s",
        .icon = "design",
        .tooltip = "Paste vector design...",
        .statustip = "Paste one of the included designs to the vector layer.",
        .alias = "DESIGN",
        .shortcut = "",
        .flags = CONTEXT_FREE,
        .action = design_command
    },
    /* 130 */
    {
        .command = "arc",
        .icon = "arc",
        .tooltip = "&Arc",
        .statustip = "Displays information about this product:  ARC",
        .alias = "ARC",
        .shortcut = "",
        .flags = REQUIRED_VIEW,
        .action = do_nothing_command
    },
    /* Terminator symbol. */
    {
        .command = "--END--",
        .icon = "END",
        .tooltip = "END",
        .statustip = "END",
        .alias = "END",
        .shortcut = "END",
        .flags = -1,
        .action = do_nothing_command
    }
};

