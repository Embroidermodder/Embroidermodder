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
 *  Ui Features for Scheme binding.
 *
 *  Use Python's PEP7 style guide.
 *      https://peps.python.org/pep-0007/
 */

#include "em2.h"

pointer
scm_create_label(scheme *sc, pointer args)
{
    SDL_Rect rect;
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }

    get_args(args, arg, 5);

    int char_width = load_int(sc, "char-width");
    int menu_item_height = load_int(sc, "menu-item-height");

    strcpy(mainwnd->widgets[mainwnd->n_widgets].label, string_value(pair_car(arg[2])));
    rect.x = get_int_from_closure(arg[0]);
    rect.y = get_int_from_closure(arg[1]);

    SDL_Color color = {0, 0, 0};
    SDL_Surface *surface = TTF_RenderText_Blended(mainwnd->font,
        mainwnd->widgets[mainwnd->n_widgets].label, color);
    /* strcpy(widgets[n_widgets].command, command); */
    rect.w = surface->w;
    rect.h = surface->h;

    mainwnd->widgets[mainwnd->n_widgets].rect = rect;
    mainwnd->widgets[mainwnd->n_widgets].mode = WIDGET_MODE_TEXT;
    mainwnd->widgets[mainwnd->n_widgets].visibility = ALWAYS_VISIBLE;

    mainwnd->widgets[mainwnd->n_widgets].texture = SDL_CreateTextureFromSurface(mainwnd->renderer, surface);
    if (!mainwnd->widgets[mainwnd->n_widgets].texture) {
        debug_message("Failed to generate ttf label.");
    }
    SDL_FreeSurface(surface);
    /* SDL_FreeTexture(texture); */

    mainwnd->n_widgets++;

    return sc->NIL;
}

pointer
scm_create_ui_rect(scheme *sc, pointer args)
{
    SDL_Rect rect;
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }

    get_args(args, arg, 7);
    if (list_length(sc, args) < 7) {
        return sc->NIL;
    }
    
    rect.x = get_int_from_closure(arg[0]);
    rect.y = get_int_from_closure(arg[1]);
    rect.w = get_int_from_closure(arg[2]);
    rect.h = get_int_from_closure(arg[3]);

    mainwnd->widgets[mainwnd->n_widgets].rect = rect;
    mainwnd->widgets[mainwnd->n_widgets].mode = WIDGET_MODE_BACKGROUND;
    mainwnd->widgets[mainwnd->n_widgets].color[0] = (unsigned char) get_int_from_closure(arg[4]);
    mainwnd->widgets[mainwnd->n_widgets].color[1] = (unsigned char) get_int_from_closure(arg[5]);
    mainwnd->widgets[mainwnd->n_widgets].color[2] = (unsigned char) get_int_from_closure(arg[6]);
    mainwnd->widgets[mainwnd->n_widgets].visibility = ALWAYS_VISIBLE;

    mainwnd->n_widgets++;

    return sc->NIL;
}

pointer
scm_set_visibility(scheme *sc, pointer args)
{
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }

    if (list_length(sc, args) < 2) {
        return sc->NIL;
    }
    get_args(args, arg, 2);

    set_visibility(string_value(pair_car(arg[0])),
        get_int_from_closure(arg[1]));

    return sc->NIL;
}

pointer
scm_create_widget(scheme *sc, pointer args)
{
    SDL_Rect rect;
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }

    if (list_length(sc, args) < 5) {
        return sc->NIL;
    }
    get_args(args, arg, 5);
    
    rect.x = get_int_from_closure(arg[0]);
    rect.y = get_int_from_closure(arg[1]);
    rect.w = get_int_from_closure(arg[2]);
    rect.h = get_int_from_closure(arg[3]);

    create_widget(rect, string_value(pair_car(arg[4])));

    return sc->NIL;
}

pointer
scm_create_icon(scheme *sc, pointer args)
{
    int n, m;
    pointer arg[10];
    if (args == sc->NIL) {
        return sc->NIL;
    }
    
    get_args(args, arg, 3);
    if (list_length(sc, args) < 3) {
        return sc->NIL;
    }

    int menubar_height = load_int(sc, "menubar-height");
    int icon_padding = load_int(sc, "icon-padding");
    int icon_size = load_int(sc, "icon-size");
    n = get_int_from_closure(arg[0]);
    m = get_int_from_closure(arg[1]);

    int icon_offset_n = n * icon_padding
        + (n-1)*(icon_size + icon_padding);
    int icon_offset_m = m * icon_padding
        + (m-1)*(icon_size + icon_padding);

    SDL_Rect rect;
    rect.x = icon_offset_n;
    rect.y = icon_offset_m + menubar_height;
    rect.w = icon_size;
    rect.h = icon_size;

    create_widget(rect, string_value(pair_car(arg[2])));

    return sc->NIL;
}

