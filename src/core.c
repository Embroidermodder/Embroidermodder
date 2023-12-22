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

/* C Standard Libraries. */
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

int
string_equal(const char *a, const char *b)
{
    return !strcmp(a, b);
}

int
find_command(const char *command)
{
	for (int i=0; i<N_COMMANDS; i++) {
		if (string_equal(command_table[i].command, command)) {
			return command_table[i].id;
		}
	}
	return COMMAND_DO_NOTHING;
}

/* The actuator changes the program state via these global variables.
 *
 * These copies of the settings struct are for restoring the state if
 * the user doesn't want to accept their changes in the settings dialog.
 */
Node settings[SETTINGS_TOTAL], dialog[SETTINGS_TOTAL],
    preview[SETTINGS_TOTAL], accept_[SETTINGS_TOTAL];

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

ToolbarData toolbar_data[MAX_TOOLBARS] = {
    {
        .id = TOOLBAR_FILE,
        .key = "File",
        .entries = {
            COMMAND_NEW,
            COMMAND_OPEN,
            COMMAND_SAVE,
            COMMAND_SAVEAS,
            COMMAND_PRINT,
            COMMAND_DESIGN_DETAILS,
            TOOLBAR_SEPERATOR,
            COMMAND_UNDO,
            COMMAND_REDO,
            TOOLBAR_SEPERATOR,
            COMMAND_HELP,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_EDIT,
        .key = "Edit",
        .entries = {
            COMMAND_CUT,
            COMMAND_COPY,
            COMMAND_PASTE,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_PAN,
        .key = "Pan",
        .entries = {
            COMMAND_PAN_REAL_TIME,
            COMMAND_PAN_POINT,
            TOOLBAR_SEPERATOR,
            COMMAND_PAN_LEFT,
            COMMAND_PAN_RIGHT,
            COMMAND_PAN_UP,
            COMMAND_PAN_DOWN,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_ZOOM,
        .key = "Zoom",
        .entries = {
            COMMAND_ZOOM_WINDOW,
            COMMAND_ZOOM_DYNAMIC,
            COMMAND_ZOOM_SCALE,
            TOOLBAR_SEPERATOR,
            COMMAND_ZOOM_CENTER,
            COMMAND_ZOOM_IN,
            COMMAND_ZOOM_OUT,
            TOOLBAR_SEPERATOR,
            COMMAND_ZOOM_SELECTED,
            COMMAND_ZOOM_ALL,
            COMMAND_ZOOM_EXTENTS,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_VIEW,
        .key = "View",
        .entries = {
            COMMAND_DAY,
            COMMAND_NIGHT,
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
            COMMAND_HELP,
            TOOLBAR_SEPERATOR,
            COMMAND_CHANGELOG,
            TOOLBAR_SEPERATOR,
            COMMAND_ABOUT,
            TOOLBAR_SEPERATOR,
            COMMAND_WHATS_THIS,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_DRAW,
        .key = "Draw",
        .entries = {
            COMMAND_ADD_POLYLINE,
            COMMAND_ADD_PATH,
            TOOLBAR_SEPERATOR,
            COMMAND_ADD_ARC,
            COMMAND_ADD_CIRCLE,
            COMMAND_ADD_ELLIPSE,
            COMMAND_ADD_LINE,
            COMMAND_ADD_POINT,
            COMMAND_ADD_REGULAR_POLYGON,
            COMMAND_ADD_POLYGON,
            COMMAND_ADD_TRIANGLE,
            TOOLBAR_SEPERATOR,
            COMMAND_ADD_HEART,
            COMMAND_ADD_DOLPHIN,
            COMMAND_TREBLECLEF,
            COMMAND_ADD_SNOWFLAKE,
            TOOLBAR_SEPERATOR,
            COMMAND_ADD_SINGLE_LINE_TEXT,
            COMMAND_ADD_TEXT_MULTI,
            COMMAND_ADD_TEXT_SINGLE,
            COMMAND_ADD_DIM_LEADER,
            COMMAND_ADD_VERTICAL_DIMENSION,
            COMMAND_ADD_HORIZONTAL_DIMENSION,
            COMMAND_ADD_IMAGE,
            COMMAND_ADD_INFINITE_LINE,
            COMMAND_ADD_RAY,
            COMMAND_ADD_RECTANGLE,
            COMMAND_ADD_ROUNDED_RECTANGLE,
            COMMAND_ADD_RUBBER,
            COMMAND_ADD_SLOT,
            TOOLBAR_END
        },
        .horizontal = 0
    },
    {
        .id = TOOLBAR_ICON,
        .key = "Icon",
        .entries = {
            COMMAND_ICON16,
            COMMAND_ICON24,
            COMMAND_ICON32,
            COMMAND_ICON48,
            COMMAND_ICON64,
            COMMAND_ICON128,
            TOOLBAR_END
        },
        .horizontal = 1
    },
    {
        .id = TOOLBAR_LAYER,
        .key = "Layer",
        .entries = {
            COMMAND_MAKE_LAYER_CURRENT,
            COMMAND_LAYER_EDITOR,
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

/*
general_props = [
    "icon-theme",
    "icon-size",
    "mdi-use-color",
    "mdi-use-logo",
    "mdi-use-texture",
    "mdi-color",
    "mdi-logo",
    "mdi-texture",
    "tip-of-the-day",
]
 */
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
    Sleep(1000);
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
    int64_t id = (int64_t)syscall(SYS_gettid);
    sprintf(thread_file, "debug-%ld.txt", id);
#endif
    FILE *f = fopen(thread_file, "a");
    int hours = (int)((ts.tv_sec/3600)%24);
    int minutes = (int)((ts.tv_sec%3600)/60);
    int seconds = (int)(ts.tv_sec%60);
    int milliseconds = (int)(ts.tv_nsec%1000);
    fprintf(f, "%2d:%2d:%2d.%3d> ",hours, minutes, seconds, milliseconds);
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

/* Possible flag system for commands. */

#define CMD_NO_ARG                      0x00
#define CMD_ONE_ARGS                    0x01
#define CMD_TWO_ARGS                    0x02
#define CMD_THREE_ARGS                  0x03
#define CMD_FOUR_ARGS                   0x04
#define CMD_FIVE_ARGS                   0x05
#define CMD_SIX_ARGS                    0x06
#define CMD_SEVEN_ARGS                  0x07
#define CMD_EIGHT_ARGS                  0x08

/* 0x08 = 0b1000, so other flags need to be 1 nibble over. */

#define CMD_NEEDS_VIEW           (0x01 << 4)
#define CMD_NEEDS_SCENE          (0x02 << 4)
#define CMD_CAN_UNDO             (0x04 << 4)
#define CMD_PATTERN              (0x07 << 4)
#define CMD_BASIC                       0x00

/* . */
CommandData command_table[MAX_COMMANDS] = {
    {
        .id = COMMAND_ABOUT, /* 0 */
        .icon = "about",
        .command = "about",
        .tooltip = "&About Embroidermodder 2",
        .statustip = "Displays information about this product: ABOUT.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_ADD_ARC, /* 1 */
        .icon = "arc",
        .command = "add-arc",
        .tooltip = "&Add Arc",
        .statustip = "Adds an arc to the vector layer.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /* .alias = "C", */
        .id = COMMAND_ADD_CIRCLE, /* 2 */
        .icon = "circle",
        .command = "circle",
        .tooltip = "Circle",
        .statustip = "Creates a circle: CIRCLE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        /* .alias = "C", */
        .id = COMMAND_ADD_DIM_LEADER, /* 3 */
        .icon = "circle",
        .command = "circle",
        .tooltip = "Circle",
        .statustip = "Creates a circle: CIRCLE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_ELLIPSE, /* 4 */
        .icon = "ellipse",
        .command = "ellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates a ellipse: ELLIPSE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_GEOMETRY, /* 5 */
        .icon = "ellipse",
        .command = "ellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates a ellipse: ELLIPSE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_HORIZONTAL_DIMENSION, /* 6 */
        .icon = "ellipse",
        .command = "ellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates a ellipse: ELLIPSE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_IMAGE, /* 7 */
        .icon = "ellipse",
        .command = "ellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates a ellipse: ELLIPSE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_INFINITE_LINE, /* 8 */
        .icon = "ellipse",
        .command = "ellipse",
        .tooltip = "Ellipse",
        .statustip = "Creates a ellipse: ELLIPSE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_LINE, /* 9 */
        .icon = "line",
        .command = "line",
        .tooltip = "&Line",
        .statustip = "Creates straight line segments: LINE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_PATH, /* 10 */
        .icon = "path",
        .command = "addpath",
        .tooltip = "&Path",
        .statustip = "Creates a 2D path: PATH.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_POINT, /* 11 */
        .icon = "point",
        .command = "point",
        .tooltip = "&Point",
        .statustip = "Creates multiple points: POINT.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_POLYGON, /* 12 */
        .icon = "polygon",
        .command = "polygon",
        .tooltip = "Pol&ygon",
        .statustip = "Creates a regular polygon: POLYGON.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_POLYLINE, /* 13 */
        .icon = "polyline",
        .command = "polyline",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline: PLINE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_RAY, /* 14 */
        .icon = "polyline",
        .command = "ray",
        .tooltip = "&Polyline",
        .statustip = "Creates a 2D polyline: PLINE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_RECTANGLE, /* 15 */
        .icon = "rectangle",
        .command = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_REGULAR_POLYGON, /* 16 */
        .icon = "rectangle",
        .command = "regular-polygon",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_ROUNDED_RECTANGLE, /* 17 */
        .icon = "rectangle",
        .command = "rounded-rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_RUBBER, /* 18 */
        .icon = "rectangle",
        .command = "rubber",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_SLOT, /* 19 */
        .icon = "rectangle",
        .command = "slot",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_TEXT_MULTI, /* 20 */
        .icon = "rectangle",
        .command = "text",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_TEXT_SINGLE, /* 21 */
        .icon = "rectangle",
        .command = "text-single",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_TO_SELECTION, /* 22 */
        .icon = "rectangle",
        .command = "add-to-selection",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_TRIANGLE, /* 23 */
        .icon = "rectangle",
        .command = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_VERTICAL_DIMENSION, /* 24 */
        .icon = "rectangle",
        .command = "rectangle",
        .tooltip = "&Rectangle",
        .statustip = "Creates a rectangular polyline: RECTANGLE.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ALERT, /* 25 */
        .icon = "warning",
        .command = "alert",
        .tooltip = "&Alert",
        .statustip = "Alert user of an issue.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ALLOW_RUBBER, /* 26 */
        .icon = "warning",
        .command = "alert",
        .tooltip = "&Alert",
        .statustip = "Alert user of an issue.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_APPEND_HISTORY, /* 27 */
        .icon = "warning",
        .command = "alert",
        .tooltip = "&Alert",
        .statustip = "Alert user of an issue.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_CALCULATE_ANGLE, /* 28 */
        .icon = "warning",
        .command = "alert",
        .tooltip = "&Alert",
        .statustip = "Alert user of an issue.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_CALCULATE_DISTANCE, /* 29 */
        .icon = "warning",
        .command = "alert",
        .tooltip = "&Alert",
        .statustip = "Alert user of an issue.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_CHANGELOG, /* 30 */
        .icon = "changelog",
        .command = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_CLEAR_RUBBER, /* 31 */
        .icon = "changelog",
        .command = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = ""
    },
    {
        .id = COMMAND_CLEAR_SELECTION, /* 32 */
        .icon = "changelog",
        .command = "changelog",
        .tooltip = "&Changelog",
        .statustip = "Describes new features in this product.",
        .shortcut = ""
    },
    {
        .id = COMMAND_COPY, /* 33 */
        .icon = "copy",
        .command = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+C",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_COPY_SELECTED, /* 34 */
        .icon = "copy",
        .command = "copy",
        .tooltip = "&Copy",
        .statustip = "Copy the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+C"
    },
    {
        .id = COMMAND_CUT, /* 35 */
        .icon = "cut",
        .command = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+X",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_CUT_SELECTED, /* 36 */
        .icon = "cut",
        .command = "cut",
        .tooltip = "Cu&t",
        .statustip = "Cut the current selection's contents to the clipboard.",
        .shortcut = "Ctrl+X"
    },
    {
        .id = COMMAND_DAY, /* 37 */
        .icon = "day",
        .command = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings: DAY.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DEBUG, /* 38 */
        .icon = "day",
        .command = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings: DAY.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_DELETE_SELECTED, /* 39 */
        .icon = "day",
        .command = "day",
        .tooltip = "&Day",
        .statustip = "Updates the current view using day vision settings: DAY.",
        .shortcut = ""
    },
    {
        .id = COMMAND_DESIGN_DETAILS, /* 40 */
        .icon = "designdetails",
        .command = "designdetails",
        .tooltip = "&Details",
        .statustip = "Details of the current design.",
        .shortcut = "Ctrl+D",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_DO_NOTHING, /* 41 */
        .icon = "donothing",
        .command = "donothing",
        .tooltip = "&Do Nothing",
        .statustip = "Does Nothing",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_END, /* 42 */
        .icon = "END",
        .command = "END",
        .tooltip = "END",
        .statustip = "END",
        .shortcut = "END"
    },
    {
        .id = COMMAND_ERROR, /* 43 */
        .icon = "END",
        .command = "END",
        .tooltip = "END",
        .statustip = "END",
        .shortcut = "END"
    },
    {
        .id = COMMAND_HELP, /* 44 */
        .icon = "help",
        .command = "help",
        .tooltip = "&Help",
        .statustip = "Displays the help file: HELP.",
        .shortcut = ""
    },
    {
        .id = COMMAND_ICON16, /* 45 */
        .icon = "icon16",
        .command = "icon 16",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_INIT, /* 46 */
        .icon = "init",
        .command = "init",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MESSAGEBOX, /* 47 */
        .icon = "init",
        .command = "init",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MIRROR_SELECTED, /* 48 */
        .icon = "init",
        .command = "init",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MOUSE_X, /* 49 */
        .icon = "init",
        .command = "init",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MOUSE_Y, /* 50 */
        .icon = "init",
        .command = "init",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MOVE_SELECTED, /* 51 */
        .icon = "init",
        .command = "init",
        .tooltip = "Icon&16",
        .statustip = "Sets the toolbar icon size to 16x16: ICON16",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_NEW, /* 52 */
        .icon = "new",
        .command = "new",
        .tooltip = "&New",
        .statustip = "Create a new file: NEW.",
        .shortcut = "Ctrl+N",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_NIGHT, /* 53 */
        .icon = "night_vision",
        .command = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings: NIGHT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_NUM_SELECTED, /* 54 */
        .icon = "night_vision",
        .command = "night",
        .tooltip = "&Night",
        .statustip = "Updates the current view using night vision settings: NIGHT",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_OPEN, /* 55 */
        .icon = "open",
        .command = "open",
        .tooltip = "&Open",
        .statustip = "Open an existing file: OPEN",
        .shortcut = "Ctrl+O",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PAN_UP, /* 56 */
        .icon = "pan-up",
        .command = "pan up",
        .tooltip = "Pan &Up",
        .statustip = "Pan the current view up: PAN UP",
        .shortcut = "up",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PASTE, /* 57 */
        .icon = "paste",
        .command = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "Ctrl+V",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PASTE_SELECTED, /* 58 */
        .icon = "paste",
        .command = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "Ctrl+V",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PERPENDICULAR_DISTANCE, /* 59 */
        .icon = "paste",
        .command = "paste",
        .tooltip = "&Paste",
        .statustip = "Paste the clipboard's contents into the current selection.",
        .shortcut = "",
        .min_args = 2,
        .gview = 1,
        .gscene = 1,
        .undo = 0
    },
    {
        .id = COMMAND_PLATFORM, /* 60 */
        .icon = "platform",
        .command = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use: PLATFORM.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PREVIEW_OFF, /* 61 */
        .icon = "platform",
        .command = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use: PLATFORM.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PREVIEW_ON, /* 62 */
        .icon = "platform",
        .command = "platform",
        .tooltip = "&Platform",
        .statustip = "List which platform is in use: PLATFORM.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PRINT, /* 63 */
        .icon = "print",
        .command = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_PRINT_AREA, /* 64 */
        .icon = "print",
        .command = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_QSNAP_X, /* 65 */
        .icon = "print",
        .command = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_QSNAP_Y, /* 66 */
        .icon = "print",
        .command = "print",
        .tooltip = "&Print",
        .statustip = "Print the design.",
        .shortcut = "Ctrl+P",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_EXIT, /* 67 */
        .icon = "exit",
        .command = "exit",
        .tooltip = "E&xit",
        .statustip = "Exit the application: EXIT",
        .shortcut = "Ctrl+Shift+Q",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_REDO, /* 68 */
        .icon = "redo",
        .command = "redo",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action: REDO",
        .shortcut = "Ctrl+Shift+Z",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ROTATE_SELECTED, /* 69 */
        .icon = "rotate",
        .command = "rotate",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action: REDO",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_RUBBER, /* 70 */
        .icon = "rotate",
        .command = "rotate",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action: REDO",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SCALE_SELECTED, /* 71 */
        .icon = "rotate",
        .command = "rotate",
        .tooltip = "&Redo",
        .statustip = "Reverses the effects of the previous undo action: REDO",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SELECT_ALL, /* 72 */
        .icon = "selectall",
        .command = "selectall",
        .tooltip = "&Select All",
        .statustip = "Selects all objects: SELECTALL.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SETTINGS_DIALOG, /* 73 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_BACKGROUND_COLOR, /* 74 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_CROSSHAIR_COLOR, /* 75 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_CURSOR_SHAPE, /* 76 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_GRID_COLOR, /* 77 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_PROMPT_PREFIX, /* 78 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_RUBBER_FILTER, /* 79 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_RUBBER_MODE, /* 80 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_RUBBER_POINT, /* 81 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SET_RUBBER_TEXT, /* 82 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_SPARE_RUBBER, /* 83 */
        .icon = "settingsdialog",
        .command = "settingsdialog",
        .tooltip = "&Settings",
        .statustip = "Configure settings specific to this product.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TIP_OF_THE_DAY, /* 84 */
        .icon = "tipoftheday",
        .command = "tips",
        .tooltip = "&Tip Of The Day",
        .statustip = "Displays a dialog with useful tips: TIPS",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TODO, /* 85 */
        .icon = "tipoftheday",
        .command = "todo",
        .tooltip = "&To Do",
        .statustip = "Adds an item to the debug.txt log to tell developers to fix this.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_UNDO, /* 86 */
        .icon = "undo",
        .command = "undo",
        .tooltip = "&Undo",
        .statustip = "Reverses the most recent action: UNDO",
        .shortcut = "Ctrl+Z",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_VERSION, /* 87 */
        .icon = "undo",
        .command = "version",
        .tooltip = "&Version",
        .statustip = "Prints out the running version of Embroidermodder.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_VULCANIZE, /* 88 */
        .icon = "undo",
        .command = "version",
        .tooltip = "&Version",
        .statustip = "Prints out the running version of Embroidermodder.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WHATS_THIS, /* 89 */
        .icon = "whatsthis",
        .command = "whatsthis",
        .tooltip = "&What's This?",
        .statustip = "What's This? Context Help!",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WINDOW_CLOSE, /* 90 */
        .icon = "windowclose",
        .command = "window close",
        .tooltip = "Cl&ose",
        .statustip = "Close the active window: CLOSE",
        .shortcut = "Ctrl+W",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WINDOW_CLOSE_ALL, /* 91 */
        .icon = "windowcloseall",
        .command = "window closeall",
        .tooltip = "Close &All",
        .statustip = "Close all the windows: CLOSEALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WINDOW_TILE, /* 92 */
        .icon = "windowtile",
        .command = "window tile",
        .tooltip = "&Tile",
        .statustip = "Tile the windows: TILE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WINDOW_CASCADE, /* 93 */
        .icon = "windowcascade",
        .command = "window cascade",
        .tooltip = "&Cascade",
        .statustip = "Cascade the windows: CASCADE",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WINDOW_NEXT, /* 94 */
        .icon = "windownext",
        .command = "window next",
        .tooltip = "Ne&xt",
        .statustip = "Move the focus to the next window: NEXT",
        .shortcut = "Ctrl+Tab",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_WINDOW_PREVIOUS, /* 95 */
        .icon = "windowprevious",
        .command = "window previous",
        .tooltip = "Pre&vious",
        .statustip = "Move the focus to the previous window: PREVIOUS",
        .shortcut = "Ctrl+Shift+Tab",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ZOOM_IN, /* 97 */
        .icon = "zoomin",
        .command = "zoom in",
        .tooltip = "Zoom &In",
        .statustip = "Zooms to increase the apparent size of objects: ZOOMIN",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEST, /* 98 */
        .icon = "test",
        .command = "test",
        .tooltip = "Coverage Test",
        .statustip = "Run all commands at least once to test that nothing crashes.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_SLEEP, /* 99 */
        .icon = "sleep",
        .command = "sleep",
        .tooltip = "Sleep for 1 second.",
        .statustip = "Sleep for 1 second.",
        .shortcut = "",
        .min_args = 0,
        .gview = 0,
        .gscene = 0,
        .undo = 0
    },
    {
        .id = COMMAND_LAYER_EDITOR, /* 100 */
        .icon = "sleep",
        .command = "sleep",
        .tooltip = "Sleep for 1 second.",
        .statustip = "Sleep for 1 second.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_MAKE_LAYER_CURRENT, /* 101 */
        .icon = "makelayercurrent",
        .command = "makelayercurrent",
        .tooltip = "&Make Layer Active",
        .statustip = "Makes the layer of a selected object the active layer",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEXT_BOLD, /* 102 */
        .icon = "textbold",
        .command = "text bold",
        .tooltip = "&Bold Text",
        .statustip = "Sets text to be bold.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEXT_ITALIC, /* 103 */
        .icon = "textitalic",
        .command = "text italic",
        .tooltip = "&Italic Text",
        .statustip = "Sets text to be italic.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEXT_UNDERLINE, /* 104 */
        .icon = "textunderline",
        .command = "text underline",
        .tooltip = "&Underline Text",
        .statustip = "Sets text to be underlined.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEXT_STRIKEOUT, /* 105 */
        .icon = "textstrikeout",
        .command = "text strikeout",
        .tooltip = "&StrikeOut Text",
        .statustip = "Sets text to be striked out.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_TEXT_OVERLINE, /* 106 */
        .icon = "textoverline",
        .command = "text overline",
        .tooltip = "&Overline Text",
        .statustip = "Sets text to be overlined.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LAYER_PREVIOUS, /* 107 */
        .icon = "layerprevious",
        .command = "layerprevious",
        .tooltip = "&Layer Previous",
        .statustip = "Restores the previous layer settings: LAYERP",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ICON24, /* 109 */
        .icon = "icon24",
        .command = "icon 24",
        .tooltip = "Icon&24",
        .statustip = "Sets the toolbar icon size to 24x24: ICON24.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_BASIC
    },
    {
        .id = COMMAND_ICON32, /* 110 */
        .icon = "icon32",
        .command = "icon 32",
        .tooltip = "Icon&32",
        .statustip = "Sets the toolbar icon size to 32x32: ICON32",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_BASIC
    },
    {
        .id = COMMAND_ICON48, /* 111 */
        .icon = "icon48",
        .command = "icon 48",
        .tooltip = "Icon&48",
        .statustip = "Sets the toolbar icon size to 48x48: ICON48",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_BASIC
    },
    {
        .id = COMMAND_ICON64, /* 112 */
        .icon = "icon64",
        .command = "icon 64",
        .tooltip = "Icon&64",
        .statustip = "Sets the toolbar icon size to 64x64: ICON64",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_BASIC
    },
    {
        .id = COMMAND_ICON128, /* 113 */
        .icon = "icon128",
        .command = "icon 128",
        .tooltip = "Icon12&8",
        .statustip = "Sets the toolbar icon size to 128x128: ICON128",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_BASIC
    },
    {
        .id = COMMAND_SAVE, /* 114 */
        .icon = "save",
        .command = "save",
        .tooltip = "&Save",
        .statustip = "Save the design to disk.",
        .shortcut = "Ctrl+S",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_SAVEAS, /* 115 */
        .icon = "saveas",
        .command = "saveas",
        .tooltip = "Save &As",
        .statustip = "Save the design under a new name and type.",
        .shortcut = "Ctrl+Shift+S",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_PAN_REAL_TIME, /* 116 */
        .icon = "panrealtime",
        .command = "pan realtime",
        .tooltip = "&Pan Realtime",
        .statustip = "Moves the view in the current viewport.",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_PAN_POINT, /* 117 */
        .icon = "panpoint",
        .command = "pan point",
        .tooltip = "&Pan Point",
        .statustip = "Moves the view by the specified distance.",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_PAN_LEFT, /* 118 */
        .icon = "panleft",
        .command = "pan left",
        .tooltip = "&Pan Left",
        .statustip = "Moves the view to the left: PANLEFT",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_PAN_RIGHT, /* 119 */
        .icon = "panright",
        .command = "pan right",
        .tooltip = "&Pan Right",
        .statustip = "Moves the view to the right: PANRIGHT",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_PAN_DOWN, /* 121 */
        .icon = "pandown",
        .command = "pan down",
        .tooltip = "&Pan Down",
        .statustip = "Moves the view down: PANDOWN",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ZOOM_REAL_TIME, /* 122 */
        .icon = "zoomrealtime",
        .command = "zoom realtime",
        .tooltip = "Zoom &Realtime",
        .statustip = "Zooms to increase or decrease the apparent size of objects in the current viewport.",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ZOOM_PREVIOUS, /* 123 */
        .icon = "zoomprevious",
        .command = "zoom previous",
        .tooltip = "Zoom &Previous",
        .statustip = "Zooms to display the previous view.",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ZOOM_WINDOW, /* 124 */
        .icon = "zoomwindow",
        .command = "zoom window",
        .tooltip = "Zoom &Window",
        .statustip = "Zooms to display an area specified by a rectangular window.",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ZOOM_DYNAMIC, /* 125 */
        .icon = "zoomdynamic",
        .command = "zoom dynamic",
        .tooltip = "Zoom &Dynamic",
        .statustip = "Zooms to display the generated portion of the drawing.",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ZOOM_OUT, /* 126 */
        .icon = "zoomout",
        .command = "zoom out",
        .tooltip = "Zoom &Out",
        .statustip = "Zooms to decrease the apparent size of objects: ZOOMOUT",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ZOOM_EXTENTS, /* 127 */
        .icon = "zoomextents",
        .command = "zoom extents",
        .tooltip = "Zoom &Extents",
        .statustip = "Zooms to display the drawing extents: ZOOMEXTENTS",
        .shortcut = "",
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_LAYERS, /* 128 */
        .icon = "layers",
        .command = "layers",
        .tooltip = "&Layers",
        .statustip = "Manages layers and layer properties:  LAYER",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_LAYER_SELECTOR, /* 129 */
        .icon = "layerselector",
        .command = "layerselector",
        .tooltip = "&Layer Selector",
        .statustip = "Dropdown selector for changing the current layer",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_TREBLECLEF, /* 130 */
        .icon = "trebleclef",
        .command = "trebleclef",
        .tooltip = "Treble Clef",
        .statustip = "Adds a treble clef design to the vector layer.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_COLOR_SELECTOR, /* 131 */
        .icon = "colorselector",
        .command = "colorselector",
        .tooltip = "&Color Selector",
        .statustip = "Dropdown selector for changing the current thread color",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_LINE_TYPE_SELECTOR, /* 132 */
        .icon = "linetypeselector",
        .command = "linetypeselector",
        .tooltip = "&Stitchtype Selector",
        .statustip = "Dropdown selector for changing the current stitch type",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_LINE_WEIGHT_SELECTOR, /* 133 */
        .icon = "lineweightselector",
        .command = "lineweightselector",
        .tooltip = "&Threadweight Selector",
        .statustip = "Dropdown selector for changing the current thread weight",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ZOOM_SCALE, /* 134 */
        .icon = "zoomscale",
        .command = "zoom scale",
        .tooltip = "Zoom &Scale",
        .statustip = "Zooms the display using a specified scale factor.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ZOOM_CENTER, /* 135 */
        .icon = "zoomcenter",
        .command = "zoom center",
        .tooltip = "Zoom &Center",
        .statustip = "Zooms to display a view specified by a center point and magnification or height.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_HIDE_ALL_LAYERS, /* 136 */
        .icon = "hidealllayers",
        .command = "hidealllayers",
        .tooltip = "&Hide All Layers",
        .statustip = "Turns the visibility off for all layers in the current drawing:  HIDEALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ZOOM_SELECTED, /* 137 */
        .icon = "zoomselected",
        .command = "zoom selected",
        .tooltip = "Zoom Selec&ted",
        .statustip = "Zooms to display the selected objects.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ZOOM_ALL, /* 138 */
        .icon = "zoomall",
        .command = "zoom all",
        .tooltip = "Zoom &All",
        .statustip = "Zooms to display the drawing extents or the grid limits.",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_ADD_HEART, /* 139 */
        .icon = "heart",
        .command = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart: HEART",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ADD_SINGLE_LINE_TEXT, /* 140 */
        .icon = "heart",
        .command = "heart",
        .tooltip = "&Heart",
        .statustip = "Creates a heart: HEART",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_SHOW_ALL_LAYERS, /* 141 */
        .icon = "showalllayers",
        .command = "showalllayers",
        .tooltip = "&Show All Layers",
        .statustip = "Turns the visibility on for all layers in the current drawing:  SHOWALL",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_FREEZE_ALL_LAYERS, /* 142 */
        .icon = "freezealllayers",
        .command = "freezealllayers",
        .tooltip = "&Freeze All Layers",
        .statustip = "Freezes all layers in the current drawing:  FREEZEALL",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_THAW_ALL_LAYERS, /* 143 */
        .icon = "thawlllayers",
        .command = "thawalllayers",
        .tooltip = "&Thaw All Layers",
        .statustip = "Thaws all layers in the current drawing:  THAWALL",
        .shortcut = "",
        .min_args = 0,
        .gview = 1,
        .gscene = 1,
        .undo = 1
    },
    {
        .id = COMMAND_LOCK_ALL_LAYERS, /* 144 */
        .icon = "lockalllayers",
        .command = "lockalllayers",
        .tooltip = "&Lock All Layers",
        .statustip = "Locks all layers in the current drawing:  LOCKALL",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_UNLOCK_ALL_LAYERS, /* 145 */
        .icon = "unlockalllayers",
        .command = "unlockalllayers",
        .tooltip = "&Unlock All Layers",
        .statustip = "Unlocks all layers in the current drawing:  UNLOCKALL",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ADD_DOLPHIN, /* 146 */
        .icon = "dolphin",
        .command = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Adds a dolphin design to the vector layer: DOLPHIN.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Draw",
        toolbar = "Draw",
        */
        .id = COMMAND_ADD_DISTANCE, /* 147 */
        .icon = "dolphin",
        .command = "dolphin",
        .tooltip = "&Dolphin",
        .statustip = "Adds a dolphin design to the vector layer: DOLPHIN.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Tools",
        toolbar = "Inquiry",
        */
        .id = COMMAND_LOCATE_POINT, /* 148 */
        .icon = "locatepoint",
        .command = "locatepoint",
        .tooltip = "&Locate Point",
        .statustip = "Displays the coordinate values of a location: ID.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_QUICKSELECT, /* 149 */
        .icon = "quickselect",
        .command = "quickselect",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /* todo */
        .id = COMMAND_SPELLCHECK, /* 150 */
        .icon = "spellcheck",
        .command = "spellcheck",
        .tooltip = "",
        .statustip = "",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Tools",
        toolbar = "Inquiry",
        */
        .id = COMMAND_DISTANCE, /* 151 */
        .icon = "distance",
        .command = "distance",
        .tooltip = "&Distance",
        .statustip = "Measures the distance and angle between two points: DIST",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify"
        */
        .id = COMMAND_MOVE, /* 152 */
        .icon = "move",
        .command = "move",
        .tooltip = "&Move",
        .statustip = "Displaces objects a specified distance in a specified direction: MOVE.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Dimension",
        toolbar = "Dimension",
        */
        .id = COMMAND_QUICKLEADER, /* 153 */
        .icon = "quickleader",
        .command = "addquickleader",
        .tooltip = "&QuickLeader",
        .statustip = "Creates a leader and annotation: QUICKLEADER.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Sandbox"
        toolbar = "Sandbox"
        */
        .id = COMMAND_RGB, /* 154 */
        .icon =  "rgb",
        .command = "rgb",
        .tooltip =  "&RGB",
        .statustip = "Updates the current view colors: RGB.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = COMMAND_ROTATE, /* 155 */
        .icon = "rotate",
        .command = "rotate",
        .tooltip = "&Rotate",
        .statustip = "Rotates objects about a base point: ROTATE.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Sandbox",
        toolbar = "Sandbox",
        */
        .id = COMMAND_SANDBOX, /* 156 */
        .icon = "sandbox",
        .command = "sandbox",
        .tooltip = "Sandbox",
        .statustip = "A sandbox to play in: SANDBOX.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ADD_SNOWFLAKE, /* 157 */
        .icon = "snowflake",
        .command = "snowflake",
        .tooltip = "&Snowflake",
        .statustip = "Creates a snowflake: SNOWFLAKE",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_ADD_STAR, /* 158 */
        .icon = "star",
        .command = "star",
        .tooltip = "&Star",
        .statustip = "Creates a star: STAR",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = COMMAND_DELETE, /* 159 */
        .icon = "erase",
        .command = "erase",
        .tooltip = "D&elete",
        .statustip = "Removes objects from a drawing: DELETE.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        /*
        menu = "Modify",
        toolbar = "Modify",
        */
        .id = COMMAND_SCALE, /* 160 */
        .icon = "scale",
        .command = "scale",
        .tooltip = "Sca&le",
        .statustip = "Enlarges or reduces objects proportionally in the X, Y, and Z directions: SCALE.",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_SINGLE_LINE_TEXT, /* 161 */
        .icon = "single-line-text",
        .command = "singlelinetext",
        .tooltip = "&Single Line Text",
        .statustip = "Creates single-line text objects: TEXT",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    },
    {
        .id = COMMAND_SYSWINDOWS, /* 162 */
        .icon = "syswindows",
        .command = "syswindows",
        .tooltip = "&SysWindows",
        .statustip = "Arrange the windows: SYSWINDOWS",
        .shortcut = "",
        .min_args = 0,
        .flags = CMD_PATTERN
    }
	/* ------------------ N_COMMANDS -------------------- */
};


WidgetData grid_geometry_widgets[] = {
	{
		.type = WIDGET_LABEL,
		.label = "Grid Center X",
        .position = {3, 0}
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
		.label = "Grid Center X",
		.single_step = 1.0,
		.lower = -1000.0,
		.upper = 1000.0,
        .key = ST_GRID_CENTER_X,
        .position = {3, 1}
	},
	{
		.type = WIDGET_LABEL,
		.label = "Grid Center Y"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
		.label = "Grid Center Y",
		.single_step = 1.0,
		.lower = -1000.0,
		.upper = 1000.0,
        .key = ST_GRID_CENTER_Y
	},
	{
		.type = WIDGET_LABEL,
		.label = "Grid Size X"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
		.label = "Grid Size X",
		.single_step = 1.0,
		.lower = 1.0,
		.upper = 1000.0,
        .key = ST_GRID_SIZE_X
	},
	{
		.type = WIDGET_LABEL,
		.label = "Grid Size Y"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
		.label = "Grid Size Y",
		.single_step = 1.0,
		.lower = 1.0,
		.upper = 1000.0,
        .key = ST_GRID_SIZE_Y
	},
    {
		.type = WIDGET_LABEL,
        .label = "Grid Spacing X"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Spacing X",
        .single_step = 1.0,
        .lower = 1.0,
        .upper = 1000.0,
        .key = ST_GRID_SPACING_X
	},
	{
		.type = WIDGET_LABEL,
        .label = "Grid Spacing Y"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Spacing Y",
        .single_step = 1.0,
        .lower = 0.001,
        .upper = 1000.0,
        .key = ST_GRID_SPACING_Y
	},
	{
		.type = WIDGET_LABEL,
        .label = "Grid Size Radius"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Size Radius",
        .single_step = 1.0,
        .lower = 0.001,
        .upper = 1000.0,
        .key = ST_GRID_SIZE_RADIUS
	},
	{
		.type = WIDGET_LABEL,
        .label = "Grid Spacing Radius"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Spacing Radius",
        .single_step = 1.0,
        .lower = 0.001,
        .upper = 1000.0,
        .key = ST_GRID_SPACING_RADIUS
	},
	{
		.type = WIDGET_LABEL,
        .label = "Grid Spacing Angle"
	},
	{
		.type = WIDGET_DOUBLE_SPINBOX,
        .label = "Grid Spacing Angle",
        .single_step = 1.0,
        .lower = 0.001,
        .upper = 180.0,
        .key = ST_GRID_SPACING_ANGLE
	},
    {
        .type = -1
    }
};

WidgetData qsnap_mode_checkboxes[] = {
	{
		.type = WIDGET_CHECKBOX,
		.label = "Endpoint",
		.key = ST_QSNAP_ENDPOINT
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Midpoint",
        .key = ST_QSNAP_MIDPOINT
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Center",
        .key = ST_QSNAP_CENTER
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Node",
        .key = ST_QSNAP_NODE
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Quadrant",
        .key = ST_QSNAP_QUADRANT
	},
	{
		.type = WIDGET_CHECKBOX,
		.label = "Intersection",
		.key = ST_QSNAP_INTERSECTION
	},
	{
		.type = WIDGET_CHECKBOX,
		.label = "Extension",
		.key = ST_QSNAP_EXTENSION
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Insertion",
        .key = ST_QSNAP_INSERTION
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Perpendicular",
        .key = ST_QSNAP_PERPENDICULAR
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Tangent",
        .key = ST_QSNAP_TANGENT
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Nearest",
        .key = ST_QSNAP_NEAREST
	},
	{
		.type = WIDGET_CHECKBOX,
        .label = "Apparent Intersection",
        .key = ST_QSNAP_APPARENT
	},
	{
		.type = WIDGET_CHECKBOX,
		.label = "Parallel",
		.key = ST_QSNAP_PARALLEL
	},
	{
		.type = -1
	}
};

