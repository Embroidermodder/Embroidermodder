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
extern Action action_list[MAX_ACTIONS];

unsigned char clear_color[] = {50, 50, 50};
unsigned char toolbar_bg_color[] = {150, 150, 200};
unsigned char bg_color[] = {100, 150, 210};
unsigned char menubar_color[] = {200, 200, 250};
unsigned char white[] = {255, 255, 255};

char *icon_layout[] = {
    "1", "1", "new",
    "2", "1", "open",
    "3", "1", "save",
    "4", "1", "save-as",
    "5", "1", "cut",
    "6", "1", "copy",
    "7", "1", "paste",
    "8", "1", "delete",
    "9", "1", "undo",
    "10", "1", "redo",
    /*
    "11", "1", "check-for-updates",
    "12", "1", "select-all",
    */
    "13", "1", "whats-this",
    "14", "1", "design-details",
    "15", "1", "print-pattern",
    "16", "1", "exit-program",

    "17", "1", "window-close",
    "18", "1", "window-close-all",
    "19", "1", "window-cascade",
    "20", "1", "window-tile",
    "21", "1", "window-next",
    "22", "1", "window-previous",

    "1", "2", "help",
    /*
    "2", "2", "changelog-dialog",
    */
    "3", "2", "tip-of-the-day-dialog",
    "4", "2", "about-dialog",

    "5", "2", "icon-16",
    "6", "2", "icon-24",
    "7", "2", "icon-32",
    "8", "2", "icon-48",
    "9", "2", "icon-64",
    "10", "2", "icon-128",

    "11", "2", "settings-dialog",
    "12", "2", "make-layer-current",

    "13", "2", "pan-real-time",
    "14", "2", "pan-point",
    "15", "2", "pan-left",
    "16", "2", "pan-right",
    "17", "2", "pan-up",
    "18", "2", "pan-down",

    "19", "2", "day",
    "20", "2", "night",

    "1", "3", "layers",
    "2", "3", "layer-selector",
    "3", "3", "layer-previous",
    "4", "3", "color-selector",
    "5", "3", "line-type-selector",
    "6", "3", "line-weight-selector",
    "7", "3", "hide-all-layers",
    "8", "3", "show-all-layers",
    "9", "3", "freeze-all-layers",
    "10", "3", "thaw-all-layers",
    "11", "3", "lock-all-layers",
    "12", "3", "unlock-all-layers",

    "13", "3", "text-bold",
    "14", "3", "text-italic",
    "15", "3", "text-underline",
    "16", "3", "text-strikeout",
    "17", "3", "text-overline",
    
    "18", "3", "zoom-real-time",
    "19", "3", "zoom-previous",
    "20", "3", "zoom-window",
    "21", "3", "zoom-dynamic",
    "22", "3", "zoom-scale",
    "23", "3", "zoom-center",
    "24", "3", "zoom-in",
    "25", "3", "zoom-out",
    "26", "3", "zoom-selected",
    "27", "3", "zoom-all",
    "28", "3", "zoom-extents",

    "1", "4", "treble-clef",
    "2", "4", "path",
    "3", "4", "circle",
    "4", "4", "line",
    "5", "4", "distance",
    "6", "4", "dolphin",
    "7", "4", "ellipse",

    "8", "4", "heart",
    "9", "4", "locate-point",
    "10", "4", "move",
    /*
    "11", "4", "export",
    "12", "4", "heart4",
    "13", "4", "heart5",
    "14", "4", "single-line-text",
    "15", "4", "spell-check",
    "16", "4", "quick-select"
    "17", "4", "rectangle",
    "18", "4", "rgb",
    "19", "4", "rotate",
    "20", "4", "sandbox",
    "21", "4", "quickleader",
    "22", "4", "snowflake",
    "23", "4", "star",
    "24", "4", "platform",
    "25", "4", "point",
    "26", "4", "polygon",
    "27", "4", "polyline",
    "28", "4", "settings-dialog",
    "29", "4", "quickleader",
    "30", "4", "locate-point",
    "31", "4", "point",
    */
    "END", "END", "END"
};

char *file_labels[] = {
    "File", "file-menu",
    "New", "new",
    "Open", "open",
    "---", "---",
    "Save", "save",
    "Save as...", "save-as",
    "Export", "export",
    "---", "---",
    "Print", "print",
    "---", "---",
    "Design Details", "design-details",
    "---", "---",
    "Exit", "exit-program",
    "END", "END"
};

char *edit_labels[] = {
    "Edit", "edit-menu",
    "Undo", "undo",
    "Redo", "redo",
    "---", "---",
    "Cut", "cut",
    "Copy", "copy",
    "Paste", "paste",
    "END", "END"
};

char *view_labels[] = {
    "View", "view-menu",
    "Day", "day-vision",
    "Night", "night-vision",
    "---", "---",
    "Icon 16", "icon-16",
    "Icon 24", "icon-24",
    "Icon 32", "icon-32",
    "Icon 48", "icon-48",
    "Icon 64", "icon-64",
    "Icon 128", "icon-128",    
    "END", "END"
};

char *settings_labels[] = {
    "Settings", "settings-menu",
    "Settings Dialog", "settings-dialog",
    "END", "END"
};

char *zoom_labels[] = {
    "Zoom", "zoom-menu",
    "Zoom Real Time", "zoom-real-time",
    "Zoom Previous", "zoom-previous",
    "---", "---",
    "Zoom Window", "zoom-window",
    "Zoom Dynamic", "zoom-dynamic",
    "Zoom Scale", "zoom-scale",
    "Zoom Center", "zoom-center",
    "---", "---",
    "Zoom In", "zoom-in",
    "Zoom Out", "zoom-out",
    "---", "---",
    "Zoom Selected", "zoom-selected",
    "Zoom All", "zoom-all",
    "Zoom Extents", "zoom-extents",
    "END", "END"
};

char *pan_labels[] = {
    "Pan", "pan-menu",
    "Pan Real Time", "pan-real-time",
    "Pan Point", "pan-point",
    "---", "---",
    "Pan Left", "pan-left",
    "Pan Right", "pan-right",
    "Pan Up", "pan-up",
    "Pan Down", "pan-down",
    "END", "END"
};
 
char *window_labels[] = {
    "Window", "window-menu",
    "Window Cascade", "window-cascade",
    "END", "END"
};

char *help_labels[] = {
    "Help", "help-menu",
    "Help", "help",
    "---", "---",
    "Changelog", "changelog",
    "---", "---",
    "Tip of the Day", "tip-of-the-day",
    "---", "---",
    "About", "about",
    "---", "---",
    "What's This?", "whats-this",
    "END", "END"
};

char *recent_labels[] = {
    "Recent", "recent-menu",
    "None", "do-nothing",
    "END", "END"
};

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
    increment("undo-history-position");
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

/* Function definitions */
int
main(int argc, char *argv[])
{
    int debug_mode = 0, testing = 0;
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

    set_int("debug-mode", debug_mode);
    set_int("testing", testing);

    main_loop();

    destroy_window(mainwnd);
    SDL_Quit();
    TTF_Quit();
    return 0;
}

void
destroy_window(Window *window)
{
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    TTF_CloseFont(window->font);
}


/* The main rendering loop.
 */
void
main_loop(void)
{
    while (get_int("running")) {
        process_input();
        render();
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
            set_int("running", 0);
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
                    set_int("running", 0);
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
            for (i=0; i<mainwnd->n_widgets; i++) {
                if (mainwnd->widgets[i].mode != WIDGET_MODE_BACKGROUND)
                if (click_detection(&(mainwnd->widgets[i]),
                    event.button.x, event.button.y)) {
                    actuator(mainwnd->widgets[i].command);
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
        if (mainwnd->widgets[i].visibility == ALWAYS_VISIBLE
           || mainwnd->widgets[i].visibility == get_int("menu-state")) {
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

int
build_menu(char *labels[], int x_offset, int menu)
{
    int i;
    int y_offset = 5;
    int menu_width = get_int("menu-width");
    create_label(x_offset, y_offset,
        labels[0], labels[1], ALWAYS_VISIBLE);

    create_ui_rect(make_rectangle(
        x_offset, 20, menu_width,
        (5 * get_int("menu-item-height")) + 5),
        white, menu);
  
    for (i=1; strcmp(labels[2*i], "END"); i++) {
        if (strcmp(labels[2*i], "---")) {
            y_offset += get_int("menu-item-height");
            create_label(x_offset, y_offset,
                labels[2*i], labels[2*i+1], menu);
        }
        else {
            horizontal_rule(
                x_offset,
                y_offset+get_int("menu-item-height"),
                menu_width, menu);
        }
    }
    return x_offset + 10 + strlen(labels[0])*get_int("char-width");
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
    puts("Embroidermodder 2.");
    puts("Copyright 2013-2022 The Embroidermodder Team");
    puts("Distributed under the terms of the zlib license.");
    puts("");

    mainwnd = (Window *)malloc(sizeof(Window));

    init_state();

    mainwnd->font = TTF_OpenFont(get_str("interface-font"), 16);
    if (mainwnd->font == NULL) {
        debug_message("Interface font failed to load.");
    }

    mainwnd->window = SDL_CreateWindow(
        get_str("title"),
        get_int("window-offset-x"),
        get_int("window-offset-y"),
        get_int("window-width"),
        get_int("window-height"),
        SDL_WINDOW_SHOWN);
    mainwnd->renderer = SDL_CreateRenderer(
        mainwnd->window, -1, SDL_RENDERER_ACCELERATED);
    mainwnd->n_widgets = 0;
    mainwnd->widgets = (widget*)malloc(sizeof(widget)*1000);

    puts("Initialising user interface...");
    
    int toolbar_offset = get_int("menubar-height")
       + get_int("menubar-padding");
    int toolbar_height = get_int("icon-size")
       + get_int("toolbar-padding");

    /* Base */   
    create_ui_rect(make_rectangle(0, 0,
        get_int("window-width"), get_int("window-height")),
        clear_color,
        ALWAYS_VISIBLE);

    /* Toolbar background */
    create_ui_rect(make_rectangle(0, 0, 
        get_int("window-width"),
        4*(get_int("toolbar-padding")
           + toolbar_height) + toolbar_offset),
        bg_color,
        ALWAYS_VISIBLE);

    /* Menubar */
    create_ui_rect(make_rectangle(0, 0,
        get_int("window-width"),
        get_int("menubar-height")),
        menubar_color,
        ALWAYS_VISIBLE);

    /* Toolbars */
    for (i=0; i<4; i++) {
        create_ui_rect(make_rectangle(
            get_int("toolbar-padding"),
            i * (get_int("toolbar-padding")
               + toolbar_height) + toolbar_offset,
            get_int("toolbar-width"),
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

    horizontal_rule(10, 170, 350, ALWAYS_VISIBLE);
    horizontal_rule(10, 240, 350, ALWAYS_VISIBLE);
    horizontal_rule(10, 310, 350, ALWAYS_VISIBLE);
    horizontal_rule(10, 380, 350, ALWAYS_VISIBLE);
    horizontal_rule(10, 450, 350, ALWAYS_VISIBLE);
    vertical_rule(10, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(80, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(150, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(220, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(290, 150, 300, ALWAYS_VISIBLE);
    vertical_rule(360, 150, 300, ALWAYS_VISIBLE);

    set_int("menu-state", EDIT_MENU);

    for (i=0; strcmp(icon_layout[3*i], "END"); i++) {
        int x = atoi(icon_layout[3*i]);
        int y = atoi(icon_layout[3*i+1]);
        char *s = icon_layout[3*i+2];
        create_icon(x, y, s);
    }

    x_offset = 10;   
    x_offset = build_menu(file_labels, x_offset, FILE_MENU);
    x_offset = build_menu(edit_labels, x_offset, EDIT_MENU);
    x_offset = build_menu(view_labels, x_offset, VIEW_MENU);
    x_offset = build_menu(settings_labels, x_offset, SETTINGS_MENU);
    x_offset = build_menu(window_labels, x_offset, WINDOW_MENU);
    x_offset = build_menu(help_labels, x_offset, HELP_MENU);
    x_offset = build_menu(recent_labels, x_offset, RECENT_MENU);
    x_offset = build_menu(zoom_labels, x_offset, ZOOM_MENU);
    x_offset = build_menu(pan_labels, x_offset, PAN_MENU);

    /* Statusbar */
    create_ui_rect(make_rectangle(
        0,
        get_int("window-height") - get_int("menubar-height"),
        get_int("window-width"),
        get_int("menubar-height")),
        white,
        ALWAYS_VISIBLE);
    create_label(
        10,
        get_int("window-height") - get_int("menubar-height"),
        get_str("statusbar-message"),
        "do_nothing",
        ALWAYS_VISIBLE);
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
    int char_width = get_int("char-width");
    int menu_item_height = get_int("menu-item-height");    
    
}
*/

void
create_label(int x, int y, char *label, char *command, int visibility)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    strcpy(mainwnd->widgets[mainwnd->n_widgets].label, label);

    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(mainwnd->font,
        mainwnd->widgets[mainwnd->n_widgets].label, color);
    strcpy(mainwnd->widgets[mainwnd->n_widgets].command, command);
    rect.w = surface->w;
    rect.h = surface->h;

    mainwnd->widgets[mainwnd->n_widgets].rect = rect;
    mainwnd->widgets[mainwnd->n_widgets].mode = WIDGET_MODE_TEXT;
    mainwnd->widgets[mainwnd->n_widgets].visibility = visibility;

    mainwnd->widgets[mainwnd->n_widgets].texture = SDL_CreateTextureFromSurface(mainwnd->renderer, surface);
    if (!mainwnd->widgets[mainwnd->n_widgets].texture) {
        debug_message("Failed to generate ttf label.");
    }
    SDL_FreeSurface(surface);
    /* SDL_FreeTexture(texture); */

    mainwnd->n_widgets++;
}

void
create_ui_rect(SDL_Rect rect, unsigned char *color, int visibility)
{
    mainwnd->widgets[mainwnd->n_widgets].rect = rect;
    mainwnd->widgets[mainwnd->n_widgets].mode = WIDGET_MODE_BACKGROUND;
    mainwnd->widgets[mainwnd->n_widgets].color[0] = color[0];
    mainwnd->widgets[mainwnd->n_widgets].color[1] = color[1];
    mainwnd->widgets[mainwnd->n_widgets].color[2] = color[2];
    mainwnd->widgets[mainwnd->n_widgets].visibility = visibility;
    strcpy(mainwnd->widgets[mainwnd->n_widgets].label, "label");

    mainwnd->n_widgets++;
}

void
create_icon(int n, int m, char *label)
{
    int menubar_height = get_int("menubar-height");
    int icon_padding = get_int("icon-padding");
    int icon_size = get_int("icon-size");
    int icon_offset_n = n * icon_padding
        + (n-1)*(icon_size + icon_padding);
    int icon_offset_m = m * icon_padding
        + (m-1)*(icon_size + icon_padding);

    SDL_Rect rect;
    rect.x = icon_offset_n;
    rect.y = icon_offset_m + menubar_height;
    rect.w = icon_size;
    rect.h = icon_size;

    create_widget(rect, label);
}

void
horizontal_rule(int x, int y, int w, int visibility)
{
    unsigned char black[] = {0, 0, 0};
    create_ui_rect(make_rectangle(x, y, w, 1), black, visibility);
}

void
vertical_rule(int x, int y, int h, int visibility)
{
    unsigned char black[] = {0, 0, 0};
    create_ui_rect(make_rectangle(x, y, 1, h), black, visibility);
}

