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

int load_window_data(int window_id, char *fname);

/* To make a new windowing system compatible with our
 * widget system create the following functions:
 *
 * void create_window(char *fname, char *title);
 *     which creates a window with the first available
 *     empty window struct.
 *
 * void process_input(int window_id);
 *     which converts the user input into instructions to the
 *     actuator.
 *
 * void render_clear(int window_id, EmbColor color);
 *     which clears the whole window with the given color.
 *
 * void render_copy(int window_id, EmbImage image);
 *     which renders an image to the .
 *
 * void render_rect(int window_id, EmbColor color, Rect rect);
 *     which renders a rectangle to the .
 *
 * void destroy_window(int window_id);
 *     which closes the given window, but the memory
 *     remains allocated, to be garbage collected at the
 *     end of runtime.
 *
 */

#define TEXT_W 120
#define TEXT_H 24

void
create_label(int window_id, int panel_id, int x, int y,
    char *label, char *command, int visibility)
{
    EmbPanel *panel = windows[window_id]->panels[panel_id];
    Rect rect;
    rect.x = x;
    rect.y = y;
    strcpy([panel->widgets[panel->n_widgets].label, label);

    unsigned char bitmap_data[TEXT_H*TEXT_W];
    unsigned char bitmap[TEXT_H][TEXT_W];
    int s, i, j, ascent, baseline;
    int width = TEXT_W, height = TEXT_H;
    float scale;
    s = 16;
    float xpos = 2;
    for (i=0; i<height; i++)
    for (j=0; j<width; j++) {
        bitmap[i][j] = 0;
    }

    rect.w = width;
    rect.h = height;

    scale = stbtt_ScaleForPixelHeight(&(w->font), s);
    stbtt_GetFontVMetrics(&(w->font), &ascent, 0, 0);
    baseline = (int) (ascent*scale);

    for (i=0; label[i]; i++) {
        int advance,lsb,x0,y0,x1,y1;
        float x_shift = xpos - (float) floor(xpos);
        stbtt_GetCodepointHMetrics(&(w->font), label[i], &advance, &lsb);
        stbtt_GetCodepointBitmapBoxSubpixel(&(w->font),
            label[i], scale, scale, x_shift, 0, &x0, &y0, &x1, &y1);
        stbtt_MakeCodepointBitmapSubpixel(&(w->font),
            &bitmap[baseline + y0][(int) xpos + x0],
            x1-x0, y1-y0, width, scale, scale, x_shift, 0, label[i]);
        xpos += (advance * scale);
        if (label[i+1]) {
            xpos += scale*stbtt_GetCodepointKernAdvance(
                &(w->font), label[i], label[i+1]);
        }
    }
    rect.w = ((int)floor(xpos));
    for (i=0; i<rect.h; i++)
    for (j=0; j<rect.w; j++) {
        bitmap_data[rect.w*i+j] = 255-bitmap[i][j];
    }
    strcpy(w->widgets[panel->n_widgets].command, command);
    w->widgets[panel->n_widgets].rect = rect;
    w->widgets[panel->n_widgets].mode = WIDGET_MODE_TEXT;
    w->widgets[panel->n_widgets].visibility = visibility;

    stbi_write_bmp("temp.bmp", rect.w, rect.h, 1, bitmap_data);

    /* Load the bitmap to EmbImage here. */
#if EM2_X11

#elif EM2_COCOA

#elif EM2_IOS

#elif EM2_ANDROID

#elif EM2_WIN32

#endif

    panel->n_widgets++;
}

void
wait(int milliseconds)
{
#if EM2_X11 || EM2_ANDROID || EM2_COCOA
    usleep(milliseconds);
#elif EM2_IOS

#elif EM2_WIN32

#endif
}

void
destroy_widget(int window_id, int panel_id, int index)
{
    EmbPanel *panel = windows[window_id]->panels[panel_id];
    panel->widgets[index] = panel->widgets[panel->n_widgets-1];
    panel->n_widgets--;
}

void
create_ui_rect(int window_id, int panel_id,
    Rect rect, EmbColor color, int visibility)
{
    EmbPanel *panel = windows[window_id]->panels[panel_id];
    panel->widgets[panel->n_widgets].rect = rect;
    panel->widgets[panel->n_widgets].mode = WIDGET_MODE_BACKGROUND;
    panel->widgets[panel->n_widgets].color[0] = color.r;
    panel->widgets[panel->n_widgets].color[1] = color.g;
    panel->widgets[panel->n_widgets].color[2] = color.b;
    panel->widgets[panel->n_widgets].visibility = visibility;
    strcpy(panel->widgets[panel->n_widgets].label, "label");

    panel->n_widgets++;
}

void
create_icon(int w, int n, int m, char *label)
{
    int icon_offset_n = n * icon_padding
        + (n-1)*(icon_size + icon_padding);
    int icon_offset_m = m * icon_padding
        + (m-1)*(icon_size + icon_padding);

    Rect rect;
    rect.x = icon_offset_n;
    rect.y = icon_offset_m + menubar_height;
    rect.w = icon_size;
    rect.h = icon_size;

    create_widget(w, rect, label);
}

void
horizontal_rule(int window_id, int panel_id, int x, int y, int width, int visibility)
{
    Rect rect = make_rectangle(x, y, width, 1);
    create_ui_rect(window_id, panel_id, rect, black, visibility);
}

void
vertical_rule(int window_id, int panel_id, int x, int y, int h, int visibility)
{
    Rect rect = make_rectangle(x, y, 1, h);
    create_ui_rect(window_id, panel_id, rect, black, visibility);
}

void
create_grid(int window_id, int panel_id)
{
    Rect line = make_rectangle(10, 170, 1, MAX_SCREEN_SIZE);
    while (line.x < MAX_SCREEN_SIZE) {
        create_ui_rect(window_id, panel_id, line, grid_color, ALWAYS_VISIBLE);
        line.x += 70;
    }

    line = make_rectangle(10, 170, MAX_SCREEN_SIZE, 1);
    while (line.y < MAX_SCREEN_SIZE) {
        create_ui_rect(window_id, panel_id, line, grid_color, ALWAYS_VISIBLE);
        line.y += 70;
    }
}

void
create_toolbars(void)
{
    int i;
    int toolbar_offset = menubar_height + menubar_padding;
    int toolbar_height = icon_size + toolbar_padding;

    TABLE(toolbars);
    load_csv(toolbars, "assets/toolbars/index.csv");

    for (i=0; strcmp(toolbars[i][0], "END"); i++) {
        printf("%s\n", toolbars[i][0]);
    }

    create_ui_rect(
        MAIN_WINDOW,
        make_rectangle(0, 0, 
            windows[MAIN_WINDOW]->dimension.w,
            4*(toolbar_padding + toolbar_height)
            + toolbar_offset),
        bg_color,
        ALWAYS_VISIBLE);

    for (i=0; i<4; i++) {
        create_ui_rect(
            MAIN_WINDOW,
            make_rectangle(
                toolbar_padding,
                i * (toolbar_padding + toolbar_height)
                    + toolbar_offset,
                windows[MAIN_WINDOW]->dimension.w - 2*toolbar_padding,
                toolbar_height),
            toolbar_bg_color,
            ALWAYS_VISIBLE);
    }

    for (i=0; strcmp(toolbars[i][0], "END"); i++) {
        TABLE(toolbar);
        int j;
        printf("%s\n", toolbars[i][0]);
        /* int visibility = atoi(toolbar[0]); */
        load_csv(toolbar, toolbars[i][0]);
        int x = atoi(toolbar[1][0]);
        int y = atoi(toolbar[2][0]);
        for (j=3; strcmp(toolbar[j][0], "END"); j++) {
            create_icon(MAIN_WINDOW, x+i-3, y, toolbar[j][0]);
        }
    }
}

/*
 * Build the classic UI dropdown menus using the layout defined
 * on file in our 'layout.json'.
 *
 * Only operates on mainwnd, since other windows are tab based
 * only.
 */
int
build_menu(int window_id, int panel_id, char *fname, int x_offset, int menu)
{
    int i;
    int y_offset = 5;
    /* Default, before the list is populated. */
    int menu_width = 100;
    EmbWindow *main = windows[window_id];
    EmbPanel *panel = main->panels[panel_id];

    TABLE(labels);
    load_csv(labels, fname);

    create_label(window_id, panel_id, x_offset, y_offset,
        labels[0][0], labels[0][1], ALWAYS_VISIBLE);
    int result = x_offset + 10;
    result += main->widgets[main->n_widgets-1].rect.w;

    Rect background = make_rectangle(
        x_offset, 20, menu_width,  (5 * menu_item_height) + 5);
    create_ui_rect(MAIN_WINDOW, background, white_color, menu);
  
    for (i=1; strcmp(labels[i][0], "END"); i++) {
        if (strcmp(labels[i][0], "---")) {
            y_offset += menu_item_height;
            create_label(MAIN_WINDOW, x_offset, y_offset,
                labels[i][0], labels[i][1], menu);
        }
        else {
            horizontal_rule(
                MAIN_WINDOW,
                x_offset,
                y_offset+menu_item_height,
                menu_width, menu);
        }
    }
    return result;
}

void
create_menubar(void)
{
    TABLE(menus);
    int i;
    int x_offset = 10;
    Rect menubar_bg = make_rectangle(0, 0, windows[MAIN_WINDOW]->dimension.w, menubar_height);
    create_ui_rect(MAIN_WINDOW, menubar_bg, menubar_color, ALWAYS_VISIBLE);

    load_csv(menus, "assets/menus/index.csv");
    print_table(menus);
    for (i=0; strcmp(menus[i][0], "END"); i++) {
        if (debug_mode) {
            printf("loading menu: %s\n", menus[i][0]);
        }
        x_offset = build_menu(menus[i][0], x_offset, FILE_MENU+i);
    }
}

void
create_statusbar(void)
{
    create_ui_rect(
        MAIN_WINDOW,
        make_rectangle(
            0,
            windows[MAIN_WINDOW]->dimension.h - menubar_height,
            windows[MAIN_WINDOW]->dimension.w,
            menubar_height),
        white_color,
        ALWAYS_VISIBLE);

    create_label(
        MAIN_WINDOW,
        10,
        windows[MAIN_WINDOW]->dimension.h - menubar_height,
        statusbar_message,
        "do-nothing",
        ALWAYS_VISIBLE);
}

void
create_scrollbars(int scene)
{
    printf("%d", scene);
}

void
create_view(int window_id)
{
    create_grid(window_id);
    create_scrollbars(window_id);
}

int
render_panel(int window_id, int panel_id)
{
    int i;
    EmbPanel *panel = windows[window_id]->panels[panel_id];

    for (i=0; i<panel->n_widgets; i++) {
        EmbWidget widget = panel->widgets[i];
        if (widget.visibility == ALWAYS_VISIBLE
           || widget.visibility == menu_state) {
            if (widget.mode == WIDGET_MODE_BLOCK) {
                render_rect(window_id, widget.color, widget.rect);
            }
            if (widget.mode == WIDGET_MODE_BACKGROUND) {
                render_rect(window_id, widget.color, widget.rect);
            }
            if (widget.mode == WIDGET_MODE_TEXT) {
                render_copy(window_id, widget.image);
            }
        }
    }
}

/* Renderer for every widget in the user interface for
 * window with index window_id.
 */
int
render(int window_id)
{
    int i;
    render_clear(window_id, clear_color);

    for (i=0; i<w->n_panels; i++) {
        render_panel(window_id, panel_id);
    }
    return 0;
}

int
load_window_data(int window_id, char *fname)
{
    EmbWindow *w = windows[window_id];
    w = (EmbWindow *)malloc(sizeof(EmbWindow));
    w->n_widgets = 0;
    w->widgets = (EmbWidget*)malloc(sizeof(EmbWidget)*MAX_WIDGETS);

    FILE *f = fopen(get_str(global_state, "interface_font"), "rb");
    if (!f) {
        debug_message("Failed to load interface font.");
        return NULL;
    }
    if (!fread(ttf_buffer, 1, 1000000, f)) {
        debug_message("Failed to read font data from file.");
        return NULL;    
    }
    stbtt_InitFont(&(w->font), ttf_buffer, 0);
    fclose(f);

    /*
    TODO: need a mechanism to check for failing to load.
    if (w->font == NULL) {
        debug_message("Interface font failed to load.");
    }
    */

    load_csv(w->data, fname);

    w->dimension.x = get_int(w->data, "window x");
    w->dimension.y = get_int(w->data, "window y");
    w->dimension.w = get_int(w->data, "window w");
    w->dimension.h = get_int(w->data, "window h");

    n_windows++;
    return 0;
}

#if EM2_X11
/* Create the window: the window and renderer variables
 * are file scope.
 *
 * In order to carry the data from file to file all the project
 * scope data is stored in here.
 */
int
create_window(int window_id, char *fname)
{
    EmbWindow *w = windows[window_id];
    load_window_data(window_id, fname);

    /* Window compatibility layer. */
    w->display = XOpenDisplay(NULL);
    w->screen = DefaultScreen(w->display);
    unsigned long black_pixel = BlackPixel(w->display, w->screen);
    unsigned long white_pixel = WhitePixel(w->display, w->screen);
    w->window = XCreateSimpleWindow(
        w->display,
        XDefaultRootWindow(w->display),
        get_int(w->data, "window x"),
        get_int(w->data, "window y"),
        get_int(w->data, "window w"),
        get_int(w->data, "window h"),
        5,
        black_pixel,
        white_pixel);
    XSelectInput(w->display, w->window,
        ExposureMask | ButtonPressMask | KeyPressMask);
    /* Make the window actually present to viewer,
     * seems to be something of a gotcha. */
    XMapWindow(w->display, w->window);
    XStoreName(w->display, w->window, get_str(w->data, "title"));

    w->gc = XCreateGC(w->display, w->window, 0, 0);
    XSetForeground(w->display, w->gc, white_pixel);
    XSetBackground(w->display, w->gc, black_pixel);

    XClearWindow(w->display, w->window);
    XMapRaised(w->display, w->window);

    puts("Initialising user interface...");
    create_toolbars();
    create_view(0);
    create_menubar();
    create_statusbar();
    return 0;
}


/* Process input: main loop step 1.
 */
EmbEvent
process_input(int window_id)
{
    EmbEvent event;
    XEvent xevent;
    XNextEvent(windows[MAIN_WINDOW]->display, &xevent);
    
    return event;
}

void
render_copy(int window_id, EmbImage image)
{

}

void
render_rect(int window_id, EmbColor color, Rect rect)
{

}

void
render_clear(int window_id, EmbColor color)
{
    XClearWindow(windows[MAIN_WINDOW]->display, windows[MAIN_WINDOW]->window);
}

void
destroy_window(int window_id)
{
    EmbWindow *w = windows[window_id];
    XFreeGC(w->display, w->gc);
    XDestroyWindow(w->display, w->window);
    XCloseDisplay(w->display);
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif

#if EM2_ANDROID
void
create_window(int window_id, char *fname)
{
    load_window_data(window_id, fname);
}

/* Process input: main loop step 1.
 */
void
process_input(int window_id)
{

}

void
render_copy(int window_id, EmbImage image)
{

}

void
render_rect(int window_id, EmbColor color, Rect rect)
{

}

void
render_clear(int window_id, EmbColor color)
{

}

/* So android doesn't have multiple application 
 * "windows" so we need some way of abstracting out
 * windows into layers of application.
 */
void
destroy_window(int window_id)
{
    EmbWindow *w = windows[window_id];
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif

#if EM2_COCOA
void
create_window(int window_id, char *fname)
{
    load_window_data(window_id, fname);
}

/* Process input: main loop step 1.
 */
EmbEvent
process_input(EmbWindow *w)
{
    EmbEvent event;
    return event;
}

void
render_copy(int window_id, EmbImage image)
{

}

void
render_rect(int window_id, EmbColor color, Rect rect)
{

}

void
render_clear(int window_id, EmbColor color)
{

}

/* Same as android reasoning.
 */     
void
destroy_window(int window_id)
{
    EmbWindow *w = windows[window_id];
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif

#if EM2_WIN32
int
create_window(int window_id, char *fname)
{
    load_window_data(window_id, fname);
    return 0;
}

/* Process input: main loop step 1.
 */
EmbEvent
process_input(EmbWindow *w)
{
    EmbEvent event;
    return event;
}

void
render_copy(int window_id, EmbImage image)
{

}

void
render_rect(int window_id, EmbColor color, Rect rect)
{

}

void
render_clear(int window_id, EmbColor color)
{

}

void
destroy_window(int window_id)
{
    EmbWindow *w = windows[window_id];
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif

#if EM2_IOS
int
create_window(int window_id, char *fname)
{
    load_window_data(window_id, fname);
    return 0;
}

/* Process input: main loop step 1.
 */
EmbEvent
process_input(EmbWindow *w)
{
    EmbEvent event;
    return event;
}

void
render_copy(int window_id, EmbImage image)
{

}

void
render_rect(int window_id, EmbColor color, Rect rect)
{

}

void
render_clear(int window_id, EmbColor color)
{

}

/* Same as android reasoning.
 */     
void
destroy_window(int window_id)
{
    EmbWindow *w = windows[window_id];
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif


#if 0
/* Process input: main loop step 1.
 */
EmbEvent
process_input(EmbWindow *w)
{
    EmbEvent event;
    int i;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    for (i=0; i<MAX_KEYS; i++) {
        event.state[i] = state[i];
    }

    /* Get keyboard and mouse state. */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
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
                    running = 0;
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
                            text_display[i][0]);
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

void
render_copy(int window_id, EmbWidget widget)
{
    EmbWindow *w = windows[window_id];
    SDL_RenderCopy(w->renderer, widget.texture, NULL, &widget.rect);
}

void
render_rect(int window_id, int widget)
{
    EmbWindow *w = windows[window_id];
    EmbWidget widget = w->widgets[i];
    SDL_SetRenderDrawColor(w->renderer, widget.color[0],
        widget.color[1], widget.color[2], 255);
    SDL_RenderFillRect(w->renderer, &widget.rect);
}

void
render_clear(int window_id, int widget)
{
    SDL_SetRenderDrawColor(w->renderer,
        clear_color.r, clear_color.g, clear_color.b,
        255);
    SDL_RenderClear(w->renderer);
}
#endif

void
create_widget(int window_id, Rect rect, char *command)
{
    char icon_path[2*MAX_STRING_LENGTH];
    EmbWindow *w = windows[window_id];
    w->widgets[w->n_widgets].rect = rect;
    w->widgets[w->n_widgets].mode = WIDGET_MODE_BLOCK;
    w->widgets[w->n_widgets].visibility = ALWAYS_VISIBLE;

    strcpy(w->widgets[w->n_widgets].command, command);

    sprintf(icon_path, "assets/icons/%s.bmp", w->widgets[w->n_widgets].command);
#if EM2_X11
#elif EM2_ANDROID
#elif EM2_IOS
#elif EM2_COCOA
#elif EM2_WIN32
#else
    SDL_Surface *surface;
    surface = SDL_LoadBMP(icon_path);
    w->widgets[w->n_widgets].texture = SDL_CreateTextureFromSurface(w->renderer, surface);
    if (!w->widgets[w->n_widgets].texture) {
        debug_message("Failed to load texture.");
        debug_message(icon_path);
    }
    SDL_FreeSurface(surface);
#endif

    w->n_widgets++;
}

int
create_spinbox(int window_id, int panel_id, char *fname, float value)
{
    EmbPanel *panel = windows[window_id]->panels[panel_id];
    TABLE(table);
    load_csv(table, fname);
    strcpy(panel->widgets[w->n_widgets].label, translate(get_str(table, "label")));
    strcpy(panel->widgets[panel->n_widgets].category, get_str(table, "category"));
    strcpy(panel->widgets[panel->n_widgets].name, get_str(table, "name"));
    panel->widgets[panel->n_widgets].single_step = get_float(table, "single_step");
    panel->widgets[panel->n_widgets].range_lower = get_float(table, "range_lower");
    panel->widgets[panel->n_widgets].range_upper = get_float(table, "range_upper");
    panel->widgets[panel->n_widgets].value = value;
    panel->widgets[panel->n_widgets].storage = value;
    panel->widgets[panel->n_widgets].enabled = !dialog_grid_load_from_file;
    panel->widgets[panel->n_widgets].visible = get_int(table, "visible");
    panel->widgets[panel->n_widgets].mode = WIDGET_MODE_SPINBOX;

    panel->n_widgets++;
    return 0;
}

/*
 * For safe packaging, and to reduce the risk of program
 * crashing errors the resources are loaded into the
 * application folder each time the program boots.
 */
void
load_image(char *path)
{
    char image_fname[4*MAX_STRING_LENGTH];
    FILE *f;
    sprintf(image_fname, "%s%s%s",
        get_str(global_state, "application_folder"),
        os_seperator, path);
    f = fopen(image_fname, "wb");
    /*
    icon = icons[path].replace("\n", "");
    icon = icon.replace(" ", "");
    decoded-image = binascii.unhexlify(icon);
    image-file.write(decoded-image);
    return tk.PhotoImage(file=image-fname);
    */
    fclose(f);
}

/* EmbPanels exist to server as window-like areas within the Windows of
 * the host operating system.
 *
 * Tabs in the tabbed interface parts (for example the settings dialog) are 
 * EmbPanels, but they are EmbPanels that do not have the EXCLUSIVE flag set,
 * but do have the FILL_SPACE flag set. Therefore they all overlay and it is the
 * only VISIBLE tab that is interactable.
 *
 * EmbPanels need to exist in a tree-like structure as they contain widgets
 * in a potentially overlapping manner and an EmbPanel can contain other 
 * EmbPanels.
 *
 * An EmbPanel does _not_ have to have any GUI capabilities and instead act
 * as a standardised container for the "middle" of the GUI tree. So we have:
 * an array of windows each containing a tree structure of panels each
 * containing widgets and only these leaf nodes can be rendered. 
 */
void
create_panel(EmbPanel *panel)
{
    panel->closeable = 1;
    panel->use_logo = 0;
    panel->use_texture = 0;
    panel->use_color = 0;
    panel->bg_logo = 0;
    panel->bg_texture = 0;
    panel->bg_color = 0;
}
