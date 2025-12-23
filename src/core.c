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
 *
 * ----------------------------------------------------------------------------
 *
 * Users having the ability to reconfigure their software reflects a
 * core principle of open source software. The full build environment for
 * Embroidermodder is too complex for most users to set up, especially on
 * Windows. So the "freedom" to alter the program would be seldom used.
 * Being able to write macros and configuration files in any text editor
 * and add it to the build by adding a load call to "manifest.toml" is therefore a core
 * feature. We as core developers won't be using this flexibility much.
 *
 * Generally, each variable set in the configuration files (in the "data/" directory)
 * is a member of the global state variable. The state variable has a fixed memory
 * allocation so there are in-built, if large, size limitations on all of the
 * data loaded this way.
 *
 * TODO: check that the actions appropriately clear selections and rubber modes
 */

#include <assert.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "core.h"

#include "commands.h"
#include "designs.h"
#include "fills.h"
#include "generators.h"

/* This function intensionally does nothing. */
int
null_function(State *state)
{
    no_arguments(state);
    return 0;
}

State state = {
    .name = "Embroidermodder",
    .version = "2.0.0-alpha",
    .usage_msg = \
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
        "\n",

    .debug = 1,
    .play_mode = 0,
    .shift = 0,
    .numOfDocs = 0,
    .docIndex = 0,

    /* Function pointer tables */
    .command_list = {
        {
            .name = "about",
            .function = about_command
        },
        {
            .name = "arc",
            .function = arc_command
        },
        {
            .name = "changelog",
            .function = changelog_command
        },
        {
            .name = "circle",
            .function = circle_command
        },
        {
            .name = "clear",
            .function = clear_command
        },
        {
            .name = "clear_rubber",
            .function = clear_rubber_command
        },
        {
            .name = "copy",
            .function = copy_command
        },
        {
            .name = "cut",
            .function = cut_command
        },
        {
            .name = "day",
            .function = day_command
        },
        {
            .name = "delete",
            .function = delete_command
        },
        {
            .name = "details",
            .function = details_command
        },
        {
            .name = "dim_leader",
            .function = dim_leader_command
        },
        {
            .name = "distance",
            .function = distance_command
        },
        {
            .name = "ellipse",
            .function = ellipse_command
        },
        {
            .name = "end",
            .function = end_command
        },
        {
            .name = "exit",
            .function = exit_command
        },
        {
            .name = "fill",
            .function = fill_command
        },
        {
            .name = "generate",
            .function = generate_command
        },
        {
            .name = "get",
            .function = get_command
        },
        {
            .name = "help",
            .function = help_command
        },
        {
            .name = "horizontal_dim",
            .function = horizontal_dim_command
        },
        {
            .name = "icon_resize",
            .function = icon_resize_command
        },
        {
            .name = "image",
            .function = image_command
        },
        {
            .name = "infinite_line",
            .function = infinite_line_command
        },
        {
            .name = "line",
            .function = line_command
        },
        {
            .name = "macro",
            .function = macro_command
        },
        {
            .name = "move",
            .function = move_command
        },
        {
            .name = "new",
            .function = new_command
        },
        {
            .name = "night",
            .function = night_command
        },
        {
            .name = "open",
            .function = open_command
        },
        {
            .name = "pan",
            .function = pan_command
        },
        {
            .name = "paste",
            .function = paste_command
        },
        {
            .name = "path",
            .function = path_command
        },
        {
            .name = "platform",
            .function = platform_command
        },
        {
            .name = "play",
            .function = play_command
        },
        {
            .name = "point",
            .function = point_command
        },
        {
            .name = "polygon",
            .function = polygon_command
        },
        {
            .name = "polyline",
            .function = polyline_command
        },
        {
            .name = "print",
            .function = print_command
        },
        {
            .name = "ray",
            .function = ray_command
        },
        {
            .name = "rectangle",
            .function = rectangle_command
        },
        {
            .name = "redo",
            .function = redo_command
        },
        {
            .name = "regular_polygon",
            .function = regular_polygon_command
        },
        {
            .name = "repeat",
            .function = repeat_command
        },
        {
            .name = "rgb",
            .function = rgb_command
        },
        {
            .name = "rotate",
            .function = rotate_command
        },
        {
            .name = "rounded_rectangle",
            .function = rounded_rectangle_command
        },
        {
            .name = "save",
            .function = save_command
        },
        {
            .name = "save_as",
            .function = save_as_command
        },
        {
            .name = "scale",
            .function = scale_command
        },
        {
            .name = "script",
            .function = script_command
        },
        {
            .name = "select_all",
            .function = select_all_command
        },
        {
            .name = "set",
            .function = set_command
        },
        {
            .name = "settings",
            .function = settings_command
        },
        {
            .name = "sleep",
            .function = sleep_command
        },
        {
            .name = "slot",
            .function = slot_command
        },
        {
            .name = "stop",
            .function = stop_command
        },
        {
            .name = "stub",
            .function = stub_command
        },
        {
            .name = "text_multi",
            .function = text_multi_command
        },
        {
            .name = "text_single",
            .function = text_single_command
        },
        {
            .name = "triangle",
            .function = triangle_command
        },
        {
            .name = "undo",
            .function = undo_command
        },
        {
            .name = "updates",
            .function = updates_command
        },
        {
            .name = "vertical_dim",
            .function = vertical_dim_command
        },
        {
            .name = "whats_this",
            .function = whats_this_command
        },
        {
            .name = "window",
            .function = window_command
        },
        {
            .name = "zoom",
            .function = zoom_command
        },
        {
            .name = "_END",
            .function = null_function
        }
    },
    .design_list = {
        {
            .name = "dolphin",
            .function = dolphin_design
        },
        {
            .name = "heart",
            .function = heart_design
        },
        {
            .name = "snowflake",
            .function = snowflake_design
        },
        {
            .name = "star",
            .function = star_design
        },
        {
            .name = "_END",
            .function = null_function
        }
    },
    .fill_list = {
        {
            .name = "brick",
            .function = brick_fill
        },
        {
            .name = "gradient",
            .function = gradient_fill
        },
        {
            .name = "satin",
            .function = satin_fill
        },
        {
            .name = "_END",
            .function = null_function
        }
    },
    .generator_list = {
        {
            .name = "drawing",
            .function = drawing_generator
        },
        {
            .name = "guilloche",
            .function = guilloche_generator
        },
        {
            .name = "knot",
            .function = knot_generator
        },
        {
            .name = "photo",
            .function = photo_generator
        },
        {
            .name = "qr",
            .function = qr_generator
        },
        {
            .name = "_END",
            .function = null_function
        }
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
    .context = CONTEXT_MAIN,
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

void
no_arguments(State *state)
{
    if (state->argument_count > 0) {
        debug("WARNING: %s requires no arguments but %d were passed to it.",
            state->command, state->argument_count);
    }
}

/*
 * Identify command id by its name in the supplied table.
 */
int
get_index(FunctionPtr table[MAX_COMMANDS], char *cmd)
{
    for (int i=0; table[i].name[0]!='_'; i++) {
        if (!strncmp(table[i].name, cmd, 50)) {
            return i;
        }
    }
    return -1;
}

/*
 * Unpack a vector from arguments supplied to the command line.
 */
EmbVector
get_vector(State *state, int index)
{
    EmbVector v;
    v.x = atof(state->arguments[index]);
    v.y = atof(state->arguments[index+1]);
    return v;
}

/*
 * FIXME
 */
void
prompt_output(const char *msg)
{
    debug(msg);
}

/*
 * FIXME
 */
const char *
translate(const char *msg)
{
    return msg;
}


