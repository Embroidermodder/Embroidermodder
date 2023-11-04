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
#include <limits.h>

#include "core.h"

/* See the "Programming principles for the C core" in the reference manual
 * before changing this file.
 */

const char *version = "2.0.0-alpha4";

/* So we can find any given piece of data in our UI for the functions to act
 * on they all lie in a tree that uses C-style tree walking during runtime.
 */
Node *root;

/* The actuator changes the program state via these global variables.
 *
 * These copies of the settings struct are for restoring the state if
 * the user doesn't want to accept their changes in the settings dialog.
 */
Node settings[SETTINGS_TOTAL], dialog[SETTINGS_TOTAL],
    preview[SETTINGS_TOTAL], accept_[SETTINGS_TOTAL];

/* . */
const char *object_names[] = {
    "Base",
    "Arc",
    "Block",
    "Circle",
    "Aligned Dimension",
    "Angular Dimension",
    "Arclength Dimension",
    "Diameter Dimension",
    "Leader Dimension",
    "Linear Dimension",
    "Ordinate Dimension",
    "Radius Dimension",
    "Ellipse",
    "Image",
    "Infinite Line",
    "Line",
    "Path",
    "Point",
    "Polygon",
    "Polyline",
    "Ray",
    "Rectangle",
    "Multiline Text",
    "Text",
    "Unknown",
    "END"
};

/* . */
const char rectangle_main_script[][MAX_STRING_LENGTH] = {
    "init",
    "clear-selection",
    "newRect = true",
    "real x1 = 0.0f",
    "real y1 = 0.0f",
    "real x2 = 0.0f",
    "real y2 = 0.0f",
    "set-prompt-prefix-tr Specify first corner point or [Chamfer/Fillet]: "
};

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

/* . */
const char *alias_table[] = {
    "exit", "quit",
    "u", "undo",
    "close", "windowclose",
    "closeall", "windowcloseall",
    "cascade", "windowcascade"
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
int32_t menubar_order[] = {
    MENU_FILE,
    MENU_EDIT,
    MENU_VIEW,
    MENU_DRAW,
    MENU_SETTINGS,
    MENU_WINDOW,
    MENU_HELP,
    -1
};

/*  . */
int32_t top_toolbar_layout[] = {
    TOOLBAR_SEPERATOR,
    TOOLBAR_FILE,
    TOOLBAR_EDIT,
    TOOLBAR_HELP,
    TOOLBAR_ICON,
    TOOLBAR_SEPERATOR,
    TOOLBAR_ZOOM,
    TOOLBAR_PAN,
    TOOLBAR_VIEW,
    TOOLBAR_SEPERATOR,
    TOOLBAR_LAYER,
    TOOLBAR_PROPERTIES,
    TOOLBAR_SEPERATOR,
    TOOLBAR_TEXT,
    TOOLBAR_END
};

/*  . */
int32_t bottom_toolbar_layout[] = {
    TOOLBAR_PROMPT,
    TOOLBAR_END
};

/*  . */
int32_t side_toolbar_layout[] = {
    TOOLBAR_DRAW,
    TOOLBAR_END
};

MenuData menu_data[MAX_MENUS];

int32_t file_menu[] = {
    ACTION_NEW,
    MENU_SEPERATOR,
    ACTION_OPEN,
    MENU_SUBMENU, MENU_RECENT,
    MENU_SEPERATOR,
    ACTION_SAVE,
    ACTION_SAVEAS,
    MENU_SEPERATOR,
    ACTION_PRINT,
    MENU_SEPERATOR,
    ACTION_WINDOW_CLOSE,
    MENU_SEPERATOR,
    ACTION_DESIGN_DETAILS,
    MENU_SEPERATOR,
    ACTION_EXIT,
    MENU_END
};

int32_t edit_menu[] = {
    ACTION_UNDO,
    ACTION_REDO,
    MENU_SEPERATOR,
    ACTION_CUT,
    ACTION_COPY,
    ACTION_PASTE,
    MENU_SEPERATOR,
    MENU_END
};

int32_t pan_menu[] = {
    MENU_ICON,
    ACTION_PAN,
    ACTION_PAN_REAL_TIME,
    ACTION_PAN_POINT,
    MENU_SEPERATOR,
    ACTION_PAN_LEFT,
    ACTION_PAN_RIGHT,
    ACTION_PAN_UP,
    ACTION_PAN_DOWN,
    MENU_END
};

int32_t zoom_menu[] = {
    MENU_ICON,
    ACTION_ZOOM,
    ACTION_ZOOM_REAL_TIME,
    ACTION_ZOOM_PREVIOUS,
    MENU_SEPERATOR,
    ACTION_ZOOM_WINDOW,
    ACTION_ZOOM_DYNAMIC,
    ACTION_ZOOM_SCALE,
    ACTION_ZOOM_CENTER,
    MENU_SEPERATOR,
    ACTION_ZOOM_IN,
    ACTION_ZOOM_OUT,
    MENU_SEPERATOR,
    ACTION_ZOOM_SELECTED,
    ACTION_ZOOM_ALL,
    ACTION_ZOOM_EXTENTS,
    MENU_END
};

int32_t view_menu[] = {
    MENU_SEPERATOR,
    MENU_SUBMENU, MENU_ZOOM,
    MENU_SUBMENU, MENU_PAN,
    MENU_SEPERATOR,
    ACTION_DAY,
    ACTION_NIGHT,
    MENU_SEPERATOR,
    MENU_END
};

int32_t settings_menu[] = {
    ACTION_SETTINGS_DIALOG,
    MENU_SEPERATOR,
    ACTION_TEST,
    MENU_END
};

int32_t window_menu[] = {
    MENU_SEPERATOR,
    MENU_END
};

int32_t help_menu[] = {
    ACTION_HELP,
    MENU_SEPERATOR,
    ACTION_CHANGELOG,
    MENU_SEPERATOR,
    ACTION_TIP_OF_THE_DAY,
    MENU_SEPERATOR,
    ACTION_ABOUT,
    MENU_SEPERATOR,
    ACTION_WHATS_THIS,
    MENU_END
};

int32_t draw_menu[] = {
    ACTION_ADD_CIRCLE,
    ACTION_ADD_PATH,
    ACTION_ADD_POLYGON,
    ACTION_ADD_POLYLINE,
    ACTION_ADD_POINT,
    ACTION_ADD_HEART,
    ACTION_ADD_SINGLE_LINE_TEXT,
    MENU_END
};

ToolbarData toolbar_data[MAX_TOOLBARS] = {
    {
        .id = TOOLBAR_FILE,
        .key = "File",
        .entries = {
            ACTION_NEW,
            ACTION_OPEN,
            ACTION_SAVE,
            ACTION_SAVEAS,
            ACTION_PRINT,
            ACTION_DESIGN_DETAILS,
            TOOLBAR_SEPERATOR,
            ACTION_UNDO,
            ACTION_REDO,
            TOOLBAR_SEPERATOR,
            ACTION_HELP,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_EDIT,
        .key = "Edit",
        .entries = {
            ACTION_CUT,
            ACTION_COPY,
            ACTION_PASTE,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_PAN,
        .key = "Pan",
        .entries = {
            ACTION_PAN_REAL_TIME,
            ACTION_PAN_POINT,
            TOOLBAR_SEPERATOR,
            ACTION_PAN_LEFT,
            ACTION_PAN_RIGHT,
            ACTION_PAN_UP,
            ACTION_PAN_DOWN,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_ZOOM,
        .key = "Zoom",
        .entries = {
            ACTION_ZOOM_WINDOW,
            ACTION_ZOOM_DYNAMIC,
            ACTION_ZOOM_SCALE,
            TOOLBAR_SEPERATOR,
            ACTION_ZOOM_CENTER,
            ACTION_ZOOM_IN,
            ACTION_ZOOM_OUT,
            TOOLBAR_SEPERATOR,
            ACTION_ZOOM_SELECTED,
            ACTION_ZOOM_ALL,
            ACTION_ZOOM_EXTENTS,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_VIEW,
        .key = "View",
        .entries = {
            ACTION_DAY,
            ACTION_NIGHT,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_SETTINGS,
        .key = "Settings",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_WINDOW,
        .key = "Window",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_HELP,
        .key = "Help",
        .entries = {
            ACTION_HELP,
            TOOLBAR_SEPERATOR,
            ACTION_CHANGELOG,
            TOOLBAR_SEPERATOR,
            ACTION_ABOUT,
            TOOLBAR_SEPERATOR,
            ACTION_WHATS_THIS,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_DRAW,
        .key = "Draw",
        .entries = {
            ACTION_ADD_CIRCLE,
            ACTION_ADD_PATH,
            ACTION_ADD_POLYGON,
            ACTION_ADD_POLYLINE,
            ACTION_ADD_POINT,
            ACTION_ADD_HEART,
            ACTION_ADD_SINGLE_LINE_TEXT,
            TOOLBAR_END
        },
        .horizontal = 0
    },
    {
        .id = TOOLBAR_ICON,
        .key = "Icon",
        .entries = {
            ACTION_ICON16,
            ACTION_ICON24,
            ACTION_ICON32,
            ACTION_ICON48,
            ACTION_ICON64,
            ACTION_ICON128,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_LAYER,
        .key = "Layer",
        .entries = {
            ACTION_MAKE_LAYER_CURRENT,
            ACTION_LAYER_EDITOR,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_PROPERTIES,
        .key = "Properties",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_TEXT,
        .key = "Text",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_PROMPT,
        .key = "Command Prompt",
        .entries = {
            TOOLBAR_END
        },
        .horizontal = 1
    }
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

const char *group_box_data[] = {
    "general", "General",
    "geometry_arc", "Geometry",
    "misc_arc", "Misc",
    "geometry_block", "Geometry",
    "geometry_circle", "Geometry",
    "geometry_dim_aligned", "Geometry",
    "geometry_dim_angular", "Geometry",
    "geometry_dim_arc_length", "Geometry",
    "geometry_dim_diameter", "Geometry",
    "geometry_dim_leader", "Geometry",
    "geometry_dim_linear", "Geometry",
    "geometry_dim_ordinate", "Geometry",
    "geometry_dim_radius", "Geometry",
    "geometry_ellipse", "Geometry",
    "geometry_image", "Geometry",
    "misc_image", "Misc",
    "geometry_infinite_line", "Geometry",
    "geometry_line", "Geometry",
    "geometry_path", "Geometry",
    "misc_path", "Misc",
    "geometry_point", "Geometry",
    "geometry_polygon", "Geometry",
    "geometry_polyline", "Geometry",
    "misc_polyline", "Misc",
    "geometry_ray", "Geometry",
    "geometry_rectangle", "Geometry",
    "geometry_text_multi", "Geometry",
    "text_text_single", "Text",
    "geometry_text_single", "Geometry",
    "misc_text_single", "Misc",
    "END", "END"
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
    "zoom",                         /* 91 */
    "test",                         /* 92 */
    "sleep"                         /* 93 */
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
    "that you can quickly change the background, crosshair and grid colors using the 'RGB' command?",
    "END"
};

const char types[][MAX_STRING_LENGTH] = {
    "NULL",
    "STRING",
    "STRING_LIST",
    "REAL",
    "INT",
    "BOOL",
    "FUNCTION",
    "VECTOR",
    "UNKNOWN"
};

/**
 * .
 */
const char *extensions[] = {
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

int general_props[] = {
    ST_ICON_THEME,
    ST_ICON_SIZE,
    ST_MDI_USE_COLOR,
    ST_MDI_USE_LOGO,
    ST_MDI_USE_TEXTURE,
    ST_MDI_COLOR,
    ST_MDI_LOGO,
    ST_MDI_TEXTURE,
    ST_TIP_OF_THE_DAY,
    -1
};

int display_props[] = {
    ST_USE_OPENGL,
    ST_ANTI_ALIAS,
    ST_TEXT_ANTI_ALIAS,
    ST_SMOOTH_PIXMAP,
    ST_HQ_ANTI_ALIAS,
    ST_NON_COSMETIC,
    ST_SHOW_SCROLLBARS,
    ST_SCROLLBAR_WIDGET_NUM,
    ST_CROSSHAIR_COLOR,
    ST_BG_COLOR,
    ST_SELECTBOX_LEFT_COLOR,
    ST_SELECTBOX_LEFT_FILL,
    ST_SELECTBOX_RIGHT_COLOR,
    ST_SELECTBOX_RIGHT_FILL,
    ST_SELECTBOX_ALPHA,
    ST_ZOOMSCALE_IN,
    ST_ZOOMSCALE_OUT,
    -1
};

int prompt_props[] = {
    ST_PROMPT_TEXT_COLOR,
    ST_PROMPT_BG_COLOR,
    ST_PROMPT_FONT_FAMILY,
    ST_PROMPT_FONT_SIZE,
    ST_SAVE_HISTORY,
    ST_HTML_OUTPUT,
    -1
};

int quick_snap_props[] = {
    ST_QSNAP_ENABLED,
    ST_QSNAP_LOCATOR_COLOR,
    ST_QSNAP_LOCATOR_SIZE,
    ST_QSNAP_APERTURE_SIZE,
    ST_QSNAP_ENDPOINT,
    ST_QSNAP_MIDPOINT,
    ST_QSNAP_CENTER,
    ST_QSNAP_NODE,
    ST_QSNAP_QUADRANT,
    ST_QSNAP_INTERSECTION,
    ST_QSNAP_EXTENSION,
    ST_QSNAP_INSERTION,
    ST_QSNAP_PERPENDICULAR,
    ST_QSNAP_TANGENT,
    ST_QSNAP_NEAREST,
    ST_QSNAP_APPARENT,
    ST_QSNAP_PARALLEL,
    -1
};

int opensave_props[] = {
    ST_OPENSAVE_FILTER,
    -1
};

/* Wrong type */
void
wrong_type_message(Node n, const char *key, int type)
{
    if (n.type == NODE_NULL) {
        return;
    }
    printf("ERROR: setting with key %s is not of %s type.", key, types[type]);
}

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
Node *
create_node(int type)
{
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->n_leaves = 0;
    new_node->max_leaves = 10;
    new_node->leaves = (Node**)malloc(new_node->max_leaves * sizeof(Node *));
    new_node->type = type;
    return new_node;
}

/*
 *
 */
int
add_leaf(Node *branch, Node *leaf)
{
    if (branch->n_leaves >= branch->max_leaves) {
        branch->max_leaves += 100;
        branch->leaves = realloc(branch->leaves,
            branch->max_leaves*sizeof(Node*));
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
free_node(Node *branch)
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
 * Returns the location of the first occurence of c in s if present. Otherwise
 * return -1.
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
Node *
find_node(Node *branch, char key[MAX_STRING_LENGTH])
{
    if (branch->type == NODE_DICTIONARY) {
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
insert_node(Node *branch, char key[MAX_STRING_LENGTH], Node *node)
{
    if (branch->type == NODE_DICTIONARY) {
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
print_tree(Node *branch, int indent)
{
    int j;
    for (j=0; j<indent; j++) {
        printf("|");
    }
    switch (branch->type) {
    case NODE_STRING: {
        printf("-%s: %s\n", branch->key, branch->data);
        break;
    }
    case NODE_DICTIONARY: {
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

/* Node needs to check for duplicate keys.
 */
Node *
create_and_add_leaf(Node *parent, char *key, char *value)
{
    Node *n = (Node*)malloc(sizeof(Node));
    n->n_leaves = 0;
    n->max_leaves = 10;
    n->leaves = (Node**)malloc(n->max_leaves * sizeof(Node *));
    if (!strcmp(value, "{}")) {
        n->type = NODE_DICTIONARY;
    }
    else {
        n->type = NODE_STRING;
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

/* Fourier series for parametric plotting. */
EmbReal
fourier_series(EmbReal arg, EmbReal *terms, int n_terms)
{
    EmbReal x = 0.0f;
    for (int i=0; i<(n_terms/3); i++) {
        x += terms[3*i+0] * sin(terms[3*i+1] + terms[3*i+2] * arg);
    }
    return x;
}

/* . */
bool
willUnderflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a-b;
    return (c < INT_MIN || c > INT_MAX);
}

/* . */
bool
willOverflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a+b;
    return (c < INT_MIN || c > INT_MAX);
}

/* Round the number "numToRound" to a multple of the number "multiple",
 * rounding up if "roundUp" is true.
 *
 * First, "multiple" is 0 then we have an invalid input so just return the
 * argument, then if the number is already a multiple of \a multiple then
 * return the argument.
 *
 * Then take the remainder off the argument and determine which way to round
 * the result.
 */
int
roundToMultiple(bool roundUp, int numToRound, int multiple)
{
    if (multiple == 0) {
        return numToRound;
    }
    int remainder = numToRound % multiple;
    if (remainder == 0) {
        return numToRound;
    }

    int result = numToRound - remainder;
    if (roundUp) {
        if (numToRound < 0) {
            return result;
        }
        return result + multiple;
    }
    /* else round down */
    if (numToRound < 0) {
        return result - multiple;
    }
    return result;
}
