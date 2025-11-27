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
 * Main entry point for the experimental SDL version of Embroidermodder 2.
 *
 * This is mainly a graphical frontend to as much as possible of the same
 * codebase as the Qt version.
 */

#include <assert.h>
#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "core.h"
#include "toml.h"

#define WIDGET_BUTTON                  0
#define WIDGET_SPINBOX                 1
#define WIDGET_CHECKBOX                2
#define WIDGET_LABEL                   3

#define UI_VIEW                        0
#define UI_TOOLBAR                     1
#define UI_MESSAGEBOX                  2
#define UI_PROMPT                      3
#define UI_                      4

typedef struct Widget_ {
    SDL_FRect bounds;
    unsigned char color[4];
    unsigned char data[200];
    int type;
    int layer;
} Widget;

State state = {
    /* Meta */
    .name = "Embroidermodder",
    .version  = "v2.0 alpha",

    /* Modes */
    .debug = 1,
    .play_mode = 0,
    .shift = 0,
    .numOfDocs = 0,
    .docIndex = 0,

    /* Widgets */

    /* String tables */
    .groupbox_order = {
        END_SYMBOL
    },
    .menubar_order = {
        END_SYMBOL
    },
    .top_toolbar_order = {
        END_SYMBOL
    },
    .bottom_toolbar_order = {
        END_SYMBOL
    },
    .left_toolbar_order = {
        END_SYMBOL
    },
    .recent_files = {
        END_SYMBOL
    },
    /* TODO: Keep this list alphabetized with the exception of the null command
     * which is kept in position zero for clarity and the terminator which
     * has to be at the end to determining the length of the array.
     */
    .commands = {
        {
            .name = "null",
            .function = null_command
        },
        {
            .name = "about",
            .function = about_command
        },
        {
            .name = "stub",
            .function = stub_command
        },
        {
            .name = "cut",
            .function = cut_command
        },
        {
            .name = "copy",
            .function = copy_command
        },
        {
            .name = "paste",
            .function = paste_command
        },
        {
            .name = "select_all",
            .function = select_all_command
        },
        {
            .name = "details",
            .function = details_command
        },
        {
            .name = "update",
            .function = update_command
        },
        {
            .name = "whats_this",
            .function = whats_this_command
        },
        {
            .name = "print",
            .function = print_command
        },
        {
            .name = "help",
            .function = help_command
        },
        {
            .name = "changelog",
            .function = changelog_command
        },
        {
            .name = "undo",
            .function = undo_command
        },
        {
            .name = "redo",
            .function = redo_command
        },
        {
            .name = "repeat",
            .function = repeat_command
        },
        {
            .name = "icon",
            .function = icon_command
        },
        {
            .name = "play",
            .function = play_command
        },
        {
            .name = "sleep",
            .function = sleep_command
        },
        {
            .name = "new",
            .function = new_command
        },
        {
            .name = "open",
            .function = open_command
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
            .name = "pan",
            .function = pan_command
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
            .name = "day",
            .function = day_command
        },
        {
            .name = "night",
            .function = night_command
        },
        {
            .name = "clear_rubber",
            .function = clear_rubber_command
        },
        {
            .name = "clear",
            .function = clear_command
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
            .name = "macro",
            .function = macro_command
        },
        {
            .name = "run",
            .function = run_command
        },
        {
            .name = "settings",
            .function = settings_command
        },
        {
            .name = "set",
            .function = set_command
        },
        /*
        {
            .name = "get",
            .function = null_command
        },
        */
        {
            .name = "text_multi",
            .function = text_multi_command
        },
        {
            .name = "text_single",
            .function = text_single_command
        },
        {
            .name = "infinite_line",
            .function = infinite_line_command
        },
        {
            .name = "ray",
            .function = ray_command
        },
        {
            .name = "line",
            .function = line_command
        },
        {
            .name = "triangle",
            .function = triangle_command
        },
        {
            .name = "rectangle",
            .function = rectangle_command
        },
        {
            .name = "rounded_rectangle",
            .function = rounded_rectangle_command
        },
        {
            .name = "arc",
            .function = arc_command
        },
        {
            .name = "circle",
            .function = circle_command
        },
        {
            .name = "slot",
            .function = slot_command
        },
        {
            .name = "ellipse",
            .function = ellipse_command
        },
        {
            .name = "point",
            .function = point_command
        },
        {
            .name = "regular_polygon",
            .function = regular_polygon_command
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
            .name = "path",
            .function = path_command
        },
        {
            .name = "image",
            .function = image_command
        },
        {
            .name = "dim_leader",
            .function = dim_leader_command
        },
        {
            .name = "horizontal_dimension",
            .function = horizontal_dimension_command
        },
        {
            .name = "vertical_dimension",
            .function = vertical_dimension_command
        },
        {
            .name = "stop",
            .function = stop_command
        },
        {
            .name = "generate",
            .function = generate_command
        },
        {
            .name = "fill",
            .function = fill_command
        },
        {
            .name = "_END",
            .function = null_command
        }
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

EmbStringTable recent_files;

int interface_padding = 10;
int menubar_height = 40;
int toolbar_height = 40;
int window_width = 640;
int window_height = 480;

void render_label(SDL_Renderer *renderer, Widget *widget);
void render_button(SDL_Renderer *renderer, Widget *widget);
void render_error(SDL_Renderer *renderer, Widget *widget);
void render_widget(SDL_Renderer *renderer, Widget *widget);
void render_menubar(SDL_Renderer *renderer);
void render_toolbars(SDL_Renderer *renderer, int n_toolbars);
void render_mdiarea(SDL_Renderer *renderer);
void render_view(SDL_Renderer *renderer);
void render_prompt(SDL_Renderer *renderer);

void
sdl_version(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        debug("ERROR: Failed to initialize SDL.");
        return;
    }

    if (!SDL_CreateWindowAndRenderer("Embroidermodder 2.0.0-alpha",
        window_width, window_height, 0, &window, &renderer)) {
        debug("ERROR: Failed to create window or renderer.");
        return;
    }

    surface = SDL_LoadBMP("images/logo-spirals.bmp");
    if (!surface) {
        debug("ERROR: Failed to create main window surface.");
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        debug("ERROR: Failed to create main window texture.");
        return;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        render_menubar(renderer);
        render_toolbars(renderer, 2);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

/*
 */
void
render_label(SDL_Renderer *renderer, Widget *widget)
{
    SDL_SetRenderDrawColor(renderer,
        widget->color[0],
        widget->color[1],
        widget->color[2],
        widget->color[3]);
    SDL_RenderFillRect(renderer, &(widget->bounds));
}

/*
 */
void
render_button(SDL_Renderer *renderer, Widget *widget)
{
    SDL_SetRenderDrawColor(renderer,
        widget->color[0],
        widget->color[1],
        widget->color[2],
        widget->color[3]);
    SDL_RenderFillRect(renderer, &(widget->bounds));
}

/*
 */
void
render_spinbox(SDL_Renderer *renderer, Widget *widget)
{
    SDL_SetRenderDrawColor(renderer,
        widget->color[0],
        widget->color[1],
        widget->color[2],
        widget->color[3]);
    SDL_RenderFillRect(renderer, &(widget->bounds));
}

/*
 */
void
render_checkbox(SDL_Renderer *renderer, Widget *widget)
{
    SDL_SetRenderDrawColor(renderer,
        widget->color[0],
        widget->color[1],
        widget->color[2],
        widget->color[3]);
    SDL_RenderFillRect(renderer, &(widget->bounds));
}

/*
 */
void
render_error(SDL_Renderer *renderer, Widget *widget)
{
    SDL_SetRenderDrawColor(renderer,
        widget->color[0],
        widget->color[1],
        widget->color[2],
        widget->color[3]);
    SDL_RenderFillRect(renderer, &(widget->bounds));
}

/*
 */
void
render_widget(SDL_Renderer *renderer, Widget *widget)
{
    switch (widget->type) {
    case WIDGET_BUTTON:
        render_button(renderer, widget);
        break;
    case WIDGET_SPINBOX:
        render_spinbox(renderer, widget);
        break;
    case WIDGET_CHECKBOX:
        render_checkbox(renderer, widget);
        break;
    case WIDGET_LABEL:
        render_label(renderer, widget);
        break;
    default:
        debug("ERROR: widget type unknown, renderering as a filled rectangle.");
        render_error(renderer, widget);
        break;
    }
}

/* TODO: establish layers to order how widgets are rendered.
 */
void
render_all_widgets(SDL_Renderer *renderer, Widget *widget_list, int n_widgets)
{
    for (int i=0; i<n_widgets; i++) {
        render_widget(renderer, widget_list);
    }
}

/*
 */
void
render_menubar(SDL_Renderer *renderer)
{
    SDL_FRect rect;
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
    rect.x = 0;
    rect.y = 0;
    rect.w = window_width;
    rect.h = menubar_height;
    SDL_RenderFillRect(renderer, &rect);
}

/*
 */
void
render_toolbars(SDL_Renderer *renderer, int n_toolbars)
{
    SDL_FRect rect;
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0x00);
    rect.x = 0;
    rect.y = menubar_height + interface_padding;
    rect.w = window_width;
    rect.h = n_toolbars * toolbar_height + (n_toolbars - 1) * interface_padding;
    SDL_RenderFillRect(renderer, &rect);
}

/*
 */
void
render_mdiarea(SDL_Renderer *renderer)
{
}

/*
 */
void
render_view(SDL_Renderer *renderer)
{
}

/*
 */
void
render_prompt(SDL_Renderer *renderer)
{
}

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

/*
 * To avoid unused arguments warnings and catch stray arguments in macros
 * we warn the user that they have passed arguments to functions that take
 * none. These are listed out in the debug.
 *
 * For functions that use more than the expected number of arguments
 * use "excess_arguments".
 *
 * This is never considered a failure condition for a function.
 */
void
no_arguments(const char *name, int argc, char *argv[])
{
    if (argc > 0) {
        int i;
        debug("%s expected no arguments but these were supplied:", name);
        for (i = 0; i < argc; i++) {
            debug("    %d: %s", i, argv[i]);
        }
    }
}

/*
 * Report to the debug log that the current command supplied more arguments
 * than the function expected.
 *
 * If the number of arguments is not the expected number then this is potentially
 * an error, particularly if the number of arguments is less than the expected
 * amount.
 */
int
excess_arguments(const char *name, int expected, int argc, char *argv[])
{
    if (argc > expected) {
        int i;
        debug("%s expected %d arguments but these were also supplied:", name, expected);
        for (i = expected; i < argc; i++) {
            debug("    %d: %s", i, argv[i]);
        }
        return 1;
    }
    if (argc < expected) {
        return -1;
    }
    return 0;
}

/*
 * If the string appears not to be specifically "false", "False" or "0" then
 * we can assume it to be true. So we just check that the first character doesn't
 * match these.
 *
 * In C, the if statement goes ahead with all values other than 0, so we can take
 * the view that "15" is "true" for example.
 */
uint8_t
to_boolean(const char *value)
{
    return !((value[0] == 'f') || (value[0] == 'F') || (value[0] == '0'));
}

