/*!
 * \file data.cpp
 * \brief TOML loading.
 *
 * Embroidermodder 2.
 *
 * Copyright 2011-2025 The Embroidermodder Team
 * Embroidermodder 2 is Open Source Software, see LICENSE.md for licensing terms.
 *
 * Read the reference manual (https://www.libembroidery.org/downloads/emrm.pdf)
 * for advice on altering this file.
 *
 * -----------------------------------------------------------------------------
 *
 * TOML++ documentation
 * https://marzer.github.io/tomlplusplus/
 */

#include <iostream>
#include <cstring>
#include <string>

#include "core.h"
#include "../extern/toml11/single_include/toml.hpp"

Dictionary state;

CommandData command_data[N_ACTIONS];

void *command_ptrs[N_ACTIONS] = {
    (void*)do_nothing_command,
    (void*)do_nothing_command,
    (void*)new_command,
    (void*)open_command,
    (void*)save_command,
    (void*)save_as_command,
    (void*)print_command,
    (void*)details_command,
    (void*)exit_command,
    (void*)cut_command,
    (void*)copy_command,
    (void*)copy_selected_command,
    (void*)paste_command,
    (void*)paste_selected_command,
    (void*)undo_command,
    (void*)redo_command,
    (void*)window_close_command,
    (void*)window_close_all_command,
    (void*)window_cascade_command,
    (void*)window_tile_command,
    (void*)window_next_command,
    (void*)window_previous_command,
    (void*)help_command,
    (void*)changelog_command,
    (void*)tip_of_the_day_command,
    (void*)about_command,
    (void*)whats_this_command,
    (void*)icon16_command,
    (void*)icon24_command,
    (void*)icon32_command,
    (void*)icon48_command,
    (void*)icon64_command,
    (void*)icon128_command,
    (void*)settings_command,
    (void*)make_layer_active_command,
    (void*)layers_command,
    (void*)layer_selector_command,
    (void*)layer_previous_command,
    (void*)color_selector_command,
    (void*)line_type_selector_command,
    (void*)line_weight_selector_command,
    (void*)hide_all_layers_command,
    (void*)show_all_layers_command,
    (void*)freeze_all_layers_command,
    (void*)thaw_all_layers_command,
    (void*)lock_all_layers_command,
    (void*)unlock_all_layers_command,
    (void*)text_bold_command,
    (void*)text_italic_command,
    (void*)text_underline_command,
    (void*)text_strikeout_command,
    (void*)text_overline_command,
    (void*)zoom_real_time_command,
    (void*)zoom_previous_command,
    (void*)zoom_window_command,
    (void*)zoom_dynamic_command,
    (void*)zoom_scale_command,
    (void*)zoom_center_command,
    (void*)zoom_in_command,
    (void*)zoom_out_command,
    (void*)zoom_selected_command,
    (void*)zoom_all_command,
    (void*)zoom_extents_command,
    (void*)panrealtime_command,
    (void*)panpoint_command,
    (void*)panleft_command,
    (void*)panright_command,
    (void*)panup_command,
    (void*)pandown_command,
    (void*)day_command,
    (void*)night_command,
    (void*)alert_command,
    (void*)get_command,
    (void*)set_command,
    (void*)do_nothing_command,
    (void*)angle_command,
    (void*)circle_command,
    (void*)debug_command,
    (void*)dimleader_command,
    (void*)disable_command,
    (void*)distance_command,
    (void*)dolphin_command,
    (void*)ellipse_command,
    (void*)enable_command,
    (void*)erase_command,
    (void*)error_command,
    (void*)heart_command,
    (void*)line_command,
    (void*)located_point_command,
    (void*)mirror_selected_command,
    (void*)move_command,
    (void*)move_selected_command,
    (void*)path_command,
    (void*)platform_command,
    (void*)point_command,
    (void*)polygon_command,
    (void*)polyline_command,
    (void*)previewoff_command,
    (void*)previewon_command,
    (void*)quickleader_command,
    (void*)rectangle_command,
    (void*)rgb_command,
    (void*)rotate_command,
    (void*)do_nothing_command,
    (void*)scale_command,
    (void*)scale_selected_command,
    (void*)select_all_command,
    (void*)single_line_text_command,
    (void*)snowflake_command,
    (void*)star_command,
    (void*)syswindows_command,
    (void*)todo_command,
    (void*)vulcanize_command,
    (void*)add_geometry_command,
    (void*)delete_command,
    (void*)grip_edit_command,
    (void*)nav_command,
    (void*)mirror_command,
    (void*)test_command,
    (void*)simulate_command,
    (void*)play_command,
    (void*)stop_command,
    (void*)pause_command,
    (void*)fast_forward_command,
    (void*)simulate_command,
    (void*)export_command,
    (void*)qr_command,
    (void*)lettering_command,
    (void*)pattern_command,
    (void*)design_command
};

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

IntMap obj_index[MAX_OBJECTS];

EmbIdList *cutCopyObjectList;

bool blinkState;

int iconSize;

bool pickAdd;

int promptInputNum = 0;

int precisionAngle;
int precisionLength;

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

/*! \brief Load the entire file with path fname into the buffer passed in.
 *
 * Caller is responsible for reporting the error, as we use this to determine
 * if the file exists as well as if it can be read.
 */
size_t
load_file_to_buffer(char *fname, char *buffer)
{
    FILE *f = fopen(fname, "r");
    if (!f) {
        return 0;        
    }
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    size_t bytes_read = fread(buffer, 1, size, f);
    fclose(f);
    if (bytes_read != size) {
        printf("ERROR: Failed to read configuration file \"%s\".\n", fname);
        return 0;
    }
    return 1;
}

/*! \brief Loads init_script 1 Mb buffer to chain together all of the
 *         strings in the init_script string table.
 *
 * \todo could reuse buffer safely between script loads.
 */
int
load_script(std::string fname)
{
    auto config = toml::parse(fname, toml::spec::v(1,1,0));
    StrList variables = get_sl("variables");
    for (size_t i=0; i<variables.size(); i++) {
        /* Override for command_data: this has a different structure. */
        if (config.contains(variables[i])) {
            StrList a = toml::find<StrList>(config, variables[i]);
            /* TODO: Check for failure here . */
            state[variables[i]] = a;
        }
    }

    return 0;
}

/* . */
std::string
get_s(std::string key)
{
    auto result = state.find(key);
    if (result == state.end()) {
        std::string s;
        debug_message("ERROR: Failed to find %s.", key.c_str());
        return s;
    }
    return result->second.s;
}

/* . */
std::vector<std::string>
get_sl(std::string key)
{
    auto result = state.find(key);
    if (result == state.end()) {
        StrList empty;
        debug_message("ERROR: Failed to find %s.", key.c_str());
        return empty;
    }
    return result->second.sl;
}

/*! \brief Load our basic tree structure of data for all of our data structures
 *         above.
 *
 * EXAMPLE
 * We want access to the `os` string, it is a static in this file.
 * The command would be `get_env_var(global, "os").s`.
 *
 * To access something deeper in the tree we use the dot operator, which
 * allows searches to run faster by encapsulating data into structures.
 * Like "file_menu.4" to give the 4th element in the file_menu array.
 *
 * For accessing data at speed we can keep the parent node pointer available,
 * so for example:
 *
 *     ScriptValue *file_menu = get_var(root, "file_menu");
 *     for (int i=0; i<file_menu->n_leaves; i++) {
 *         printf("%s\n", file_menu->leaves[i].s);
 *     }
 *
 * To get the length of an array we .
 *
 * Rather than loading necessary configuration data from file at load, it is
 * compiled into the program. However, the ability to change the UI as a
 * user, without re-compiling the program, can be preserved by overriding the string
 * tables from a custom "overrides.ini" file in the configuration directory.
 */
int
load_global_state(char *asset_dir)
{
    std::string assets(asset_dir);

    /* These entries aren't basic string tables or they are and they are necessary
     * to load for load_script and the rest of this function to work correctly.
     */
    auto config = toml::parse("assets/manifest.toml", toml::spec::v(1,1,0));
    if (config.contains("manifest")) {
        StrList a = toml::find<StrList>(config, "manifest");
        /* TODO: Check for failure here . */
        state["manifest"] = a;
    }
    else {
        return 1;
    }
    if (config.contains("variables")) {
        StrList a = toml::find<StrList>(config, "variables");
        /* TODO: Check for failure here . */
        state["variables"] = a;
    }
    else {
        return 1;
    }

    config = toml::parse("assets/command_data.toml", toml::spec::v(1,1,0));
    if (config.contains("command_data")) {
        auto command_data_loaded = config.at("command_data");
        /* TODO: Check for failure here . */
        for (int i=0; i<N_ACTIONS; i++) {
            auto table = command_data_loaded.at(i);

            std::string command = table.at("command").as_string();
            strcpy(command_data[i].command, command.c_str());

            std::string icon = table.at("icon").as_string();
            strcpy(command_data[i].icon, icon.c_str());

            std::string tooltip = table.at("tooltip").as_string();
            strcpy(command_data[i].tooltip, tooltip.c_str());

            std::string statustip = table.at("statustip").as_string();
            strcpy(command_data[i].statustip, statustip.c_str());

            std::string alias = table.at("alias").as_string();
            strcpy(command_data[i].alias, alias.c_str());

            std::string shortcut = table.at("shortcut").as_string();
            strcpy(command_data[i].shortcut, shortcut.c_str());

            /* FIXME: actually parse flags out of the string */
            std::string flags = table.at("flags").as_string();
            if (flags == "REQUIRED_VIEW") {
                command_data[i].flags = REQUIRED_VIEW;
            }
            if (flags == "REQUIRED_VIEW | CLEAR_SELECTION") {
                command_data[i].flags = REQUIRED_VIEW | CLEAR_SELECTION;
            }
            if (flags == "CONTEXT_FREE") {
                command_data[i].flags = CONTEXT_FREE;
            }
            if (flags == "CONTEXT_FREE | CLEAR_SELECTION") {
                command_data[i].flags = CONTEXT_FREE | CLEAR_SELECTION;
            }

            command_data[i].action = command_ptrs[i];
        }
    }
    else {
        return 1;
    }

    StrList manifest = get_sl("manifest");
    for (size_t i=0; i<manifest.size(); i++) {
        std::string fname = "assets/" + manifest[i];
        std::cout << "Loading \"" << fname << "\"..." << std::endl;
        int error = load_script(fname);
        if (error) {
            return error;
        }
    }

    /* Load "_command" keys into the command_data table. */
    return 0;
}

