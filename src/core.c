/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2023 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "core.h"

/* See the "Programming principles for the C core" in the reference manual
 * before changing this file.
 */

/* So we can find any given piece of data in our UI for the functions to act
 * on they all lie in a tree that uses C-style tree walking during runtime.
 */
CNode *root;

const char *details_labels[] = {
    "Total Stitches:",
    "Real Stitches:",
    "Jump Stitches:",
    "Trim Stitches:",
    "Total Colors:",
    "Color Changes:",
    "Left:",
    "Top:",
    "Right:",
    "Bottom:",
    "Width:",
    "Height:",
    "END"
};

const char *default_prompt_style[] = {
    "color",                      "#000000", // Match -------|
    "background-color",           "#FFFFFF", //              |
    "selection-color",            "#FFFFFF", //              |
    "selection-background-color", "#000000", // Match -------|
    "font-family",              "Monospace",
    "font-style",                  "normal",
    "font-size",                     "12px"
    "END"
};

const ActionData action_table[] = {
    {
        .type = "action",
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer."
    }
};

const char *usage_msg = ""
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ \n"
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\\n"
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/\n"
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|\n"
    " _____________________________________________________________________________ \n"
    "|                                                                             |\n"
    "|                     https://www.libembroidery.org                           |\n"
    "|_____________________________________________________________________________|\n"
    "\n"
    "Usage: embroidermodder [options] files ...\n"
    "\n"
    "Options:\n"
    "  -d, --debug      Print lots of debugging information.\n"
    "  -h, --help       Print this message and exit.\n"
    "  -v, --version    Print the version number of embroidermodder and exit.\n"
    "\n";

/*  . */
const char *menubar_order[] = {
    "file",
    "edit",
    "view",
    "draw",
    "settings",
    "window",
    "help",
    "END"
};

/*  . */
const char *top_toolbar_layout[] = {
    "---",
    "file",
    "edit",
    "help",
    "icon",
    "---",
    "zoom",
    "pan",
    "view",
    "---",
    "layer",
    "properties",
    "---",
    "text",
    "END"
};

/*  . */
const char *bottom_toolbar_layout[] = {
    "prompt",
    "END"
};

/*  . */
const char *side_toolbar_layout[] = {
    "draw",
    "END"
};

const char *file_menu[] = {
	"new",
	"---",
	"open",
	"submenu recent",
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
};

const char *edit_menu[] = {
	"undo",
	"redo",
	"---",
	"cut",
	"copy",
	"paste",
	"---",
	"END"
};

const char *pan_menu[] = {
	"icon pan",
	"panrealtime",
	"panpoint",
	"---",
	"panleft",
	"panright",
	"panup",
	"pandown",
	"END"
};

const char *zoom_menu[] = {
	"icon zoom",
	"zoomrealtime",
	"zoomprevious",
	"---",
	"zoomwindow",
	"zoomdynamic",
	"zoomscale",
	"zoomcenter",
	"---",
	"zoomin",
	"zoomout",
	"---",
	"zoomselected",
	"zoomall",
	"zoomextents",
	"END"
};

const char *view_menu[] = {
	"---",
	"submenu zoom",
	"submenu pan",
	"---",
	"day",
	"night",
	"---",
	"END"
};

const char *settings_menu[] = {
	"settingsdialog",
	"---",
	"END"
};

const char *window_menu[] = {
	"---",
	"END"
};

const char *help_menu[] = {
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
};

const char *draw_menu[] = {
	"circle",
	"path",
	"polygon",
	"polyline",
	"point",
	"heart",
	"single-line-text",
	"END"
};

const char *toolbar_list[] = {
    "file", "fileToolbar",
    "edit", "editToolbar",
    "view", "viewToolbar",
    "zoom", "zoomToolbar",
    "pan", "panToolbar",
    "icon", "iconToolbar",
    "help", "helpToolbar",
    "draw", "drawToolbar",
    "END"
};

const char *file_toolbar[] = {
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
};

const char *edit_toolbar[] = {
	"cut",
	"copy",
	"paste",
	"END"
};

const char *view_toolbar[] = {
	"day",
	"night",
	"END"
};

const char *zoom_toolbar[] = {
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
};

const char *pan_toolbar[] = {
	"panrealtime",
	"panpoint",
	"---",
	"panleft",
	"panright",
	"panup",
	"pandown",
	"END"
};

const char *icon_toolbar[] = {
	"icon16",
	"icon24",
	"icon32",
	"icon48",
	"icon64",
	"icon128",
	"END"
};

const char *help_toolbar[] = {
	"help",
	"---",
	"changelog",
	"---",
	"about",
	"---",
	"whatsthis",
	"END"
};

const char *draw_toolbar[] = {
	"circle",
	"path",
	"polygon",
	"polyline",
	"point",
	"heart",
	"single-line-text",
	"END"
};

/* TODO: "Aligned"
 * TODO: "Fit"
 */
const char *justify_options[] = {
    "Left",
    "Center",
    "Right",
    "Aligned",
    "Middle",
    "Fit",
    "Top Left",
    "Top Center",
    "Top Right",
    "Middle Left",
    "Middle Center",
    "Middle Right",
    "Bottom Left",
    "Bottom Center",
    "Bottom Right",
    "END"
};

/* The group box list is not changeable at runtime, so it's fixed length
 * and a constant.
 */
const char *group_box_types[] = {
    "general",
    "geometry_arc",
    "misc_arc",
    "geometry_block",
    "geometry_circle",
    "geometry_dim_aligned",
    "geometry_dim_angular",
    "geometry_dim_arc_length",
    "geometry_dim_diameter",
    "geometry_dim_leader",
    "geometry_dim_linear",
    "geometry_dim_ordinate",
    "geometry_dim_radius",
    "geometry_ellipse",
    "geometry_image",
    "misc_image",
    "geometry_infinite_line",
    "geometry_line",
    "geometry_path",
    "misc_path",
    "geometry_point",
    "geometry_polygon",
    "geometry_polyline",
    "misc_polyline",
    "geometry_ray",
    "geometry_rectangle",
    "geometry_text_multi",
    "text_text_single",
    "geometry_text_single",
    "misc_text_single",
    "END"
};

const int32_t group_box_ids[] = {
    OBJ_TYPE_NULL,
    OBJ_TYPE_ARC,
    OBJ_TYPE_ARC,
    OBJ_TYPE_BLOCK,
    OBJ_TYPE_CIRCLE,
    OBJ_TYPE_DIMALIGNED,
    OBJ_TYPE_DIMANGULAR,
    OBJ_TYPE_DIMARCLENGTH,
    OBJ_TYPE_DIMDIAMETER,
    OBJ_TYPE_DIMLEADER,
    OBJ_TYPE_DIMLINEAR,
    OBJ_TYPE_DIMORDINATE,
    OBJ_TYPE_DIMRADIUS,
    OBJ_TYPE_ELLIPSE,
    OBJ_TYPE_IMAGE,
    OBJ_TYPE_IMAGE,
    OBJ_TYPE_INFINITELINE,
    OBJ_TYPE_LINE,
    OBJ_TYPE_PATH,
    OBJ_TYPE_PATH,
    OBJ_TYPE_POINT,
    OBJ_TYPE_POLYGON,
    OBJ_TYPE_POLYLINE,
    OBJ_TYPE_POLYLINE,
    OBJ_TYPE_RAY,
    OBJ_TYPE_RECTANGLE,
    OBJ_TYPE_TEXTMULTI,
    OBJ_TYPE_TEXTSINGLE,
    OBJ_TYPE_TEXTSINGLE,
    OBJ_TYPE_TEXTSINGLE
};

/* . */
const char *button_list[] = {
    "SNAP",
    "GRID",
    "RULER",
    "ORTHO",
    "POLAR",
    "QSNAP",
    "QTRACK",
    "LWT",
    "END"
};

/* . */
const char *command_labels[] = {
    "about",                        /* 0 */
    "add_arc",                      /* 1 */
    "add_circle",                   /* 2 */
    "add_dim_leader",               /* 3 */
    "add_ellipse",                  /* 4 */
    "add",                          /* 5 */
    "add_horizontal_dimension",     /* 6 */
    "add_image",                    /* 7 */
    "add_infinite_line",            /* 8 */
    "add_line",                     /* 9 */
    "add_path",                     /* 10 */
    "add_point",                    /* 11 */
    "add_polygon",                  /* 12 */
    "add_polyline",                 /* 13 */
    "add_ray",                      /* 14 */
    "add_rectangle",                /* 15 */
    "add_regular_polygon",          /* 16 */
    "add_rounded_rectangle",        /* 17 */
    "add_rubber",                   /* 18 */
    "add_slot",                     /* 19 */
    "add_text_multi",               /* 20 */
    "add_text_single",              /* 21 */
    "add_to_selection",             /* 22 */
    "add_triangle",                 /* 23 */
    "add_vertical_dimension",       /* 24 */
    "alert",                        /* 25 */
    "allow_rubber",                 /* 26 */
    "append_history",               /* 27 */
    "calculate_angle",              /* 28 */
    "calculate_distance",           /* 29 */
    "changelog",                    /* 30 */
    "clear_rubber",                 /* 31 */
    "clear_selection",              /* 32 */
    "copy",                         /* 33 */
    "copy_selected",                /* 34 */
    "cut",                          /* 35 */
    "cut_selected",                 /* 36 */
    "day_vision",                   /* 37 */
    "debug",                        /* 38 */
    "delete_selected",              /* 39 */
    "design_details",               /* 40 */
    "do_nothing",                   /* 41 */
    "end",                          /* 42 */
    "error",                        /* 43 */
    "help",                         /* 44 */
    "icon",                         /* 45 */
    "init",                         /* 46 */
    "messagebox",                   /* 47 */
    "mirror_selected",              /* 48 */
    "mouse_x",                      /* 49 */
    "mouse_y",                      /* 50 */
    "move_selected",                /* 51 */
    "new",                          /* 52 */
    "night_vision",                 /* 53 */
    "num_selected",                 /* 54 */
    "open",                         /* 55 */
    "pan",                          /* 56 */
    "paste",                        /* 57 */
    "paste_selected",               /* 58 */
    "perpendicular_distance",       /* 59 */
    "platform",                     /* 60 */
    "preview_off",                  /* 61 */
    "preview_on",                   /* 62 */
    "print",                        /* 63 */
    "print_area",                   /* 64 */
    "qsnap_x",                      /* 65 */
    "qsnap_y",                      /* 66 */
    "quit",                         /* 67 */
    "redo",                         /* 68 */
    "rotate_selected",              /* 69 */
    "rubber",                       /* 70 */
    "scale_selected",               /* 71 */
    "select_all",                   /* 72 */
    "settings_dialog",              /* 73 */
    "set_background_color",         /* 74 */
    "set_crosshair_color",          /* 75 */
    "set_cursor_shape",             /* 76 */
    "set_grid_color",               /* 77 */
    "set_prompt_prefix",            /* 78 */
    "set_rubber_filter",            /* 79 */
    "set_rubber_mode",              /* 80 */
    "set_rubber_point",             /* 81 */
    "set_rubber_text",              /* 82 */
    "spare_rubber",                 /* 83 */
    "tip_of_the_day",               /* 84 */
    "todo",                         /* 85 */
    "undo",                         /* 86 */
    "version",                      /* 87 */
    "vulcanize",                    /* 88 */
    "whats_this",                   /* 89 */
    "window",                       /* 90 */
    "zoom"                          /* 91 */
};

char rubber_modes[N_RUBBER_MODES][MAX_STRING_LENGTH] = {
    "CIRCLE_1P_RAD",
    "CIRCLE_1P_DIA",
    "CIRCLE_2P",
    "CIRCLE_3P",
    "CIRCLE_TTR",
    "CIRCLE_TTT",
    "DIMLEADER_LINE",
    "ELLIPSE_LINE",
    "ELLIPSE_MAJORDIAMETER_MINORRADIUS",
    "ELLIPSE_MAJORRADIUS_MINORRADIUS",
    "ELLIPSE_ROTATION",
    "LINE",
    "POLYGON",
    "POLYGON_INSCRIBE",
    "POLYGON_CIRCUMSCRIBE",
    "POLYLINE",
    "RECTANGLE",
    "TEXTSINGLE",
    "END"
};

/* . */
const char *all_line_editors[] = {
    "general", "general_layer", "blank", "Layer", "combobox", "false",
    "general", "general_color", "blank", "Color", "combobox", "false",
    "general", "general_line_type", "blank", "LineType", "combobox", "false",
    "general", "general_line_weight", "blank", "LineWeight", "combobox", "false",

    "geometry_arc", "arc_center_x", "blank", "Center X", "double", "lineEditArcCenterX",
    "geometry_arc", "arc_center_y", "blank", "Center Y", "double", "lineEditArcCenterY",
    "geometry_arc", "arc_radius", "blank", "Radius", "double", "lineEditArcRadius",
    "geometry_arc", "arc_start_angle", "blank", "Start Angle", "double", "lineEditArcStartAngle",
    "geometry_arc", "arc_end_angle", "blank", "End Angle", "double", "lineEditArcEndAngle",
    "geometry_arc", "arc_start_x", "blank", "Start X", "double", "",
    "geometry_arc", "arc_start_y", "blank", "Start Y", "double", "",
    "geometry_arc", "arc_end_x", "blank", "End X", "double", "",
    "geometry_arc", "arc_end_y", "blank", "End Y", "double", "",
    "geometry_arc", "arc_area", "blank", "Area", "double", "",
    "geometry_arc", "arc_length", "blank", "Arc Length", "double", "",
    "geometry_arc", "arc_chord", "blank", "Chord", "double", "",
    "geometry_arc", "arc_inc_angle", "blank", "Included Angle", "double",  "",

    "misc_arc", "arc-clockwise", "blank", "Clockwise", "combobox", "true",

    "geometry_block", "block-x", "blank", "Position X", "double", "",
    "geometry_block", "block-y", "blank", "Position Y", "double", "",

    "geometry_circle", "circle_center_x", "blank", "Center X", "double", "lineEditCircleCenterX",
    "geometry_circle", "circle_center_y", "blank", "Center Y", "double", "lineEditCircleCenterY",
    "geometry_circle", "circle_radius", "blank", "Radius", "double", "lineEditCircleRadius",
    "geometry_circle", "circle_diameter", "blank", "Diameter", "double", "lineEditCircleDiameter",
    "geometry_circle", "circle_area", "blank", "Area", "double", "lineEditCircleArea",
    "geometry_circle", "circle_circumference", "blank", "Circumference", "double", "lineEditCircleCircumference",

    "geometry_dim_aligned", "dim-aligned-x", "blank", "Position X", "double", "",

    "geometry_dim_angular", "dim-angular-x", "blank", "Position X", "double", "",

    "geometry_dim_arc_length", "dim-arc-length-x", "blank", "Position X", "double", "",

    "geometry_dim_diameter", "dim-diameter-x", "blank", "Position X", "double", "",

    "geometry_dim_leader", "dim-linear-x", "blank", "Position X", "double", "",

    "geometry_dim_linear", "dim-linear-x", "blank", "Position X", "double", "",

    "geometry_dim_ordinate", "dim-ordinate-x", "blank", "Position X", "double", "",

    "geometry_dim_radius", "dim-radius-x", "blank", "Position X", "double", "",

    "geometry_ellipse", "ellipse_center_x", "blank", "Center X", "double", "lineEditEllipseCenterX",
    "geometry_ellipse", "ellipse_center_y", "blank", "Center Y", "double", "lineEditEllipseCenterY",
    "geometry_ellipse", "ellipse_semi_major_axis", "blank", "Semi-Major Axis", "double", "lineEditEllipseSemiMajorAxis",
    "geometry_ellipse", "ellipse_semi_minor_axis", "blank", "Semi-Minor Axis", "double", "lineEditEllipseSemiMinorAxis",
    "geometry_ellipse", "ellipse_major_axis", "blank", "Major Axis", "double", "lineEditEllipseMajorAxis",
    "geometry_ellipse", "ellipse_minor_axis", "blank", "Minor Axis", "double", "lineEditEllipseMinorAxis",

    "geometry_image", "image-x", "blank", "Position X", "double", "",
    "geometry_image", "image-y", "blank", "Position Y", "double", "",
    "geometry_image", "image-width", "blank", "Width", "double", "",
    "geometry_image", "image-height", "blank", "Height", "double", "",

    "misc_image", "image-name", "blank", "Name", "double", "",
    "misc_image", "image-path", "blank", "Path", "double", "",

    "geometry_infinite_line", "infinite-line-x1", "blank", "Start X", "double", "lineEditInfiniteLineX1",
    "geometry_infinite_line", "infinite-line-y1", "blank", "Start Y", "double", "lineEditInfiniteLineY1",
    "geometry_infinite_line", "infinite-line-x2", "blank", "2nd X", "double", "lineEditInfiniteLineX2",
    "geometry_infinite_line", "infinite-line-y2", "blank", "2nd Y", "double", "lineEditInfiniteLineY2",
    "geometry_infinite_line", "infinite-line-vector-x", "blank", "Vector X", "double", "",
    "geometry_infinite_line", "infinite-line-vector-y", "blank", "Vector Y", "double", "",

    "geometry_line", "line-start-x", "blank", "Start X", "double", "lineEditLineStartX",
    "geometry_line", "line-start-y", "blank", "Start Y", "double", "lineEditLineStartY",
    "geometry_line", "line-end-x", "blank", "End X", "double", "lineEditLineEndX",
    "geometry_line", "line-end-y", "blank", "End Y", "double", "lineEditLineEndY",
    "geometry_line", "line-delta-x", "blank", "Delta X", "double", "",
    "geometry_line", "line-delta-y", "blank", "Delta Y", "double", "",
    "geometry_line", "line-angle", "blank", "Angle", "double", "",
    "geometry_line", "line-length", "blank", "Length", "double", "",

    "geometry_path", "path_vertex_num", "blank", "Vertex #", "combobox", "",
    "geometry_path", "path_vertex_x", "blank", "Vertex X", "double", "",
    "geometry_path", "path_vertex_y", "blank", "Vertex Y", "double", "",
    "geometry_path", "path_area", "blank", "Area", "double", "",
    "geometry_path", "path_length", "blank", "Length", "double", "",

    "misc_path", "path_closed", "blank", "Closed", "combobox", "",

    "geometry_point", "point_x", "blank", "Position X", "double", "lineEditPointX",
    "geometry_point", "point_y", "blank", "Position Y", "double", "lineEditPointY",

    "geometry_polygon", "polygon_center_x", "blank", "Center X", "double", "",
    "geometry_polygon", "polygon_center_y", "blank", "Center Y", "double", "",
    "geometry_polygon", "polygon-radius-vertex", "blank", "Vertex Radius", "double", "",
    "geometry_polygon", "polygon-radius-side", "blank", "Side Radius", "double", "",
    "geometry_polygon", "polygon-diameter-vertex", "blank", "Vertex Diameter", "double", "",
    "geometry_polygon", "polygon-diameter-side", "blank", "Side Diameter", "double", "",
    "geometry_polygon", "polygon-interior-angle", "blank", "Interior Angle", "double", "",

    "geometry_polyline", "polyline-vertex-num", "blank", "Vertex #", "combobox", "false",
    "geometry_polyline", "polyline-vertex-x", "blank", "Vertex X", "double", "",
    "geometry_polyline", "polyline-vertex-y", "blank", "Vertex Y", "double", "",
    "geometry_polyline", "polyline-vertex-area", "blank", "Area", "double", "",
    "geometry_polyline", "polyline-vertex-length", "blank", "Length", "double", "",

    "misc_polyline", "polyline-vertex-closed", "blank", "Closed", "combobox", "false",

    "geometry_ray", "ray-x1", "blank", "Start X", "double", "",
    "geometry_ray", "ray-y1", "blank", "Start Y", "double", "",
    "geometry_ray", "ray-x2", "blank", "2nd X", "double", "",
    "geometry_ray", "ray-y2", "blank", "2nd Y", "double", "",
    "geometry_ray", "ray-vector-x", "blank", "Vector X", "double", "",
    "geometry_ray", "ray-vector-y", "blank", "Vector Y", "double", "",

    "geometry_rectangle", "rectangle_corner1_x", "blank", "Corner 1 X", "double", "",
    "geometry_rectangle", "rectangle_corner1_y", "blank", "Corner 1 Y", "double", "",
    "geometry_rectangle", "rectangle_corner2_x", "blank", "Corner 2 X", "double", "",
    "geometry_rectangle", "rectangle_corner2_y", "blank", "Corner 2 Y", "double", "",
    "geometry_rectangle", "rectangle_corner3_x", "blank", "Corner 3 X", "double", "",
    "geometry_rectangle", "rectangle_corner3_y", "blank", "Corner 3 Y", "double", "",
    "geometry_rectangle", "rectangle_corner4_x", "blank", "Corner 4 X", "double", "",
    "geometry_rectangle", "rectangle_corner4_y", "blank", "Corner 4 Y", "double", "",
    "geometry_rectangle", "rectangle_width", "blank", "Width", "double", "",
    "geometry_rectangle", "rectangle_height", "blank", "Height", "double", "",
    "geometry_rectangle", "rectangle_area", "blank", "Area", "double", "",

    "text_text_single", "text-single-contents", "blank", "Contents", "string", "lineEditTextSingleContents",
    "text_text_single", "text-single-font", "blank", "Font", "fontcombobox", "comboBoxTextSingleFont",
    "text_text_single", "text-single-justify", "blank", "Justify", "combobox", "comboBoxTextSingleJustify",
    "text_text_single", "text-single-height", "blank", "Height", "double", "lineEditTextSingleHeight",
    "text_text_single", "text-single-rotation", "blank", "Rotation", "double", "lineEditTextSingleRotation",

    "geometry_text_single", "text-single-x", "blank", "Position X", "double", "lineEditTextSingleX",
    "geometry_text_single", "text_single_y", "blank", "Position Y", "double", "lineEditTextSingleY",

    "misc_text_single", "text_single_backward", "blank", "Backward", "combobox", "comboBoxTextSingleBackward",
    "misc_text_single", "text_single_upside_down", "blank", "UpsideDown", "combobox", "comboBoxTextSingleUpsideDown",

    "geometry_text_multi", "text-multi-x", "blank", "Position X", "double", "",
    "geometry_text_multi", "text-multi-y", "blank", "Position Y", "double", "",
    "END"
};

const char *tips[] = {
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
    "that you can quickly change the background, crosshair and grid colors using the 'RGB' command?"
};

/* Check that RBG values are in the range (0,255) inclusive. */
unsigned char
validRGB(int r, int g, int b)
{
    unsigned char result = (r>=0);
    result &= (r<256);
    result &= (g>=0);
    result &= (g<256);
    result &= (b>=0);
    result &= (b<256);
    return result;
}

/*
 */
CNode *
create_node(int type)
{
    CNode *new_node = (CNode*)malloc(sizeof(CNode));
    new_node->n_leaves = 0;
    new_node->max_leaves = 10;
    new_node->leaves = (CNode**)malloc(new_node->max_leaves * sizeof(CNode *));
    new_node->type = type;
    return new_node;
}

/*
 *
 */
int
add_leaf(CNode *branch, CNode *leaf)
{
    if (branch->n_leaves >= branch->max_leaves) {
        branch->max_leaves += 100;
        branch->leaves = realloc(branch->leaves,
            branch->max_leaves*sizeof(CNode*));
    }
    branch->leaves[branch->n_leaves] = leaf;
    branch->n_leaves++;
    return 0;
}

/*
 * We traverse each level from highest to lowest index so at each stage the
 * readout of n_leaves remains correct.
 *
 * To remove_leaf, just call this on a leaf node rather than a larger branch.
 */
void
free_node(CNode *branch)
{
    if (branch->n_leaves > 0) {
        branch->n_leaves--;
        free_node(branch->leaves[branch->n_leaves]);
    }
    else {
        free(branch);
    }
}

/*
 *
 */
int
str_contains(char *s, char c)
{
    int i;
    for (i=0; s[i]; i++) {
        if (s[i] == c) {
            return i;
        }
    }
    return -1;
}

/*
 * TODO: use the C dot notation for traversing down the branches of a tree.
 *
 * For example: (root, "setting.font_size")
 *
 * In Python notation this would be represented as:
 *     root["setting"]["font_size"]
 *
 */
CNode *
find_node(CNode *branch, char key[MAX_STRING_LENGTH])
{
    if (branch->type == CNODE_TYPE_DICTIONARY) {
        int pos = str_contains(key, '.');
        if (pos >= 0) {
            int i;
            char working_copy[2*MAX_STRING_LENGTH];
            strncpy(working_copy, key, MAX_STRING_LENGTH);
            working_copy[pos] = 0;
            for (i=0; i<branch->n_leaves; i++) {
                if (!strncmp(working_copy, branch->leaves[i]->key, MAX_STRING_LENGTH)) {
                    return find_node(branch->leaves[i], working_copy+pos+1);
                }
            }
        }
        else {
            int i;
            for (i=0; i<branch->n_leaves; i++) {
                if (!strncmp(key, branch->leaves[i]->key, MAX_STRING_LENGTH)) {
                    return branch->leaves[i];
                }
            }
        }
    }
    else {
        /* debug_message("ERROR: cannot search for keys in a non-dictionary type."); */
    }
    return NULL;
}

/*
 * FIXME
 */
int
insert_node(CNode *branch, char key[MAX_STRING_LENGTH], CNode *node)
{
    if (branch->type == CNODE_TYPE_DICTIONARY) {
        int pos = str_contains(key, '.');
        if (pos >= 0) {
            int i;
            char working_copy[2*MAX_STRING_LENGTH];
            strncpy(working_copy, key, MAX_STRING_LENGTH);
            working_copy[pos] = 0;
            for (i=0; i<branch->n_leaves; i++) {
                if (!strncmp(working_copy, branch->leaves[i]->key, MAX_STRING_LENGTH)) {
                    insert_node(branch->leaves[i], working_copy+pos+1, node);
                }
            }
        }
        else {
            add_leaf(branch, node);
            return 1;
        }
    }
    else {
        /* debug_message("ERROR: cannot search for keys in a non-dictionary type."); */
    }
    return 0;
}


/*
 * Print out all the data for the tree for debugging.
 */
void
print_tree(CNode *branch, int indent)
{
    int j;
    for (j=0; j<indent; j++) {
        printf("|");
    }
    switch (branch->type) {
    case CNODE_TYPE_STRING: {
        printf("-%s: %s\n", branch->key, branch->data);
        break;
    }
    case CNODE_TYPE_DICTIONARY: {
        int i;
        printf("-dictionary %s\n", branch->key);
        for (i=0; i<branch->n_leaves; i++) {
            print_tree(branch->leaves[i], indent+1);
        }
        break;
    }
    default: {
        printf("-UNKNOWN\n");
        break;
    }
    }
}

/* CNode needs to check for duplicate keys.
 */
CNode *
create_and_add_leaf(CNode *parent, char *key, char *value)
{
    CNode *n = (CNode*)malloc(sizeof(CNode));
    n->n_leaves = 0;
    n->max_leaves = 10;
    n->leaves = (CNode**)malloc(n->max_leaves * sizeof(CNode *));
    if (!strcmp(value, "{}")) {
        n->type = CNODE_TYPE_DICTIONARY;
    }
    else {
        n->type = CNODE_TYPE_STRING;
    }
    strncpy(n->key, key, MAX_STRING_LENGTH);
    strncpy(n->data, value, MAX_STRING_LENGTH);
    add_leaf(parent, n);
    return n;
}

/*
 */
Cvector *
cvector_create(size_t element_size)
{
    Cvector *a = malloc(sizeof(Cvector));
    a->size = 0;
    a->max_length = VECTOR_CHUNK_SIZE;
    a->element_size = element_size;
    a->buffer = (void *) malloc(a->element_size * a->size);
    return a;
}

/*
 */
void
cvector_append(Cvector *a, Cvector *b)
{
    if (a->element_size != b->element_size) {
        puts("ERROR: element_size mismatch.");
        return;
    }
    if (a->max_length < a->size + b->size) {
        a->max_length += VECTOR_CHUNK_SIZE;
        a->buffer = (void *)realloc(a->buffer,
            a->max_length * sizeof(a->element_size));
    }
    memcpy(a->buffer + a->element_size * a->size, b->buffer,
        a->element_size*b->size);
}

/*
 */
void
cvector_add_cstr(Cvector *a, char *b)
{
    Cvector c;
    c.buffer = b;
    c.element_size = 1;
    c.size = strlen(b) + 1;
    c.max_length = c.size;
    cvector_append(a, &c);
}

/*
 */
Cvector *
cvector_copy(Cvector *a)
{
    Cvector *c = cvector_create(a->element_size);
    cvector_append(c, a);
    return c;
}

/*
 */
void
cvector_free(Cvector *vector)
{
    free(vector->buffer);
    free(vector);
}

/* . */
int
string_array_length(const char *list[])
{
    int i = 0;
    for (i=0; strcmp(list[i], "END"); i++) {}
	return i;
}
