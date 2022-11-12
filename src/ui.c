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
 
/* To make a new windowing system compatible with our
 * widget system create the following functions:
 *
 * void create_window(char *fname);
 *     which creates a window with the first available
 *     empty window struct, using the data in the CSV file "fname".
 *
 * void process_input(EmbWindow *window);
 *     which converts the user input into instructions to the
 *     actuator.
 *
 * void render_clear(EmbWindow *window, EmbColor color);
 *     which clears the whole window with the given color.
 *
 * void render_copy(EmbWindow *window, EmbImage image);
 *     which renders an image to the .
 *
 * void render_rect(EmbWindow *window, EmbColor color, Rect rect);
 *     which renders a rectangle to the .
 *
 * void destroy_window(EmbWindow *window);
 *     which closes the given window, but the memory
 *     remains allocated, to be garbage collected at the
 *     end of runtime.
 *
 */

#define TEXT_W 120
#define TEXT_H 24

void
create_label(EmbWindow *w, EmbPanel *panel, int position[2],
    char *label, char *command, int visibility)
{
    EmbWidget *widget = panel->widgets[panel->n_widgets];
    Rect rect;
    rect.x = position[0];
    rect.y = position[1];
    strcpy(widget->label, label);

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
    strcpy(widget->command, command);
    widget->rect = rect;
    widget->mode = WIDGET_MODE_TEXT;
    widget->visibility = visibility;

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
destroy_widget(EmbPanel *panel, int index)
{
    panel->widgets[index] = panel->widgets[panel->n_widgets-1];
    panel->n_widgets--;
}

void
create_ui_rect(EmbPanel *panel,
    Rect rect, EmbColor color, int visibility)
{
    EmbWidget *widget;
    widget->rect = rect;
    widget->mode = WIDGET_MODE_BACKGROUND;
    widget->color[0] = color.r;
    widget->color[1] = color.g;
    widget->color[2] = color.b;
    widget->visibility = visibility;
    strcpy(widget->label, "label");

    panel->n_widgets++;
}

void
create_icon(EmbWindow *window, int n, int m, char *label)
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

    create_widget(window, panel, rect, label);
}

void
horizontal_rule(EmbPanel *panel, int position[2], int width, int visibility)
{
    Rect rect = make_rectangle(position[0], position[1], width, 1);
    create_ui_rect(panel, rect, black, visibility);
}

void
vertical_rule(EmbPanel *panel, int position[2], int h, int visibility)
{
    Rect rect = make_rectangle(position[0], position[1], 1, h);
    create_ui_rect(panel, rect, black, visibility);
}

void
create_grid(EmbPanel *panel)
{
    Rect line = make_rectangle(10, 170, 1, MAX_SCREEN_SIZE);
    while (line.x < MAX_SCREEN_SIZE) {
        create_ui_rect(panel, line, grid_color, ALWAYS_VISIBLE);
        line.x += 70;
    }

    line = make_rectangle(10, 170, MAX_SCREEN_SIZE, 1);
    while (line.y < MAX_SCREEN_SIZE) {
        create_ui_rect(panel, line, grid_color, ALWAYS_VISIBLE);
        line.y += 70;
    }
}

void
create_toolbars(EmbWindow *window)
{
    int i;
    int toolbar_offset = menubar_height + menubar_padding;
    int toolbar_height = icon_size + toolbar_padding;

    TABLE(toolbars);
    load_csv(toolbars, "assets/toolbars/index.csv");

    for (i=0; strcmp(toolbars[i][0], "END"); i++) {
        printf("%s\n", toolbars[i][0]);
    }

    Rect rect = make_rectangle(0, 0, 
            window->dimension.w,
            4*(toolbar_padding + toolbar_height)
            + toolbar_offset);
    EmbPanel *panel = window->panels[TOOLBAR_PANEL];
    create_ui_rect(panel, rect, bg_color, ALWAYS_VISIBLE);

    for (i=0; i<4; i++) {
        Rect rect = make_rectangle(
                toolbar_padding,
                i * (toolbar_padding + toolbar_height)
                    + toolbar_offset,
                window->dimension.w - 2*toolbar_padding,
                toolbar_height);
        create_ui_rect(panel, rect, toolbar_bg_color, ALWAYS_VISIBLE);
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
build_menu(EmbPanel *panel, char *fname, int x_offset, int menu)
{
    int i;
    int y_offset = 5;
    /* Default, before the list is populated. */
    int menu_width = 100;

    TABLE(labels);
    load_csv(labels, fname);

    int position[2] = {x_offset, y_offset};
    create_label(panel, position, labels[0][0], labels[0][1], ALWAYS_VISIBLE);
    int result = x_offset + 10;
    result += main->widgets[main->n_widgets-1].rect.w;

    Rect background = make_rectangle(
        x_offset, 20, menu_width,  (5 * menu_item_height) + 5);
    create_ui_rect(panel, background, white_color, menu);
  
    for (i=1; strcmp(labels[i][0], "END"); i++) {
        if (strcmp(labels[i][0], "---")) {
            position[1] += menu_item_height;
            create_label(panel, position,
                labels[i][0], labels[i][1], menu);
        }
        else {
            int pos[2];
            pos[0] = position[0];
            pos[1] = position[1]+menu_item_height;
            horizontal_rule(panel, pos, menu_width, menu);
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
create_statusbar(EmbWindow *w)
{
    Rect rect = make_rectangle(
            0,
            w->dimension.h - menubar_height,
            w->dimension.w,
            menubar_height);
    create_ui_rect(MAIN_WINDOW, rect, white_color, ALWAYS_VISIBLE);

    int position[] = {
        10,
        w->dimension.h - menubar_height
    };
    create_label(MAIN_WINDOW, position, statusbar_message,
        "do-nothing", ALWAYS_VISIBLE);
}

void
create_scrollbars(EmbPanel *panel)
{
    printf("%d", panel->closeable);
}

void
create_view(Window *window)
{
    create_grid(window);
    create_scrollbars(window->panels[window->n_panels]);
    window->n_panels++;
}

int
render_panel(EmbWindow *window, EmbPanel *panel)
{
    int i;
    for (i=0; i<panel->n_widgets; i++) {
        EmbWidget widget = panel->widgets[i];
        if (widget.visibility == ALWAYS_VISIBLE
           || widget.visibility == menu_state) {
            if (widget.mode == WIDGET_MODE_BLOCK) {
                render_rect(window, panel, widget.color, widget.rect);
            }
            if (widget.mode == WIDGET_MODE_BACKGROUND) {
                render_rect(window, panel, widget.color, widget.rect);
            }
            if (widget.mode == WIDGET_MODE_TEXT) {
                render_copy(window, panel, widget.image);
            }
        }
    }
}

/* Renderer for every widget in the user interface for
 * window with index window_id.
 */
int
render(EmbWindow *window)
{
    int i;
    render_clear(window, clear_color);

    for (i=0; i<window->n_panels; i++) {
        render_panel(window, window->panels[i]);
    }
    return 0;
}

EmbWindow *
load_window(char *fname)
{
    EmbWindow *w = (EmbWindow *)malloc(sizeof(EmbWindow));
    w->n_panels = 0;
    w->panels = (EmbPanel*)malloc(sizeof(EmbPanel)*MAX_PANELS);

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
    return w;
}

#if EM2_X11
/* Create the window: the window and renderer variables
 * are file scope.
 *
 * In order to carry the data from file to file all the project
 * scope data is stored in here.
 */
EmbWindow *
create_window(char *fname)
{
    EmbWindow *w = load_window_data(fname);

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
    create_toolbars(w->panels[w->n_panels]);
    create_view(w->panels[w->n_panels]);
    create_menubar(w->panels[w->n_panels]);
    create_statusbar(w->panels[w->n_panels]);
    w->n_panels += 4;
    return w;
}


/* Process input: main loop step 1.
 */
EmbEvent
process_input(Window *window)
{
    EmbEvent event;
    XEvent xevent;
    XNextEvent(window->display, &xevent);
    
    return event;
}

void
render_copy(EmbWindow *window, EmbPanel *panel, EmbImage image)
{

}

void
render_rect(EmbWindow *window, EmbPanel *panel, EmbColor color, Rect rect)
{

}

void
render_clear(EmbWindow *window, EmbColor color)
{
    XClearWindow(window->display, window->window);
}

void
destroy_window(EmbWindow *w)
{
    XFreeGC(w->display, w->gc);
    XDestroyWindow(w->display, w->window);
    XCloseDisplay(w->display);
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif

#if EM2_ANDROID
EmbWindow *
create_window(char *fname)
{
    EmbWindow *window = load_window_data(fname);

    return window;
}

/* Process input: main loop step 1.
 */
void
process_input(EmbWindow *window)
{

}

void
render_copy(Window *window, EmbImage image)
{

}

void
render_rect(Window *window, EmbColor color, Rect rect)
{

}

void
render_clear(Window *window, EmbColor color)
{

}

/* So android doesn't have multiple application 
 * "windows" so we need some way of abstracting out
 * windows into layers of application.
 */
void
destroy_window(Window *window)
{
    EmbWindow *w = windows[window_id];
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif

#if EM2_COCOA
EmbWindow *
create_window(char *fname)
{
    EmbWindow *window = load_window_data(fname);

    return window;
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
render_copy(Window *window, EmbImage image)
{

}

void
render_rect(Window *window, EmbColor color, Rect rect)
{

}

void
render_clear(Window *window, EmbColor color)
{

}

/* Same as android reasoning.
 */     
void
destroy_window(EmbWindow *window)
{
    EmbWindow *w = windows[window_id];
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif

#if EM2_WIN32
int
create_window(EmbWindow *window, char *fname)
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
render_copy(Window *window, EmbImage image)
{

}

void
render_rect(Window *window, EmbColor color, Rect rect)
{

}

void
render_clear(Window *window, EmbColor color)
{

}

void
destroy_window(Window *window)
{
    EmbWindow *w = windows[window_id];
    /* Should anything try to access the data after this point */
    w = NULL;
}
#endif

#if EM2_IOS
int
create_window(Window *window, char *fname)
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
render_copy(Window *window, EmbImage image)
{

}

void
render_rect(Window *window, EmbColor color, Rect rect)
{

}

void
render_clear(Window *window, EmbColor color)
{

}

/* Same as android reasoning.
 */     
void
destroy_window(Window *window)
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
render_copy(Window *window, EmbWidget widget)
{
    EmbWindow *w = windows[window_id];
    SDL_RenderCopy(w->renderer, widget.texture, NULL, &widget.rect);
}

void
render_rect(Window *window, int widget)
{
    EmbWindow *w = windows[window_id];
    EmbWidget widget = w->widgets[i];
    SDL_SetRenderDrawColor(w->renderer, widget.color[0],
        widget.color[1], widget.color[2], 255);
    SDL_RenderFillRect(w->renderer, &widget.rect);
}

void
render_clear(Window *window, int widget)
{
    SDL_SetRenderDrawColor(w->renderer,
        clear_color.r, clear_color.g, clear_color.b,
        255);
    SDL_RenderClear(w->renderer);
}
#endif

void
create_widget(EmbWindow *window, EmbPanel *panel, Rect rect, char *command)
{
    char icon_path[2*MAX_STRING_LENGTH];
    EmbWidget *widget = panel->widgets[panel->n_widgets];
    widget->rect = rect;
    widget->mode = WIDGET_MODE_BLOCK;
    widget->visibility = ALWAYS_VISIBLE;

    strcpy(widget->command, command);

    sprintf(icon_path, "assets/icons/%s.bmp", widget->command);
#if EM2_X11
#elif EM2_ANDROID
#elif EM2_IOS
#elif EM2_COCOA
#elif EM2_WIN32
#else
    SDL_Surface *surface;
    surface = SDL_LoadBMP(icon_path);
    widget->texture = SDL_CreateTextureFromSurface(w->renderer, surface);
    if (!widget->texture) {
        debug_message("Failed to load texture.");
        debug_message(icon_path);
    }
    SDL_FreeSurface(surface);
#endif

    panel->n_widgets++;
}

void
create_int_label(int w, int position[2], int value)
{
    char s[MAX_STRING_LENGTH];
    sprintf(s, "%d", value);
    int position[] = {x, y};
    create_label(w, position, s, "do-nothing", ALWAYS_VISIBLE);
}

void
create_measurement_label(int w, int position[2], int spacing, char *label, float value)
{
    char s[MAX_STRING_LENGTH];
    create_label(w, position, label, "do-nothing", ALWAYS_VISIBLE);
    sprintf(s, "%f mm", value);
    position[1] += spacing;
    create_label(w, position, s, "do-nothing", ALWAYS_VISIBLE);
}

void
create_labelled_int(EmbPanel *panel, int position[2], int spacing, char *label, int value)
{
    create_label(panel, position, label, "do-nothing", ALWAYS_VISIBLE);
    position[1] += spacing;
    create_int_label(panel, position, value);
}

int
create_spinbox(EmbPanel *panel, char *fname, float value)
{
    TABLE(table);
    load_csv(table, fname);
    strcpy(panel->widgets[panel->n_widgets].label, translate(get_str(table, "label")));
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
load_image(EmbPanel *panel, char *path)
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
