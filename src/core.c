/*
 * Embroidermodder 2
 * Copyright 2011-2025 The Embroidermodder Team
 *
 * Embroidermodder 2 is free and open software under the zlib license:
 * see LICENSE.md for details.
 *
 * ----------------------------------------------------------------------------
 *
 * Core state manipulation tools and the declaration of the state.
 */

#include <assert.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core.h"

State state = {
    .debug = 1,
    .play_mode = 0,
    .shift = 0,
    .numOfDocs = 0,
    .docIndex = 0,

    /* String tables */
    .command_names = {
        [CMD_NULL] = "null",
        [CMD_ABOUT] = "about",
        [CMD_STUB] = "stub",
        [CMD_CUT] = "cut",
        [CMD_COPY] = "copy",
        [CMD_PASTE] = "paste",
        [CMD_SELECT_ALL] = "select_all",
        [CMD_DETAILS] = "details",
        [CMD_UPDATES] = "update",
        [CMD_WHATS_THIS] = "whats_this",
        [CMD_PRINT] = "print",
        [CMD_HELP] = "help",
        [CMD_CHANGELOG] = "changelog",
        [CMD_UNDO] = "undo",
        [CMD_REDO] = "redo",
        [CMD_REPEAT] = "repeat",
        [CMD_ICON16] = "icon16",
        [CMD_ICON24] = "icon24",
        [CMD_ICON32] = "icon32",
        [CMD_ICON48] = "icon48",
        [CMD_ICON64] = "icon64",
        [CMD_ICON128] = "icon128",
        [CMD_PLAY] = "play",
        [CMD_SLEEP] = "sleep",
        [CMD_NEW] = "new",
        [CMD_OPEN] = "open",
        [CMD_SAVE] = "save",
        [CMD_SAVE_AS] = "save_as",
        [CMD_PAN_LEFT] = "pan_left",
        [CMD_PAN_DOWN] = "pan_down",
        [CMD_PAN_RIGHT] = "pan_right",
        [CMD_PAN_UP] = "pan_up",
        [CMD_PAN_POINT] = "pan_point",
        [CMD_PAN_REAL_TIME] = "pan_real_time",
        [CMD_WINDOW_CASCADE] = "window_cascade",
        [CMD_WINDOW_CLOSE_ALL] = "window_close_all",
        [CMD_WINDOW_CLOSE] = "window_close",
        [CMD_WINDOW_NEXT] = "window_next",
        [CMD_WINDOW_PREVIOUS] = "window_previous",
        [CMD_WINDOW_TILE] = "window_tile",
        [CMD_ZOOM_ALL] = "zoom_all",
        [CMD_ZOOM_CENTER] = "zoom_center",
        [CMD_ZOOM_DYNAMIC] = "zoom_dynamic",
        [CMD_ZOOM_EXTENTS] = "zoom_extents",
        [CMD_ZOOM_IN] = "zoom_in",
        [CMD_ZOOM_OUT] = "zoom_out",
        [CMD_ZOOM_PREVIOUS] = "zoom_previous",
        [CMD_ZOOM_REAL_TIME] = "zoom_real_time",
        [CMD_ZOOM_SCALE] = "zoom_scale",
        [CMD_ZOOM_SELECTED] = "zoom_selected",
        [CMD_ZOOM_WINDOW] = "zoom_window",
        [CMD_DAY] = "day",
        [CMD_NIGHT] = "night",
        [CMD_CLEAR_RUBBER] = "clear_rubber",
        [CMD_CLEAR_SELECTION] = "clear",
        [CMD_END] = "end",
        [CMD_EXIT] = "exit",
        [CMD_MACRO] = "macro",
        [CMD_SCRIPT] = "run",
        [CMD_SETTINGS] = "settings",
        [CMD_SET] = "set",
        [CMD_GET] = "get",
        [CMD_TEXT_MULTI] = "text_multi",
        [CMD_TEXT_SINGLE] = "text_single",
        [CMD_INFINITE_LINE] = "infinite_line",
        [CMD_RAY] = "ray",
        [CMD_LINE] = "line",
        [CMD_TRIANGLE] = "triangle",
        [CMD_RECTANGLE] = "rectangle",
        [CMD_ROUNDED_RECTANGLE] = "rounded_rectangle",
        [CMD_ARC] = "arc",
        [CMD_CIRCLE] = "circle",
        [CMD_SLOT] = "slot",
        [CMD_ELLIPSE] = "ellipse",
        [CMD_POINT] = "point",
        [CMD_REGULAR_POLYGON] = "regular_polygon",
        [CMD_POLYGON] = "polygon",
        [CMD_POLYLINE] = "polyline",
        [CMD_PATH] = "path",
        [CMD_IMAGE] = "image",
        [CMD_DIM_LEADER] = "dim_leader",
        [CMD_HORIZONTAL_DIM] = "horizontal_dimension",
        [CMD_VERTICAL_DIM] = "vertical_dimension",
        [CMD_STOP] = "stop",
        [CMD_GENERATE] = "generate",
        [CMD_FILL] = "fill",
        [N_COMMANDS] = "_END"
    },
    .generate_list = {
        [GEN_PHOTO] = "photo",
        [GEN_DRAWING] = "drawing",
        [GEN_QR] = "qr",
        [GEN_GUILLOCHE] = "guilloche",
        [GEN_KNOT] = "knot",
        [N_GEN] = "_END"
    },
    .fill_list = {
        [FILL_SATIN] = "satin",
        [FILL_GRADIENT] = "gradient",
        [FILL_BRICK] = "brick",
        [N_FILLS] = "_END"
    },

    /* Testing */
    .testing = 0,
    .test_script_pos = 0,

    /* Simulation */
    .simulation_start = 0,
    /* In millimeters per second, so note that 1000.0 mm/s = 1 m/s.
     * TODO: This needs to be a setting.
     */
    .machine_speed = 1000.0f,
    .stitch_time = 0.05f,

    /* User interaction mode */
    .context_flag = CONTEXT_MAIN,
    .mode = 0
};

const char *usage_msg = \
    " ___ _____ ___  ___   __  _ ___  ___ ___   _____  __  ___  ___  ___ ___    ___ "           "\n" \
    "| __|     | _ \\| _ \\ /  \\| |   \\| __| _ \\ |     |/  \\|   \\|   \\| __| _ \\  |__ \\" "\n" \
    "| __| | | | _ <|   /| () | | |) | __|   / | | | | () | |) | |) | __|   /  / __/"           "\n" \
    "|___|_|_|_|___/|_|\\_\\\\__/|_|___/|___|_|\\_\\|_|_|_|\\__/|___/|___/|___|_|\\_\\ |___|"   "\n" \
    " _____________________________________________________________________________ "           "\n" \
    "|                                                                             | "          "\n" \
    "|                   http://embroidermodder.github.io                          | "          "\n" \
    "|_____________________________________________________________________________| "          "\n" \
    "                                                                               "           "\n" \
    "Usage: embroidermodder [options] files ..."                                      "\n" \
   //80CHARS======================================================================MAX
    "Options:"                                                                        "\n" \
    "  -d, --debug      Print lots of debugging information."                         "\n" \
    "  -h, --help       Print this message and exit."                                 "\n" \
    "  -v, --version    Print the version number of embroidermodder and exit."        "\n" \
    "\n";

const char* _appName_ = "Embroidermodder";
const char* _appVer_  = "v2.0 alpha";

uint8_t
willUnderflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a-b;
    return (c < INT_MIN || c > INT_MAX);
}

uint8_t
willOverflowInt32(int64_t a, int64_t b)
{
    assert(LLONG_MAX>INT_MAX);
    int64_t c = (int64_t)a+b;
    return (c < INT_MIN || c > INT_MAX);
}

int32_t
roundToMultiple(bool roundUp, int32_t numToRound, int32_t multiple)
{
    if (multiple == 0) {
        return numToRound;
    }
    int32_t remainder = numToRound % multiple;
    if (remainder == 0) {
        return numToRound;
    }

    if (numToRound < 0 && roundUp) {
        return numToRound - remainder;
    }
    if (roundUp) {
        return numToRound + multiple - remainder;
    }
    //else round down
    if (numToRound < 0 && !roundUp) {
        return numToRound - multiple - remainder;
    }
    return numToRound - remainder;
}

/*
 * Timestamps all debugging output with the ISO formatted GMT time
 * and writes it to the file "debug.log".
 */
void
debug(const char *msg, ...)
{
    char time_s[200];
    time_t t;
    char formatted_msg[1000];
    int n_args;
    va_list args;
    struct tm* time_data;
    char arg_fmt[10];

    /* Get time string, in case of failure return the error message in the
     * time_s string.
     */
    time(&t);
    time_data = gmtime(&t);
    if (time_data != NULL) {
        size_t bytes = strftime(time_s, sizeof(time_s),
            "%Y-%m-%d %H:%M:%S", time_data);
        if (bytes == 0) {
            sprintf(time_s, "strftime failed");
        }
    }
    else {
        sprintf(time_s, "localtime failed");
    }

    /* Argument parsing */
    int pos = 0;
    formatted_msg[0] = 0;
    va_start(args, msg);
    for (int i=0; msg[i]; i++) {
        /* If the formatting character '%' appears, and it is not followed
         * by '%' then we need to interpret another argument.
         */
        if ((msg[i] == '%') && (msg[i+1] != '%') && (msg[i])) {
            switch (msg[i+1]) {
            default:
            case 's':
                sprintf(formatted_msg, "%s%s",
                    formatted_msg, va_arg(args, const char*));
                break;
            case 'c':
                /* FIXME:
                sprintf(formatted_msg, "%s%c",
                    formatted_msg, va_arg(args, char));
                */
                break;
            case 'd':
                sprintf(formatted_msg, "%s%d",
                    formatted_msg, va_arg(args, int32_t));
                break;
            case 'f':
                sprintf(formatted_msg, "%s%f",
                    formatted_msg, va_arg(args, float));
                break;
            }
            pos = strlen(formatted_msg);
            i++;
        }
        else {
            formatted_msg[pos] = msg[i];
            formatted_msg[pos+1] = 0;
            pos++;
        }
    }
    va_end(args);

    /* Output */
    FILE *f = fopen("debug.log", "a");
    fprintf(f, "(%s) %s\n", time_s, formatted_msg);
    printf("(%s) %s\n", time_s, formatted_msg);
    fclose(f);
}

#if 0
/*
 * Lua interface to core commands.
 *
 * User based interaction comes via this interface so we can parse
 * the differences between interaction contexts.
 *
 * TODO: currently can't pass arguments through to core commands.
 */
int
cmd_f(lua_State *L)
{
    ScriptValue args[2];
    if (!unpack_args(L, "cmd_f", args, "s")) {
        return 0;
    }
    if (state.context_flag == CONTEXT_MAIN) {
        run_cmd("clear_rubber");
        /* Some selection based commands need to override this. */
        run_cmd("clear");
    }
    run_cmd(args[0].s);
    /* TODO: conditional on if the command is open ended or not. */
    run_cmd("end");
    return 0;
}

/**
 * @brief Set a state variable to a lua variable.
 *
 * EXAMPLE: set("text_font", "Arial")
 *
 * FIXME: this is an untested draft.
 */
int
set_f(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        debug("set: first argument not a string");
        return 0;
    }
    const char *key = lua_tostring(L, 1);
    ScriptValue value;
    value.b = false;
    if (lua_isboolean(L, 2)) {
        value.b = lua_toboolean(L, 2);
        return 0;
    }
    if (lua_isinteger(L, 2)) {
        value.i = lua_tointeger(L, 2);
        return 0;
    }
    if (lua_isstring(L, 2)) {
        strncpy(value.s, lua_tostring(L, 2), 200);
        return 0;
    }
    //FIXME: _mainWin->set(key, value);
    return 0;
}

/**
 * @brief Get a state variable and pass it as a lua variable.
 *
 * EXAMPLE: font = get("text_font")
 *
 * @fixme this is an untested draft.
 */
int
get_f(lua_State *L)
{
    if (!lua_isstring(L, 1)) {
        debug("get: first argument not a string");
        return 0;
    }
    //FIXME: ScriptValue result = _mainWin->get(lua_tostring(L, 1));
    //FIXME: lua_pushnumber(L, result.i);
    return 1;
}

/*
 * Load TOML data and initialise lua registerables.
 *
 * Lua in Embroidermodder 2 uses a 2 stage boot process.
 *
 * 1. Built-in style hookups to Embroidermodder2 features which are
 *    lua functions.
 * 2. Commands written in lua-only which represent the QActions used by
 *    the interface in the menus, toolbars and command line.
 *
 * Each command loaded in boot step 3 has up to 5 contexts
 * (see the list of defines in the headers: grep for "CONTEXT_") and will switch
 * depending on what context it is called in.
 */
bool
script_env_boot(void)
{
    /** Setting up Lua. */
    Lua = luaL_newstate();
    luaL_openlibs(Lua);

    /*
     * Bootstrapping function to support complex scripting.
     *
     * Allow scripts to act on the program state using the command line like this::
     *
     *     -- Convert an example file from Tajima dst to Brother pes format.
     *     cmd("open example.dst")
     *     cmd("saveas example.pes")
     */
    lua_register(Lua, "cmd", cmd_f);
    lua_register(Lua, "get", get_f);
    lua_register(Lua, "set", set_f);

    return true;
}

/**
 * @brief Free up the memory used by the scripting environment.
 *
 * This is necessary since all the script variables and headers are only
 * present in this file. Currently just the Lua state.
 */
void
script_env_free(void)
{
    lua_close(Lua);
}
#endif

