/*
 * Embroidermodder 2.
 *
 * Copyright 2011-2024 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * String Data
 */

#include "core.h"

State state = {

/* TODO: find a source for compiler flags for operating system detection.
 */
#if defined(_WIN32)
    .os = "Windows",
#elif defined(__CYGWIN__)
    .os = "Cygwin",
#elif defined(__linux__)
    .os = "Linux",
#elif defined(__unix__)
    .os = "Unix",
#elif defined(__APPLE__) || defined(__MACH__)
    .os = "Mac OS",
#elif defined(__FREEBSD__)
    .os = "FreeBSD",
#elif defined(__ANDROID__)
    .os = "Android",
#else
    .os = "Unknown Operating System",
#endif

    /* -------------------------- Versions ------------------------------ */

    .embroidermodder_version = "2.0.0-alpha4",
    .libembroidery_version = "1.0.0-alpha",
    .EmbroideryMobile_version = "1.0.0-alpha",
    .PET_version = "1.0.0-alpha",

    /* ---------------------------- Paths ------------------------------ */

    .circle_origin_path = "M 0.0 1.0 " \
        "A -1.0 -1.0 2.0 2.0 1 90.0 360.0 " \
        "A -1.0 -1.0 2.0 2.0 1 90.0 -360.0 " \
        "L 0.0 -1.0 " \
        "A -1.0 -1.0 2.0 2.0 1 270.0 90.0 " \
        "L -1.0 0.0 " \
        "A -1.0 -1.0 2.0 2.0 1 180.0 -90.0 ",
//      "Z"

    .one_path = "m 0.05 0.00 " \
        "l 0.45 0.00 " \
        "m 0.00 -0.75 " \
        "m 0.25 -1.00 " \
        "l 0.25 0.00",

    .two_path = "m 0.00 -0.75 " \
        "a 0.00 -1.00 0.50 0.50 1 180.00 -216.87 " \
        "l 0.00 0.00 " \
        "l 0.50 0.00",

    .three_path = "arc m 0.00 -0.50 0.50 0.50 195.00 " \
        "a 0.00 -0.50 0.50 0.50 195.00, 255.00 " \
        "a 0.00 -1.00 0.50 0.50 270.00, 255.00",

    .four_path = "m 0.50 -0.00 " \
        "l 0.50 -1.00 " \
        "l 0.00 -0.50 " \
        "l 0.50 -0.50",

    .five_path = "m 0.50 -1.00 " \
        "l 0.00 -1.00 " \
        "l 0.00 -0.50 " \
        "l 0.25 -0.50 " \
        "a 0.00 -0.50 0.50 0.50 90.00 -180.00 " \
        "l 0.00 0.00",

    .six_path = "E 0.25 -0.25 0.25 0.25 " \
        " m 0.00 -0.25 " \
        "l 0.00 -0.75 " \
        "a 0.00 -1.00 0.50 0.50 180.00, -140.00",

    .seven_path = "m 0.00 -1.00 " \
        "l 0.50 -1.00 " \
        "l 0.25 -0.25 " \
        "l 0.25 -0.00",

    .eight_path = "E 0.25 -0.25 0.25 0.25 " \
        "E 0.25 -0.75 0.25 0.25",

    .nine_path = "E 0.25 -0.75 0.25 0.25 " \
        "m 0.50 -0.75 " \
        "l 0.50 -0.25 " \
        "a 0.00 -0.50 0.50 0.50 0.00, -140.00",

    .zero_path = "m 0.00 -0.75 " \
        "l 0.00 -0.25 " \
        "a 0.00 -0.50 0.50 0.50 180.00, 180.00 " \
        "l 0.50 -0.75 " \
        "a 0.00 -1.00 0.50 0.50 0.00, 180.00",

    .minus_path = "m 0.00 -0.50 " \
        "l 0.50 -0.50",

    .apostrophe_path = "m 0.25 -1.00 " \
        "l 0.25 -0.75",

    .quote_path = "m 0.10 -1.00 " \
        "l 0.10 -0.75 " \
        "m 0.40 -1.00 " \
        "l 0.40 -0.75",

    /* ---------------------------- Menus ------------------------------ */

    .menu_list = {
        "&File",
        "&Edit",
        "&View",
        "&Draw",
        "&Tools",
        "&Modify",
        "S&andbox",
        "&Dimension",
        "&Window",
        "&Help",
        "Open &Recent",
        "&Zoom",
        "&Pan",
        "END"
    },

    .menubar_full_list = {
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
    },

    .menubar_no_docs = {
        MENU_FILE,
        MENU_EDIT,
        MENU_WINDOW,
        MENU_HELP,
        TERMINATOR_SYMBOL
    },

    .file_menu = {
        "new",
        "---",
        "open",
        ">Open &Recent",
        "---",
        "save",
        "saveas",
        "---",
        "print",
        "---",
        "windowclose",
        "---",
        "designdetails",
        "---",
        "exit",
        "END"
    },

    .edit_menu = {
        "undo",
        "redo",
        "---",
        "cut",
        "copy",
        "paste",
        "---",
        "settingsdialog",
        "END"
    },

    .view_menu = {
        ">&Zoom",
        ">&Pan",
        "---",
        "day",
        "night",
        "---",
        "END"
    },

    .zoom_menu = {
        "zoomwindow",
        "zoomdynamic",
        "zoomscale",
        "---",
        "zoomcenter",
        "zoomin",
        "zoomout",
        "---",
        "zoomselected",
        "zoomall",
        "zoomextents",
        "END"
    },

    .pan_menu = {
        "panrealtime",
        "panpoint",
        "---",
        "panleft",
        "panright",
        "panup",
        "pandown",
        "END"
    },

    .help_menu = {
        "help",
        "---",
        "changelog",
        "---",
        "tipoftheday",
        "---",
        "about",
        "---",
        "whatsthis",
        "END"
    },

    .draw_menu = {
        "path",
        "polyline",
        "---",
        "arc",
        "circle",
        "ellipse",
        "line",
        "point",
        "polygon",
        "rectangle",
        "singlelinetext",
        "---",
        "dolphin",
        "heart",
        "snowflake",
        "star",
        "END"
    },

    .tools_menu = {
        "distance",
        "locatepoint",
        "selectall",
        "END"
    },

    .modify_menu = {
        "erase",
        "move",
        "rotate",
        "scale",
        "END"
    },

    .dimension_menu = {
        "quickleader",
        "END"
    },

    .sandbox_menu = {
        "sandbox",
        "END"
    },

    .recent_menu = {
        "END"
    },

    .window_menu = {
        "END"
    },

    /* ---------------------------- Toolbars ------------------------------ */

    .toolbar_list = {
        "File",
        "Edit",
        "View",
        "Zoom",
        "Pan",
        "Icon",
        "Help",
        "Layer",
        "Properties",
        "Text",
        "Command Prompt",
        "Draw",
        "Modify",
        "Inquiry",
        "Dimension",
        "Sandbox",
        "END"
    },

    .top_toolbar = {
        TOOLBAR_BREAK,
        TOOLBAR_FILE,
        TOOLBAR_EDIT,
        TOOLBAR_HELP,
        TOOLBAR_ICON,
        TOOLBAR_BREAK,
        TOOLBAR_ZOOM,
        TOOLBAR_PAN,
        TOOLBAR_VIEW,
        TOOLBAR_BREAK,
        TOOLBAR_LAYER,
        TOOLBAR_PROPERTIES,
        TOOLBAR_BREAK,
        TOOLBAR_TEXT,
        TERMINATOR_SYMBOL
    },

    .left_toolbar = {
        TOOLBAR_DRAW,
        TOOLBAR_MODIFY,
        TOOLBAR_BREAK,
        TERMINATOR_SYMBOL
    },

    .bottom_toolbar = {
        TOOLBAR_PROMPT,
        TERMINATOR_SYMBOL
    },

    .toolbar_horizontal = {
        TOOLBAR_VIEW,
        TOOLBAR_ZOOM,
        TOOLBAR_LAYER,
        TOOLBAR_PROPERTIES,
        TOOLBAR_TEXT,
        TOOLBAR_PROMPT,
        TERMINATOR_SYMBOL
    },

    .toolbars_when_docs = {
        TOOLBAR_VIEW,
        TOOLBAR_ZOOM,
        TOOLBAR_PAN,
        TOOLBAR_ICON,
        TOOLBAR_HELP,
        TOOLBAR_LAYER,
        TOOLBAR_TEXT,
        TOOLBAR_PROPERTIES,
        TOOLBAR_DRAW,
        TERMINATOR_SYMBOL
    },

    .file_toolbar = {
        "new",
        "open",
        "save",
        "saveas",
        "print",
        "designdetails",
        "---",
        "undo",
        "redo",
        "---",
        "help",
        "END"
    },

    .edit_toolbar = {
        "cut",
        "copy",
        "paste",
        "END"
    },

    .view_toolbar = {
        "day",
        "night",
        "END"
    },

    .zoom_toolbar = {
        "zoomwindow",
        "zoomdynamic",
        "zoomscale",
        "---",
        "zoomcenter",
        "zoomin",
        "zoomout",
        "---",
        "zoomselected",
        "zoomall",
        "zoomextents",
        "END"
    },

    .pan_toolbar = {
        "panrealtime",
        "panpoint",
        "---",
        "panleft",
        "panright",
        "panup",
        "pandown",
        "END"
    },

    .icon_toolbar = {
        "icon16",
        "icon24",
        "icon32",
        "icon48",
        "icon64",
        "icon128",
        "END"
    },

    .help_toolbar = {
        "help",
        "---",
        "changelog",
        "---",
        "about",
        "---",
        "whatsthis",
        "END"
    },

    .draw_toolbar = {
        "path",
        "polyline",
        "---",
        "arc",
        "circle",
        "ellipse",
        "line",
        "point",
        "polygon",
        "rectangle",
        "singlelinetext",
        "---",
        "dolphin",
        "heart",
        "snowflake",
        "star",
        "END"
    },

    .inquiry_toolbar = {
        "distance",
        "locatepoint",
        "END"
    },

    .modify_toolbar = {
        "erase",
        "move",
        "rotate",
        "scale",
        "selectall",
        "END"
    },

    .dimension_toolbar = {
        "quickleader",
        "END"
    },

    .sandbox_toolbar = {
        "sandbox",
        "END"
    },

    .layer_toolbar = {
        "END"
    },

    .properties_toolbar = {
        "END"
    },

    .text_toolbar = {
        "END"
    },

    .prompt_toolbar = {
        "END"
    },

    /* ---------------------------- Groups ------------------------------- */

    .grid_load_from_file_group = {
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
    },

    .defined_origin_group = {
        "labelGridCenterX",
        "spinBoxGridCenterX",
        "labelGridCenterY",
        "spinBoxGridCenterY",
        "labelGridSizeX",
        "spinBoxGridSizeX",
        "labelGridSizeY",
        "spinBoxGridSizeY",
        "END"
    },

    .rectangular_grid_group = {
        "labelGridSizeX",
        "spinBoxGridSizeX",
        "labelGridSizeY",
        "spinBoxGridSizeY",
        "labelGridSpacingX",
        "spinBoxGridSpacingX",
        "labelGridSpacingY",
        "spinBoxGridSpacingY",
        "END"
    },

    .circular_grid_group = {
        "labelGridSizeRadius",
        "spinBoxGridSizeRadius",
        "labelGridSpacingRadius",
        "spinBoxGridSpacingRadius",
        "labelGridSpacingAngle",
        "spinBoxGridSpacingAngle",
        "END"
    },

    /* FIXME:. */
    .center_on_origin_group = {
        "END"
    },

    /* ---------------------------- Settings ------------------------------- */

    .settings_tab_labels = {
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
        "END"
    },

    .preview_to_dialog = {
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
    },

    .accept_to_dialog = {
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
    },

    .render_hints = {
        DISPLAY_USE_OPENGL,
        DISPLAY_RENDERHINT_AA,
        DISPLAY_RENDERHINT_TEXT_AA,
        DISPLAY_RENDERHINT_SMOOTH_PIX,
        DISPLAY_RENDERHINT_HIGH_AA,
        DISPLAY_RENDERHINT_NONCOSMETIC,
        TERMINATOR_SYMBOL
    },

    /* ---------------------------- Objects ------------------------------- */

    .object_names = {
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
        "Unknown"
    },

    /* ---------------------------- Testing ------------------------------- */

    .coverage_test = {
        "new",
        "icon16",
        "icon24",
        "icon36",
        "exit",
        "END"
    },

    /* ---------------------------- Misc ---------------------------------- */

    .tips = {
        "we need more tips?",
        "you can change the color of the display through settings?",
        "you can hide the scrollbars to increase the viewable area through settings?",
        "you can change the icon size for increased visibility?",
        "you can toggle the grid on and off by pressing the button in the statusbar?",
        "the grid size can be changed to match your hoop size through settings?",
        "the crosshair size is based on a percentage of your screen size? Setting it to 100 may help you visually line things up better.",
        "you can pan by pressing the middle mouse button and dragging your mouse across the screen?",
        "you can open and edit multiple designs simultaneously?",
        "you that many embroidery machines support the .dst format?",
        "that you can zoom in and out using your mouse wheel?",
        "that you can use circular and isometric grids?",
        "about our command line format converter?",
        "that you can use the 'DAY' and 'NIGHT' commands to quickly switch the view colors to commonly used white or black?",
        "that you can quickly change the background, crosshair and grid colors using the 'RGB' command?",
        "END"
    },

    .extensions = {
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
    }
};


EmbStringTable editor_list = {
    "ArcCenterX",
    "ArcCenterY",
    "ArcRadius",
    "ArcStartAngle",
    "ArcEndAngle",
    "ArcStartX",
    "ArcStartY",
    "ArcEndX",
    "ArcEndY",
    "ArcArea",
    "ArcLength",
    "ArcChord",
    "ArcIncAngle",
    "BlockPositionX",
    "BlockPositionY",
    "CircleCenterX",
    "CircleCenterY",
    "CircleRadius",
    "CircleDiameter",
    "CircleArea",
    "CircleCircumference",
    "EllipseCenterX",
    "EllipseCenterY",
    "EllipseRadiusMajor",
    "EllipseRadiusMinor",
    "EllipseDiameterMajor",
    "EllipseDiameterMinor",
    "ImageX",
    "ImageY",
    "ImageWidth",
    "ImageHeight",
    "InfiniteLineX1",
    "InfiniteLineY1",
    "InfiniteLineX2",
    "InfiniteLineY2",
    "InfiniteLineVectorX",
    "InfiniteLineVectorY",
    "LineStartX",
    "LineStartY",
    "LineEndX",
    "LineEndY",
    "LineDeltaX",
    "LineDeltaY",
    "LineAngle",
    "LineLength",
    "PathVertexX",
    "PathVertexY",
    "PathArea",
    "PathLength",
    "PointX",
    "PointY",
    "PolygonCenterX",
    "PolygonCenterY",
    "PolygonRadiusVertex",
    "PolygonRadiusSide",
    "PolygonDiameterVertex",
    "PolygonDiameterSide",
    "PolygonInteriorAngle",
    "PolylineVertexX",
    "PolylineVertexY",
    "PolylineArea",
    "PolylineLength",
    "RayX1",
    "RayY1",
    "RayX2",
    "RayY2",
    "RayVectorX",
    "RayVectorY",
    "RectangleCorner1X",
    "RectangleCorner1Y",
    "RectangleCorner2X",
    "RectangleCorner2Y",
    "RectangleCorner3X",
    "RectangleCorner3Y",
    "RectangleCorner4X",
    "RectangleCorner4Y",
    "RectangleWidth",
    "RectangleHeight",
    "RectangleArea",
    "TextMultiX",
    "TextMultiY",
    "TextSingleHeight",
    "TextSingleRotation",
    "TextSingleX",
    "TextSingleY",
    "TextSingleContents",
    "TextSingleHeight",
    "TextSingleRotation",
    "END"
};

EmbStringTable combobox_list = {
    "GeneralLayer",
    "GeneralColor",
    "GeneralLineType",
    "GeneralLineWeight",
    "ArcClockwise",
    "PathVertexNum",
    "PathClosed",
    "PolylineVertexNum",
    "PolylineClosed",
    "TextSingleJustify",
    "TextSingleBackward",
    "TextSingleUpsideDown",
    "END"
};

EmbStringTable layer_list = {
    "linetypebylayer", "0", "",
    "linetypebylayer", "1", "",
    "linetypebylayer", "2", "",
    "linetypebylayer", "3", "",
    "linetypebylayer", "4", "",
    "linetypebylayer", "5", "",
    "linetypebylayer", "6", "",
    "linetypebylayer", "7", "",
    "linetypebylayer", "8", "",
    "linetypebylayer", "9", "",
    "END", "END", "END"
};

EmbStringTable color_list = {
    "colorbylayer", "ByLayer", "",
    "colorbyblock", "ByBlock", "",
    "colorred", "Red", "255,0,0",
    "coloryellow", "Yellow", "255,255,0",
    "colorgreen", "Green", "0,255,0",
    "colorcyan", "Cyan", "0,255,255",
    "colorblue", "Blue", "0,0,255",
    "colormagenta", "Magenta", "255,0,255",
    "colorwhite", "White", "255,255,255",
    "colorother", "Other...", "",
    "END", "END", "END"
};

EmbStringTable line_type_list = {
    "linetypebylayer", "ByLayer", "",
    "linetypebyblock", "ByBlock", "",
    "linetypecontinuous", "Continuous", "",
    "linetypehidden", "Hidden", "",
    "linetypecenter", "Center", "",
    "linetypeother", "Other...", "",
    "END", "END", "END"
};

/* TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight */
EmbStringTable line_weight_list = {
    "lineweightbylayer", "ByLayer", "-2.00",
    "lineweightbyblock", "ByBlock", "-1.00",
    "lineweightdefault", "Default", "0.00",
    "lineweight01", "0.00 mm", "0.00",
    "lineweight02", "0.05 mm", "0.05",
    "lineweight03", "0.15 mm", "0.15",
    "lineweight04", "0.20 mm", "0.20",
    "lineweight05", "0.25 mm", "0.25",
    "lineweight06", "0.30 mm", "0.30",
    "lineweight07", "0.35 mm", "0.35",
    "lineweight08", "0.40 mm", "0.40",
    "lineweight09", "0.45 mm", "0.45",
    "lineweight10", "0.50 mm", "0.50",
    "lineweight11", "0.55 mm", "0.55",
    "lineweight12", "0.60 mm", "0.60",
    "lineweight13", "0.65 mm", "0.65",
    "lineweight14", "0.70 mm", "0.70",
    "lineweight15", "0.75 mm", "0.75",
    "lineweight16", "0.80 mm", "0.80",
    "lineweight17", "0.85 mm", "0.85",
    "lineweight18", "0.90 mm", "0.90",
    "lineweight19", "0.95 mm", "0.95",
    "lineweight20", "1.00 mm", "1.00",
    "lineweight21", "1.05 mm", "1.05",
    "lineweight22", "1.10 mm", "1.10",
    "lineweight23", "1.15 mm", "1.15",
    "lineweight24", "1.20 mm", "1.20",
    "END", "END", "END"
};

EmbStringTable text_size_list = {
    "6 pt", "6", "",
    "8 pt", "8", "",
    "9 pt", "9", "",
    "10 pt", "10", "",
    "11 pt", "11", "",
    "12 pt", "12", "",
    "14 pt", "14", "",
    "18 pt", "18", "",
    "24 pt", "24", "",
    "30 pt", "30", "",
    "36 pt", "36", "",
    "48 pt", "48", "",
    "60 pt", "60", "",
    "72 pt", "72", "",
    "END", "END", "END"
};

