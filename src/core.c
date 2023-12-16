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
