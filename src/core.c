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

#include "core.h"

/* See the "Programming principles for the C core" in the reference manual
 * before changing this file.
 */

const char details_labels[][MAX_STRING_LENGTH] = {
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

const char default_prompt_style[][MAX_STRING_LENGTH] = {
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
char menubar_order[MAX_MENUBAR_LENGTH][MAX_STRING_LENGTH] = {
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
char file_toolbar[MAX_TOOLBAR_LENGTH][MAX_STRING_LENGTH] = {
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

/*  . */
char edit_toolbar[MAX_TOOLBAR_LENGTH][MAX_STRING_LENGTH] = {
    "cut",
    "copy",
    "paste",
    "END"
};

/*  . */
char view_toolbar[MAX_TOOLBAR_LENGTH][MAX_STRING_LENGTH] = {
    "day",
    "night",
    "END"
};

/*  . */
char zoom_toolbar[MAX_TOOLBAR_LENGTH][MAX_STRING_LENGTH] = {
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

/* A submenu. */
char pan_toolbar[][MAX_STRING_LENGTH] = {
    "panrealtime",
    "panpoint",
    "---",
    "panleft",
    "panright",
    "panup",
    "pandown",
    "END"
};

/*  . */
char icon_toolbar[][MAX_STRING_LENGTH] = {
    "icon16",
    "icon24",
    "icon32",
    "icon48",
    "icon64",
    "icon128",
    "END"
};

/*  . */
char help_toolbar[][MAX_STRING_LENGTH] = {
    "help",
    "---",
    "changelog",
    "---",
    "about",
    "---",
    "whatsthis",
    "END"
};

/*  . */
char draw_toolbar[][MAX_STRING_LENGTH] = {
    "circle",
    "path",
    "polygon",
    "polyline",
    "point",
    "heart",
    "single-line-text",
    "END"
};

/*  . */
char top_toolbar_layout[][MAX_STRING_LENGTH] = {
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
char bottom_toolbar_layout[][MAX_STRING_LENGTH] = {
    "prompt",
    "END"
};

/*  . */
char side_toolbar_layout[][MAX_STRING_LENGTH] = {
    "draw",
    "END"
};

/*  . */
char file_menu[][MAX_STRING_LENGTH] = {
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

/*  . */
char edit_menu[][MAX_STRING_LENGTH] = {
    "undo",
    "redo",
    "---",
    "cut",
    "copy",
    "paste",
    "---",
    "END"
};

char pan_menu[][MAX_STRING_LENGTH] = {
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

char zoom_menu[][MAX_STRING_LENGTH] = {
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

char view_menu[MAX_MENU_LENGTH][MAX_STRING_LENGTH] = {
    "---",
    "submenu zoom",
    "submenu pan",
    "---",
    "day",
    "night",
    "---",
    "END"
};

char settings_menu[MAX_MENU_LENGTH][MAX_STRING_LENGTH] = {
    "settingsdialog",
    "---",
    "END"
};

char window_menu[MAX_MENU_LENGTH][MAX_STRING_LENGTH] = {
    "---",
    "END"
};

char help_menu[MAX_MENU_LENGTH][MAX_STRING_LENGTH] = {
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

char draw_menu[MAX_MENU_LENGTH][MAX_STRING_LENGTH] = {
    "circle",
    "path",
    "polygon",
    "polyline",
    "point",
    "heart",
    "single-line-text",
    "END"
};

/* The group box list is not changeable at runtime, so it's fixed length
 * and a constant.
 */
const char group_box_list[][MAX_STRING_LENGTH] = {
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

char command_labels[N_ACTIONS][MAX_STRING_LENGTH] = {
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
    "TEXTSINGLE"
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
