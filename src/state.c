/*
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

const char *help_msg = \
     " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ \n" \
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\\n" \
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/\n" \
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|\n" \
    " _____________________________________________________________________________ \n" \
    "|                                                                             |\n" \
    "|                   https://www.libembroidery.org                             |\n" \
    "|_____________________________________________________________________________|\n" \
    "\n" \
    "Usage: embroidermodder [options] files ...\n" \
    "Options:\n" \
    "  -d, --debug      Print lots of debugging information.\n" \
    "  -h, --help       Print this message and exit.\n" \
    "  -v, --version    Print the version number of embroidermodder and exit.\n" \
    "\n";

ScriptEnv *global;
ScriptValue *config;
int n_variables = 0;

char formatFilterOpen[MAX_LONG_STRING];
char formatFilterSave[MAX_LONG_STRING];
char open_filesPath[MAX_STRING_LENGTH];
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

const ScriptValue script_null = {
    .r = 0.0F,
    .i = 0,
    .b = false,
    .s = "",
    .label = "NULL",
    .type = SCRIPT_NULL
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
char *_appVer_  = "v2.0 alpha";
bool exitApp = false;
int testing_mode = 0;

/* TODO: find a source for compiler flags for operating system detection.
 */
#if defined(_WIN32)
const char *os = "Windows";
#elif defined(__CYGWIN__)
const char *os = "Cygwin";
#elif defined(__linux__)
const char *os = "Linux";
#elif defined(__unix__)
const char *os = "Unix";
#elif defined(__APPLE__) || defined(__MACH__)
const char *os = "Mac OS";
#elif defined(__FREEBSD__)
const char *os = "FreeBSD";
#elif defined(__ANDROID__)
const char *os = "Android";
#else
const char *os = "Unknown Operating System";
#endif

/* -------------------------- Versions ------------------------------ */

const char *embroidermodder_version = "2.0.0-alpha4";
const char *libembroidery_version = "1.0.0-alpha";
const char *EmbroideryMobile_version = "1.0.0-alpha";
const char *PET_version = "1.0.0-alpha";

/* ---------------------------- Paths ------------------------------ */

const char *circle_origin_path = "M 0.0 1.0 " \
    "A -1.0 -1.0 2.0 2.0 1 90.0 360.0 " \
    "A -1.0 -1.0 2.0 2.0 1 90.0 -360.0 " \
    "L 0.0 -1.0 " \
    "A -1.0 -1.0 2.0 2.0 1 270.0 90.0 " \
    "L -1.0 0.0 " \
    "A -1.0 -1.0 2.0 2.0 1 180.0 -90.0 ";
//  "Z";

const char *one_path = "m 0.05 0.00 " \
    "l 0.45 0.00 " \
    "m 0.00 -0.75 " \
    "m 0.25 -1.00 " \
    "l 0.25 0.00";

const char *two_path = "m 0.00 -0.75 " \
    "a 0.00 -1.00 0.50 0.50 1 180.00 -216.87 " \
    "l 0.00 0.00 " \
    "l 0.50 0.00";

const char *three_path = "arc m 0.00 -0.50 0.50 0.50 195.00 " \
    "a 0.00 -0.50 0.50 0.50 195.00, 255.00 " \
    "a 0.00 -1.00 0.50 0.50 270.00, 255.00";

const char *four_path = "m 0.50 -0.00 " \
    "l 0.50 -1.00 " \
    "l 0.00 -0.50 " \
    "l 0.50 -0.50";

const char *five_path = "m 0.50 -1.00 " \
    "l 0.00 -1.00 " \
    "l 0.00 -0.50 " \
    "l 0.25 -0.50 " \
    "a 0.00 -0.50 0.50 0.50 90.00 -180.00 " \
    "l 0.00 0.00";

const char *six_path = "E 0.25 -0.25 0.25 0.25 " \
    " m 0.00 -0.25 " \
    "l 0.00 -0.75 " \
    "a 0.00 -1.00 0.50 0.50 180.00, -140.00";

const char *seven_path = "m 0.00 -1.00 " \
    "l 0.50 -1.00 " \
    "l 0.25 -0.25 " \
    "l 0.25 -0.00";

const char *eight_path = "E 0.25 -0.25 0.25 0.25 " \
    "E 0.25 -0.75 0.25 0.25";

const char *nine_path = "E 0.25 -0.75 0.25 0.25 " \
    "m 0.50 -0.75 " \
    "l 0.50 -0.25 " \
    "a 0.00 -0.50 0.50 0.50 0.00, -140.00";

const char *zero_path = "m 0.00 -0.75 " \
    "l 0.00 -0.25 " \
    "a 0.00 -0.50 0.50 0.50 180.00, 180.00 " \
    "l 0.50 -0.75 " \
    "a 0.00 -1.00 0.50 0.50 0.00, 180.00";

const char *minus_path = "m 0.00 -0.50 " \
    "l 0.50 -0.50";

const char *apostrophe_path = "m 0.25 -1.00 " \
    "l 0.25 -0.75";

const char *quote_path = "m 0.10 -1.00 " \
    "l 0.10 -0.75 " \
    "m 0.40 -1.00 " \
    "l 0.40 -0.75";

/* ---------------------------- Menus ------------------------------ */

char *menu_list[] = {
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
    END_SYMBOL
};

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

char *file_menu[] = {
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
    END_SYMBOL
};

char *edit_menu[] = {
    "undo",
    "redo",
    "---",
    "cut",
    "copy",
    "paste",
    "---",
    "settingsdialog",
    END_SYMBOL
};

char *view_menu[] = {
    ">&Zoom",
    ">&Pan",
    "---",
    "day",
    "night",
    "---",
    END_SYMBOL
};

char *zoom_menu[] = {
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
    END_SYMBOL
};

char *pan_menu[] = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    END_SYMBOL
};

char *help_menu[] = {
    "help",
    "---",
    "changelog",
    "---",
    "tipoftheday",
    "---",
    "about",
    "---",
    "whatsthis",
    END_SYMBOL
};

char *draw_menu[] = {
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
    END_SYMBOL
};

char *tools_menu[] = {
    "distance",
    "locatepoint",
    "selectall",
    END_SYMBOL
};

char *modify_menu[] = {
    "erase",
    "move",
    "rotate",
    "scale",
    END_SYMBOL
};

char *dimension_menu[] = {
    "quickleader",
    END_SYMBOL
};

char *sandbox_menu[] = {
    "simulate",
    "play",
    "pause",
    "rewind",
    "fast-forward",
    "stop",
    "export-video",
    "---",
    "qr",
    "lettering",
    "pattern",
    "design",
    END_SYMBOL
};

char *recent_menu[] = {
    END_SYMBOL
};

char *window_menu[] = {
    END_SYMBOL
};

/* ---------------------------- Toolbars ------------------------------ */

char *toolbar_list[] = {
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
    END_SYMBOL
};

int top_toolbar[] = {
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
    TOOLBAR_ZOOM,
    TOOLBAR_LAYER,
    TOOLBAR_PROPERTIES,
    TOOLBAR_TEXT,
    TOOLBAR_PROMPT,
    TERMINATOR_SYMBOL
};

int toolbars_when_docs[] = {
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
};

char *file_toolbar[] = {
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
    END_SYMBOL
};

char *edit_toolbar[] = {
    "cut",
    "copy",
    "paste",
    END_SYMBOL
};

char *view_toolbar[] = {
    "day",
    "night",
    END_SYMBOL
};

char *zoom_toolbar[] = {
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
    END_SYMBOL
};

char *pan_toolbar[] = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    END_SYMBOL
};

char *icon_toolbar[] = {
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128",
    END_SYMBOL
};

char *help_toolbar[] = {
    "help",
    "---",
    "changelog",
    "---",
    "about",
    "---",
    "whatsthis",
    END_SYMBOL
};

char *draw_toolbar[] = {
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
    END_SYMBOL
};

char *inquiry_toolbar[] = {
    "distance",
    "locatepoint",
    END_SYMBOL
};

char *modify_toolbar[] = {
    "erase",
    "move",
    "rotate",
    "scale",
    "selectall",
    END_SYMBOL
};

char *dimension_toolbar[] = {
    "quickleader",
    END_SYMBOL
};

char *sandbox_toolbar[] = {
    "sandbox",
    END_SYMBOL
};

char *layer_toolbar[] = {
    END_SYMBOL
};

char *properties_toolbar[] = {
    END_SYMBOL
};

char *text_toolbar[] = {
    END_SYMBOL
};

char *prompt_toolbar[] = {
    END_SYMBOL
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

/* ---------------------------- Testing ------------------------------- */

char *coverage_test[] = {
    "new",
    "icon16",
    "icon24",
    "icon36",
    "exit",
    END_SYMBOL
};

/* ---------------------------- Misc ---------------------------------- */

char *tips[] = {
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
    END_SYMBOL
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
    END_SYMBOL
};

char *editor_list[] = {
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
    END_SYMBOL
};

char *combobox_list[] = {
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
    END_SYMBOL
};

char *layer_list[] = {
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
    END_SYMBOL, END_SYMBOL, END_SYMBOL
};

char *color_list[] = {
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
    END_SYMBOL
};

char *line_type_list[] = {
    "linetypebylayer", "ByLayer", "",
    "linetypebyblock", "ByBlock", "",
    "linetypecontinuous", "Continuous", "",
    "linetypehidden", "Hidden", "",
    "linetypecenter", "Center", "",
    "linetypeother", "Other...", "",
    END_SYMBOL
};

/* TODO: Thread weight is weird. See http://en.wikipedia.org/wiki/Thread_(yarn)#Weight */
char *line_weight_list[] = {
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
    END_SYMBOL, END_SYMBOL, END_SYMBOL
};

char *text_size_list[] = {
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
    END_SYMBOL, END_SYMBOL, END_SYMBOL
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

/*
 *
 * Shortcuts should match: https://doc.qt.io/qt-6/qkeysequence.html#standard-shortcuts
 * Apple platforms may need an additional argument like .apple_shortcut
 */

#define PLACEHOLDER_ACTION \
    { \
        .command = "donothing", \
        .arguments = "", \
        .icon = "no-symbol", \
        .tooltip = "This action is unfinished.", \
        .statustip = "This action is unfinished.", \
        .alias = "", \
        .shortcut = "", \
        .flags = CONTEXT_FREE \
    }

Command command_data[MAX_COMMANDS] = {
    /* ACTION_NULL                    0 */
    PLACEHOLDER_ACTION,
    /* 1 */
    {
        .command = "donothing",
        .arguments = "",
        .icon = "x_circle",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = CONTEXT_FREE
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
        .flags = CONTEXT_FREE | CLEAR_SELECTION
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
    },
    /* ACTION_COPY_SELECTED          11 */
    PLACEHOLDER_ACTION,
    /* 12 */
    {
        .command = "paste",
        .arguments = "",
        .icon = "clipboard_document",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection. Command: PASTE.",
        .alias = "PASTE",
        .shortcut = "Ctrl+V",
        .flags = REQUIRED_VIEW
    },
    /* ACTION_PASTE_SELECTED         13 */
    PLACEHOLDER_ACTION,
    /* 14 */
    {
        .command = "undo",
        .arguments = "",
        .icon = "arrow_uturn_left",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action. Command: UNDO.",
        .alias = "U",
        .shortcut = "Ctrl+Z",
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = CONTEXT_FREE
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = CONTEXT_FREE
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
    },
    /* 45 */
    {
        .command = "lockalllayers",
        .arguments = "",
        .icon = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .alias = "LOCKALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    /* 46 */
    {
        .command = "unlockalllayers",
        .arguments = "",
        .icon = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .alias = "UNLOCKALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
    },
    /* 52 */
    {
        .command = "zoomrealtime",
        .arguments = "",
        .icon = "zoomrealtime",
        .tooltip = "Zoom &Real Time",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport. Command: ZOOMREALTIME",
        .alias = "ZOOMREALTIME",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    /* 53 */
    {
        .command = "zoomprevious",
        .arguments = "",
        .icon = "zoomprevious",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view. Command: ZOOMPREVIOUS.",
        .alias = "ZOOMPREVIOUS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    /* 54 */
    {
        .command = "zoomwindow",
        .arguments = "",
        .icon = "zoomwindow",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window. Command: ZOOMIN",
        .alias = "ZOOMWINDOW",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    /* 55 */
    {
        .command = "zoomdynamic",
        .arguments = "",
        .icon = "zoomdynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing. Command: ZOOMDYNAMIC.",
        .alias = "ZOOMDYNAMIC",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    /* 56 */
    {
        .command = "zoomscale",
        .arguments = "",
        .icon = "zoomscale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor. Command: ZOOMSCALE.",
        .alias = "ZOOMSCALE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    /* 57 */
    {
        .command = "zoomcenter",
        .arguments = "",
        .icon = "zoomcenter",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height. Command: ZOOMCENTER.",
        .alias = "ZOOMCENTER",
        .shortcut = "",
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
    },
    /* 60 */
    {
        .command = "zoomselected",
        .arguments = "",
        .icon = "zoomselected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects. Command: ZOOMSELECTED.",
        .alias = "ZOOMSELECTED",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    /* 61 */
    {
        .command = "zoomall",
        .arguments = "",
        .icon = "zoomall",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits. Command: ZOOMALL.",
        .alias = "ZOOMALL",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    },
    /* 62 */
    {
        .command = "zoomextents",
        .arguments = "",
        .icon = "zoomextents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents. Command: ZOOMEXTENTS",
        .alias = "ZOOMEXTENTS",
        .shortcut = "",
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
    },
    /* 74 */
    {
        .command = "clear",
        .arguments = "",
        .icon = "clear",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing. Command: DONOTHING.",
        .alias = "DONOTHING",
        .shortcut = "",
        .flags = REQUIRED_VIEW | CLEAR_SELECTION
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
        .flags = REQUIRED_VIEW | CLEAR_SELECTION
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
    },
    /* ACTION_DIM_LEADER             78 */
    PLACEHOLDER_ACTION,
    /* 79 */
    {
        .command = "disable",
        .arguments = "",
        .icon = "disable",
        .tooltip = "&Disable",
        .statustip = "Disable one of the pre-defined global boolean variables.",
        .alias = "DISABLE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
    },
    /* 103 */
    PLACEHOLDER_ACTION,
    /* 104 */
    {
        .command = "scale",
        .arguments = "",
        .icon = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions:  SCALE",
        .alias = "SC,SCALE",
        .shortcut = "",
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
    },
    /* ACTION_ADD                   113 */
    PLACEHOLDER_ACTION,
    /* ACTION_DELETE                114 */
    PLACEHOLDER_ACTION,
    /* ACTION_GRIP_EDIT             115 */
    PLACEHOLDER_ACTION,
    /* ACTION_NAV                   116 */
    PLACEHOLDER_ACTION,
    /* ACTION_MIRROR                117 */
    PLACEHOLDER_ACTION,
    /* 118 */
    {
        .command = "test",
        .arguments = "",
        .icon = "test",
        .tooltip = "&Test",
        .statustip = "Run a sequence of commands for QA. Command: TEST.",
        .alias = "TEST",
        .shortcut = "",
        .flags = REQUIRED_VIEW
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
        .flags = CONTEXT_FREE
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
        .flags = CONTEXT_FREE
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
        .flags = REQUIRED_VIEW
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
        .flags = CONTEXT_FREE
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = REQUIRED_VIEW
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
        .flags = CONTEXT_FREE
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
        .flags = CONTEXT_FREE
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
        .flags = CONTEXT_FREE
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
        .flags = CONTEXT_FREE
    },
    /* 130 */
    {
        .command = "arc",
        .icon = "arc",
        .tooltip = "&Arc",
        .statustip = "Displays information about this product:  ARC",
        .alias = "ARC",
        .shortcut = "",
        .flags = REQUIRED_VIEW
    }
};

