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

/* C/C++ Standard Libraries. */
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <assert.h>

/* We assume here that all free systems and MacOS are POSIX compliant. */
#if defined(WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#endif

#include "core.h"

/* See the "Programming principles for the C core" in the reference manual
 * before changing this file.
 */

const char *version = "2.0.0-alpha4";

/* The actuator changes the program state via these global variables.
 *
 * These copies of the settings struct are for restoring the state if
 * the user doesn't want to accept their changes in the settings dialog.
 */
Node settings[SETTINGS_TOTAL], dialog[SETTINGS_TOTAL],
    preview[SETTINGS_TOTAL], accept_[SETTINGS_TOTAL];

char *coverage_test_script[] = {
    "new",
    "icon 16",
    "icon 32",
    "icon 64",
    "icon 128",
    "icon 24",
    "zoom in",
    "zoom extents",
    "pan up",
    "pan down",
    "pan right",
    "pan left",
    "quit",
    "END"
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
    ACTION_ADD_POLYLINE,
    ACTION_ADD_PATH,
    ACTION_ADD_REGULAR_POLYGON,
    ACTION_ADD_POLYGON,
    MENU_SEPERATOR,
    ACTION_ADD_ARC,
    ACTION_ADD_CIRCLE,
    ACTION_ADD_ELLIPSE,
    ACTION_ADD_LINE,
    ACTION_ADD_POINT,
    MENU_SEPERATOR,
    ACTION_ADD_HEART,
    ACTION_ADD_DOLPHIN,
    ACTION_TREBLECLEF,
    ACTION_ADD_SNOWFLAKE,
    MENU_SEPERATOR,
    ACTION_ADD_SINGLE_LINE_TEXT,
    ACTION_ADD_TEXT_MULTI,
    ACTION_ADD_TEXT_SINGLE,
    ACTION_ADD_DIM_LEADER,
    ACTION_ADD_VERTICAL_DIMENSION,
    ACTION_ADD_HORIZONTAL_DIMENSION,
    ACTION_ADD_IMAGE,
    ACTION_ADD_INFINITE_LINE,
    ACTION_ADD_RAY,
    ACTION_ADD_RECTANGLE,
    ACTION_ADD_ROUNDED_RECTANGLE,
    ACTION_ADD_RUBBER,
    ACTION_ADD_SLOT,
    ACTION_ADD_TRIANGLE,
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
            ACTION_ADD_POLYLINE,
            ACTION_ADD_PATH,
            TOOLBAR_SEPERATOR,
            ACTION_ADD_ARC,
            ACTION_ADD_CIRCLE,
            ACTION_ADD_ELLIPSE,
            ACTION_ADD_LINE,
            ACTION_ADD_POINT,
            ACTION_ADD_REGULAR_POLYGON,
            ACTION_ADD_POLYGON,
            ACTION_ADD_TRIANGLE,
            TOOLBAR_SEPERATOR,
            ACTION_ADD_HEART,
            ACTION_ADD_DOLPHIN,
            ACTION_TREBLECLEF,
            ACTION_ADD_SNOWFLAKE,
            TOOLBAR_SEPERATOR,
            ACTION_ADD_SINGLE_LINE_TEXT,
            ACTION_ADD_TEXT_MULTI,
            ACTION_ADD_TEXT_SINGLE,
            ACTION_ADD_DIM_LEADER,
            ACTION_ADD_VERTICAL_DIMENSION,
            ACTION_ADD_HORIZONTAL_DIMENSION,
            ACTION_ADD_IMAGE,
            ACTION_ADD_INFINITE_LINE,
            ACTION_ADD_RAY,
            ACTION_ADD_RECTANGLE,
            ACTION_ADD_ROUNDED_RECTANGLE,
            ACTION_ADD_RUBBER,
            ACTION_ADD_SLOT,
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

void
emb_sleep(int seconds)
{
#if defined(WIN32)
    sleep(1);
#else
    usleep(1000000);
#endif
}

/* platformString.
 * TODO: Append QSysInfo to string where applicable.
 */
char *
platformString(void)
{
#if defined(WIN32)
    return "Windows";
#else
    struct utsname platform;
    uname(&platform);
    return platform.sysname;
#endif
}

/* Tokenize our command using a 1 character deliminator. */
int
tokenize(char **argv, char *str, const char delim)
{
    int argc = 0;
    argv[argc] = 0;
    if (strlen(str) == 0) {
        return 0;
    }
    for (int i=0; str[i]; i++) {
        if (str[i] == delim) {
            str[i] = 0;
            argc++;
            argv[argc] = str+i;
        }
    }
    return argc;
}

/* Debug message to logfile, append only.
 *
 * For debugging code running on other machines append these messages to log
 * file.
 *
 * Timestamps are added to each message so we can trace in what order things
 * happen.
 *
 * TODO: fix the thread safety on all systems, this may crash the program
 * if there's two processes trying to write to the same file at the same time.
 */
void
debug_message(char *fmt, ...)
{
    va_list arg_list;
    va_start(arg_list, fmt);
    char thread_file[MAX_STRING_LENGTH];
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    sprintf(thread_file, "debug.txt");
    /* An attempt at thread safety. */
#if defined(SYS_gettid)
    sprintf(thread_file, "debug-%ld.txt", (int64_t)syscall(SYS_gettid));
#endif
    FILE *f = fopen(thread_file, "a");
    fprintf(f, "%.2ld:%.2ld:%.2ld.%.3ld> ",
        (ts.tv_sec/3600)%24, (ts.tv_sec%3600)/60, ts.tv_sec%60, ts.tv_nsec%1000);
    vfprintf(f, fmt, arg_list);
    fprintf(f, "\n");
    fclose(f);
    va_end(arg_list);
}

/* Utility function for add_to_path. */
void
get_n_reals(float result[], char *argv[], int n, int offset)
{
    for (int i=0; i<n; i++) {
        result[i] = atof(argv[offset+i]);
    }
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

/* The group box list is not changeable at runtime, so it's fixed length
 * and a constant.
 */
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
EditorData all_line_editors[MAX_EDITORS] = {
    {
        .groupbox = GB_GENERAL,
        .key = ED_GENERAL_LAYER,
        .icon = "blank",
        .label = "Layer",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GENERAL,
        .key = ED_GENERAL_COLOR,
        .icon = "blank",
        .label = "Color",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GENERAL,
        .key = ED_GENERAL_LINE_TYPE,
        .icon = "blank",
        .label = "LineType",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GENERAL,
        .key = ED_GENERAL_LINE_WEIGHT,
        .icon = "blank",
        .label = "LineWeight",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_CENTER_X,
        .icon = "blank",
        .label = "Center X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcCenterX"
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_CENTER_Y,
        .icon = "blank",
        .label = "Center Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcCenterY"
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_RADIUS,
        .icon = "blank",
        .label = "Radius",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcRadius"
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_START_ANGLE,
        .icon = "blank",
        .label = "Start Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcStartAngle"
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_END_ANGLE,
        .icon = "blank",
        .label = "End Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditArcEndAngle",
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_START_X,
        .icon = "blank",
        .label = "Start X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_START_Y,
        .icon = "blank",
        .label = "Start Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_END_X,
        .icon = "blank",
        .label = "End X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_END_Y,
        .icon = "blank",
        .label = "End Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_LENGTH,
        .icon = "blank",
        .label = "Arc Length",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_CHORD,
        .icon = "blank",
        .label = "Chord",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ARC,
        .key = ED_ARC_INC_ANGLE,
        .icon = "blank",
        .label = "Included Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_ARC,
        .key = ED_ARC_CLOCKWISE,
        .icon = "blank",
        .label = "Clockwise",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_BLOCK,
        .key = ED_BLOCK_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_BLOCK,
        .key = ED_BLOCK_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_CENTER_X,
        .icon = "blank",
        .label = "Center X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleCenterX",
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_CENTER_Y,
        .icon = "blank",
        .label = "Center Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleCenterY"
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_RADIUS,
        .icon = "blank",
        .label = "Radius",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleRadius"
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_DIAMETER,
        .icon = "blank",
        .label = "Diameter",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleDiameter"
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleArea"
    },
    {
        .groupbox = GB_GEOM_CIRCLE,
        .key = ED_CIRCLE_CIRCUMFERENCE,
        .icon = "blank",
        .label = "Circumference",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditCircleCircumference"
    },
    {
        .groupbox = GB_GEOM_DIM_ALIGNED,
        .key = ED_DIM_ALIGNED_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_ANGULAR,
        .key = ED_DIM_ANGULAR_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_ARC_LENGTH,
        .key = ED_DIM_ARC_LENGTH_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_DIAMETER,
        .key = ED_DIM_DIAMETER_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_LEADER,
        .key = ED_DIM_LEADER_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_LINEAR,
        .key = ED_DIM_LINEAR_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_ORDINATE,
        .key = ED_GEOM_DIM_ORDINATE_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_DIM_RADIUS,
        .key = ED_DIM_RADIUS_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_CENTER_X,
        .icon = "blank",
        .label = "Center X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseCenterX"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_CENTER_Y,
        .icon = "blank",
        .label = "Center Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseCenterY"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_SEMI_MAJOR_AXIS,
        .icon = "blank",
        .label = "Semi-Major Axis",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseSemiMajorAxis"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_SEMI_MINOR_AXIS,
        .icon = "blank",
        .label = "Semi-Minor Axis",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseSemiMinorAxis"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_MAJOR_AXIS,
        .icon = "blank",
        .label = "Major Axis",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseMajorAxis"
    },
    {
        .groupbox = GB_GEOM_ELLIPSE,
        .key = ED_ELLIPSE_MINOR_AXIS,
        .icon = "blank",
        .label = "Minor Axis",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditEllipseMinorAxis"
    },
    {
        .groupbox = GB_GEOM_IMAGE,
        .key = ED_IMAGE_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_IMAGE,
        .key = ED_IMAGE_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_IMAGE,
        .key = ED_IMAGE_WIDTH,
        .icon = "blank",
        .label = "Width",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_IMAGE,
        .key = ED_IMAGE_HEIGHT,
        .icon = "blank",
        .label = "Height",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_IMAGE,
        .key = ED_IMAGE_NAME,
        .icon = "blank",
        .label = "Name",
        .type = EDITOR_STRING,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_IMAGE,
        .key = ED_IMAGE_PATH,
        .icon = "blank",
        .label = "Path",
        .type = EDITOR_STRING,
        .map_signal = ""
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_X1,
        .icon = "blank",
        .label = "Start X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditInfiniteLineX1"
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_Y1,
        .icon = "blank",
        .label = "Start Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditInfiniteLineY1"
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_X2,
        .icon = "blank",
        .label = "2nd X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditInfiniteLineX2",
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_Y2,
        .icon = "blank",
        .label = "2nd Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditInfiniteLineY2",
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_VECTOR_X,
        .icon = "blank",
        .label = "Vector X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_INFINITE_LINE,
        .key = ED_INFINITE_LINE_VECTOR_Y,
        .icon = "blank",
        .label = "Vector Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_START_X,
        .icon = "blank",
        .label = "Start X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditLineStartX"
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_START_Y,
        .icon = "blank",
        .label = "Start Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditLineStartY"
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_END_X,
        .icon = "blank",
        .label = "End X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditLineEndX"
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_END_Y,
        .icon = "blank",
        .label = "End Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditLineEndY"
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_DELTA_X,
        .icon = "blank",
        .label = "Delta X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_DELTA_Y,
        .icon = "blank",
        .label = "Delta Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_ANGLE,
        .icon = "blank",
        .label = "Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_LINE,
        .key = ED_LINE_LENGTH,
        .icon = "blank",
        .label = "Length",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_VERTEX_NUM,
        .icon = "blank",
        .label = "Vertex #",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_VERTEX_X,
        .icon = "blank",
        .label = "Vertex X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_VERTEX_Y,
        .icon = "blank",
        .label = "Vertex Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_PATH,
        .key = ED_PATH_LENGTH,
        .icon = "blank",
        .label = "Length",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_PATH,
        .key = ED_PATH_CLOSED,
        .icon = "blank",
        .label = "Closed",
        .type = EDITOR_COMBOBOX,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POINT,
        .key = ED_POINT_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditPointX"
    },
    {
        .groupbox = GB_GEOM_POINT,
        .key = ED_POINT_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditPointY"
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_CENTER_X,
        .icon = "blank",
        .label = "Center X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_CENTER_Y,
        .icon = "blank",
        .label = "Center Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_RADIUS_VERTEX,
        .icon = "blank",
        .label = "Vertex Radius",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_RADIUS_SIDE,
        .icon = "blank",
        .label = "Side Radius",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_DIAMETER_VERTEX,
        .icon = "blank",
        .label = "Vertex Diameter",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_DIAMETER_SIDE,
        .icon = "blank",
        .label = "Side Diameter",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYGON,
        .key = ED_POLYGON_INTERIOR_ANGLE,
        .icon = "blank",
        .label = "Interior Angle",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_NUM,
        .icon = "blank",
        .label = "Vertex #",
        .type = EDITOR_COMBOBOX,
        .map_signal = "",
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_X,
        .icon = "blank",
        .label = "Vertex X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_Y,
        .icon = "blank",
        .label = "Vertex Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_POLYLINE,
        .key = ED_POLYLINE_VERTEX_LENGTH,
        .icon = "blank",
        .label = "Length",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_MISC_POLYLINE,
        .key = ED_POLYLINE_VERTEX_CLOSED,
        .icon = "blank",
        .label = "Closed",
        .type = EDITOR_COMBOBOX,
        .map_signal = "",
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_X1,
        .icon = "blank",
        .label = "Start X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_Y1,
        .icon = "blank",
        .label = "Start Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_X2,
        .icon = "blank",
        .label = "2nd X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_Y2,
        .icon = "blank",
        .label = "2nd Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_VECTOR_X,
        .icon = "blank",
        .label = "Vector X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RAY,
        .key = ED_RAY_VECTOR_Y,
        .icon = "blank",
        .label = "Vector Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER1_X,
        .icon = "blank",
        .label = "Corner 1 X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER1_Y,
        .icon = "blank",
        .label = "Corner 1 Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER2_X,
        .icon = "blank",
        .label = "Corner 2 X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER2_Y,
        .icon = "blank",
        .label = "Corner 2 Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER3_X,
        .icon = "blank",
        .label = "Corner 3 X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER3_Y,
        .icon = "blank",
        .label = "Corner 3 Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER4_X,
        .icon = "blank",
        .label = "Corner 4 X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_CORNER4_Y,
        .icon = "blank",
        .label = "Corner 4 Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_WIDTH,
        .icon = "blank",
        .label = "Width",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_HEIGHT,
        .icon = "blank",
        .map_signal = "Height",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_RECTANGLE,
        .key = ED_RECTANGLE_AREA,
        .icon = "blank",
        .label = "Area",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_CONTENTS,
        .icon = "blank",
        .label = "Contents",
        .type = EDITOR_STRING,
        .map_signal = "lineEditTextSingleContents"
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_FONT,
        .icon = "blank",
        .label = "Font",
        .type = EDITOR_FONT,
        .map_signal = "comboBoxTextSingleFont"
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_JUSTIFY,
        .icon = "blank",
        .label = "Justify",
        .type = EDITOR_COMBOBOX,
        .map_signal = "comboBoxTextSingleJustify"
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_HEIGHT,
        .icon = "blank",
        .label = "Height",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditTextSingleHeight",
    },
    {
        .groupbox = GB_TEXT_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_ROTATION,
        .icon = "blank",
        .label = "Rotation",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditTextSingleRotation",
    },
    {
        .groupbox = GB_GEOM_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditTextSingleX"
    },
    {
        .groupbox = GB_GEOM_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = "lineEditTextSingleY"
    },
    {
        .groupbox = GB_MISC_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_BACKWARD,
        .icon = "blank",
        .label = "Backward",
        .type = EDITOR_COMBOBOX,
        .map_signal = "comboBoxTextSingleBackward"
    },
    {
        .groupbox = GB_MISC_TEXT_SINGLE,
        .key = ED_TEXT_SINGLE_UPSIDE_DOWN,
        .icon = "blank",
        .label = "UpsideDown",
        .type = EDITOR_COMBOBOX,
        .map_signal = "comboBoxTextSingleUpsideDown"
    },
    {
        .groupbox = GB_GEOM_TEXT_MULTI,
        .key = ED_TEXT_MULTI_X,
        .icon = "blank",
        .label = "Position X",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    {
        .groupbox = GB_GEOM_TEXT_MULTI,
        .key = ED_TEXT_MULTI_Y,
        .icon = "blank",
        .label = "Position Y",
        .type = EDITOR_DOUBLE,
        .map_signal = ""
    },
    /* end symbol */
    {
        .groupbox = -1,
        .key = -1,
        .icon = "END",
        .label = "END",
        .type = -1,
        .map_signal = "END"
    }
};

EditorData all_spinbox_editors[] = {
    /* end symbol */
    {
        .groupbox = -1,
        .key = -1,
        .icon = "END",
        .label = "END",
        .type = -1,
        .map_signal = "END"
    }
};

/* This imitates an initiation file, and if none is present it is loaded
 * using the same parser.
 *
 * Our ini syntax is minimal: all lines with an equals sign '=' are parsed
 * comments are lines without the sign, everything to the left is the key,
 * everything to the right is the value.
 *
 * There are 3 types: int, float and string (char array). Boolean values are
 * treated as ints and arrays are strings with comma seperation.
 *
 * 1. If the first char of the value string is a number but not period is present
 *    then it is an int.
 * 2. If a period is present and it starts with a number then it is a float.
 * 3. Otherwise it is a string.
 * 4. Colors start with an open parenthesis, so we know that the string passed
 *    should then be converted into a color in the next loop.
 *
 *------------------------------------------------------------------------------
 *
 * Add a settings page entry to each row, allowing copying every entry on a tab
 * to dialog/config/accept_ as a loop.
 */
Setting settings_data[SETTINGS_TOTAL] = {
    {
        .id = ST_LANGUAGE,
        .key = "general_language",
        .value = "default",
        .type = 's'
    },
    {
        .id = ST_ICON_THEME,
        .key = "general_icon_theme",
        .value = "default",
        .type = 's'
    },
    {
        .id = ST_ICON_SIZE,
        .key = "general_icon_size",
        .value = "16",
        .type = 'i'
    },
    {
        .id = ST_MDI_USE_LOGO,
        .key = "general_mdi_bg_use_logo",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_MDI_USE_TEXTURE,
        .key = "general_mdi_bg_use_texture",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_MDI_USE_COLOR,
        .key = "general_mdi_bg_use_color",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_MDI_LOGO,
        .key = "general_mdi_bg_logo",
        .value = "images/logo-spirals.png",
        .type = 's'
    },
    {
        .id = ST_MDI_TEXTURE,
        .key = "general_mdi_bg_texture",
        .value = "images/texture-spirals.png",
        .type = 's'
    },
    {
        .id = ST_MDI_COLOR,
        .key = "general_mdi_bg_color",
        .value = "255",
        .type = 'i'
    },
    {
        .id = ST_TIP_OF_THE_DAY,
        .key = "general_tip_of_the_day",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_CURRENT_TIP,
        .key = "general_current_tip",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SYSTEM_HELP_BROWSER,
        .key = "general_system_help_browser",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_WINDOW_POS_X,
        .key = "window_position_x",
        .value = "100",
        .type = 'i'
    },
    {
        .id = ST_WINDOW_POS_Y,
        .key = "window_position_y",
        .value = "100",
        .type = 'i'
    },
    {
        .id = ST_WINDOW_SIZE_X,
        .key = "window_size_x",
        .value = "800",
        .type = 'i'
    },
    {
        .id = ST_WINDOW_SIZE_Y,
        .key = "window_size_y",
        .value = "600",
        .type = 'i'
    },
    {
        .id = ST_USE_OPENGL,
        .key = "display_use_open_gl",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_ANTI_ALIAS,
        .key = "display_render_hint_anti_alias",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_ANTI_ALIAS,
        .key = "display_render_hint_text_anti_alias",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SMOOTH_PIXMAP,
        .key = "display_render_hint_smooth_pixmap",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_HQ_ANTI_ALIAS,
        .key = "display_render_hint_high_quality_anti_alias",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_NON_COSMETIC,
        .key = "display_render_hint_non_cosmetic",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SHOW_SCROLLBARS,
        .key = "display_show_scrollbars",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_SCROLLBAR_WIDGET_NUM,
        .key = "display_scrollbar_widget_num",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_CROSSHAIR_COLOR,
        .key = "display_crosshair_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_BG_COLOR,
        .key = "display_background_color",
        .value = "16777215",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_LEFT_COLOR,
        .key = "display_selectbox_left_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_LEFT_FILL,
        .key = "display_selectbox_left_fill",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_RIGHT_COLOR,
        .key = "display_selectbox_right_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_RIGHT_FILL,
        .key = "display_selectbox_right_fill",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTBOX_ALPHA,
        .key = "display_selectbox_alpha",
        .value = "32",
        .type = 'i'
    },
    {
        .id = ST_ZOOMSCALE_IN,
        .key = "display_zoomscale_in",
        .value = "2.0",
        .type = 'r'
    },
    {
        .id = ST_ZOOMSCALE_OUT,
        .key = "display_zoomscale_out",
        .value = "0.5",
        .type = 'r'
    },
    {
        .id = ST_CROSSHAIR_PERCENT,
        .key = "display_crosshair_percent",
        .value = "5.0",
        .type = 'r'
    },
    {
        .id = ST_DISPLAY_UNITS,
        .key = "display_units",
        .value = "mm",
        .type = 's'
    },
    {
        .id = ST_PROMPT_TEXT_COLOR,
        .key = "prompt_text_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_PROMPT_BG_COLOR,
        .key = "prompt_background_color",
        .value = "16777215",
        .type = 'i'
    },
    {
        .id = ST_PROMPT_FONT_FAMILY,
        .key = "prompt_font_family",
        .value = "Monospace",
        .type = 's'
    },
    {
        .id = ST_PROMPT_FONT_STYLE,
        .key = "prompt_font_style",
        .value = "normal",
        .type = 'i'
    },
    {
        .id = ST_PROMPT_FONT_SIZE,
        .key = "prompt_font_size",
        .value = "12",
        .type = 'r'
    },
    {
        .id = ST_SAVE_HISTORY,
        .key = "prompt_save_history",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_HTML_OUTPUT,
        .key = "prompt_save_history_as_html",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_HISTORY_FILE,
        .key = "prompt_save_history_filename",
        .value = "prompt.log",
        .type = 's'
    },
    {
        .id = ST_OPENSAVE_FILTER,
        .key = "opensave_custom_filter",
        .value = "supported",
        .type = 's'
    },
    {
        .id = ST_OPENSAVE_FORMAT,
        .key = "opensave_open_format",
        .value = "*.*",
        .type = 's'
    },
    {
        .id = ST_OPEN_THUMBNAIL,
        .key = "opensave_open_thumbnail",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SAVE_FORMAT,
        .key = "opensave_save_format",
        .value = "*.*",
        .type = 's'
    },
    {
        .id = ST_SAVE_THUMBNAIL,
        .key = "opensave_save_thumbnail",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_RECENT_MAX,
        .key = "opensave_recent_max",
        .value = "10",
        .type = 'i'
    },
    {
        .id = ST_RECENT_FILES,
        .key = "opensave_recent_list_of_files",
        .value = "",
        .type = 's'
    },
    {
        .id = ST_RECENT_DIRECTORY,
        .key = "opensave_recent_directory",
        .value = "samples",
        .type = 's'
    },
    {
        .id = ST_TRIM_NUM_JUMPS,
        .key = "opensave_trim_dst_num_jumps",
        .value = "5",
        .type = 'i'
    },
    {
        .id = ST_DEFAULT_PRINTER,
        .key = "printing_default_device",
        .value = "",
        .type = 's'
    },
    {
        .id = ST_USE_LAST_PRINTER,
        .key = "printing_use_last_device",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_PRINT_DISABLE_BG,
        .key = "printing_disable_bg",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_GRID_ON_LOAD,
        .key = "grid_show_on_load",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_SHOW_ORIGIN,
        .key = "grid_show_origin",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_MATCH_GRID_CROSSHAIR,
        .key = "grid_color_match_crosshair",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_GRID_COLOR,
        .key = "grid_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_GRID_LOAD_FROM_FILE,
        .key = "grid_load_from_file",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_GRID_TYPE,
        .key = "grid_type",
        .value = "Rectangular",
        .type = 's'
    },
    {
        .id = ST_GRID_CENTER_ORIGIN,
        .key = "grid_center_on_origin",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_GRID_CENTER_X,
        .key = "grid_center_x",
        .value = "0.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_CENTER_Y,
        .key = "grid_center_y",
        .value = "0.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SIZE_X,
        .key = "grid_size_x",
        .value = "100.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SIZE_Y,
        .key = "grid_size_y",
        .value = "100.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SPACING_X,
        .key = "grid_spacing_x",
        .value = "25.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SPACING_Y,
        .key = "grid_spacing_y",
        .value = "25.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SIZE_RADIUS,
        .key = "grid_size_radius",
        .value = "50.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SPACING_RADIUS,
        .key = "grid_spacing_radius",
        .value = "25.0",
        .type = 'r'
    },
    {
        .id = ST_GRID_SPACING_ANGLE,
        .key = "grid_spacing_angle",
        .value = "45.0",
        .type = 'r'
    },
    {
        .id = ST_RULER_ON_LOAD,
        .key = "ruler_show_on_load",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_RULER_METRIC,
        .key = "ruler_metric",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_RULER_COLOR,
        .key = "ruler_color",
        .value = "14479360",
        .type = 'i'
    },
    {
        .id = ST_RULER_SIZE,
        .key = "ruler_pixel_size",
        .value = "20",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_ENABLED,
        .key = "quicksnap_enabled",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_LOCATOR_COLOR,
        .key = "quicksnap_locator_color",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_LOCATOR_SIZE,
        .key = "quicksnap_locator_size",
        .value = "4",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_APERTURE_SIZE,
        .key = "quicksnap_aperture_size",
        .value = "10",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_ENDPOINT,
        .key = "quicksnap_endpoint",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_MIDPOINT,
        .key = "quicksnap_midpoint",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_CENTER,
        .key = "quicksnap_center",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_NODE,
        .key = "quicksnap_node",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_QUADRANT,
        .key = "quicksnap_quadrant",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_INTERSECTION,
        .key = "quicksnap_intersection",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_EXTENSION,
        .key = "quicksnap_extension",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_INSERTION,
        .key = "quicksnap_insertion",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_PERPENDICULAR,
        .key = "quicksnap_perpendicular",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_TANGENT,
        .key = "quicksnap_tangent",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_NEAREST,
        .key = "quicksnap_nearest",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_APPARENT,
        .key = "quicksnap_apparent",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_QSNAP_PARALLEL,
        .key = "quicksnap_parallel",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_LWT_SHOW,
        .key = "lineweight_show_line_weight",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_LWT_REAL,
        .key = "lineweight_real_render",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_LWT_DEFAULT,
        .key = "lineweight_default_line_weight",
        .value = "0.0",
        .type = 'r'
    },
    {
        .id = ST_SELECTION_PICK_FIRST,
        .key = "selection_pick_first",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_PICK_ADD,
        .key = "selection_pick_add",
        .value = "1",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_PICK_DRAG,
        .key = "selection_pick_drag",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_COOLGRIP_COLOR,
        .key = "selection_coolgrip_color",
        .value = "255",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_HOTGRIP_COLOR,
        .key = "selection_hotgrip_color",
        .value = "25500",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_GRIP_SIZE,
        .key = "selection_grip_size",
        .value = "4",
        .type = 'i'
    },
    {
        .id = ST_SELECTION_PICKBOX_SIZE,
        .key = "selection_pickbox_size",
        .value = "4",
        .type = 'i'
    },
    {
        .id = ST_TEXT_FONT,
        .key = "text_font",
        .value = "Arial",
        .type = 's'
    },
    {
        .id = ST_TEXT_SIZE,
        .key = "text_size",
        .value = "12.0",
        .type = 'r'
    },
    {
        .id = ST_TEXT_ANGLE,
        .key = "text_angle",
        .value = "0.0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_BOLD,
        .key = "text_style_bold",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_ITALIC,
        .key = "text_style_italic",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_UNDERLINE,
        .key = "text_style_underline",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_STRIKEOUT,
        .key = "text_style_strikeout",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TEXT_OVERLINE,
        .key = "text_style_overline",
        .value = "0",
        .type = 'i'
    },
    {
        .id = ST_TICK_DEPTH,
        .key = "ruler_tick_depth",
        .value = "0.5",
        .type = 'r'
    },
    {
        .id = ST_MAJOR_TICK_SEPERATION,
        .key = "major_tick_seperation",
        .value = "0.4",
        .type = 'r'
    },
    {
        .id = ST_NEEDLE_SPEED,
        .key = "needle_speed",
        .value = "100.0",
        .type = 'r'
    },
    {
        .id = ST_STITCH_TIME,
        .key = "stitch_time",
        .value = "0.01",
        .type = 'r'
    }
};

/* . */
CommandData command_table[MAX_COMMANDS] = {
    {
        .id = COMMAND_ABOUT,
        .command = "about",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_TREBLECLEF,
        .command = "trebleclef",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DO_NOTHING,
        .command = "donothing",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_NEW,
        .command = "new",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_OPEN,
        .command = "open",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SAVE,
        .command = "save",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SAVEAS,
        .command = "saveas",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PRINT,
        .command = "print",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DESIGN_DETAILS,
        .command = "designdetails",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_EXIT,
        .command = "exit",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_CUT,
        .command = "cut",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_COPY,
        .command = "copy",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PASTE,
        .command = "paste",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_UNDO,
        .command = "undo",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_REDO,
        .command = "redo",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WINDOW,
        .command = "window",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_HELP,
        .command = "help",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_CHANGELOG,
        .command = "changelog",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TIP_OF_THE_DAY,
        .command = "tips",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WHATS_THIS,
        .command = "whatsthis",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ICON,
        .command = "icon",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SETTINGS_DIALOG,
        .command = "settingsdialog",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MAKE_LAYER_CURRENT,
        .command = "makelayercurrent",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LAYERS,
        .command = "layers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LAYER_SELECTOR,
        .command = "layerselector",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LAYER_PREVIOUS,
        .command = "layerprevious",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_COLOR_SELECTOR,
        .command = "colorselector",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LINE_TYPE_SELECTOR,
        .command = "linetypeselector",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LINE_WEIGHT_SELECTOR,
        .command = "lineweightselector",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_HIDE_ALL_LAYERS,
        .command = "hidealllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SHOW_ALL_LAYERS,
        .command = "showalllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_FREEZE_ALL_LAYERS,
        .command = "freezealllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_THAW_ALL_LAYERS,
        .command = "thawalllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LOCK_ALL_LAYERS,
        .command = "lockalllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_UNLOCK_ALL_LAYERS,
        .command = "unlockalllayers",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEXT,
        .command = "text",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ZOOM,
        .command = "zoom",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PAN,
        .command = "pan",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DAY,
        .command = "day",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_NIGHT,
        .command = "night",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SPELLCHECK,
        .command = "spellcheck",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_QUICKSELECT,
        .command = "quickselect",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DISTANCE,
        .command = "distance",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DELETE,
        .command = "erase",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MOVE,
        .command = "move",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PLATFORM,
        .command = "platform",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_QUICKLEADER,
        .command = "addquickleader",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_RGB,
        .command = "rgb",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ROTATE,
        .command = "rotate",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SANDBOX,
        .command = "sandbox",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SCALE,
        .command = "scale",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SELECT_ALL,
        .command = "selectall",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SINGLE_LINE_TEXT,
        .command = "singlelinetext",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SYSWINDOWS,
        .command = "syswindows",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_EXIT,
        .command = "exit",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEST,
        .command = "test",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SLEEP,
        .command = "sleep",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_END,
        .command = "END",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    }
};

/* . */
ActionData action_table[MAX_ACTIONS] = {
    {
        .id = ACTION_ABOUT,
        .icon = "about",
        .command = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product: ABOUT.",
        .shortcut = ""
    },
    {
        .id = ACTION_TREBLECLEF,
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = ""
    },
    {
        .id = ACTION_DO_NOTHING,
        .icon = "donothing",
        .command = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing",
        .shortcut = ""
    },
    {
        .id = ACTION_NEW,
        .icon = "new",
        .command = "new",
        .tooltip = "&New",
        .statustip = "Create a new file: NEW.",
        .shortcut = "Ctrl+N"
    },
    {
        .id = ACTION_OPEN,
        .icon = "open",
        .command = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file: OPEN",
        .shortcut = "Ctrl+O"
    },
    {
        .id = ACTION_SAVE,
        .icon = "save",
        .command = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk.",
        .shortcut = "Ctrl+S"
    },
    {
        .id = ACTION_SAVEAS,
        .icon = "saveas",
        .command = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name and type.",
        .shortcut = "Ctrl+Shift+S"
    },
    {
        .id = ACTION_PRINT,
        .icon = "print",
        .command = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P"
    },
    {
        .id = ACTION_DESIGN_DETAILS,
        .icon = "designdetails",
        .command = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design.",
        .shortcut = "Ctrl+D"
    },
    {
        .id = ACTION_EXIT,
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT",
        .shortcut = "Ctrl+Q"
    },
    {
        .id = ACTION_CUT,
        .icon = "cut",
        .command = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+X"
    },
    {
        .id = ACTION_COPY,
        .icon = "copy",
        .command = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+C"
    },
    {
        .id = ACTION_PASTE,
        .icon = "paste",
        .command = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "Ctrl+V"
    },
    {
        .id = ACTION_UNDO,
        .icon = "undo",
        .command = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action: UNDO",
        .shortcut = "Ctrl+Z"
    },
    {
        .id = ACTION_REDO,
        .icon = "redo",
        .command = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action: REDO",
        .shortcut = "Ctrl+Shift+Z"
    },
    {
        .id = ACTION_WINDOW_CLOSE,
        .icon = "windowclose",
        .command = "window close",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window: CLOSE",
        .shortcut = "Ctrl+W"
    },
    {
        .id = ACTION_WINDOW_CLOSE_ALL,
        .icon = "windowcloseall",
        .command = "window closeall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows: CLOSEALL",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_CASCADE,
        .icon = "windowcascade",
        .command = "window cascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows: CASCADE",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_TILE,
        .icon = "windowtile",
        .command = "window tile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows: TILE",
        .shortcut = ""
    },
    {
        .id = ACTION_WINDOW_NEXT,
        .icon = "windownext",
        .command = "window next",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window: NEXT",
        .shortcut = "Ctrl+Tab"
    },
    {
        .id = ACTION_WINDOW_PREVIOUS,
        .icon = "windowprevious",
        .command = "window previous",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window: PREVIOUS",
        .shortcut = "Ctrl+Shift+Tab"
    },
    {
        .id = ACTION_HELP,
        .icon = "help",
        .command = "help",
        .tooltip = "&Help",
        .statustip = "Displays the help file: HELP.",
        .shortcut = ""
    },
    {
        .id = ACTION_CHANGELOG,
        .icon = "changelog",
        .command = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = ""
    },
    {
        .id = ACTION_TIP_OF_THE_DAY,
        .icon = "tipoftheday",
        .command = "tips",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips: TIPS",
        .shortcut = ""
    },
    {
        .id = ACTION_WHATS_THIS,
        .icon = "whatsthis",
        .command = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help!",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON16,
        .icon = "icon16",
        .command = "icon 16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON24,
        .icon = "icon24",
        .command = "icon 24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24: ICON24.",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON32,
        .icon = "icon32",
        .command = "icon 32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32: ICON32",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON48,
        .icon = "icon48",
        .command = "icon 48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48: ICON48",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON64,
        .icon = "icon64",
        .command = "icon 64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64: ICON64",
        .shortcut = ""
    },
    {
        .id = ACTION_ICON128,
        .icon = "icon128",
        .command = "icon 128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128: ICON128",
        .shortcut = ""
    },
    {
        .id = ACTION_SETTINGS_DIALOG,
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = ""
    },
    {
        .id = ACTION_MAKE_LAYER_CURRENT,
        .icon = "makelayercurrent",
        .command = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYERS,
        .icon = "layers",
        .command = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYER_SELECTOR,
        .icon = "layerselector",
        .command = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .shortcut = ""
    },
    {
        .id = ACTION_LAYER_PREVIOUS,
        .icon = "layerprevious",
        .command = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings: LAYERP",
        .shortcut = ""
    },
    {
        .id = ACTION_COLOR_SELECTOR,
        .icon = "colorselector",
        .command = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .shortcut = ""
    },
    {
        .id = ACTION_LINE_TYPE_SELECTOR,
        .icon = "linetypeselector",
        .command = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .shortcut = ""
    },
    {
        .id = ACTION_LINE_WEIGHT_SELECTOR,
        .icon = "lineweightselector",
        .command = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .shortcut = ""
    },
    {
        .id = ACTION_HIDE_ALL_LAYERS,
        .icon = "hidealllayers",
        .command = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .shortcut = ""
    },
    {
        .id = ACTION_SHOW_ALL_LAYERS,
        .icon = "showalllayers",
        .command = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .shortcut = ""
    },
    {
        .id = ACTION_FREEZE_ALL_LAYERS,
        .icon = "freezealllayers",
        .command = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .shortcut = ""
    },
    {
        .id = ACTION_THAW_ALL_LAYERS,
        .icon = "thawlllayers",
        .command = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .shortcut = ""
    },
    {
        .id = ACTION_LOCK_ALL_LAYERS,
        .icon = "lockalllayers",
        .command = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .shortcut = ""
    },
    {
        .id = ACTION_UNLOCK_ALL_LAYERS,
        .icon = "unlockalllayers",
        .command = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_BOLD,
        .icon = "textbold",
        .command = "text bold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_ITALIC,
        .icon = "textitalic",
        .command = "text italic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_UNDERLINE,
        .icon = "textunderline",
        .command = "text underline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_STRIKEOUT,
        .icon = "textstrikeout",
        .command = "text strikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEXT_OVERLINE,
        .icon = "textoverline",
        .command = "text overline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_REAL_TIME,
        .icon = "zoomrealtime",
        .command = "zoom realtime",
        .tooltip = "Zoom &Realtime",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_PREVIOUS,
        .icon = "zoomprevious",
        .command = "zoom previous",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_WINDOW,
        .icon = "zoomwindow",
        .command = "zoom window",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_DYNAMIC,
        .icon = "zoomdynamic",
        .command = "zoom dynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_SCALE,
        .icon = "zoomscale",
        .command = "zoom scale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_CENTER,
        .icon = "zoomcenter",
        .command = "zoom center",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_IN,
        .icon = "zoomin",
        .command = "zoom in",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects: ZOOMIN",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_OUT,
        .icon = "zoomout",
        .command = "zoom out",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects: ZOOMOUT",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_SELECTED,
        .icon = "zoomselected",
        .command = "zoom selected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_ALL,
        .icon = "zoomall",
        .command = "zoom all",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits.",
        .shortcut = ""
    },
    {
        .id = ACTION_ZOOM_EXTENTS,
        .icon = "zoomextents",
        .command = "zoom extents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents: ZOOMEXTENTS",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_REAL_TIME,
        .icon = "panrealtime",
        .command = "pan realtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport.",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_POINT,
        .icon = "panpoint",
        .command = "pan point",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_LEFT,
        .icon = "panleft",
        .command = "pan left",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left: PANLEFT",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_RIGHT,
        .icon = "panright",
        .command = "pan right",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right: PANRIGHT",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_UP,
        .icon = "panup",
        .command = "pan up",
        .tooltip = "&Pan Up",
        .statustip = "Moves the view up: PANUP",
        .shortcut = ""
    },
    {
        .id = ACTION_PAN_DOWN,
        .icon = "pandown",
        .command = "pan down",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down: PANDOWN",
        .shortcut = ""
    },
    {
        .id = ACTION_DAY,
        .icon = "day",
        .command = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings: DAY.",
        .shortcut = ""
    },
    {
        .id = ACTION_NIGHT,
        .icon = "night_vision",
        .command = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings: NIGHT",
        .shortcut = ""
    },
    {
        /* todo */
        .id = ACTION_SPELLCHECK,
        .icon = "spellcheck",
        .command = "spellcheck",
        .tooltip = "",
        .statustip = "",
        .shortcut = ""
    },
    {
        .id = ACTION_QUICKSELECT,
        .icon = "quickselect",
        .command = "quickselect",
        .tooltip = "",
        .statustip = "",
        .shortcut = ""
    },
    {
        /* .alias = "C", */
        .id = ACTION_ADD_CIRCLE,
        .icon = "circle",
        .command = "circle",
        .tooltip = "Circle",
        .statustip = "Creates a circle: CIRCLE",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_HEART,
        .icon = "heart",
        .command = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart: HEART",
        .shortcut = ""
    },
    {
        /*
        menu = "Tools",
        toolbar = "Inquiry",
        */
        .id = ACTION_DISTANCE,
        .icon = "distance",
        .command = "distance",
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points: DIST",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_DOLPHIN,
        .icon = "dolphin",
        .command = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Adds a dolphin design to the vector layer: DOLPHIN.",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_ELLIPSE,
        .icon = "ellipse",
        .command = "ellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates a ellipse: ELLIPSE",
        .shortcut = ""
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = ACTION_DELETE,
        .icon = "erase",
        .command = "erase",
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing: DELETE.",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_LINE,
        .icon = "line",
        .command = "line",
        .tooltip = "&Line",
        .statustip = "Creates straight line segments: LINE.",
        .shortcut = ""
    },
    {
        /*
        menu = "Tools",
        toolbar = "Inquiry",
        */
        .id = ACTION_LOCATE_POINT,
        .icon = "locatepoint",
        .command = "locatepoint",
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location: ID.",
        .shortcut = ""
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify"
        */
        .id = ACTION_MOVE,
        .icon = "move",
        .command = "move",
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction: MOVE.",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw"
        */
        .id = ACTION_ADD_PATH,
        .icon = "path",
        .command = "addpath",
        .tooltip = "&Path",
        .statustip = "Creates a 2D path: PATH.",
        .shortcut = ""
    },
    {
        .id = ACTION_PLATFORM,
        .icon = "platform",
        .command = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use: PLATFORM.",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_POINT,
        .icon = "point",
        .command = "addpoint",
        .tooltip = "&Point",
        .statustip = "Creates multiple points: POINT.",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_POLYGON,
        .icon = "polygon",
        .command = "addpolygon",
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon: POLYGON.",
        .shortcut = ""
    },
    {
        /*
        .menu = "Draw",
        .toolbar = "Draw",
        */
        .id = ACTION_ADD_POLYLINE,
        .icon = "polyline",
        .command = "addpolyline",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline: PLINE.",
        .shortcut = ""
    },
    {
        /*
        menu = "Dimension",
        toolbar = "Dimension",
        */
        .id = ACTION_QUICKLEADER,
        .icon = "quickleader",
        .command = "addquickleader",
        .tooltip = "&QuickLeader",
        .statustip = "Creates a leader and annotation: QUICKLEADER.",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_RECTANGLE,
        .icon = "rectangle",
        .command = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = ""
    },
    {
        /*
        menu = "Sandbox"
        toolbar = "Sandbox"
        */
        .id = ACTION_RGB,
        .icon =  "rgb",
        .command = "rgb",
        .tooltip =  "&RGB",
        .statustip = "Updates the current view colors: RGB.",
        .shortcut = ""
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = ACTION_ROTATE,
        .icon = "rotate",
        .command = "rotate",
        .tooltip = "&Rotate",
        .statustip = "Rotates objects about a base point: ROTATE.",
        .shortcut = ""
    },
    {
        /*
        menu = "Sandbox",
        toolbar = "Sandbox",
        */
        .id = ACTION_SANDBOX,
        .icon = "sandbox",
        .command = "sandbox",
        .tooltip = "Sandbox",
        .statustip = "A sandbox to play in: SANDBOX.",
        .shortcut = ""
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = ACTION_SCALE,
        .icon = "scale",
        .command = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions: SCALE.",
        .shortcut = ""
    },
    {
        .id = ACTION_SELECT_ALL,
        .icon = "selectall",
        .command = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects: SELECTALL.",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_SINGLE_LINE_TEXT,
        .icon = "single-line-text",
        .command = "singlelinetext",
        .tooltip = "&Single Line Text",
        .statustip = "Creates single-line text objects: TEXT",
        .shortcut = ""
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = ACTION_ADD_SNOWFLAKE,
        .icon = "snowflake",
        .command = "snowflake",
        .tooltip = "&Snowflake",
        .statustip = "Creates a snowflake: SNOWFLAKE",
        .shortcut = ""
    },
    {
        /*
        menu "Draw",
        toolbar "Draw",
        */
        .id = ACTION_ADD_STAR,
        .icon = "star",
        .command = "star",
        .tooltip = "&Star",
        .statustip = "Creates a star: STAR",
        .shortcut = ""
    },
    {
        .id = ACTION_SYSWINDOWS,
        .icon = "syswindows",
        .command = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows: SYSWINDOWS",
        .shortcut = ""
    },
    {
        .id = ACTION_TREBLECLEF,
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = ""
    },
    {
        .id = ACTION_EXIT,
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT.",
        .shortcut = ""
    },
    {
        .id = ACTION_TEST,
        .icon = "test",
        .command = "test",
        .tooltip = "Coverage Test",
        .statustip = "Run all commands at least once to test that nothing crashes.",
        .shortcut = ""
    },
    {
        .id = ACTION_SLEEP,
        .icon = "sleep",
        .command = "sleep",
        .tooltip = "Sleep for 1 second.",
        .statustip = "Sleep for 1 second.",
        .shortcut = ""
    },
    {
        .id = ACTION_END,
        .icon = "END",
        .command = "END",
        .tooltip = "END",
        .statustip = "END",
        .shortcut = "END"
    }
};
