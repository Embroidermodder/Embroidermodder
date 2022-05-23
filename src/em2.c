/*
 *  Embroidermodder 2.
 *
 *  ------------------------------------------------------------
 *
 *  Copyright 2013-2022 The Embroidermodder Team
 *  Embroidermodder 2 is Open Source Software.
 *  See LICENSE for licensing terms.
 *
 *  ------------------------------------------------------------
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "em2.h"

Window *mainwnd;

int
load_scheme_file(scheme *sc, char *fname)
{
    FILE *fin;
    fin = fopen(fname, "r");
    if (!fin) {
        puts("Failed to load initiation file.");
        return 3;
    }
    scheme_load_named_file(sc, fin, fname);
    fclose(fin);
    return 0;
}

char *
load_str(scheme *sc, char *key)
{
    pointer b = scheme_apply0(sc, key);
    return scheme_string_value(b);
}

char *
load_str_from_table(scheme *sc, char *key, int i)
{
    char call[200];
    pointer b;
    sprintf(call, "(define (output) (vector-ref (%s) %d))", key, i);
    scheme_load_string(sc, call);
    return load_str(sc, "output");
}

double
load_real(scheme *sc, char *key)
{
    pointer b = scheme_apply0(sc, key);
    return scheme_rvalue(b);
}

int
load_int(scheme *sc, char *key)
{
    pointer b = scheme_apply0(sc, key);
    return scheme_ivalue(b);
}

/*
 * ACTUATOR
 *
 * In order to have a complex version of saving work, with backups,
 * undo history and forks we need a good recording of what has happened.
 *
 * An action has been taken, we are at the current head of the stack.
 *
 * The action string is a (hopefully valid) lisp expression
 * that is sent to the scheme state.
 */
int
actuator(scheme *sc, char *action)
{
    set_int(sc, "undo-history-position",
        load_int(sc, "undo-history-position")+1);
    /*
    if (load_int("undo-history-max")
        <= load_int("undo-history-position")) {
        int i;
        for (i=0; i<load_int("undo-history-max")-undo_history_chunk_size; i++) {
            strcpy(undo_history[i], undo_history[i+undo_history_chunk_size]);
        }
        undo_history_position -= undo_history_chunk_size;
    }
    
    strcpy(undo_history[undo_history_position], action);
    */

    printf("action: %s\n", action);
    scheme_apply0(sc, action);

    return 0;
}

/* Function definitions */
int
main(int argc, char *argv[])
{
    int debug_mode = 0, testing = 0;
    int file, i;

    for (i=1; i<argc; i++) {
        if (!strcmp(argv[i], "--debug") || !strcmp(argv[i], "-d")) {
            debug_mode = 1;
            printf("DEBUG MODE\n");
        }
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            puts("Help message (FIXME)");
        }
        if (!strcmp(argv[i], "--version") || !strcmp(argv[i], "-v")) {
            printf("Embroidermodder 2.0.0-alpha\n");
        }
        if (!strcmp(argv[i], "--test")) {
            testing = 1;
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 1;
    }

    if (TTF_Init() < 0) {
        return 2;
    }

    create_window();

    /* Open tabs here */
    /*
    if (len(filesToOpen) > 0) {
        main_win.open_files_selected(argv, argc);
    }

    if (argc > 10) {
        argc = 10;
    }
    for (file=1; file<argc; file++) {
        pattern[file-1] = embPattern_create();
        embPattern_readAuto(pattern[file-1], argv[file]);
    }*/

    if (testing) {
        /* scheme_apply0(sc, "(crash-test)"); */
    }
    else {
        main_loop();
    }

    destroy_window(mainwnd);
    SDL_Quit();
    TTF_Quit();
    return 0;
}

void
destroy_window(Window *window)
{
    SDL_DestroyRenderer(mainwnd->renderer);
    SDL_DestroyWindow(mainwnd->window);
    TTF_CloseFont(mainwnd->font);
}


/* The main rendering loop.
 */
void
main_loop(void)
{
    while (mainwnd->running) {
        process_input(mainwnd->sc);
        render(mainwnd);
        SDL_RenderPresent(mainwnd->renderer);
        SDL_Delay(50);
    }
}

/* Process input: main loop step 1.
 */
void
process_input()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    int i;
    /* Get keyboard and mouse state. */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            mainwnd->running = 0;
            continue;
        }
        if (event.type == SDL_TEXTINPUT) {
            /* strcat(text_display[text_cursor[1]], event.text.text); */
        }
        /*
        if (event.type == SDL_TEXTEDITING) {
            strcat(text_display[text], event.text.text);
        }
        */
        if (event.type == SDL_KEYDOWN) {
            if (state[SDL_SCANCODE_LCTRL]) {
                if (state[SDL_SCANCODE_S]) {
                    save_from_buffer();
                }
                if (state[SDL_SCANCODE_Q]) {
                    mainwnd->running = 0;
                }
                if (state[SDL_SCANCODE_D]) {
                    /* delete line
                    for (i=text_cursor[1]; i<line_n-1; i++) {
                        strcpy(text_display[i], text_display[i+1]);
                    }
                    line_n--; */
                }
            }
            if (state[SDL_SCANCODE_BACKSPACE]) {
                /* text_display[text_cursor[1]][0] = 0; */
            }
            if (state[SDL_SCANCODE_ESCAPE]) {
            
            }
            if (state[SDL_SCANCODE_R]) {
                
            }
            /*
            if (render_mode == RENDER_TEXT_EDITOR) {
                if (state[SDL_SCANCODE_DOWN]) {
                    text_cursor[1]++;
                }
                if (state[SDL_SCANCODE_UP]) {
                    text_cursor[1]--;
                    if (text_cursor[1] < 0) {
                        text_cursor[1] = 0;
                    }
                }
                if (state[SDL_SCANCODE_RIGHT]) {
                    text_cursor[0]++;
                }
                if (state[SDL_SCANCODE_LEFT]) {
                    text_cursor[0]--;
                    if (text_cursor[0] < 0) {
                        text_cursor[0] = 0;
                    }
                }
                if (state[SDL_SCANCODE_RETURN]) {
                    /* Add line
                    for (i=line_n-1; i>text_cursor[1]; i--) {
                        strcpy(text_display[i+1],
                            text_display[i]);
                    }
                    line_n++;
                    text_display[text_cursor[1]][0] = 0;
                }
            }
            */
            continue;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            for (i=0; i<mainwnd->n_widgets; i++) {
                if (mainwnd->widgets[i].mode != WIDGET_MODE_BACKGROUND)
                if (click_detection(&(mainwnd->widgets[i]),
                    event.button.x, event.button.y)) {
                    actuator(mainwnd->sc, mainwnd->widgets[i].command);
                }
            }
        }
    }
}

int
print_all_variables(void)
{
    ;
    return 1;
}

/* Renderer for patterns.
 */
int
render(void)
{
    int i;
    for (i=0; i<mainwnd->n_widgets; i++) {
        if (mainwnd->widgets[i].visibility != INVISIBLE) {
            if (mainwnd->widgets[i].mode == WIDGET_MODE_BLOCK) {
                SDL_RenderCopy(mainwnd->renderer,
                    mainwnd->widgets[i].texture,
                    NULL,
                    &mainwnd->widgets[i].rect);
            }
            if (mainwnd->widgets[i].mode == WIDGET_MODE_BACKGROUND) {
                SDL_SetRenderDrawColor(mainwnd->renderer, mainwnd->widgets[i].color[0],
                mainwnd->widgets[i].color[1], mainwnd->widgets[i].color[2], 255);
                SDL_RenderFillRect(mainwnd->renderer, &mainwnd->widgets[i].rect);
            }
            if (mainwnd->widgets[i].mode == WIDGET_MODE_TEXT) {
                SDL_RenderCopy(mainwnd->renderer,
                    mainwnd->widgets[i].texture, NULL, &mainwnd->widgets[i].rect);
            }
        }
    }
    return 0;
}

int
click_detection(widget *w, int x, int y)
{
    return (w->rect.x < x)
        && (x < w->rect.x + w->rect.w)
        && (w->rect.y < y)
        && (y < w->rect.y + w->rect.h);
}

void
make_rectangle(SDL_Rect *rect, int x, int y, int w, int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

int
get_int_from_closure(pointer args)
{
    return (int)ivalue(pair_car(args));
}

void
get_args(pointer args, pointer arg[10], int n)
{
    int i;
    arg[0] = args;
    for (i=1; i<n; i++) {
        arg[i] = pair_cdr(arg[i-1]);
    }
}

void
create_widget(SDL_Rect rect, char *command)
{
    char icon_path[2*MAX_STRING_LENGTH];
    SDL_Surface *surface;
    mainwnd->widgets[mainwnd->n_widgets].rect = rect;
    mainwnd->widgets[mainwnd->n_widgets].mode = WIDGET_MODE_BLOCK;
    mainwnd->widgets[mainwnd->n_widgets].visibility = ALWAYS_VISIBLE;

    strcpy(mainwnd->widgets[mainwnd->n_widgets].command, command);

    sprintf(icon_path, "assets/icons/%s.png", mainwnd->widgets[mainwnd->n_widgets].command);
    surface = IMG_Load(icon_path);
    mainwnd->widgets[mainwnd->n_widgets].texture = SDL_CreateTextureFromSurface(mainwnd->renderer, surface);
    if (!mainwnd->widgets[mainwnd->n_widgets].texture) {
        debug_message("Failed to load texture.");
        debug_message(icon_path);
    }
    SDL_FreeSurface(surface);

    mainwnd->n_widgets++;
}

int
get_widget_by_label(char *label)
{
    int i;
    for (i=0; i<mainwnd->n_widgets; i++) {
        if (!strcmp(mainwnd->widgets[i].label, label)) {
            return i;
        }
    }
    return -1;
}

void
set_visibility(char *label, int visibility)
{
    int i;
    for (i=0; i<mainwnd->n_widgets; i++) {
        if (!strcmp(mainwnd->widgets[i].label, label)) {
            mainwnd->widgets[i].visibility = visibility;
        }
    }
}

void
set_int(scheme *sc, char *label, int i)
{
    char call[200];
    sprintf(call, "(define (%s) %d)", label, i);
    scheme_load_string(sc, call);
}

/* Create the window: the window and renderer variables
 * are file scope.
 *
 * In order to carry the data from file to file all the project
 * scope data is stored in here.
 */
void
create_window(void)
{
    int i;
    mainwnd = (Window *)malloc(sizeof(Window));
    scheme *sc;
    if (!scheme_init(&sc)) {
        puts("Could not initialise TinyScheme.");
        return;
    }

    puts("Booting...");
    if (load_scheme_file(sc, "assets/init.scm")) {
        debug_message("Failed to load assets/init.scm");
        return;
    }
    puts("Initialising...");
    if (load_scheme_file(sc, "assets/config.scm")) {
        debug_message("Failed to load assets/config.scm");
        return;
    }

    for (i=0; action_list[i].command[0] != 'E'; i++) {
        scheme_define(
            sc,
            sc->global_env,
            mk_symbol(sc, action_list[i].command),
            mk_foreign_func(sc, action_list[i].function));
    }

    mainwnd->sc = sc;
    mainwnd->running = 1;
    mainwnd->debug_mode = 1;
    mainwnd->track_mode = 0;
    mainwnd->show_lwt = 1;
    mainwnd->show_ortho = 1;

    mainwnd->font = TTF_OpenFont(load_str(sc, "interface-font"), 16);
    if (mainwnd->font == NULL) {
        debug_message("Interface font failed to load.");
    }

    mainwnd->window = SDL_CreateWindow(
        load_str(sc, "title"),
        load_int(sc, "window-offset-x"),
        load_int(sc, "window-offset-y"),
        load_int(sc, "window-width"),
        load_int(sc, "window-height"),
        SDL_WINDOW_SHOWN);
    mainwnd->renderer = SDL_CreateRenderer(
        mainwnd->window, -1, SDL_RENDERER_ACCELERATED);
    mainwnd->n_widgets = 0;
    mainwnd->widgets = (widget*)malloc(sizeof(widget)*1000);

    if (load_scheme_file(mainwnd->sc, "assets/ui.scm")) {
        debug_message("Failed to load widgets from scheme file assets/ui.scm.");
    }
}

/* Write the current settings to the standard file as scheme.
 *
 * The idea is that these override the settings in config.scm
 * so only what has changed needs to be written.
 */
void
write_settings(void)
{
    debug_message("Writing settings...");
    /* 
    settings_fname = APPLICATION_FOLDER + os.sep + "settings.json"
    json_str = json.dumps(settings, indent=4)
    if os.path.isfile(settings_fname)
        with open(settings_fname, "w", encoding="utf-8") as settings_file:
            settings_file.write(json_str)
    else:
        print("Failed to open settings file to write state.");


    settings["window_x"] = pos().x();
    settings["window_y"] = pos().y();
    settings["window_width"] = size().width();
    settings["window_height"] = size().height();
    */
}

/* .
 */
int
find_mdi_window(char *file_name)
{
    char *canonical_path;
    debug_message("find_mdi_window(fileName)");
    /*
    canonical_path = canonical_file_path(file_name);

    for (subWindow in mdi_area.sub_window_list(void)) {
        if (subWindow.getCurrentFile() == canonicalFilePath) {
            return subWindow;
        }
    }
    */
    return 0;
}

