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
    int remainder = numToRound % multiple;
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

