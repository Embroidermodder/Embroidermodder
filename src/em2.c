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
Window *settings_dialog_wnd;
Window *about_dialog_wnd;
Window *property_editor_wnd;

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
actuator(char *action)
{
    increment(mainwnd->state, "undo-history-position");
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
    int i;
    for (i=0; strcmp(action_list[i].command, "END"); i++) {
        if (!strcmp(action_list[i].command, action)) {
            action_list[i].function();
            return 0;
        }
    }

    return 0;
}

int running = 1;
int debug_mode = 0;
int testing = 0;

/* Function definitions */
int
main(int argc, char *argv[])
{
    int i;

    for (i=1; i<argc; i++) {
        if (!strcmp(argv[i], "--debug") || !strcmp(argv[i], "-d")) {
            debug_mode = 1;
            printf("DEBUG MODE\n");
        }
        if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
            puts(" _____________________________________________________________________________");
            puts("|                                                                             |");
            puts("|                         EMBROIDERMODDER 2.0.0-alpha                         |");
            puts("|                         http://embroidermodder.org                          |");
            puts("|_____________________________________________________________________________|");
            puts("Usage: embroidermodder [options] files ...");
            puts("");
            puts("Options:");
            puts("-d, --debug      Print lots of debugging information.");
            puts("-h, --help       Print this message and exit.");
            puts("-v, --version    Print the version number of embroidermodder and exit.");
            puts("");
            return 1;
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

    puts("Embroidermodder 2.");
    puts("Copyright 2013-2022 The Embroidermodder Team");
    puts("Distributed under the terms of the zlib license.");
    puts("");
    mainwnd = create_window(state);
    menus_init();

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

    /* The main rendering loop. */
    while (get_int(mainwnd->state, "running")) {
        process_input(mainwnd);
        render(mainwnd);
        SDL_RenderPresent(mainwnd->renderer);
        /* if other windows are open, update them here */
        SDL_Delay(50);
    }

    destroy_window(mainwnd);
    SDL_Quit();
    TTF_Quit();
    return 0;
}

void
destroy_window(Window *w)
{
    SDL_DestroyRenderer(w->renderer);
    SDL_DestroyWindow(w->window);
    TTF_CloseFont(w->font);
}

/* Process input: main loop step 1.
 */
void
process_input(Window *w)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    int i;
    /* Get keyboard and mouse state. */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            set_int(w->state, "running", 0);
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
                    set_int(w->state, "running", 0);
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
                    */
                    /* Add line */
                    /*
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
            for (i=0; i<w->n_widgets; i++) {
                if (w->widgets[i].mode != WIDGET_MODE_BACKGROUND)
                if (click_detection(&(w->widgets[i]),
                    event.button.x, event.button.y)) {
                    actuator(w->widgets[i].command);
                }
            }
        }
    }
}

int
print_all_variables(Window *w)
{
    int i;
    for (i=0; strcmp(w->state[2*i], "None"); i++) {
        printf("%s: %s\n", w->state[2*i], w->state[2*i+1]);
    }
    return 1;
}

/* Renderer for patterns.
 */
int
render(Window *w)
{
    int i;
    for (i=0; i<w->n_widgets; i++) {
        if (w->widgets[i].visibility == ALWAYS_VISIBLE
           || w->widgets[i].visibility == get_int(w->state, "menu-state")) {
            if (w->widgets[i].mode == WIDGET_MODE_BLOCK) {
                SDL_RenderCopy(w->renderer,
                    w->widgets[i].texture,
                    NULL,
                    &w->widgets[i].rect);
            }
            if (w->widgets[i].mode == WIDGET_MODE_BACKGROUND) {
                SDL_SetRenderDrawColor(w->renderer, w->widgets[i].color[0],
                w->widgets[i].color[1], w->widgets[i].color[2], 255);
                SDL_RenderFillRect(w->renderer, &w->widgets[i].rect);
            }
            if (w->widgets[i].mode == WIDGET_MODE_TEXT) {
                SDL_RenderCopy(w->renderer,
                    w->widgets[i].texture, NULL, &w->widgets[i].rect);
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

SDL_Rect
make_rectangle(int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
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
create_widget(Window *w, SDL_Rect rect, char *command)
{
    char icon_path[2*MAX_STRING_LENGTH];
    SDL_Surface *surface;
    w->widgets[w->n_widgets].rect = rect;
    w->widgets[w->n_widgets].mode = WIDGET_MODE_BLOCK;
    w->widgets[w->n_widgets].visibility = ALWAYS_VISIBLE;

    strcpy(w->widgets[w->n_widgets].command, command);

    sprintf(icon_path, "assets/icons/%s.png", w->widgets[w->n_widgets].command);
    surface = IMG_Load(icon_path);
    w->widgets[w->n_widgets].texture = SDL_CreateTextureFromSurface(w->renderer, surface);
    if (!w->widgets[w->n_widgets].texture) {
        debug_message("Failed to load texture.");
        debug_message(icon_path);
    }
    SDL_FreeSurface(surface);

    w->n_widgets++;
}

int
get_widget_by_label(Window *w, char *label)
{
    int i;
    for (i=0; i<w->n_widgets; i++) {
        if (!strcmp(w->widgets[i].label, label)) {
            return i;
        }
    }
    return -1;
}

void
set_visibility(Window *w, char *label, int visibility)
{
    int i;
    for (i=0; i<w->n_widgets; i++) {
        if (!strcmp(w->widgets[i].label, label)) {
            w->widgets[i].visibility = visibility;
        }
    }
}

/* Only operates on mainwnd, since other windows are tab based
 * only.
 */
int
build_menu(char *labels[], int x_offset, int menu)
{
    int i;
    int y_offset = 5;
    /* Default, before the list is populated. */
    int menu_width = 100;
    create_label(mainwnd, x_offset, y_offset,
        labels[0], labels[1], ALWAYS_VISIBLE);
    int result = x_offset + 10;
    result += mainwnd->widgets[mainwnd->n_widgets-1].rect.w;

    create_ui_rect(mainwnd, 
        make_rectangle(
            x_offset,
            20,
            menu_width,
            (5 * menu_item_height) + 5),
        white, menu);
  
    for (i=1; strcmp(labels[2*i], "END"); i++) {
        if (strcmp(labels[2*i], "---")) {
            y_offset += menu_item_height;
            create_label(mainwnd, x_offset, y_offset,
                labels[2*i], labels[2*i+1], menu);
        }
        else {
            horizontal_rule(
                mainwnd,
                x_offset,
                y_offset+menu_item_height,
                menu_width, menu);
        }
    }
    return result;
}


/* Create the window: the window and renderer variables
 * are file scope.
 *
 * In order to carry the data from file to file all the project
 * scope data is stored in here.
 */
Window *
create_window(char state[2*MAX_VARIABLES][MAX_STRING_LENGTH])
{
    int i;
    Window *w = (Window *)malloc(sizeof(Window));
    w->n_widgets = 0;
    w->widgets = (widget*)malloc(sizeof(widget)*1000);

    init_state(w, state);

    w->font = TTF_OpenFont(get_str(w->state, "interface-font"), 16);
    if (w->font == NULL) {
        debug_message("Interface font failed to load.");
    }

    w->window = SDL_CreateWindow(
        get_str(w->state, "title"),
        get_int(w->state, "window-offset-x"),
        get_int(w->state, "window-offset-y"),
        get_int(w->state, "window-width"),
        get_int(w->state, "window-height"),
        SDL_WINDOW_SHOWN);
    w->renderer = SDL_CreateRenderer(
        w->window, -1, SDL_RENDERER_ACCELERATED);

    puts("Initialising user interface...");
    
    int toolbar_offset = menubar_height + menubar_padding;
    int toolbar_height = icon_size + toolbar_padding;

    /* Base */   
    create_ui_rect(
        w,
        make_rectangle(0, 0,
            get_int(w->state, "window-width"),
            get_int(w->state, "window-height")),
        clear_color,
        ALWAYS_VISIBLE);

    /* Toolbar background */
    create_ui_rect(
        w,
        make_rectangle(0, 0, 
            get_int(w->state, "window-width"),
            4*(toolbar_padding + toolbar_height)
            + toolbar_offset),
        bg_color,
        ALWAYS_VISIBLE);

    /* Menubar */
    create_ui_rect(
        w,
        make_rectangle(0, 0,
            get_int(w->state, "window-width"),
            menubar_height),
        menubar_color,
        ALWAYS_VISIBLE);

    /* Toolbars */
    for (i=0; i<4; i++) {
        create_ui_rect(
            w,
            make_rectangle(
                toolbar_padding,
                i * (toolbar_padding + toolbar_height)
                + toolbar_offset,
                get_int(w->state, "toolbar-width"),
                toolbar_height),
            toolbar_bg_color,
            ALWAYS_VISIBLE);
    }

    int x_offset = 10;   

    /*
    visibility groups
    "always"
    "file-toolbar"
    "edit-toolbar"
    "window-toolbar"
 
    "never" (use only for debugging)

    ui palette
    interface-background
    button-hover
    */

    /* UI SETTINGS
     * -----------
     * These may change causing us to need to reload the file.
     */

    horizontal_rule(w, 10, 170, 350, ALWAYS_VISIBLE);
    horizontal_rule(w, 10, 240, 350, ALWAYS_VISIBLE);
    horizontal_rule(w, 10, 310, 350, ALWAYS_VISIBLE);
    horizontal_rule(w, 10, 380, 350, ALWAYS_VISIBLE);
    horizontal_rule(w, 10, 450, 350, ALWAYS_VISIBLE);
    vertical_rule(w, 10, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(w, 80, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(w, 150, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(w, 220, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(w, 290, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(w, 360, 150, 300, ALWAYS_VISIBLE);

    set_int(w->state, "menu-state", EDIT_MENU);

    for (i=0; strcmp(icon_layout[3*i], "END"); i++) {
        int x = atoi(icon_layout[3*i]);
        int y = atoi(icon_layout[3*i+1]);
        char *s = icon_layout[3*i+2];
        create_icon(w, x, y, s);
    }

    /* Statusbar */
    create_ui_rect(
        w,
        make_rectangle(
        0,
        get_int(w->state, "window-height")
            - menubar_height,
        get_int(w->state, "window-width"),
        menubar_height),
        white,
        ALWAYS_VISIBLE);

    create_label(
        w,
        10,
        get_int(w->state, "window-height")
            - menubar_height,
        statusbar_message,
        "do_nothing",
        ALWAYS_VISIBLE);

    return w;
}


/* ------------------------------------------------------------
; Crash test all basic actions.

(define (crash-test)
  (begin
    (do_nothing)

    (debug-message "File actions")
    (new-file)
    ; Set file name procedurally
    (open-file)
    (save-file)
    (save-as-file)

    (debug-message "Edit actions")
    ; Place some objects to act on.
    (cut)
    (copy)
    (paste)

    ; Crash test set icon size to x calls.
    (icon16)
    (icon24)
    (icon32)
    (icon48)
    (icon64)
    (icon128)))
*/
void
crash_test(void)
{

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
    debug_message("find_mdi_window(fileName)");
    printf("%s\n", file_name);
    /*
    char *canonical_path;
    canonical_path = canonical_file_path(file_name);

    for (subWindow in mdi_area.sub_window_list(void)) {
        if (subWindow.getCurrentFile() == canonicalFilePath) {
            return subWindow;
        }
    }
    */
    return 0;
}

/*
void
menu_widget(char *label, int column, int row, int visibility)
{
    int char_width = get_int(mainwnd->state, "char-width");
    int menu_item_height = get_int(mainwnd->state, "menu-item-height");    
    
}
*/

void
create_label(
    Window *w,
    int x, int y,
    char *label,
    char *command,
    int visibility)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    strcpy(w->widgets[w->n_widgets].label, label);

    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(w->font,
        w->widgets[w->n_widgets].label, color);
    strcpy(w->widgets[w->n_widgets].command, command);
    rect.w = surface->w;
    rect.h = surface->h;

    w->widgets[w->n_widgets].rect = rect;
    w->widgets[w->n_widgets].mode = WIDGET_MODE_TEXT;
    w->widgets[w->n_widgets].visibility = visibility;

    w->widgets[w->n_widgets].texture = SDL_CreateTextureFromSurface(w->renderer, surface);
    if (!w->widgets[w->n_widgets].texture) {
        debug_message("Failed to generate ttf label.");
    }
    SDL_FreeSurface(surface);
    /* SDL_FreeTexture(texture); */

    w->n_widgets++;
}

void
create_ui_rect(Window *w,
    SDL_Rect rect, unsigned char *color, int visibility)
{
    w->widgets[w->n_widgets].rect = rect;
    w->widgets[w->n_widgets].mode = WIDGET_MODE_BACKGROUND;
    w->widgets[w->n_widgets].color[0] = color[0];
    w->widgets[w->n_widgets].color[1] = color[1];
    w->widgets[w->n_widgets].color[2] = color[2];
    w->widgets[w->n_widgets].visibility = visibility;
    strcpy(w->widgets[w->n_widgets].label, "label");

    w->n_widgets++;
}

void
create_icon(Window *w, int n, int m, char *label)
{
    int icon_padding = get_int(w->state, "icon-padding");
    int icon_offset_n = n * icon_padding
        + (n-1)*(icon_size + icon_padding);
    int icon_offset_m = m * icon_padding
        + (m-1)*(icon_size + icon_padding);

    SDL_Rect rect;
    rect.x = icon_offset_n;
    rect.y = icon_offset_m + menubar_height;
    rect.w = icon_size;
    rect.h = icon_size;

    create_widget(w, rect, label);
}

void
horizontal_rule(
    Window *w,
    int x,
    int y,
    int width,
    int visibility)
{
    unsigned char black[] = {0, 0, 0};
    create_ui_rect(w, make_rectangle(x, y, width, 1), black, visibility);
}

void
vertical_rule(
    Window *w,
    int x,
    int y,
    int h,
    int visibility)
{
    unsigned char black[] = {0, 0, 0};
    create_ui_rect(w, make_rectangle(x, y, 1, h), black, visibility);
}

